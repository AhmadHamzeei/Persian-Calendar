iconsdir = $(datadir)/icons/hicolor

nobase_dist_icons_DATA = \
	16x16/apps/acal.png \
	22x22/apps/acal.png \
	24x24/apps/acal.png \
	32x32/apps/acal.png \
	48x48/apps/acal.png \
	scalable/apps/acal.svg \
	$(NULL)

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(iconsdir)

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

pixmapsdir = $(datadir)/icons/ubuntu-mono-dark/acal
dist_pixmaps_DATA = \
	pixmaps/ubuntu-mono-dark/persian-calendar-1.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-2.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-3.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-4.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-5.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-6.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-7.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-8.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-9.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-10.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-11.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-12.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-13.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-14.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-15.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-16.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-17.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-18.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-19.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-20.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-21.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-22.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-23.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-24.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-25.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-26.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-27.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-28.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-29.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-30.png \
	pixmaps/ubuntu-mono-dark/persian-calendar-31.png \
	$(NULL)

desktopdir = $(datadir)/applications
desktop_in_files = acal.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST = \
	$(desktop_DATA) \
	$(desktop_in_files) \
	$(NULL)

CLEANFILES = \
	$(desktop_DATA) \
	$(NULL)

DISTCLEANFILES = \
	$(desktop_DATA) \
	$(NULL)

-include $(top_srcdir)/git.mk
