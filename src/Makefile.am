bin_PROGRAMS = acal

EXTRA_DIST = \
	acal.gresource.xml \
	$(acal_RC) \
	$(NULL)

acal_RC = \
	resources/calendar.glade \
	$(NULL)

acal-resources.c: $(acal_RC) acal.gresource.xml
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --generate-source --c-name acal $(srcdir)/acal.gresource.xml

acal-resources.h: $(acal_RC) acal.gresource.xml
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)/resources --generate-header --c-name acal $(srcdir)/acal.gresource.xml

acal_RESOURCES = \
	acal-resources.c \
	acal-resources.h \
	$(NULL)

INCLUDES = \
	-DDATADIR=\""$(datadir)"\" \
	-DLOCALEDIR=\""$(localedir)"\" \
	$(NULL)

acal_SOURCES = \
	acal.c \
	$(acal_RESOURCES) \
	$(NULL)

acal_CPPFLAGS = $(DEPENDENCIES_CFLAGS)
acal_LDADD = $(DEPENDENCIES_LIBS)
acal_LDFLAGS = -export-dynamic

CLEANFILES = $(acal_RESOURCES)
DISTCLEANFILES = $(acal_RESOURCES)

-include $(top_srcdir)/git.mk
