/*
 * This file contains device specific hooks.
 * Always enclose hooks to #if MR_DEVICE_HOOKS >= ver
 * with corresponding hook version!
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>

#include <private/android_filesystem_config.h>

#include <log.h>
#include <util.h>
#include <multirom.h>
#include <libbootimg.h>

#define GATEKEEPER_DATA_DIR "/data/misc/gatekeeper"
#define GATEKEEPER_COLDBOOT_PATH "/data/misc/gatekeeper/.coldboot"

char* os_version = NULL;
char* os_level = NULL;
#if MR_DEVICE_HOOKS >= 1

int mrom_hook_after_android_mounts(const char *busybox_path, const char *base_path, int type)
{
    // wiping the data of a primary or secondary ROM causes the ROM to delete
    // all lockscreen accounts from the gatekeeper on next boot, preventing
    // the user from logging into the other ROMs.
    // To work around this, create the .coldboot file to prevent the wipe.
    if (access(GATEKEEPER_COLDBOOT_PATH, F_OK) == -1)
    {
        // the permission should be fixed during the first boot
        int err = mkdir_recursive(GATEKEEPER_DATA_DIR, 0700);
        if (err)
            ERROR("failed to mkdir " GATEKEEPER_DATA_DIR ": %s", strerror(err));
        else
        {
            int fd = open(GATEKEEPER_COLDBOOT_PATH, O_WRONLY|O_TRUNC|O_CREAT,
                S_IRUSR|S_IWUSR);
            if (fd < 0)
                ERROR("failed to open " GATEKEEPER_COLDBOOT_PATH ": %s",
                    strerror(errno));
            else
            {
                fchown(fd, AID_SYSTEM, AID_SYSTEM);
                close(fd);
            }
        }
    }
    if (type == ROM_DEFAULT)
        return 0;

    // Delete the old gatekeeper workaround
    // remove after people stop using Beta 1-4
    return 0;
}
#endif /* MR_DEVICE_HOOKS >= 1 */


#if MR_DEVICE_HOOKS >= 2
void mrom_hook_before_fb_close(void)
{
}
#endif /* MR_DEVICE_HOOKS >= 2 */


#if MR_DEVICE_HOOKS >= 3
void tramp_hook_before_device_init(void)
{
    // setup the clearpad touchscreen
}
#endif /* MR_DEVICE_HOOKS >= 3 */

#if MR_DEVICE_HOOKS >= 4
int mrom_hook_allow_incomplete_fstab(void)
{
    return 0;
}
#endif

#if MR_DEVICE_HOOKS >= 5

void mrom_hook_fixup_bootimg_cmdline(char *bootimg_cmdline, size_t bootimg_cmdline_cap)
{
}

int mrom_hook_has_kexec(void)
{
    return 0;
}
#endif

#if MR_DEVICE_HOOKS >= 6
static int fork_and_exec(char *cmd, char** env)
{
    pid_t pID = fork();
    if(pID == 0)
    {
        stdio_to_null();
        setpgid(0, getpid());
        if (strstr(cmd, "hwservicemanager")) {
            setuid(1000);
        } else if (strstr(cmd, "keymaster") || strstr(cmd, "qsee")) {
            setenv("OSVER", os_version, 1);
            setenv("OSPATCH", os_level, 1);
        }
        setenv("LD_LIBRARY_PATH", "/mrom_enc", 1);
        setenv("LD_PRELOAD", "/mrom_enc/libmultirom_fake_properties.so /mrom_enc/libmultirom_fake_propertywait.so", 1);
        execve(cmd, NULL, environ);
        ERROR("Failed to exec %s: %s\n", cmd[0], strerror(errno));
        _exit(127);
    }
    return pID;
}

static int fork_and_exec_with_strace(char *cmd, char** env)
{
    pid_t pID = fork();
    if(pID == 0)
    {
        char* args[] = {"/mrom_enc/strace", "-o", NULL, "-f", NULL, NULL};
        char strace_arg1[100];
        sprintf(strace_arg1, "strace-%s", (cmd + 10));
        args[2] = strace_arg1;
        args[4] = cmd;
        INFO("running %s %s %s %s %s\n", args[0], args[1], args[2], args[3], args[4]);
        stdio_to_null();
        setpgid(0, getpid());
        execve(args[0], args, env);
        ERROR("Failed to exec %s: %s\n", args[0], strerror(errno));
        _exit(127);
    }
    return pID;
}

int fork_exec_with_stdout(char *cmd, char *const *envp)
{
   int fd[2];
   char* command[] = {cmd, NULL};
   if(pipe2(fd, O_CLOEXEC) < 0)
        return -1;

    pid_t pid = fork();
    if (pid < 0)
    {
        close(fd[0]);
        close(fd[1]);
        return pid;
    }

    if(pid == 0) // child
    {
        close(fd[0]);
        dup2(fd[1], 1);  // send stdout to the pipe
        dup2(fd[1], 2);  // send stderr to the pipe
        close(fd[1]);

        if (execve(command[0], command, envp) == -1) {
            ERROR("execve failed %s\n", strerror(errno));
        }
        _exit(127);
    }
    else
    {
        close(fd[1]);

        char *res = malloc(512);
        char buffer[512];
        int size = 512, written = 0, len;
        /*while ((len = read(fd[0], buffer, sizeof(buffer))) > 0)
        {
            if(written + len + 1 > size)
            {
                size = written + len + 256;
                res = realloc(res, size);
            }
            memcpy(res+written, buffer, len);
            written += len;
            res[written] = 0;
        }

        close(fd[0]);*/

        //waitpid(pid, exit_code, 0);

        if(written == 0)
        {
            free(res);
            return pid;
        }
        return pid;
    }
    return pid;
}

static int qseecomd_pid = -1;
static int hwservice_pid = -1;
static int keymaster_pid = -1;
static int gatekeeper_pid = -1;
static int servicemanager_pid = -1;

struct pollfd property_poller;

void wait_for_property(char* property) {

    int rc = 0;
    char buf[256];
    memset(buf, 0, sizeof(buf));
    while (1) {
        rc = poll(&property_poller, 1, 5000);

        if (rc == 0) {
            INFO("property wait timed out\n");
            break;
        } else {
            INFO("property arrived\n");
            read(property_poller.fd, buf, sizeof(buf));
            INFO("received property %s\n", buf);
            if (strstr(buf, property)) {
                break;
            }
        }
    }
}

void setup_property_listener() {

    int fromlen;
    int i, s, ns, len;
    struct sockaddr_un saun, fsaun;

    /*
     * Get a socket to work with.  This socket will
     * be in the UNIX domain, and will be a
     * stream socket.
     */
    if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
        ERROR("server: socket\n");
    }

    /*
     * Create the address we will be binding to.
     */
    saun.sun_family = AF_UNIX;
    strcpy(saun.sun_path, "/property_socket");

    /*
     * Try to bind the address to the socket.  We
     * unlink the name first so that the bind won't
     * fail.
     *
     * The third argument indicates the "length" of
     * the structure, not just the length of the
     * socket name.
     */
    len = sizeof(saun.sun_family) + strlen(saun.sun_path);

    if (bind(s, (struct sockaddr *)&saun, sizeof(struct sockaddr_un)) < 0) {
        ERROR("server: bind\n");
    }

    property_poller.fd = s;
    property_poller.events = POLLIN;

}

void tramp_hook_encryption_setup(void)
{

    // start qseecomd
    char* env[] = {"LD_LIBRARY_PATH=/mrom_enc", "LD_PRELOAD=/mrom_enc/libmultirom_fake_properties.so /mrom_enc/libmultirom_fake_propertywait.so", NULL};

    symlink("/dev/block/platform/soc/1d84000.ufshc", "/dev/block/bootdevice");
    struct bootimg img;
    if(libbootimg_init_load(&img, "/dev/block/bootdevice/by-name/boot", LIBBOOTIMG_LOAD_HDR_ONLY) < 0)
    {
        ERROR("Could not open boot image\n");
    }

    os_version = libbootimg_get_osversion(&img.hdr);
    os_level = libbootimg_get_oslevel(&img.hdr);

    INFO("OS version: %s Security patch: %s\n", os_version, os_level);
    libbootimg_destroy(&img);

    setup_property_listener();

    // setup links and permissions based on TWRP's
    chmod("/dev/qseecom", 0660);
    chmod("/dev/urandom", 0666);
    chmod("/dev/binder", 0777);
    chmod("/dev/hwbinder", 0777);
    chmod("/dev/vndbinder", 0777);
    chmod("/dev/kmsg", 0777);
    chmod("/dev/binder", 0777);
    chown("/dev/qseecom", AID_SYSTEM, AID_DRMRPC);
    chown("/dev/ion", AID_SYSTEM, AID_SYSTEM);
    chmod("/mrom_enc/adbd", 0777);
    chmod("/mrom_enc/busybox", 0777);
    chmod("/mrom_enc/strace", 0777);
    chmod("/mrom_enc/qseecomd", 0755);
    chmod("/mrom_enc/hwservicemanager", 0755);
    chmod("/mrom_enc/android.hardware.keymaster@3.0-service-qti", 0755);
    INFO("chmods done. now running binaries\n");

    /*int exit_code = 0;
    static char *const encmnt_cmd[] = { "/mrom_enc/hwservicemanager", NULL};
    static char *const encmnt_envp[] = { "LD_LIBRARY_PATH=/mrom_enc/", "LD_PRELOAD=/mrom_enc/libmultirom_fake_properties.so", NULL };*/
    hwservice_pid = fork_and_exec("/mrom_enc/hwservicemanager", env);
    //hwservice_pid = fork_and_exec("/mrom_enc/hwservicemanager", env);
    if (hwservice_pid == -1)
        ERROR("Failed to fork for hwservice; should never happen!\n");
    else
        INFO("hwservice started: pid=%d\n", hwservice_pid);

    //sleep(5);
    //
    wait_for_property("hwservicemanager.ready");

    qseecomd_pid = fork_and_exec("/mrom_enc/qseecomd", env);
    if (qseecomd_pid == -1)
        ERROR("Failed to fork for qseecomd; should never happen!\n");
    else
        INFO("qseecomd started: pid=%d\n", qseecomd_pid);

    //sleep(5);
    wait_for_property("sys.listeners.registered");

    keymaster_pid = fork_and_exec("/mrom_enc/android.hardware.keymaster@3.0-service-qti", env);
    if (keymaster_pid == -1)
        ERROR("Failed to fork for keymaster; should never happen!\n");
    else
        INFO("keymaster started: pid=%d\n", keymaster_pid);

    //sleep(5);

    /*servicemanager_pid = fork_and_exec("/mrom_enc/servicemanager", env);
    if (servicemanager_pid == -1)
        ERROR("Failed to fork for servicemanager; should never happen!\n");
    else
        INFO("servicemanager started: pid=%d\n", servicemanager_pid);*/

}
void tramp_hook_encryption_cleanup(void)
{
    struct stat info;
    int rc;
    if (qseecomd_pid != -1)
    {
        //rc = kill(-servicemanager_pid, SIGTERM); // kill the entire process group
        rc = kill(-qseecomd_pid, SIGTERM); // kill the entire process group
        waitpid(qseecomd_pid, NULL, 0);
        ERROR("qseecomd killed %d\n", rc);
    }
    if (keymaster_pid != -1)
    {
        rc = kill(-keymaster_pid, SIGTERM); // kill the entire process group
        waitpid(keymaster_pid, NULL, 0);
        ERROR("keymaster killed %d\n", rc);
    }
    if (hwservice_pid != -1)
    {
        rc = kill(-hwservice_pid, SIGTERM); // kill the entire process group
        waitpid(hwservice_pid, NULL, 0);
        ERROR("hwservicemanager killed %d\n", rc);
    }
    // make sure we're removing our symlink
    if (lstat("/dev/block/bootdevice", &info) >= 0 && S_ISLNK(info.st_mode))
        remove("/dev/block/bootdevice");
    INFO("cleaned up after qseecomd\n");
}

static void replace_tag(char *cmdline, size_t cap, const char *tag, const char *what)
{
    char *start, *end;
    char *str = cmdline;
    char *str_end = str + strlen(str);
    size_t replace_len = strlen(what);

    while((start = strstr(str, tag)))
    {
        end = strstr(start, " ");
        if(!end)
            end = str_end;
        else if(replace_len == 0)
            ++end;

        if(end != start)
        {

            size_t len = str_end - end;
            if((start - cmdline)+replace_len+len > cap)
                len = cap - replace_len - (start - cmdline);
            memmove(start+replace_len, end, len+1); // with \0
            memcpy(start, what, replace_len);
        }

        str = start+replace_len;
    }
}

void mrom_hook_fixup_full_cmdline(char *bootimg_cmdline, size_t bootimg_cmdline_cap)
{
    // force dm-verity to Logging mode to prevent scary "your phone is corrupt" message
    replace_tag(bootimg_cmdline, bootimg_cmdline_cap, "androidboot.veritymode=", "androidboot.veritymode=logging");
}
#endif
