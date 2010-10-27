## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the `src' subdirectory.

include SourcesAndHeaders.am

bin_PROGRAMS = c-ratfor
c_ratfor_SOURCES = $(ratfor_c_files) $(ratfor_h_files)

# Enable as many compiler warnings as possible.  Also, make compiler
# warnings fatal, if the user aked so.
AM_CFLAGS = $(WARN_CFLAGS) $(WERROR_CFLAGS)

## vim: ft=automake noet sw=4 ts=4

