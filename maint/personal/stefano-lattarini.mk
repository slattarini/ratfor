# -*- Makefile -*-
# Customization file specific to Stefano Lattarini.
# Included by developer.mk.
#
# DO NOT DISTRIBUTE THIS FILE!
#

huge_strict-distcheck := $(call normalize-boolean,$(HUGE_STRICTDISTCHECK))

stefano-lattarini_uname := $(shell uname)


## ----------- ##
##  GNU/Linux  ##
## ----------- ##

ifeq ($(stefano-lattarini_uname),Linux)

  stefano-lattarini_devel-dir := $(HOME)/devel/linux
  stefano-lattarini_have-recognized-uname := yes

  # C/C++ compilers to be used to compile public domain ratfor, when
  # doing a strict-distcheck.
  strict_distcheck_c_compilers = \
    /usr/local/bin/pcc \
    /usr/bin/g++ \
    /usr/bin/gcc
  ifeq ($(huge_strict-distcheck),yes)
    strict_distcheck_c_compilers += \
      $(stefano-lattarini_devel-dir)/bin/gcc-2.95-wrapper \
      /opt/vintage/bin/gcc-4.0 \
      /usr/bin/tcc \
      /usr/bin/gcc-3.4
  endif

  # Fortran compilers to be used by the ratfor testsuite, when doing a
  # strict-distcheck.
  strict_distcheck_f77_compilers = \
    NONE \
    /usr/bin/fort77 \
    /usr/bin/gfortran
  ifeq ($(huge_strict-distcheck),yes)
    strict_distcheck_f77_compilers += \
      /opt/vintage/bin/gfortran-4.0
  endif

  # Shells to be used to run the configure script and the tests scripts,
  # when doing a strict-distcheck.
  # NOTE: We can't use zsh directly in `$(strict_distcheck_shells)',
  # because a zsh as CONFIG_SHELL breaks the automake-generated makefiles
  # (the default behaviour of zsh is not even remotely bourne-compatible).
  # We use instead something like `SOMEDIR/sh', which is expected to be a
  # symlink to zsh; this works, since it makes zsh execute in bourne
  # compatibility mode (as it sees itself having an `sh' basename at
  # startup).
  strict_distcheck_shells = \
    $(stefano-lattarini_devel-dir)/bin/zsh4/sh \
    /bin/ksh \
    /opt/bin/heirloom-sh
  ifeq ($(huge_strict-distcheck),yes)
    strict_distcheck_shells += \
      $(stefano-lattarini_devel-dir)/bin/zsh3/sh \
      /bin/mksh \
      /bin/pdksh \
      /bin/dash \
      /opt/vintage/bin/bash-3.2 \
      /opt/vintage/bin/bash-3.0 \
      /opt/vintage/bin/bash-2.05 \
      /bin/bash
  endif

endif # $(stefano-lattarini_uname) == Linux


## --------- ##
##  FreeBSD  ##
## --------- ##

ifeq ($(stefano-lattarini_uname),FreeBSD)

  stefano-lattarini_have-recognized-uname := yes

  # C/C++ compilers to be used to compile public domain ratfor, when
  # doing a strict-distcheck.
  strict_distcheck_c_compilers = \
    /usr/local/bin/pcc \
    /usr/bin/g++ \
    /usr/bin/gcc
  ifeq ($(huge_strict-distcheck),yes)
    strict_distcheck_c_compilers += \
      /usr/local/bin/gcc34
  endif

  # Fortran compilers to be used by the ratfor testsuite, when doing a
  # strict-distcheck.
  strict_distcheck_f77_compilers = \
    NONE \
    /usr/local/opt/bin/g77 \
    /usr/local/bin/gfortran44
  ifeq ($(huge_strict-distcheck),yes)
    strict_distcheck_f77_compilers += \
      /usr/local/bin/g95
  endif

  # Shells to be used to run the configure script and the tests scripts,
  # when doing a strict-distcheck.
  # NOTE: We can't use zsh directly in `$(strict_distcheck_shells)',
  # because a zsh as CONFIG_SHELL breaks the automake-generated makefiles
  # (the default behaviour of zsh is not even remotely bourne-compatible).
  # We use instead something like `$(abs_maintdir)/zsh/sh', which is
  # epected to be a symlink to zsh; this works, since it makes zsh execute
  # in bourne-compatibility mode (as it sees itself having an `sh' basename
  # at startup).
  strict_distcheck_shells = \
    /bin/sh \
    /usr/local/bin/ksh \
    /usr/local/bin/mksh \
    /usr/local/bin/bash
  ifeq ($(huge_strict-distcheck),yes)
    strict_distcheck_shells += \
      /usr/local/bin/ksh93 \
      /usr/local/bin/pdksh
	##XXX: zsh???
  endif

endif # $(stefano-lattarini_uname) == FreeBSD


## -------------- ##
##  Sanity check  ##
## -------------- ##

ifneq ($(stefano-lattarini_have-recognized-uname),yes)
  $(error System has unrecognized uname '$(stefano-lattarini_uname)')
endif


# vim: ft=make ts=4 sw=4 noet
