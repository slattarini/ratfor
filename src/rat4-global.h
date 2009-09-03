#ifndef RAT4_GLOBAL_H
#  define RAT4_GLOBAL_H 1
#include "rat4-common.h"
int startlab;                   /*  start label (configurable with `-l' switch)  */
char fcname[MAXNAME];           /*  text of current function name                */
int fordep;                     /*  current depth of for statements              */
char forstk[MAXFORSTK];         /*  stack of reinit strings                      */
int swtop;                      /*  current switch entry; init=0                 */
int swlast;                     /*  next available position; init=1              */
int swstak[MAXSWITCH];          /*  switch information stack                     */
int xfer;                       /*  YES if just made transfer, NO otherwise      */
int leaveC;                     /*  YES if comments should be kept, NO otherwise */
int label;                      /*  next label returned by labgen                */
int level;                      /*  level of file inclusion; init = 1            */
int linect[NFILES];             /*  line count on input file[level]; init = 1    */
FILE *infile[NFILES];           /*  file number[level]; init infile[1] = STDIN   */
char *filename[NFILES];         /*  filename of input file[level]                */
int avail;                      /*  first first location in table; init = 1      */
int tabptr[127];                /*  name pointers; init = 0                      */
int nfiles;                     /*  number of files                              */
#endif
/* vim: set ft=c ts=4 sw=4 et : */
