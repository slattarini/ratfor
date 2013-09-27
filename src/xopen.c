/* xopen.c -- this file is part of C Ratfor.
 * Copyright (C) 2009, 2010 Stefano Lattarini <stefano.lattarini@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rat4-common.h"
#include <fcntl.h>      /* for open(2) */
#include <unistd.h>     /* for fstat(2) */
#include <sys/stat.h>   /* for `S_I*' constants */
#include <errno.h>
#include "xopen.h"

FILE *
xopen(const char *path, enum io_mode mode,
      void (* error_handler)(const char *, ...))
{
    int fd, creat_mode, open_mode;
    FILE *file = NULL, *default_file;
    const char *fdopen_mode, *open_how, *action;
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
        abort(); /* can't happen */
        break;
    }

    /* simple kludge to avoid code duplication */
#   define FAILED_(s_) do { action = (s_); goto open_failed; } while (0);

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
    error_handler("%s: cannot %s %s: %s", path, action, open_how,
                  strerror(errno));
    if (file != NULL)
        fclose(file);
    return NULL;

#   undef FAILED_
}

/* vim: set ft=c ts=4 sw=4 et : */
