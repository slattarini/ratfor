#include "rat4-common.h"
#include <fcntl.h>      /* for open(2) */
#include <unistd.h>     /* for fstat(2) */
#include <sys/stat.h>   /* for `S_I*' constants */

#include "error.h"      /* for `fatal()' and `synerr()' */
#include "xopen.h"


FILE *
xopen(const char *path, enum io_mode mode,
      void (* error_handler)(const char *, ...))
{
    int fd, creat_mode, open_mode;
    FILE *file = NULL, *default_file;
    const char *fdopen_mode, *open_how;
    struct stat st;

    switch (mode) {
    case IO_MODE_READ:
        default_file = stdin;
        open_mode = O_RDONLY;
        creat_mode = 0;
        fdopen_mode = "r";
        open_how = "for reading";
        break;
    case IO_MODE_WRITE:
        default_file = stdout;
        open_mode = O_WRONLY|O_CREAT;
        creat_mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
        fdopen_mode = "w";
        open_how = "for writing";
        break;
    default:
        abort(); /* CANTHAPPEN */
        break;
    }

    /* simple kludge to avoid code duplication */
#   define FAILED_(s_) \
        do { \
            error_handler("%s: cannot %s %s: %s", path, s_, open_how, \
                          strerror(errno)); \
            goto open_failed; \
        } while (false);

    if (STREQ(path, "-")) {
        return default_file;
    } else if ((fd = open(path, open_mode, creat_mode)) < 0) {
        FAILED_("open");
    } else if (fstat(fd, &st) < 0) {
        FAILED_("stat");
    } else if (S_ISDIR(st.st_mode)) {
        errno = EISDIR;
        FAILED_("open");
    } else if ((file = fdopen(fd, fdopen_mode)) == NULL) {
        FAILED_("fdopen");
    } else {
        return file;
    }

open_failed:
    if (file != NULL)
        fclose(file);
    return NULL;

#   undef FAILED_
}

/* vim: set ft=c ts=4 sw=4 et : */
