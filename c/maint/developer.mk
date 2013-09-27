# -*- Makefile -*-
# Copied from SteLib at 2010-02-03 18:08:13 +0100.  DO NOT EDIT!
# Included by maint.mk
#
#  This file is meant to be used by maintainers only; it try to detect
#  which developer is using it (by looking at the output of `id' command)
#  and include the proper *.mk file with  definitions specific to that
#  maintainer.  This inclusion can be skipped by setting the variable
#  `NO_DEVELOPER_PERSONAL_MAKEFILE' to `yes' (either in make or in the
#  environment)
#
#  Copyright (C) 2009, 2010 Stefano Lattarini.
#  Written by Stefano Lattarini <stefano.lattarini@gmail.com>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
