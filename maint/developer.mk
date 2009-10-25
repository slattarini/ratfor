# -*- Makefile -*-
# Copied from SteLib at 2009-10-25 16:29:29 +0100.  DO NOT EDIT!
# Included by maint.mk
#
# This file is meant to be used by maintainers only; it try to detect
# which developer is using it (by looking at the output of `id' command)
# and include the proper *.mk file with  definitions specific to that
# maintainer.  This inclusion can be skipped by setting the variable
# `NO_DEVELOPER_PERSONAL_MAKEFILE' to `yes' (either in make or in the
# environment)
#

NO_DEVELOPER_PERSONAL_MAKEFILE ?:= \
  $(shell echo "$${NO_DEVELOPER_PERSONAL_MAKEFILE-no}")

ifeq ($(NO_DEVELOPER_PERSONAL_MAKEFILE),yes)

  ## do nothing

else

_developer_full_name := $(shell \
  awk -v uid=`id -u` -F: '($$3 == uid) { print $$5 }' /etc/passwd | \
    sed -e 's/,.*$$//' | tr '[A-Z]' '[a-z]' | sed -e 's/[^a-z0-9_-]/-/g')

-include $(maintdir)/personal/$(_developer_full_name).mk

endif

# vim: ft=make ts=4 sw=4 noet
