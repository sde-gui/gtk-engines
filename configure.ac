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
	engines/smooth/src/config.h
])

AC_CONFIG_FILES([
Makefile
engines/Makefile
engines/crux/Makefile
engines/crux/pixmaps/Makefile
engines/hc/Makefile
engines/industrial/Makefile
engines/lighthouseblue/Makefile
engines/metal/Makefile
engines/mist/Makefile
engines/redmond/Makefile
engines/smooth/Makefile
engines/thinice/Makefile
themes/Makefile
themes/Crux/Makefile
themes/Crux/gtk-2.0/Makefile
themes/Industrial/Makefile
themes/Industrial/gtk-2.0/Makefile
themes/LighthouseBlue/Makefile
themes/LighthouseBlue/gtk-2.0/Makefile
themes/Metal/Makefile
themes/Metal/gtk-2.0/Makefile
themes/Mist/Makefile
themes/Mist/gtk-2.0/Makefile
themes/Redmond/Makefile
themes/Redmond/gtk/Makefile
themes/Redmond/gtk-2.0/Makefile
gtk-engines-2.pc
gtk-engines.spec
])

AC_OUTPUT
