#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *redirect_path(const char *path, char *buf, size_t buf_size) {
    if (!path) return NULL;
    const char *sysroot = getenv("POCKETSAMP_SYSROOT");
    if (!sysroot || sysroot[0] == '\0') return path;

    if (strcmp(path, "/etc/resolv.conf") == 0 ||
        strcmp(path, "/etc/hosts") == 0 ||
        strcmp(path, "/etc/nsswitch.conf") == 0 ||
        strcmp(path, "/etc/localtime") == 0 ||
        strncmp(path, "/etc/ssl/", 9) == 0 ||
        strcmp(path, "/etc/ssl") == 0) {
        int n = snprintf(buf, buf_size, "%s%s", sysroot, path);
        if (n > 0 && (size_t)n < buf_size) {
            return buf;
        }
    }
    return path;
}

int open(const char *pathname, int flags, ...) {
    static int (*real_open)(const char *, int, ...) = NULL;
    if (!real_open) real_open = dlsym(RTLD_NEXT, "open");
    char buf[PATH_MAX];
    const char *target = redirect_path(pathname, buf, sizeof(buf));
    if (flags & O_CREAT
#ifdef O_TMPFILE
        || (flags & O_TMPFILE) == O_TMPFILE
#endif
    ) {
        va_list ap;
        va_start(ap, flags);
        mode_t mode = (mode_t)va_arg(ap, int);
        va_end(ap);
        return real_open(target, flags, mode);
    }
    return real_open(target, flags);
}

int open64(const char *pathname, int flags, ...) {
    static int (*real_open64)(const char *, int, ...) = NULL;
    if (!real_open64) real_open64 = dlsym(RTLD_NEXT, "open64");
    char buf[PATH_MAX];
    const char *target = redirect_path(pathname, buf, sizeof(buf));
    if (flags & O_CREAT
#ifdef O_TMPFILE
        || (flags & O_TMPFILE) == O_TMPFILE
#endif
    ) {
        va_list ap;
        va_start(ap, flags);
        mode_t mode = (mode_t)va_arg(ap, int);
        va_end(ap);
        return real_open64(target, flags, mode);
    }
    return real_open64(target, flags);
}

int openat(int dirfd, const char *pathname, int flags, ...) {
    static int (*real_openat)(int, const char *, int, ...) = NULL;
    if (!real_openat) real_openat = dlsym(RTLD_NEXT, "openat");
    char buf[PATH_MAX];
    const char *target = redirect_path(pathname, buf, sizeof(buf));
    if (flags & O_CREAT
#ifdef O_TMPFILE
        || (flags & O_TMPFILE) == O_TMPFILE
#endif
    ) {
        va_list ap;
        va_start(ap, flags);
        mode_t mode = (mode_t)va_arg(ap, int);
        va_end(ap);
        return real_openat(dirfd, target, flags, mode);
    }
    return real_openat(dirfd, target, flags);
}

int openat64(int dirfd, const char *pathname, int flags, ...) {
    static int (*real_openat64)(int, const char *, int, ...) = NULL;
    if (!real_openat64) real_openat64 = dlsym(RTLD_NEXT, "openat64");
    char buf[PATH_MAX];
    const char *target = redirect_path(pathname, buf, sizeof(buf));
    if (flags & O_CREAT
#ifdef O_TMPFILE
        || (flags & O_TMPFILE) == O_TMPFILE
#endif
    ) {
        va_list ap;
        va_start(ap, flags);
        mode_t mode = (mode_t)va_arg(ap, int);
        va_end(ap);
        return real_openat64(dirfd, target, flags, mode);
    }
    return real_openat64(dirfd, target, flags);
}

FILE *fopen(const char *pathname, const char *mode) {
    static FILE *(*real_fopen)(const char *, const char *) = NULL;
    if (!real_fopen) real_fopen = dlsym(RTLD_NEXT, "fopen");
    char buf[PATH_MAX];
    return real_fopen(redirect_path(pathname, buf, sizeof(buf)), mode);
}

FILE *fopen64(const char *pathname, const char *mode) {
    static FILE *(*real_fopen64)(const char *, const char *) = NULL;
    if (!real_fopen64) real_fopen64 = dlsym(RTLD_NEXT, "fopen64");
    char buf[PATH_MAX];
    return real_fopen64(redirect_path(pathname, buf, sizeof(buf)), mode);
}

int access(const char *pathname, int mode) {
    static int (*real_access)(const char *, int) = NULL;
    if (!real_access) real_access = dlsym(RTLD_NEXT, "access");
    char buf[PATH_MAX];
    return real_access(redirect_path(pathname, buf, sizeof(buf)), mode);
}
