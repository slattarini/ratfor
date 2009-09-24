#ifndef RAT4_LABGEN_H
#  define RAT4_LABGEN_H 1
BEGIN_C_DECLS

int labgen(int);
int set_starting_label(int);
bool can_label_conflict(int);

END_C_DECLS
#endif
/* vim: set ft=c ts=4 sw=4 et : */
