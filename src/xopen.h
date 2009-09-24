#ifndef RAT4_XOPEN_H
#  define RAT4_XOPEN_H 1
enum io_mode { IO_MODE_READ, IO_MODE_WRITE };
BEGIN_C_DECLS
FILE *xopen(const char *, enum io_mode, void (*)(const char *, ...));
END_C_DECLS
#endif
/* vim: set ft=c ts=4 sw=4 et : */
