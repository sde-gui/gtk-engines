AC_INIT([gtk-engines], [2.5.0])
AC_CONFIG_SRCDIR([README])

AM_INIT_AUTOMAKE

dnl Initialize maintainer mode
AM_MAINTAINER_MODE

AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl Initialize libtool
AM_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.4.0,,
                  AC_MSG_ERROR([GTK+-2.0 is required to compile gtk-engines]))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
AC_SUBST(GTK_VERSION)

AM_CONFIG_HEADER([
	engines/crux/src/config.h
])

AC_CONFIG_FILES([
Makefile
engines/Makefile
engines/crux/Makefile
engines/crux/pixmaps/Makefile
engines/hc/Makefile
engines/industrial/Makefile
engines/lighthouseblue/Makefile
engines/mist/Makefile
engines/thinice/Makefile
metal/Makefile
metal/Theme/Makefile
metal/Theme/gtk-2.0/Makefile
redmond95/Makefile
redmond95/Theme/Makefile
redmond95/Theme/gtk-2.0/Makefile
gtk-engines-2.pc
gtk-engines.spec
])

AC_OUTPUT
