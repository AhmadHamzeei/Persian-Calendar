iconsdir = $(datadir)/icons/hicolor

nobase_dist_icons_DATA = \
	16x16/apps/acal.png \
	22x22/apps/acal.png \
	24x24/apps/acal.png \
	32x32/apps/acal.png \
	48x48/apps/acal.png \
	scalable/apps/acal.svg \
	$(NULL)

gtk_update_icon_cache = gtk-update-icon-cache-3.0 -f -t $(iconsdir)

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache
update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating GTK icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated. After (un)install, run this:"; \
		echo "*** $(gtk_update_icon_cache)"; \
	fi

pixmapsdir = $(datadir)/icons/hicolor/256x256/status
dist_pixmaps_DATA = \
	256x256/acal-dark-1.png \
	256x256/acal-dark-2.png \
	256x256/acal-dark-3.png \
	256x256/acal-dark-4.png \
	256x256/acal-dark-5.png \
	256x256/acal-dark-6.png \
	256x256/acal-dark-7.png \
	256x256/acal-dark-8.png \
	256x256/acal-dark-9.png \
	256x256/acal-dark-10.png \
	256x256/acal-dark-11.png \
	256x256/acal-dark-12.png \
	256x256/acal-dark-13.png \
	256x256/acal-dark-14.png \
	256x256/acal-dark-15.png \
	256x256/acal-dark-16.png \
	256x256/acal-dark-17.png \
	256x256/acal-dark-18.png \
	256x256/acal-dark-19.png \
	256x256/acal-dark-20.png \
	256x256/acal-dark-21.png \
	256x256/acal-dark-22.png \
	256x256/acal-dark-23.png \
	256x256/acal-dark-24.png \
	256x256/acal-dark-25.png \
	256x256/acal-dark-26.png \
	256x256/acal-dark-27.png \
	256x256/acal-dark-28.png \
	256x256/acal-dark-29.png \
	256x256/acal-dark-30.png \
	256x256/acal-dark-31.png \
	256x256/acal-light-1.png \
	256x256/acal-light-2.png \
	256x256/acal-light-3.png \
	256x256/acal-light-4.png \
	256x256/acal-light-5.png \
	256x256/acal-light-6.png \
	256x256/acal-light-7.png \
	256x256/acal-light-8.png \
	256x256/acal-light-9.png \
	256x256/acal-light-10.png \
	256x256/acal-light-11.png \
	256x256/acal-light-12.png \
	256x256/acal-light-13.png \
	256x256/acal-light-14.png \
	256x256/acal-light-15.png \
	256x256/acal-light-16.png \
	256x256/acal-light-17.png \
	256x256/acal-light-18.png \
	256x256/acal-light-19.png \
	256x256/acal-light-20.png \
	256x256/acal-light-21.png \
	256x256/acal-light-22.png \
	256x256/acal-light-23.png \
	256x256/acal-light-24.png \
	256x256/acal-light-25.png \
	256x256/acal-light-26.png \
	256x256/acal-light-27.png \
	256x256/acal-light-28.png \
	256x256/acal-light-29.png \
	256x256/acal-light-30.png \
	256x256/acal-light-31.png \
	$(NULL)

desktopdir = $(datadir)/applications
desktop_in_files = acal.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

gsettings_SCHEMAS = ir.ubuntu.acal.gschema.xml
@GSETTINGS_RULES@

EXTRA_DIST = \
	$(desktop_DATA) \
	$(desktop_in_files) \
	$(gsettings_SCHEMAS) \
	$(NULL)

CLEANFILES = \
	$(desktop_DATA) \
	$(NULL)

DISTCLEANFILES = \
	$(desktop_DATA) \
	$(NULL)

-include $(top_srcdir)/git.mk
