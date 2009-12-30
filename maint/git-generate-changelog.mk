# -*- Makefile -*-
# Copied from SteLib at 2009-12-30 13:37:57 +0100.  DO NOT EDIT!
# Contains maintainer-specific rules to generate the ChangeLog file from
# the Git log messages.

# Commits before this date shouldn't be put in the ChangeLog
# (user-overridable).
changelog-start-date ?= '1970-01-01 UTC'
# Other options to be passed to git-to-changelog (user-overridable).
gitlog-to-changelog-extra-options ?= --author-date
# The path/name of the gitlog-to-changelog script (user overridable).
gitlog-to-changelog-script ?= $(top_srcdir)/build-aux/gitlog-to-changelog

# Generate ChangeLog file from git log messages. User-overridable.
.PHONY: ChangeLog
ChangeLog:
	$(AM_V_at)rm -f $@.tmp
	$(AM_V_at)if test ! -d $(top_srcdir)/.git; then \
	  echo "No .git directory found in top_srcdir \`$(top_srcdir)'" >&2; \
	  echo "Cannot generate $@." >&2; \
	  exit 1; \
	fi
	$(AM_V_GEN)\
	  GIT_DIR=$(top_srcdir)/.git GIT_WORK_TREE=$(builddir) \
	    $(gitlog-to-changelog-script) --since=$(changelog-start-date) \
	      $(gitlog-to-changelog-extra-options) >$@.tmp
	$(AM_V_at)chmod a-w $@.tmp
	$(AM_V_at)mv -f $@.tmp $@

.PHONY: git-changelog-gen-mostlyclean
mostlyclean: git-changelog-gen-mostlyclean
mostlyclean-git-changelog-gen:
	rm -f ChangeLog.tmp

.PHONY: git-changelog-gen-mostlyclean
maintainerclean: git-changelog-gen-maintainerclean
git-changelog-gen-maintainerclean:
	rm -f ChangeLog


# vim: ft=make ts=4 sw=4 noet
