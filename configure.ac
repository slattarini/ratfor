# -*- Autoconf -*-
# configure.ac file for C Ratfor
# Process this file with autoconf to produce a configure script.

m4_pattern_forbid([^_RAT4_])
m4_pattern_forbid([^RAT4_])
m4_pattern_forbid([^_AX_])
m4_pattern_forbid([^AX_])

AC_PREREQ([2.69])
AC_INIT([C Ratfor], [0.9a], [stefano.lattarini@gmail.com])

AC_CONFIG_SRCDIR([src/rat4-common.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SUBDIRS([tests/blackbox])

AM_INIT_AUTOMAKE([1.14 subdir-objects foreign -Wall no-installinfo
                  no-texinfo.tex])
AM_SILENT_RULES([yes])

AC_PROG_CC

# We'll build a static library for unit testing.
AC_PROG_RANLIB
AM_PROG_AR

AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

for func in getopt strerror vfprintf strdup; do
    AC_CHECK_FUNC([$func], [:],
        [AC_MSG_ERROR([No proper `$func' subroutine found])])
done
AS_UNSET([func])

# Get a list of as many compiler warnings as possible, and save it in the
# shell variable `all_gcc_warnings'.
gl_MANYWARN_ALL_GCC([all_gcc_warnings])

# The C compiler flag `-pedantic' can confuse third party compilers
# (e.g. SunStudio cc), so try it only if we know that the C compiler
# is gcc.
if test x"$GCC" = x"yes"; then
    AS_VAR_APPEND([all_gcc_warnings], [" -pedantic"])
fi

# Now will set up the list of the undesired warnings.
nw="" # short name, for lazy typists.
nw="$nw -Wdeclaration-after-statement"
nw="$nw -Wbad-function-cast"
nw="$nw -Wc++-compat"
nw="$nw -Wstrict-prototypes"
nw="$nw -Wold-style-definition"
nw="$nw -Wmissing-prototypes"
nw="$nw -Wnested-externs"
# Don't let system headers trigger warnings.
nw="$nw -Wsystem-headers"
# All compiler preprocessors support "#if UNDEF".
nw="$nw -Wundef"
# All compilers nowadays support ANSI C
nw="$nw -Wtraditional"
# This warning is useless since we don't use the `-fstrict-overflow'
# switch; moreover, it might easily lead to false positives.
nw="$nw -Wstrict-overflow"
# These warnings usually don't point to mistakes.
nw="$nw -Wconversion"
# These warnings seems useless, and I don't know I to work around them.
nw="$nw -Wcast-qual"
# Will prolly require fiddling with __attribute__.
nw="$nw -Wmissing-noreturn"
nw="$nw -Wmissing-format-attribute"
# Still too many warnings with this enabled; needs investigation.
nw="$nw -Wtraditional-conversion"
# Still too many spurious warnings with this enabled; needs investigation.
nw="$nw -Wunreachable-code"


# Save the list in a variable with a "more proper" name, get rid
# of temporary variable `nw'.
undesired_gcc_warnings=$nw
AS_UNSET([nw])

# Filter out all the undesired GCC warnings, ans save the list of the
# desired warnings in the shell variable `desired_gcc_warnings'.
gl_MANYWARN_COMPLEMENT([desired_gcc_warnings],
                       [$all_gcc_warnings],
                       [$undesired_gcc_warnings])

# Enable all the desired GCC warnings.
for w in $desired_gcc_warnings; do
  gl_WARN_ADD([$w], [WARN_CFLAGS])
done
AS_UNSET([w])

# Make list of GCC warnings accessible to Makefiles.
AC_SUBST([WARN_CFLAGS])

# Do output.

AC_CONFIG_FILES([
    Makefile
    doc/Makefile
    src/Makefile
    tests/Makefile
    tests/t/Makefile
    tests/t/lib/Makefile
    tests/t/sanity-checks/Makefile
    tests/t/tests/Makefile
    tests/graybox/Makefile
    tests/unit/Makefile
])

AC_OUTPUT
