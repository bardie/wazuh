/* Copyright (C) 2009 Trend Micro Inc.
 * All rights reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

/* Functions to handle operation with files
 */

#include "shared.h"

#ifndef WIN32
#include <regex.h>
#else
#include <aclapi.h>
#endif

/* Vista product information */
#ifdef WIN32

#ifndef PRODUCT_UNLICENSED
#define PRODUCT_UNLICENSED 0xABCDABCD
#endif
#ifndef PRODUCT_UNLICENSED_C
#define PRODUCT_UNLICENSED_C "Product Unlicensed "
#endif

#ifndef PRODUCT_BUSINESS
#define PRODUCT_BUSINESS 0x00000006
#endif
#ifndef PRODUCT_BUSINESS_C
#define PRODUCT_BUSINESS_C "Business Edition "
#endif

#ifndef PRODUCT_BUSINESS_N
#define PRODUCT_BUSINESS_N 0x00000010
#endif
#ifndef PRODUCT_BUSINESS_N_C
#define PRODUCT_BUSINESS_N_C "Business Edition "
#endif

#ifndef PRODUCT_CLUSTER_SERVER
#define PRODUCT_CLUSTER_SERVER 0x00000012
#endif
#ifndef PRODUCT_CLUSTER_SERVER_C
#define PRODUCT_CLUSTER_SERVER_C "Cluster Server Edition "
#endif

#ifndef PRODUCT_CLUSTER_SERVER_V
#define PRODUCT_CLUSTER_SERVER_V 0x00000040
#endif
#ifndef PRODUCT_CLUSTER_SERVER_V_C
#define PRODUCT_CLUSTER_SERVER_V_C "Server Hyper Core V "
#endif

#ifndef PRODUCT_DATACENTER_SERVER
#define PRODUCT_DATACENTER_SERVER 0x00000008
#endif
#ifndef PRODUCT_DATACENTER_SERVER_C
#define PRODUCT_DATACENTER_SERVER_C "Datacenter Edition (full) "
#endif

#ifndef PRODUCT_DATACENTER_SERVER_CORE
#define PRODUCT_DATACENTER_SERVER_CORE 0x0000000C
#endif
#ifndef PRODUCT_DATACENTER_SERVER_CORE_C
#define PRODUCT_DATACENTER_SERVER_CORE_C "Datacenter Edition (core) "
#endif

#ifndef PRODUCT_DATACENTER_SERVER_CORE_V
#define PRODUCT_DATACENTER_SERVER_CORE_V 0x00000027
#endif
#ifndef PRODUCT_DATACENTER_SERVER_CORE_V_C
#define PRODUCT_DATACENTER_SERVER_CORE_V_C "Datacenter Edition (core) "
#endif

#ifndef PRODUCT_DATACENTER_SERVER_V
#define PRODUCT_DATACENTER_SERVER_V 0x00000025
#endif
#ifndef PRODUCT_DATACENTER_SERVER_V_C
#define PRODUCT_DATACENTER_SERVER_V_C "Datacenter Edition (full) "
#endif

#ifndef PRODUCT_ENTERPRISE
#define PRODUCT_ENTERPRISE 0x00000004
#endif
#ifndef PRODUCT_ENTERPRISE_C
#define PRODUCT_ENTERPRISE_C "Enterprise Edition "
#endif

#ifndef PRODUCT_ENTERPRISE_N
#define PRODUCT_ENTERPRISE_N 0x0000001B
#endif
#ifndef PRODUCT_ENTERPRISE_N_C
#define PRODUCT_ENTERPRISE_N_C "Enterprise Edition "
#endif

#ifndef PRODUCT_ENTERPRISE_SERVER
#define PRODUCT_ENTERPRISE_SERVER 0x0000000A
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_C
#define PRODUCT_ENTERPRISE_SERVER_C "Enterprise Edition (full) "
#endif

#ifndef PRODUCT_ENTERPRISE_SERVER_CORE
#define PRODUCT_ENTERPRISE_SERVER_CORE 0x0000000E
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_CORE_C
#define PRODUCT_ENTERPRISE_SERVER_CORE_C "Enterprise Edition (core) "
#endif

#ifndef PRODUCT_ENTERPRISE_SERVER_CORE_V
#define PRODUCT_ENTERPRISE_SERVER_CORE_V 0x00000029
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_CORE_V_C
#define PRODUCT_ENTERPRISE_SERVER_CORE_V_C "Enterprise Edition (core) "
#endif

#ifndef PRODUCT_ENTERPRISE_SERVER_IA64
#define PRODUCT_ENTERPRISE_SERVER_IA64 0x0000000F
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_IA64_C
#define PRODUCT_ENTERPRISE_SERVER_IA64_C "Enterprise Edition for Itanium-based Systems "
#endif

#ifndef PRODUCT_ENTERPRISE_SERVER_V
#define PRODUCT_ENTERPRISE_SERVER_V 0x00000026
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_V_C
#define PRODUCT_ENTERPRISE_SERVER_V_C "Enterprise Edition (full) "
#endif

#ifndef PRODUCT_HOME_BASIC
#define PRODUCT_HOME_BASIC 0x00000002
#endif
#ifndef PRODUCT_HOME_BASIC_C
#define PRODUCT_HOME_BASIC_C "Home Basic Edition "
#endif

#ifndef PRODUCT_HOME_BASIC_N
#define PRODUCT_HOME_BASIC_N 0x00000005
#endif
#ifndef PRODUCT_HOME_BASIC_N_C
#define PRODUCT_HOME_BASIC_N_C "Home Basic Edition "
#endif

#ifndef PRODUCT_HOME_PREMIUM
#define PRODUCT_HOME_PREMIUM 0x00000003
#endif
#ifndef PRODUCT_HOME_PREMIUM_C
#define PRODUCT_HOME_PREMIUM_C "Home Premium Edition "
#endif

#ifndef PRODUCT_HOME_PREMIUM_N
#define PRODUCT_HOME_PREMIUM_N 0x0000001A
#endif
#ifndef PRODUCT_HOME_PREMIUM_N_C
#define PRODUCT_HOME_PREMIUM_N_C "Home Premium Edition "
#endif

#ifndef PRODUCT_HOME_SERVER
#define PRODUCT_HOME_SERVER 0x00000013
#endif
#ifndef PRODUCT_HOME_SERVER_C
#define PRODUCT_HOME_SERVER_C "Home Server Edition "
#endif

#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT
#define PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT 0x0000001E
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT_C
#define PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT_C "Essential Business Server Management Server "
#endif

#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING
#define PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING 0x00000020
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING_C
#define PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING_C "Essential Business Server Messaging Server "
#endif

#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY
#define PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY 0x0000001F
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY_C
#define PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY_C "Essential Business Server Security Server "
#endif

#ifndef PRODUCT_SERVER_FOR_SMALLBUSINESS
#define PRODUCT_SERVER_FOR_SMALLBUSINESS 0x00000018
#endif
#ifndef PRODUCT_SERVER_FOR_SMALLBUSINESS_C
#define PRODUCT_SERVER_FOR_SMALLBUSINESS_C "Small Business Edition "
#endif

#ifndef PRODUCT_SMALLBUSINESS_SERVER
#define PRODUCT_SMALLBUSINESS_SERVER 0x00000009
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER_C
#define PRODUCT_SMALLBUSINESS_SERVER_C "Small Business Server "
#endif

#ifndef PRODUCT_SMALLBUSINESS_SERVER_PREMIUM
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM 0x00000019
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_C
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_C "Small Business Server Premium Edition "
#endif

#ifndef PRODUCT_STANDARD_SERVER
#define PRODUCT_STANDARD_SERVER 0x00000007
#endif
#ifndef PRODUCT_STANDARD_SERVER_C
#define PRODUCT_STANDARD_SERVER_C "Standard Edition "
#endif

#ifndef PRODUCT_STANDARD_SERVER_CORE
#define PRODUCT_STANDARD_SERVER_CORE 0x0000000D
#endif
#ifndef PRODUCT_STANDARD_SERVER_CORE_C
#define PRODUCT_STANDARD_SERVER_CORE_C "Standard Edition (core) "
#endif

#ifndef PRODUCT_STANDARD_SERVER_CORE_V
#define PRODUCT_STANDARD_SERVER_CORE_V 0x00000028
#endif
#ifndef PRODUCT_STANDARD_SERVER_CORE_V_C
#define PRODUCT_STANDARD_SERVER_CORE_V_C "Standard Edition "
#endif

#ifndef PRODUCT_STANDARD_SERVER_V
#define PRODUCT_STANDARD_SERVER_V 0x00000024
#endif
#ifndef PRODUCT_STANDARD_SERVER_V_C
#define PRODUCT_STANDARD_SERVER_V_C "Standard Edition "
#endif

#ifndef PRODUCT_STARTER
#define PRODUCT_STARTER 0x0000000B
#endif
#ifndef PRODUCT_STARTER_C
#define PRODUCT_STARTER_C "Starter Edition "
#endif

#ifndef PRODUCT_STORAGE_ENTERPRISE_SERVER
#define PRODUCT_STORAGE_ENTERPRISE_SERVER 0x00000017
#endif
#ifndef PRODUCT_STORAGE_ENTERPRISE_SERVER_C
#define PRODUCT_STORAGE_ENTERPRISE_SERVER_C "Storage Server Enterprise Edition "
#endif

#ifndef PRODUCT_STORAGE_EXPRESS_SERVER
#define PRODUCT_STORAGE_EXPRESS_SERVER 0x00000014
#endif
#ifndef PRODUCT_STORAGE_EXPRESS_SERVER_C
#define PRODUCT_STORAGE_EXPRESS_SERVER_C "Storage Server Express Edition "
#endif

#ifndef PRODUCT_STORAGE_STANDARD_SERVER
#define PRODUCT_STORAGE_STANDARD_SERVER 0x00000015
#endif
#ifndef PRODUCT_STORAGE_STANDARD_SERVER_C
#define PRODUCT_STORAGE_STANDARD_SERVER_C "Storage Server Standard Edition "
#endif

#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER
#define PRODUCT_STORAGE_WORKGROUP_SERVER 0x00000016
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER_C
#define PRODUCT_STORAGE_WORKGROUP_SERVER_C "Storage Server Workgroup Edition "
#endif

#ifndef PRODUCT_ULTIMATE
#define PRODUCT_ULTIMATE 0x00000001
#endif
#ifndef PRODUCT_ULTIMATE_C
#define PRODUCT_ULTIMATE_C "Ultimate Edition "
#endif

#ifndef PRODUCT_ULTIMATE_N
#define PRODUCT_ULTIMATE_N 0x0000001C
#endif
#ifndef PRODUCT_ULTIMATE_N_C
#define PRODUCT_ULTIMATE_N_C "Ultimate Edition "
#endif

#ifndef PRODUCT_WEB_SERVER
#define PRODUCT_WEB_SERVER 0x00000011
#endif
#ifndef PRODUCT_WEB_SERVER_C
#define PRODUCT_WEB_SERVER_C "Web Server Edition "
#endif

#ifndef PRODUCT_WEB_SERVER_CORE
#define PRODUCT_WEB_SERVER_CORE 0x0000001D
#endif
#ifndef PRODUCT_WEB_SERVER_CORE_C
#define PRODUCT_WEB_SERVER_CORE_C "Web Server Edition "
#endif

#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL
#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL 0x0000003C
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL_C
#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL_C "Essential Server Solution Additional "
#endif

#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC
#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC 0x0000003E
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC_C
#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC_C "Essential Server Solution Additional SVC "
#endif

#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT
#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT 0x0000003B
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT_C
#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT_C "Essential Server Solution Management "
#endif

#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC
#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC 0x0000003D
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC_C
#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC_C "Essential Server Solution Management SVC "
#endif

#ifndef PRODUCT_HOME_PREMIUM_SERVER
#define PRODUCT_HOME_PREMIUM_SERVER 0x00000022
#endif
#ifndef PRODUCT_HOME_PREMIUM_SERVER_C
#define PRODUCT_HOME_PREMIUM_SERVER_C "Home Server 2011 "
#endif

#ifndef PRODUCT_HYPERV
#define PRODUCT_HYPERV 0x0000002A
#endif
#ifndef PRODUCT_HYPERV_C
#define PRODUCT_HYPERV_C "Hyper-V Server "
#endif

#ifndef PRODUCT_MULTIPOINT_PREMIUM_SERVER
#define PRODUCT_MULTIPOINT_PREMIUM_SERVER 0x0000004D
#endif
#ifndef PRODUCT_MULTIPOINT_PREMIUM_SERVER_C
#define PRODUCT_MULTIPOINT_PREMIUM_SERVER_C "MultiPoint Server Premium (full installation) "
#endif

#ifndef PRODUCT_MULTIPOINT_STANDARD_SERVER
#define PRODUCT_MULTIPOINT_STANDARD_SERVER 0x0000004C
#endif
#ifndef PRODUCT_MULTIPOINT_STANDARD_SERVER_C
#define PRODUCT_MULTIPOINT_STANDARD_SERVER_C "MultiPoint Server Standard (full installation) "
#endif

#ifndef PRODUCT_STANDARD_SERVER_SOLUTIONS
#define PRODUCT_STANDARD_SERVER_SOLUTIONS 0x00000034
#endif
#ifndef PRODUCT_STANDARD_SERVER_SOLUTIONS_C
#define PRODUCT_STANDARD_SERVER_SOLUTIONS_C "Server Solutions Premium "
#endif

#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER_CORE
#define PRODUCT_STORAGE_WORKGROUP_SERVER_CORE 0x0000002D
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER_CORE_C
#define PRODUCT_STORAGE_WORKGROUP_SERVER_CORE_C "Storage Server Workgroup (core installation) "
#endif

#define mkstemp(x) 0
#define mkdir(x, y) mkdir(x)

#endif /* WIN32 */

const char *__local_name = "unset";

/* Set the name of the starting program */
void OS_SetName(const char *name)
{
    __local_name = name;
    return;
}

time_t File_DateofChange(const char *file)
{
    struct stat file_status;

    if (stat(file, &file_status) < 0) {
        return (-1);
    }

    return (file_status.st_mtime);
}

ino_t File_Inode(const char *file)
{
    struct stat buffer;
    return stat(file, &buffer) ? 0 : buffer.st_ino;
}

int IsDir(const char *file)
{
    struct stat file_status;
    if (stat(file, &file_status) < 0) {
        return (-1);
    }
    if (S_ISDIR(file_status.st_mode)) {
        return (0);
    }
    return (-1);
}

int IsFile(const char *file)
{
    struct stat buf;
	return (!stat(file, &buf) && S_ISREG(buf.st_mode)) ? 0 : -1;
}

off_t FileSize(const char * path) {
    struct stat buf;
    return stat(path, &buf) ? -1 : buf.st_size;
}

int CreatePID(const char *name, int pid)
{
    char file[256];
    FILE *fp;

    if (isChroot()) {
        snprintf(file, 255, "%s/%s-%d.pid", OS_PIDFILE, name, pid);
    } else {
        snprintf(file, 255, "%s%s/%s-%d.pid", DEFAULTDIR,
                 OS_PIDFILE, name, pid);
    }

    fp = fopen(file, "a");
    if (!fp) {
        return (-1);
    }

    fprintf(fp, "%d\n", pid);

    if (chmod(file, 0640) != 0) {
        fclose(fp);
        return (-1);
    }

    fclose(fp);

    return (0);
}

char *GetRandomNoise()
{
    FILE *fp;
    char buf[2048 + 1];
    size_t n;

    /* Reading urandom */
    fp = fopen("/dev/urandom", "r");
    if(!fp)
    {
        return(NULL);
    }

    n = fread(buf, 1, 2048, fp);
    fclose(fp);

    if (n == 2048) {
        buf[2048] = '\0';
        return(strdup(buf));
    } else {
        return NULL;
    }
}

int DeletePID(const char *name)
{
    char file[256];

    if (isChroot()) {
        snprintf(file, 255, "%s/%s-%d.pid", OS_PIDFILE, name, (int)getpid());
    } else {
        snprintf(file, 255, "%s%s/%s-%d.pid", DEFAULTDIR,
                 OS_PIDFILE, name, (int)getpid());
    }

    if (File_DateofChange(file) < 0) {
        return (-1);
    }

    if (unlink(file)) {
        mferror(DELETE_ERROR, file, errno, strerror(errno));
    }

    return (0);
}

void DeleteState() {
    char path[PATH_MAX + 1];

    if (strcmp(__local_name, "unset")) {
#ifdef WIN32
        snprintf(path, sizeof(path), "%s.state", __local_name);
#else
        snprintf(path, sizeof(path), "%s" OS_PIDFILE "/%s.state", isChroot() ? "" : DEFAULTDIR, __local_name);
#endif
        unlink(path);
    } else {
        merror("At DeleteState(): __local_name is unset.");
    }
}

int UnmergeFiles(const char *finalpath, const char *optdir, int mode)
{
    int ret = 1;
    int state_ok;
    size_t i = 0, n = 0, files_size = 0;
    char *files;
    char * copy;
    char final_name[2048 + 1];
    char buf[2048 + 1];
    FILE *fp;
    FILE *finalfp;

    finalfp = fopen(finalpath, mode == OS_BINARY ? "rb" : "r");
    if (!finalfp) {
        merror("Unable to read merged file: '%s'.", finalpath);
        return (0);
    }

    while (1) {
        /* Read header portion */
        if (fgets(buf, sizeof(buf) - 1, finalfp) == NULL) {
            break;
        }

        /* Initiator */
        if (buf[0] != '!') {
            continue;
        }

        /* Get file size and name */
        files_size = (size_t) atol(buf + 1);

        files = strchr(buf, '\n');
        if (files) {
            *files = '\0';
        }

        files = strchr(buf, ' ');
        if (!files) {
            ret = 0;
            continue;
        }
        files++;
        state_ok = 1;

        if (optdir) {
            snprintf(final_name, 2048, "%s/%s", optdir, files);

            // Check that final_name is inside optdir

            if (w_ref_parent_folder(final_name)) {
                merror("Unmerging '%s': unable to unmerge '%s' (it contains '..')", finalpath, final_name);
                state_ok = 0;
            }
        } else {
            strncpy(final_name, files, 2048);
            final_name[2048] = '\0';
        }

        // Create directory

        copy = strdup(final_name);

        if (mkdir_ex(dirname(copy))) {
            merror("Unmerging '%s': couldn't create directory '%s'", finalpath, files);
            state_ok = 0;
        }

        free(copy);

        /* Open filename */

        if (state_ok) {
            if (fp = fopen(final_name, mode == OS_BINARY ? "wb" : "w"), !fp) {
                ret = 0;
                merror("Unable to unmerge file '%s'.", final_name);
            }
        } else {
            fp = NULL;
            ret = 0;
        }

        if (files_size < sizeof(buf) - 1) {
            i = files_size;
            files_size = 0;
        } else {
            i = sizeof(buf) - 1;
            files_size -= sizeof(buf) - 1;
        }

        while ((n = fread(buf, 1, i, finalfp)) > 0) {
            buf[n] = '\0';

            if (fp) {
                fwrite(buf, n, 1, fp);
            }

            if (files_size == 0) {
                break;
            } else {
                if (files_size < sizeof(buf) - 1) {
                    i = files_size;
                    files_size = 0;
                } else {
                    i = sizeof(buf) - 1;
                    files_size -= sizeof(buf) - 1;
                }
            }
        }

        if (fp) {
            fclose(fp);
        }
    }

    fclose(finalfp);
    return (ret);
}

int MergeAppendFile(const char *finalpath, const char *files, const char *tag)
{
    size_t n = 0;
    long files_size = 0;
    char buf[2048 + 1];
    const char *tmpfile;
    FILE *fp;
    FILE *finalfp;

    /* Create a new entry */
    if (files == NULL) {
        finalfp = fopen(finalpath, "w");
        if (!finalfp) {
            merror("Unable to create merged file: '%s'.", finalpath);
            return (0);
        }

        if (tag) {
            fprintf(finalfp, "#%s\n", tag);
        }

        fclose(finalfp);

        if (chmod(finalpath, 0640) < 0) {
            merror(CHMOD_ERROR, finalpath, errno, strerror(errno));
            return 0;
        }

        return (1);
    }

    finalfp = fopen(finalpath, "a");
    if (!finalfp) {
        merror("Unable to append merged file: '%s'.", finalpath);
        return (0);
    }

    fp = fopen(files, "r");
    if (!fp) {
        merror("Unable to merge file '%s'.", files);
        fclose(finalfp);
        return (0);
    }

    fseek(fp, 0, SEEK_END);
    files_size = ftell(fp);

    tmpfile = strrchr(files, '/');
    if (tmpfile) {
        tmpfile++;
    } else {
        tmpfile = files;
    }

    if (tag) {
        fprintf(finalfp, "#%s\n", tag);
    }

    fprintf(finalfp, "!%ld %s\n", files_size, tmpfile);

    fseek(fp, 0, SEEK_SET);

    while ((n = fread(buf, 1, sizeof(buf) - 1, fp)) > 0) {
        buf[n] = '\0';
        fwrite(buf, n, 1, finalfp);
    }

    fclose(fp);

    fclose(finalfp);
    return (1);
}

int MergeFiles(const char *finalpath, char **files, const char *tag)
{
    int i = 0, ret = 1;
    size_t n = 0;
    long files_size = 0;

    char *tmpfile;
    char buf[2048 + 1];
    FILE *fp;
    FILE *finalfp;

    finalfp = fopen(finalpath, "w");
    if (!finalfp) {
        merror("Unable to create merged file: '%s'.", finalpath);
        return (0);
    }

    if (tag) {
        fprintf(finalfp, "#%s\n", tag);
    }

    while (files[i]) {
        fp = fopen(files[i], "r");
        if (!fp) {
            merror("Unable to merge file '%s'.", files[i]);
            i++;
            ret = 0;
            continue;
        }

        fseek(fp, 0, SEEK_END);
        files_size = ftell(fp);

        /* Remove last entry */
        tmpfile = strrchr(files[i], '/');
        if (tmpfile) {
            tmpfile++;
        } else {
            tmpfile = files[i];
        }

        fprintf(finalfp, "!%ld %s\n", files_size, tmpfile);

        fseek(fp, 0, SEEK_SET);
        while ((n = fread(buf, 1, sizeof(buf) - 1, fp)) > 0) {
            buf[n] = '\0';
            fwrite(buf, n, 1, finalfp);
        }

        fclose(fp);
        i++;
    }

    fclose(finalfp);
    return (ret);
}


#ifndef WIN32
/* Get basename of path */
char *basename_ex(char *path)
{
    return (basename(path));
}

/* Rename file or directory */
int rename_ex(const char *source, const char *destination)
{
    if (rename(source, destination)) {
        mferror(RENAME_ERROR, source, destination, errno, strerror(errno));

        return (-1);
    }

    return (0);
}

/* Create a temporary file */
int mkstemp_ex(char *tmp_path)
{
    int fd;
    mode_t old_mask = umask(0177);

    fd = mkstemp(tmp_path);
    umask(old_mask);

    if (fd == -1) {
        mferror(MKSTEMP_ERROR, tmp_path, errno, strerror(errno));

        return (-1);
    }

    /* mkstemp() only implicitly does this in POSIX 2008 */
    if (fchmod(fd, 0600) == -1) {
        close(fd);

        mferror(CHMOD_ERROR, tmp_path, errno, strerror(errno));

        if (unlink(tmp_path)) {
            mferror(DELETE_ERROR, tmp_path, errno, strerror(errno));
        }

        return (-1);
    }

    close(fd);
    return (0);
}

static const char *get_unix_version()
{
    FILE *os_release, *cmd_output, *version_release, *cmd_output_ver;
    char buff[256];
    static char string[256];
    char *tag, *end;
    char *name = NULL;
    char *id = NULL;
    char *version = NULL;

    // Try to open /etc/os-release
    os_release = fopen("/etc/os-release", "r");
    // Try to open /usr/lib/os-release
    if(!os_release) os_release = fopen("/usr/lib/os-release", "r");

    if(os_release){
        while (fgets(buff, sizeof(buff)- 1, os_release)) {
            tag = strtok(buff, "=");
            if (strcmp (tag,"NAME") == 0){
                if (!name) {
                    name = strtok(NULL, "\n");
                    if (name[0] == '\"' && (end = strchr(++name, '\"'), end)) {
                        *end = '\0';
                    }
                    name = strdup(name);
                }
            } else if (strcmp (tag,"VERSION") == 0){
                if (!version) {
                    version = strtok(NULL, "\n");
                    if (version[0] == '\"' && (end = strchr(++version, '\"'), end)) {
                        *end = '\0';
                    }
                    version = strdup(version);
                }
            } else if (strcmp (tag,"ID") == 0){
                if (!id) {
                    id = strtok(NULL, " \n");
                    if (id[0] == '\"' && (end = strchr(++id, '\"'), end)) {
                        *end = '\0';
                    }
                    id = strdup(id);
                }
            }
        }
        fclose(os_release);
    }
    // Linux old distributions without 'os-release' file
    else {
        regex_t regexCompiled;
        regmatch_t match[2];
        int match_size;
        // CentOS
        if (version_release = fopen("/etc/centos-release","r"), version_release){
            name = strdup("CentOS Linux");
            id = strdup("centos");
            static const char *pattern = " ([0-9][0-9]*\\.[0-9][0-9]*)\\.*";
            if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
                merror_exit("Can not compile regular expression.");
            }
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                if(regexec(&regexCompiled, buff, 2, match, 0) == 0){
                    match_size = match[1].rm_eo - match[1].rm_so;
                    version = malloc(match_size +1);
                    snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    break;
                }
            }
            regfree(&regexCompiled);
            fclose(version_release);
        // Fedora
        } else if (version_release = fopen("/etc/fedora-release","r"), version_release){
            name = strdup("Fedora");
            id = strdup("fedora");
            static const char *pattern = " ([0-9][0-9]*) ";
            if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
                merror_exit("Can not compile regular expression.");
            }
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                if(regexec(&regexCompiled, buff, 2, match, 0) == 0){
                    match_size = match[1].rm_eo - match[1].rm_so;
                    version = malloc(match_size +1);
                    snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    break;
                }
            }
            regfree(&regexCompiled);
            fclose(version_release);
        // RedHat
        } else if (version_release = fopen("/etc/redhat-release","r"), version_release){
            static const char *pattern = "([0-9][0-9]*\\.[0-9][0-9]*)\\.*";
            if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
                merror_exit("Can not compile regular expression.");
            }
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                if (strstr(buff, "CentOS")){
                    if (!(name || id)) {
                        name = strdup("CentOS");
                        id = strdup("centos");
                    }
                }else if (strstr(buff, "Fedora")){
                    if (!(name || id)) {
                        name = strdup("Fedora");
                        id = strdup("fedora");
                    }
                }else{
                    if (!(name || id)) {
                        name = strdup("Red Hat Enterprise Linux");
                        id = strdup("rhel");
                    }
                }

                if(regexec(&regexCompiled, buff, 2, match, 0) == 0){
                    match_size = match[1].rm_eo - match[1].rm_so;
                    version = malloc(match_size +1);
                    snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    break;
                }
            }
            regfree(&regexCompiled);
            fclose(version_release);
        // Ubuntu
        } else if (version_release = fopen("/etc/lsb-release","r"), version_release){
            name = strdup("Ubuntu");
            id = strdup("ubuntu");
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                tag = strtok(buff, "=");
                if (strcmp(tag,"DISTRIB_RELEASE") == 0){
                    version = strdup(strtok(NULL, "\n"));
                    break;
                }
            }

            fclose(version_release);
        // Gentoo
        } else if (version_release = fopen("/etc/gentoo-release","r"), version_release){
            name = strdup("Gentoo");
            id = strdup("gentoo");
            static const char *pattern = " ([0-9][0-9]*\\.[0-9][0-9]*)\\.*";
            if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
                merror_exit("Can not compile regular expression.");
            }
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                if(regexec(&regexCompiled, buff, 2, match, 0) == 0){
                    match_size = match[1].rm_eo - match[1].rm_so;
                    version = malloc(match_size +1);
                    snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    break;
                }
            }
            regfree(&regexCompiled);
            fclose(version_release);
        // SuSE
        } else if (version_release = fopen("/etc/SuSE-release","r"), version_release){
            name = strdup("SuSE Linux");
            id = strdup("suse");
            static const char *pattern = ".*VERSION = ([0-9][0-9]*)";
            if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
                merror_exit("Can not compile regular expression.");
            }
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                if(regexec(&regexCompiled, buff, 2, match, 0) == 0){
                    match_size = match[1].rm_eo - match[1].rm_so;
                    version = malloc(match_size +1);
                    snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    break;
                }
            }
            regfree(&regexCompiled);
            fclose(version_release);
        // Arch
        } else if (version_release = fopen("/etc/arch-release","r"), version_release){
            name = strdup("Arch Linux");
            id = strdup("arch");
            static const char *pattern = "([0-9][0-9]*\\.[0-9][0-9]*)\\.*";
            if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
                merror_exit("Can not compile regular expression.");
            }
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                if(regexec(&regexCompiled, buff, 2, match, 0) == 0){
                    match_size = match[1].rm_eo - match[1].rm_so;
                    version = malloc(match_size +1);
                    snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    break;
                }
            }
            regfree(&regexCompiled);
            fclose(version_release);
        // Debian
        } else if (version_release = fopen("/etc/debian_version","r"), version_release){
            name = strdup("Debian GNU/Linux");
            id = strdup("debian");
            static const char *pattern = "([0-9][0-9]*\\.[0-9][0-9]*)\\.*";
            if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
                merror_exit("Can not compile regular expression.");
            }
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                if(regexec(&regexCompiled, buff, 2, match, 0) == 0){
                    match_size = match[1].rm_eo - match[1].rm_so;
                    version = malloc(match_size +1);
                    snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    break;
                }
            }
            regfree(&regexCompiled);
            fclose(version_release);
        // Slackware
        } else if (version_release = fopen("/etc/slackware-version","r"), version_release){
            name = strdup("Slackware");
            id = strdup("slackware");
            static const char *pattern = " ([0-9][0-9]*\\.[0-9][0-9]*)\\.*";
            if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
                merror_exit("Can not compile regular expression.");
            }
            while (fgets(buff, sizeof(buff) - 1, version_release)) {
                if(regexec(&regexCompiled, buff, 2, match, 0) == 0){
                    match_size = match[1].rm_eo - match[1].rm_so;
                    version = malloc(match_size +1);
                    snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    break;
                }
            }
            regfree(&regexCompiled);
            fclose(version_release);
        } else if (cmd_output = popen("uname", "r"), cmd_output) {
            if(fgets(buff,sizeof(buff) - 1, cmd_output) == NULL){
                mdebug1("Can not read from command output (uname).");
            } else if (strcmp(strtok(buff, "\n"),"Darwin") == 0){ // Mac OS
                name = strdup("Darwin");
                id = strdup("darwin");
                if (cmd_output_ver = popen("uname -r", "r"), cmd_output_ver) {
                    if(fgets(buff, sizeof(buff) - 1, cmd_output_ver) == NULL){
                        mdebug1("Can not read from command output (uname -r).");
                    } else if (w_regexec("([0-9][0-9]*\\.[0-9][0-9]*)\\.*", buff, 2, match)){
                        match_size = match[1].rm_eo - match[1].rm_so;
                        version = malloc(match_size +1);
                        snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    }
                    pclose(cmd_output_ver);
                }
            } else if (strcmp(strtok(buff, "\n"),"SunOS") == 0){ // Sun OS
                name = strdup("SunOS");
                id = strdup("sunos");
                if (cmd_output_ver = popen("uname -r", "r"), cmd_output_ver) {
                    if(fgets(buff, sizeof(buff) - 1, cmd_output_ver) == NULL){
                        mdebug1("Can not read from command output (uname -r).");
                    } else if (w_regexec("([0-9][0-9]*\\.[0-9][0-9]*)\\.*", buff, 2, match)){
                        match_size = match[1].rm_eo - match[1].rm_so;
                        version = malloc(match_size +1);
                        snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    }
                    pclose(cmd_output_ver);
                }
            } else if (strcmp(strtok(buff, "\n"),"HP-UX") == 0){ // HP-UX
                name = strdup("HP-UX");
                id = strdup("hpux");
                if (cmd_output_ver = popen("uname -r", "r"), cmd_output_ver) {
                    if(fgets(buff, sizeof(buff) - 1, cmd_output_ver) == NULL){
                        mdebug1("Can not read from command output (uname -r).");
                    } else if (w_regexec("([0-9][0-9]*\\.[0-9][0-9]*)\\.*", buff, 2, match)){
                        match_size = match[1].rm_eo - match[1].rm_so;
                        version = malloc(match_size +1);
                        snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    }
                    pclose(cmd_output_ver);
                }
            } else if (strcmp(strtok(buff, "\n"),"OpenBSD") == 0 ||
                       strcmp(strtok(buff, "\n"),"NetBSD")  == 0 ||
                       strcmp(strtok(buff, "\n"),"FreeBSD") == 0 ){ // BSD
                name = strdup("BSD");
                id = strdup("bsd");
                if (cmd_output_ver = popen("uname -r", "r"), cmd_output_ver) {
                    if(fgets(buff, sizeof(buff) - 1, cmd_output_ver) == NULL){
                        mdebug1("Can not read from command output (uname -r).");
                    } else if (w_regexec("([0-9][0-9]*\\.[0-9][0-9]*)\\.*", buff, 2, match)){
                        match_size = match[1].rm_eo - match[1].rm_so;
                        version = malloc(match_size +1);
                        snprintf (version, match_size +1, "%.*s", match_size, buff + match[1].rm_so);
                    }

                    pclose(cmd_output_ver);
                }
            } else if (strcmp(strtok(buff, "\n"),"Linux") == 0){ // Linux undefined
                name = strdup("Linux");
                id = strdup("linux");
            }
            pclose(cmd_output);
        }
    }

    if (!name)
        name = strdup("\0");
    if (!version)
        version = strdup("\0");
    if (!id)
        id = strdup("\0");

    if (snprintf (string, 255, "%s|%s: %s", name, id, version) >= 255) {
        merror("Getting UNIX version: string too large.");
    }

    free(name);
    free(version);
    free(id);

    return string;
}

/* Get uname. Memory must be freed after use */
const char *getuname()
{
    struct utsname uts_buf;
    const char *os_version;
    static char muname[512] = "";

    if (!muname[0]){
        if (uname(&uts_buf) >= 0) {
            if (os_version = get_unix_version(), os_version){
                snprintf(muname, 512, "%s %s %s %s %s [%s] - %s %s",
                         uts_buf.sysname,
                         uts_buf.nodename,
                         uts_buf.release,
                         uts_buf.version,
                         uts_buf.machine,
                         os_version,
                         __ossec_name, __ossec_version);
            }
            else {
                snprintf(muname, 512, "%s %s %s %s %s - %s %s",
                         uts_buf.sysname,
                         uts_buf.nodename,
                         uts_buf.release,
                         uts_buf.version,
                         uts_buf.machine,
                         __ossec_name, __ossec_version);
            }
        } else {
            snprintf(muname, 512, "No system info available -  %s %s",
                     __ossec_name, __ossec_version);
        }
    }

    return muname;
}

/* Daemonize a process without closing stdin/stdout/stderr */
void goDaemonLight()
{
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        merror(FORK_ERROR, errno, strerror(errno));
        return;
    } else if (pid) {
        exit(0);
    }

    /* Become session leader */
    if (setsid() < 0) {
        merror(SETSID_ERROR, errno, strerror(errno));
        return;
    }

    /* Fork again */
    pid = fork();
    if (pid < 0) {
        merror(FORK_ERROR, errno, strerror(errno));
        return;
    } else if (pid) {
        exit(0);
    }

    dup2(1, 2);

    /* Go to / */
    if (chdir("/") == -1) {
        merror(CHDIR_ERROR, "/", errno, strerror(errno));
    }

    nowDaemon();
}

/* Daemonize a process */
void goDaemon()
{
    int fd;
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        merror(FORK_ERROR, errno, strerror(errno));
        return;
    } else if (pid) {
        exit(0);
    }

    /* Become session leader */
    if (setsid() < 0) {
        merror(SETSID_ERROR, errno, strerror(errno));
        return;
    }

    /* Fork again */
    pid = fork();
    if (pid < 0) {
        merror(FORK_ERROR, errno, strerror(errno));
        return;
    } else if (pid) {
        exit(0);
    }

    /* Dup stdin, stdout and stderr to /dev/null */
    if ((fd = open("/dev/null", O_RDWR)) >= 0) {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);

        close(fd);
    }

    /* Go to / */
    if (chdir("/") == -1) {
        merror(CHDIR_ERROR, "/", errno, strerror(errno));
    }

    nowDaemon();
}

#else /* WIN32 */

int checkVista()
{
    const char *m_uname;
    isVista = 0;

    m_uname = getuname();
    if (!m_uname) {
        merror(MEM_ERROR, errno, strerror(errno));
        return (0);
    }

    /* Check if the system is Vista (must be called during the startup) */
    if (strstr(m_uname, "Windows Server 2008") ||
            strstr(m_uname, "Vista") ||
            strstr(m_uname, "Windows 7") ||
            strstr(m_uname, "Windows 8") ||
            strstr(m_uname, "Windows 10") ||
            strstr(m_uname, "Windows Server 2012") ||
            strstr(m_uname, "Windows Server 2016")) {
        isVista = 1;
        minfo("System is Vista or newer (%s).", m_uname);
    } else {
        minfo("System is older than Vista (%s).", m_uname);
    }

    return (isVista);
}

/* Get basename of path */
char *basename_ex(char *path)
{
    return (PathFindFileNameA(path));
}

/* Rename file or directory */
int rename_ex(const char *source, const char *destination)
{
    if (!MoveFileEx(source, destination, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        mferror("Could not move (%s) to (%s) which returned (%lu)", source, destination, GetLastError());

        return (-1);
    }

    return (0);
}

/* Create a temporary file */
int mkstemp_ex(char *tmp_path)
{
    DWORD dwResult;
    int result;
    int status = -1;

    HANDLE h = NULL;
    PACL pACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    EXPLICIT_ACCESS ea[2];
    SECURITY_ATTRIBUTES sa;

    PSID pAdminGroupSID = NULL;
    PSID pSystemGroupSID = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthNT = {SECURITY_NT_AUTHORITY};

#if defined(_MSC_VER) && _MSC_VER >= 1500
    result = _mktemp_s(tmp_path, strlen(tmp_path) + 1);

    if (result != 0) {
        mferror("Could not create temporary file (%s) which returned (%d)", tmp_path, result);

        return (-1);
    }
#else
    if (_mktemp(tmp_path) == NULL) {
        mferror("Could not create temporary file (%s) which returned [(%d)-(%s)]", tmp_path, errno, strerror(errno));

        return (-1);
    }
#endif

    /* Create SID for the BUILTIN\Administrators group */
    result = AllocateAndInitializeSid(
                 &SIDAuthNT,
                 2,
                 SECURITY_BUILTIN_DOMAIN_RID,
                 DOMAIN_ALIAS_RID_ADMINS,
                 0, 0, 0, 0, 0, 0,
                 &pAdminGroupSID
             );

    if (!result) {
        mferror("Could not create BUILTIN\\Administrators group SID which returned (%lu)", GetLastError());

        goto cleanup;
    }

    /* Create SID for the SYSTEM group */
    result = AllocateAndInitializeSid(
                 &SIDAuthNT,
                 1,
                 SECURITY_LOCAL_SYSTEM_RID,
                 0, 0, 0, 0, 0, 0, 0,
                 &pSystemGroupSID
             );

    if (!result) {
        mferror("Could not create SYSTEM group SID which returned (%lu)", GetLastError());

        goto cleanup;
    }

    /* Initialize an EXPLICIT_ACCESS structure for an ACE */
    ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));

    /* Add Administrators group */
    ea[0].grfAccessPermissions = GENERIC_ALL;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (LPTSTR)pAdminGroupSID;

    /* Add SYSTEM group */
    ea[1].grfAccessPermissions = GENERIC_ALL;
    ea[1].grfAccessMode = SET_ACCESS;
    ea[1].grfInheritance = NO_INHERITANCE;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[1].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[1].Trustee.ptstrName = (LPTSTR)pSystemGroupSID;

    /* Set entries in ACL */
    dwResult = SetEntriesInAcl(2, ea, NULL, &pACL);

    if (dwResult != ERROR_SUCCESS) {
        mferror("Could not set ACL entries which returned (%lu)", dwResult);

        goto cleanup;
    }

    /* Initialize security descriptor */
    pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(
              LPTR,
              SECURITY_DESCRIPTOR_MIN_LENGTH
          );

    if (pSD == NULL) {
        mferror("Could not initialize SECURITY_DESCRIPTOR because of a LocalAlloc() failure which returned (%lu)", GetLastError());

        goto cleanup;
    }

    if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) {
        mferror("Could not initialize SECURITY_DESCRIPTOR because of an InitializeSecurityDescriptor() failure which returned (%lu)", GetLastError());

        goto cleanup;
    }

    /* Set owner */
    if (!SetSecurityDescriptorOwner(pSD, NULL, FALSE)) {
        mferror("Could not set owner which returned (%lu)", GetLastError());

        goto cleanup;
    }

    /* Set group owner */
    if (!SetSecurityDescriptorGroup(pSD, NULL, FALSE)) {
        mferror("Could not set group owner which returned (%lu)", GetLastError());

        goto cleanup;
    }

    /* Add ACL to security descriptor */
    if (!SetSecurityDescriptorDacl(pSD, TRUE, pACL, FALSE)) {
        mferror("Could not set SECURITY_DESCRIPTOR DACL which returned (%lu)", GetLastError());

        goto cleanup;
    }

    /* Initialize security attributes structure */
    sa.nLength = sizeof (SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = pSD;
    sa.bInheritHandle = FALSE;

    h = CreateFileA(
            tmp_path,
            GENERIC_WRITE,
            0,
            &sa,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

    if (h == INVALID_HANDLE_VALUE) {
        mferror("Could not create temporary file (%s) which returned (%lu)", tmp_path, GetLastError());

        goto cleanup;
    }

    if (!CloseHandle(h)) {
        mferror("Could not close file handle to (%s) which returned (%lu)", tmp_path, GetLastError());

        goto cleanup;
    }

    /* Success */
    status = 0;

cleanup:
    if (pAdminGroupSID) {
        FreeSid(pAdminGroupSID);
    }

    if (pSystemGroupSID) {
        FreeSid(pSystemGroupSID);
    }

    if (pACL) {
        LocalFree(pACL);
    }

    if (pSD) {
        LocalFree(pSD);
    }

    return (status);
}

/* Get uname for Windows */
const char *getuname()
{
    int ret_size = OS_SIZE_1024 - 2;
    static char ret[OS_SIZE_1024 + 1] = "";
    char os_v[128 + 1];
    FILE *cmd_output;
    char *command;
    size_t buf_tam = 100;
    char read_buff[buf_tam];
    int add_infoEx = 1;

    typedef void (WINAPI * PGNSI)(LPSYSTEM_INFO);
    typedef BOOL (WINAPI * PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

    /* See http://msdn.microsoft.com/en-us/library/windows/desktop/ms724429%28v=vs.85%29.aspx */
    OSVERSIONINFOEX osvi;
    SYSTEM_INFO si;
    PGNSI pGNSI;
    PGPI pGPI;
    BOOL bOsVersionInfoEx;
    DWORD dwType;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (!(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi))) {
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx((OSVERSIONINFO *)&osvi)) {
            return (NULL);
        }
    }

    if (ret[0] != '\0') {
        return ret;
    }

    switch (osvi.dwPlatformId) {
        /* Test for the Windows NT product family */
        case VER_PLATFORM_WIN32_NT:
            if (osvi.dwMajorVersion == 6 && (osvi.dwMinorVersion == 0 || osvi.dwMinorVersion == 1) ) {
                if (osvi.dwMinorVersion == 0) {
                    if (osvi.wProductType == VER_NT_WORKSTATION ) {
                        strncat(ret, "Microsoft Windows Vista ", ret_size - 1);
                    } else {
                        strncat(ret, "Microsoft Windows Server 2008 ", ret_size - 1);
                    }
                } else if (osvi.dwMinorVersion == 1) {
                    if (osvi.wProductType == VER_NT_WORKSTATION ) {
                        strncat(ret, "Microsoft Windows 7 ", ret_size - 1);
                    } else {
                        strncat(ret, "Microsoft Windows Server 2008 R2 ", ret_size - 1);
                    }
                }

                ret_size -= strlen(ret) + 1;


                /* Get product version */
                pGPI = (PGPI) GetProcAddress(
                              GetModuleHandle(TEXT("kernel32.dll")),
                              "GetProductInfo");

                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
                    pGPI( 6, 0, 0, 0, &dwType);
                else
                    pGPI( 6, 1, 0, 0, &dwType);

                switch (dwType) {
                    case PRODUCT_UNLICENSED:
                        strncat(ret, PRODUCT_UNLICENSED_C, ret_size - 1);
                        break;
                    case PRODUCT_BUSINESS:
                        strncat(ret, PRODUCT_BUSINESS_C, ret_size - 1);
                        break;
                    case PRODUCT_BUSINESS_N:
                        strncat(ret, PRODUCT_BUSINESS_N_C, ret_size - 1);
                        break;
                    case PRODUCT_CLUSTER_SERVER:
                        strncat(ret, PRODUCT_CLUSTER_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_DATACENTER_SERVER:
                        strncat(ret, PRODUCT_DATACENTER_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_DATACENTER_SERVER_CORE:
                        strncat(ret, PRODUCT_DATACENTER_SERVER_CORE_C, ret_size - 1);
                        break;
                    case PRODUCT_DATACENTER_SERVER_CORE_V:
                        strncat(ret, PRODUCT_DATACENTER_SERVER_CORE_V_C, ret_size - 1);
                        break;
                    case PRODUCT_DATACENTER_SERVER_V:
                        strncat(ret, PRODUCT_DATACENTER_SERVER_V_C, ret_size - 1);
                        break;
                    case PRODUCT_ENTERPRISE:
                        strncat(ret, PRODUCT_ENTERPRISE_C, ret_size - 1);
                        break;
                    case PRODUCT_ENTERPRISE_N:
                        strncat(ret, PRODUCT_ENTERPRISE_N_C, ret_size - 1);
                        break;
                    case PRODUCT_ENTERPRISE_SERVER:
                        strncat(ret, PRODUCT_ENTERPRISE_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_ENTERPRISE_SERVER_CORE:
                        strncat(ret, PRODUCT_ENTERPRISE_SERVER_CORE_C, ret_size - 1);
                        break;
                    case PRODUCT_ENTERPRISE_SERVER_CORE_V:
                        strncat(ret, PRODUCT_ENTERPRISE_SERVER_CORE_V_C, ret_size - 1);
                        break;
                    case PRODUCT_ENTERPRISE_SERVER_IA64:
                        strncat(ret, PRODUCT_ENTERPRISE_SERVER_IA64_C, ret_size - 1);
                        break;
                    case PRODUCT_ENTERPRISE_SERVER_V:
                        strncat(ret, PRODUCT_ENTERPRISE_SERVER_V_C, ret_size - 1);
                        break;
                    case PRODUCT_HOME_BASIC:
                        strncat(ret, PRODUCT_HOME_BASIC_C, ret_size - 1);
                        break;
                    case PRODUCT_HOME_BASIC_N:
                        strncat(ret, PRODUCT_HOME_BASIC_N_C, ret_size - 1);
                        break;
                    case PRODUCT_HOME_PREMIUM:
                        strncat(ret, PRODUCT_HOME_PREMIUM_C, ret_size - 1);
                        break;
                    case PRODUCT_HOME_PREMIUM_N:
                        strncat(ret, PRODUCT_HOME_PREMIUM_N_C, ret_size - 1);
                        break;
                    case PRODUCT_HOME_SERVER:
                        strncat(ret, PRODUCT_HOME_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT:
                        strncat(ret, PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT_C, ret_size - 1);
                        break;
                    case PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING:
                        strncat(ret, PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING_C, ret_size - 1);
                        break;
                    case PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY:
                        strncat(ret, PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY_C, ret_size - 1);
                        break;
                    case PRODUCT_SERVER_FOR_SMALLBUSINESS:
                        strncat(ret, PRODUCT_SERVER_FOR_SMALLBUSINESS_C, ret_size - 1);
                        break;
                    case PRODUCT_SMALLBUSINESS_SERVER:
                        strncat(ret, PRODUCT_SMALLBUSINESS_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
                        strncat(ret, PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_C, ret_size - 1);
                        break;
                    case PRODUCT_STANDARD_SERVER:
                        strncat(ret, PRODUCT_STANDARD_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_STANDARD_SERVER_CORE:
                        strncat(ret, PRODUCT_STANDARD_SERVER_CORE_C, ret_size - 1);
                        break;
                    case PRODUCT_STANDARD_SERVER_CORE_V:
                        strncat(ret, PRODUCT_STANDARD_SERVER_CORE_V_C, ret_size - 1);
                        break;
                    case PRODUCT_STANDARD_SERVER_V:
                        strncat(ret, PRODUCT_STANDARD_SERVER_V_C, ret_size - 1);
                        break;
                    case PRODUCT_STARTER:
                        strncat(ret, PRODUCT_STARTER_C, ret_size - 1);
                        break;
                    case PRODUCT_STORAGE_ENTERPRISE_SERVER:
                        strncat(ret, PRODUCT_STORAGE_ENTERPRISE_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_STORAGE_EXPRESS_SERVER:
                        strncat(ret, PRODUCT_STORAGE_EXPRESS_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_STORAGE_STANDARD_SERVER:
                        strncat(ret, PRODUCT_STORAGE_STANDARD_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_STORAGE_WORKGROUP_SERVER:
                        strncat(ret, PRODUCT_STORAGE_WORKGROUP_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_ULTIMATE:
                        strncat(ret, PRODUCT_ULTIMATE_C, ret_size - 1);
                        break;
                    case PRODUCT_ULTIMATE_N:
                        strncat(ret, PRODUCT_ULTIMATE_N_C, ret_size - 1);
                        break;
                    case PRODUCT_WEB_SERVER:
                        strncat(ret, PRODUCT_WEB_SERVER_C, ret_size - 1);
                        break;
                    case PRODUCT_WEB_SERVER_CORE:
                        strncat(ret, PRODUCT_WEB_SERVER_CORE_C, ret_size - 1);
                        break;

                }
                ret_size -= strlen(ret) + 1;
            } else if (osvi.dwMajorVersion == 6 && (osvi.dwMinorVersion == 2 || osvi.dwMinorVersion == 3)) {
                command = "wmic os get caption";
                char *end;
                cmd_output = popen(command, "r");
                if (!cmd_output) {
                    merror("Unable to execute command: '%s'.", command);
                }
                if (strncmp(fgets(read_buff, buf_tam, cmd_output),"Caption",7) == 0) {
                    if (!fgets(read_buff, buf_tam, cmd_output)){
                        merror("Can't get Version.");
                        strncat(ret, "Microsoft Windows unknown version ", ret_size - 1);
                    }
                    else if (end = strpbrk(read_buff,"\r\n"), end) {
                        *end = '\0';
                        int i = strlen(read_buff) - 1;
                        while(read_buff[i] == 32){
                            read_buff[i] = '\0';
                            i--;
                        }
                        strncat(ret, read_buff, ret_size - 1);
                    }else
                        strncat(ret, "Microsoft Windows unknown version ", ret_size - 1);
                }

                pclose(cmd_output);
                add_infoEx = 0;
                ret_size -= strlen(ret) + 1;

            } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
                pGNSI = (PGNSI) GetProcAddress(
                            GetModuleHandle("kernel32.dll"),
                            "GetNativeSystemInfo");
                if (NULL != pGNSI) {
                    pGNSI(&si);
                }

                if ( GetSystemMetrics(89) )
                    strncat(ret, "Microsoft Windows Server 2003 R2 ",
                            ret_size - 1);
                else if (osvi.wProductType == VER_NT_WORKSTATION &&
                         si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
                    strncat(ret,
                            "Microsoft Windows XP Professional x64 Edition ",
                            ret_size - 1 );
                } else {
                    strncat(ret, "Microsoft Windows Server 2003, ", ret_size - 1);
                }

                ret_size -= strlen(ret) + 1;
            } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {
                strncat(ret, "Microsoft Windows XP ", ret_size - 1);

                ret_size -= strlen(ret) + 1;
            } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
                strncat(ret, "Microsoft Windows 2000 ", ret_size - 1);

                ret_size -= strlen(ret) + 1;
            } else if (osvi.dwMajorVersion <= 4) {
                strncat(ret, "Microsoft Windows NT ", ret_size - 1);

                ret_size -= strlen(ret) + 1;
            } else {
                strncat(ret, "Microsoft Windows Unknown ", ret_size - 1);

                ret_size -= strlen(ret) + 1;
            }

            /* Test for specific product on Windows NT 4.0 SP6 and later */
            if (add_infoEx){
                if (bOsVersionInfoEx) {
                    /* Test for the workstation type */
                    if (osvi.wProductType == VER_NT_WORKSTATION &&
                            si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64) {
                        if ( osvi.dwMajorVersion == 4 ) {
                            strncat(ret, "Workstation 4.0 ", ret_size - 1);
                        } else if ( osvi.wSuiteMask & VER_SUITE_PERSONAL ) {
                            strncat(ret, "Home Edition ", ret_size - 1);
                        } else {
                            strncat(ret, "Professional ", ret_size - 1);
                        }

                        /* Fix size */
                        ret_size -= strlen(ret) + 1;
                    }

                    /* Test for the server type */
                    else if ( osvi.wProductType == VER_NT_SERVER ||
                              osvi.wProductType == VER_NT_DOMAIN_CONTROLLER ) {
                        if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
                            if (si.wProcessorArchitecture ==
                                    PROCESSOR_ARCHITECTURE_IA64 ) {
                                if ( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                                    strncat(ret,
                                            "Datacenter Edition for Itanium-based Systems ",
                                            ret_size - 1);
                                else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                                    strncat(ret,
                                            "Enterprise Edition for Itanium-based Systems ",
                                            ret_size - 1);

                                ret_size -= strlen(ret) + 1;
                            } else if ( si.wProcessorArchitecture ==
                                        PROCESSOR_ARCHITECTURE_AMD64 ) {
                                if ( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                                    strncat(ret, "Datacenter x64 Edition ",
                                            ret_size - 1 );
                                else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                                    strncat(ret, "Enterprise x64 Edition ",
                                            ret_size - 1 );
                                else
                                    strncat(ret, "Standard x64 Edition ",
                                            ret_size - 1 );

                                ret_size -= strlen(ret) + 1;
                            } else {
                                if ( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                                    strncat(ret, "Datacenter Edition ",
                                            ret_size - 1 );
                                else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE ) {
                                    strncat(ret, "Enterprise Edition ", ret_size - 1);
                                } else if ( osvi.wSuiteMask == VER_SUITE_BLADE ) {
                                    strncat(ret, "Web Edition ", ret_size - 1 );
                                } else {
                                    strncat(ret, "Standard Edition ", ret_size - 1);
                                }

                                ret_size -= strlen(ret) + 1;
                            }
                        } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
                            if ( osvi.wSuiteMask & VER_SUITE_DATACENTER ) {
                                strncat(ret, "Datacenter Server ", ret_size - 1);
                            } else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE ) {
                                strncat(ret, "Advanced Server ", ret_size - 1 );
                            } else {
                                strncat(ret, "Server ", ret_size - 1);
                            }

                            ret_size -= strlen(ret) + 1;
                        } else if (osvi.dwMajorVersion <= 4) { /* Windows NT 4.0 */
                            if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                                strncat(ret, "Server 4.0, Enterprise Edition ",
                                        ret_size - 1 );
                            else {
                                strncat(ret, "Server 4.0 ", ret_size - 1);
                            }

                            ret_size -= strlen(ret) + 1;
                        }
                    }
                }
                /* Test for specific product on Windows NT 4.0 SP5 and earlier */
                else {
                    HKEY hKey;
                    char szProductType[81];
                    DWORD dwBufLen = 80;
                    LONG lRet;

                    lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                         "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
                                         0, KEY_QUERY_VALUE, &hKey );
                    if (lRet == ERROR_SUCCESS) {
                        char __wv[32];

                        lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
                                                (LPBYTE) szProductType, &dwBufLen);
                        RegCloseKey( hKey );

                        if ((lRet == ERROR_SUCCESS) && (dwBufLen < 80) ) {
                            if (lstrcmpi( "WINNT", szProductType) == 0 ) {
                                strncat(ret, "Workstation ", ret_size - 1);
                            } else if (lstrcmpi( "LANMANNT", szProductType) == 0 ) {
                                strncat(ret, "Server ", ret_size - 1);
                            } else if (lstrcmpi( "SERVERNT", szProductType) == 0 ) {
                                strncat(ret, "Advanced Server " , ret_size - 1);
                            }

                            ret_size -= strlen(ret) + 1;

                            memset(__wv, '\0', 32);
                            snprintf(__wv, 31,
                                     "%d.%d ",
                                     (int)osvi.dwMajorVersion,
                                     (int)osvi.dwMinorVersion);

                            strncat(ret, __wv, ret_size - 1);
                            ret_size -= strlen(__wv) + 1;
                        }
                    }
                }
            }
            /* Display service pack (if any) and build number */
            if ( osvi.dwMajorVersion == 4 &&
                    lstrcmpi( osvi.szCSDVersion, "Service Pack 6" ) == 0 ) {
                HKEY hKey;
                LONG lRet;
                char __wp[64];

                memset(__wp, '\0', 64);
                /* Test for SP6 versus SP6a */
                lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                     "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
                                     0, KEY_QUERY_VALUE, &hKey );
                if ( lRet == ERROR_SUCCESS )
                    snprintf(__wp, 63, "Service Pack 6a [Ver: %i.%i.%d]",
                             (int)osvi.dwMajorVersion,
                             (int)osvi.dwMinorVersion,
                             (int)osvi.dwBuildNumber & 0xFFFF );
                else { /* Windows NT 4.0 prior to SP6a */
                    snprintf(__wp, 63, "%s [Ver: %i.%i.%d]",
                             osvi.szCSDVersion,
                             (int)osvi.dwMajorVersion,
                             (int)osvi.dwMinorVersion,
                             (int)osvi.dwBuildNumber & 0xFFFF );
                }

                strncat(ret, __wp, ret_size - 1);
                ret_size -= strlen(__wp) + 1;
                RegCloseKey( hKey );
            } else if (osvi.dwMajorVersion == 6 && (osvi.dwMinorVersion == 2 || osvi.dwMinorVersion == 3)) {
                char __wp[64];
                memset(__wp, '\0', 64);
                command = "wmic os get Version";
                cmd_output = popen(command, "r");
                if (!cmd_output) {
                    merror("Unable to execute command: '%s'.", command);
                    snprintf(__wp, 63, " [Ver: %s]", "desc");
                }
                if (strncmp(fgets(read_buff, buf_tam, cmd_output),"Version",7) == 0) {
                    if (!fgets(read_buff, buf_tam, cmd_output)){
                        merror("Can't get Version.");
                        snprintf(__wp, 63, " [Ver: %s]", "desc");
                    }
                    else {
                        snprintf(__wp, 63, " [Ver: %s]", strtok(read_buff," "));
                    }
                }

                pclose(cmd_output);

                strncat(ret, __wp, ret_size - 1);
                ret_size -= strlen(__wp) + 1;

            } else {
                char __wp[64];

                memset(__wp, '\0', 64);

                snprintf(__wp, 63, "%s [Ver: %i.%i.%d]",
                         osvi.szCSDVersion,
                         (int)osvi.dwMajorVersion,
                         (int)osvi.dwMinorVersion,
                         (int)osvi.dwBuildNumber & 0xFFFF );

                strncat(ret, __wp, ret_size - 1);
                ret_size -= strlen(__wp) + 1;
            }
            break;

        /* Test for Windows Me/98/95 */
        case VER_PLATFORM_WIN32_WINDOWS:
            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
                strncat(ret, "Microsoft Windows 95 ", ret_size - 1);
                ret_size -= strlen(ret) + 1;
            }

            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
                strncat(ret, "Microsoft Windows 98 ", ret_size - 1);
                ret_size -= strlen(ret) + 1;
            }

            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
                strncat(ret, "Microsoft Windows Millennium Edition",
                        ret_size - 1);

                ret_size -= strlen(ret) + 1;
            }
            break;

        case VER_PLATFORM_WIN32s:
            strncat(ret, "Microsoft Win32s", ret_size - 1);
            ret_size -= strlen(ret) + 1;
            break;
    }

    /* Add OSSEC-HIDS version */
    snprintf(os_v, 128, " - %s %s", __ossec_name, __ossec_version);
    strncat(ret, os_v, ret_size - 1);

    return (ret);
}


#endif /* WIN32 */

// Delete directory recursively

int rmdir_ex(const char *name) {
    if (rmdir(name) == 0) {
        return 0;
    }

    switch (errno) {
    case ENOTDIR:   // Not a directory

#ifdef WIN32
    case EINVAL:    // Not a directory
#endif
        return unlink(name);

    case ENOTEMPTY: // Directory not empty
        // Erase content and try to erase again
        return cldir_ex(name) || rmdir(name) ? -1 : 0;

    default:
        return -1;
    }
}

// Delete directory content

int cldir_ex(const char *name) {
    return cldir_ex_ignore(name, NULL);
}

// Delete directory content with exception list

int cldir_ex_ignore(const char * name, const char ** ignore) {
    DIR *dir;
    struct dirent *dirent;
    char path[PATH_MAX + 1];

    // Erase content

    dir = opendir(name);

    if (!dir) {
        return -1;
    }

    while (dirent = readdir(dir), dirent) {
        // Skip "." and ".."
        if ((dirent->d_name[0] == '.' && (dirent->d_name[1] == '\0' || (dirent->d_name[1] == '.' && dirent->d_name[2] == '\0'))) || w_str_in_array(dirent->d_name, ignore)) {
            continue;
        }

        if (snprintf(path, PATH_MAX + 1, "%s/%s", name, dirent->d_name) > PATH_MAX) {
            closedir(dir);
            return -1;
        }

        if (rmdir_ex(path) < 0) {
            closedir(dir);
            return -1;
        }
    }

    return closedir(dir);
}

int TempFile(File *file, const char *source, int copy) {
    FILE *fp_src;
    int fd;
    char template[OS_FLSIZE + 1];
    mode_t old_mask;

    snprintf(template, OS_FLSIZE, "%s.XXXXXX", source);
    old_mask = umask(0177);

    fd = mkstemp(template);
    umask(old_mask);

    if (fd < 0) {
        return -1;
    }

#ifndef WIN32
    if (fchmod(fd, 0640) < 0) {
        close(fd);
        unlink(template);
        return -1;
    }
#endif

    file->fp = fdopen(fd, "w");

    if (!file->fp) {
        close(fd);
        unlink(template);
        return -1;
    }

    if (copy) {
        size_t count_r;
        size_t count_w;
        char buffer[4096];

        fp_src = fopen(source, "r");

        if (!fp_src) {
            file->name = strdup(template);
            return 0;
        }

        while (!feof(fp_src)) {
            count_r = fread(buffer, 1, 4096, fp_src);

            if (ferror(fp_src)) {
                fclose(fp_src);
                fclose(file->fp);
                unlink(template);
                return -1;
            }

            count_w = fwrite(buffer, 1, count_r, file->fp);

            if (count_w != count_r || ferror(file->fp)) {
                fclose(fp_src);
                fclose(file->fp);
                unlink(template);
                return -1;
            }
        }

        fclose(fp_src);
    }

    file->name = strdup(template);
    return 0;
}

int OS_MoveFile(const char *src, const char *dst) {
    FILE *fp_src;
    FILE *fp_dst;
    size_t count_r;
    size_t count_w;
    char buffer[4096];
    int status = 0;

    if (rename(src, dst) == 0) {
        return 0;
    }

    mdebug1("Couldn't rename %s: %s", dst, strerror(errno));

    fp_src = fopen(src, "r");

    if (!fp_src) {
        merror("Couldn't open file '%s'", src);
        return -1;
    }

    fp_dst = fopen(dst, "w");

    if (!fp_dst) {
        merror("Couldn't open file '%s'", dst);
        fclose(fp_src);
        unlink(src);
        return -1;
    }

    while (!feof(fp_src)) {
        count_r = fread(buffer, 1, 4096, fp_src);

        if (ferror(fp_src)) {
            merror("Couldn't read file '%s'", src);
            status = -1;
            break;
        }

        count_w = fwrite(buffer, 1, count_r, fp_dst);

        if (count_w != count_r || ferror(fp_dst)) {
            merror("Couldn't write file '%s'", dst);
            status = -1;
            break;
        }
    }

    fclose(fp_src);
    fclose(fp_dst);
    unlink(dst);
    return status;
}

// Make directory recursively
int mkdir_ex(const char * path) {
    char sep;
    char * temp = strdup(path);
    char * psep;
    char * next;

#ifndef WIN32
    for (next = temp; psep = strchr(next, '/'), psep; next = psep + 1) {
#else
    for (next = temp; psep = strchr(next, '/'), psep || (psep = strchr(next, '\\'), psep); next = psep + 1) {
#endif

        sep = *psep;
        *psep = '\0';

        if (*temp && mkdir(temp, 0770) < 0) {
            switch (errno) {
            case EEXIST:
                if (IsDir(temp) < 0) {
                    merror("Couldn't make dir '%s': not a directory.", temp);
                    free(temp);
                    return -1;
                }

                break;

            case EISDIR:
                break;

            default:
                merror("Couldn't make dir '%s': %s", temp, strerror(errno));
                free(temp);
                return -1;
            }
        }

        *psep = sep;
    }

    free(temp);

    if (mkdir(path, 0770) < 0) {
        switch (errno) {
        case EEXIST:
            if (IsDir(path) < 0) {
                merror("Couldn't make dir '%s': not a directory.", path);
                return -1;
            }

            break;

        case EISDIR:
            break;

        default:
            merror("Couldn't make dir '%s': %s", path, strerror(errno));
            return -1;
        }
    }

    return 0;
}

int w_ref_parent_folder(const char * path) {
    const char * str;
    char * ptr;

    switch (path[0]) {
    case '\0':
        return 0;

    case '.':
        switch (path[1]) {
        case '\0':
            return 0;

        case '.':
            switch (path[2]) {
            case '\0':
                return 1;

            case '/':
#ifdef WIN32
            case '\\':
#endif
                return 1;
            }
        }
    }

#ifdef WIN32
    for (str = path; ptr = strstr(str, "/.."), ptr || (ptr = strstr(str, "\\.."), ptr); str = ptr + 3) {
        if (ptr[3] == '\0' || ptr[3] == '/' || ptr[3] == '\\') {
#else
    for (str = path; ptr = strstr(str, "/.."), ptr; str = ptr + 3) {
        if (ptr[3] == '\0' || ptr[3] == '/') {
#endif
            return 1;
        }
    }

    return 0;
}
