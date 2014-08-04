SUBDIRS = data po src

EXTRA_DIST = \
	autogen.sh \
	$(NULL)

# Build ChangeLog from git history
ChangeLog:
	@if test -f $(top_srcdir)/.git/HEAD; then \
		git log --stat > $@; \
	fi

dist: ChangeLog

.PHONY: ChangeLog

MAINTAINERCLEANFILES = \
$(GITIGNORE_MAINTAINERCLEANFILES_TOPLEVEL) \
$(GITIGNORE_MAINTAINERCLEANFILES_MAKEFILE_IN) \
$(GITIGNORE_MAINTAINERCLEANFILES_M4_LIBTOOL)

-include $(top_srcdir)/git.mk
