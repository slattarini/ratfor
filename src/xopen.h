#ifndef RAT4_XOPEN_H
#  define RAT4_XOPEN_H 1
enum io_mode { IO_MODE_READ, IO_MODE_WRITE };
FILE *xopen(const char *, enum io_mode, void (*)(const char *, ...));
#endif
/* vim: set ft=c ts=4 sw=4 et : */
