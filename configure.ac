# initialize autoconf
AC_INIT([Acal], [0.2], [ahmadhamzeei@gmail.com])

# define main source & invoke automake
AC_CONFIG_SRCDIR([src/acal.c])
AM_INIT_AUTOMAKE

# check for c compiler, pkg-config, gettext, intltool, glib-compile-resources, gsettings
AC_PROG_CC
AM_PROG_CC_C_O
PKG_PROG_PKG_CONFIG
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.1])
IT_PROG_INTLTOOL([0.40])
AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)
GLIB_GSETTINGS

# minimum lib versions
GLIB_REQUIRED=2.39.92
GTK_REQUIRED=3.4.2
APPINDICATOR_REQUIRED=0.4.92
GSETTINGS_DESKTOP_SCHEMAS_REQUIRED=0.0.1

# check for glib, gtk-3, appindicator & libjalali
PKG_CHECK_MODULES([DEPENDENCIES], [
  glib-2.0 >= $GLIB_REQUIRED
  gtk+-3.0 >= $GTK_REQUIRED
  appindicator3-0.1 >= $APPINDICATOR_REQUIRED
  gsettings-desktop-schemas >= $GSETTINGS_DESKTOP_SCHEMAS_REQUIRED
])
AC_SEARCH_LIBS([jalali_update], [jalali], [], [
  AC_MSG_ERROR([Unable to find libjalali])
])
AC_CHECK_HEADERS([jalali/jalali.h])

# Needed by intltool
GETTEXT_PACKAGE=acal
AC_SUBST([GETTEXT_PACKAGE])

AC_CONFIG_FILES([
  Makefile
  data/Makefile
  po/Makefile.in
  src/Makefile
])

AC_OUTPUT
