AC_INIT([gtk-engines], [2.7.5])
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

enableval="n"
disableval="y"

AC_ARG_ENABLE(clearlooks, 	[  --disable-clearlooks		skip ClearLooks engine/theme], 		[clearlooks=$enableval], 	[clearlooks=$disableval])
AC_ARG_ENABLE(crux, 		[  --disable-crux		skip Crux engine/theme], 		[crux=$enableval], 		[crux=$disableval])
AC_ARG_ENABLE(hc,		[  --disable-hc 		skip HC(high contrast) engine], 	[hc=$enableval], 		[hc=$disableval])
AC_ARG_ENABLE(industrial, 	[  --disable-industrial		skip Industrial engine/theme], 		[industrial=$enableval], 	[industrial=$disableval])
AC_ARG_ENABLE(mist, 		[  --disable-mist		skip Mist engine/theme], 		[mist=$enableval], 		[mist=$disableval])
AC_ARG_ENABLE(redmond,		[  --disable-redmond		skip Redmond engine/theme], 		[redmond=$enableval], 		[redmond=$disableval])
AC_ARG_ENABLE(smooth, 		[  --disable-smooth		skip Smooth engine], 			[smooth=$enableval], 		[smooth=$disableval])
AC_ARG_ENABLE(glide,           [  --disable-glide             skip Glide engine],                    [glide=$enableval],            [glide=$disableval])
AC_ARG_ENABLE(thinice,		[  --disable-thinice		skip ThinIce engine/theme], 		[thinice=$enableval], 		[thinice=$disableval])

AC_ARG_ENABLE(animation, 	[  --enable-animation		compile clearlooks with animation support], 	[animation=$disableval], 	[animation=$enableval])

AC_ARG_ENABLE(themes, 		[  --disable-themes		skip installing default themes], 	[themes=$enableval], 		[themes=$disableval])

BUILD_ENGINES=""
BUILD_THEMES=""

if test $clearlooks = "y"; then
	BUILD_ENGINES="$BUILD_ENGINES clearlooks"
	if test $themes = "y"; then
		BUILD_THEMES="$BUILD_THEMES Clearlooks"
	fi
fi

if test $crux = "y"; then
	BUILD_ENGINES="$BUILD_ENGINES crux"
	if test $themes = "y"; then
		BUILD_THEMES="$BUILD_THEMES Crux"
	fi
fi

if test $hc = "y"; then
	BUILD_ENGINES="$BUILD_ENGINES hc"
fi

if test $industrial = "y"; then
	BUILD_ENGINES="$BUILD_ENGINES industrial"
	if test $themes = "y"; then
		BUILD_THEMES="$BUILD_THEMES Industrial"
	fi
fi

if test $mist = "y"; then
	BUILD_ENGINES="$BUILD_ENGINES mist"
	if test $themes = "y"; then
		BUILD_THEMES="$BUILD_THEMES Mist"
	fi
fi

if test $redmond = "y"; then
	BUILD_ENGINES="$BUILD_ENGINES redmond"
	if test $themes = "y"; then
		BUILD_THEMES="$BUILD_THEMES Redmond"
	fi
fi

if test $glide = "y"; then
	BUILD_ENGINES="$BUILD_ENGINES glide"
fi

if test $smooth = "y"; then
        BUILD_ENGINES="$BUILD_ENGINES smooth"
fi

if test $thinice = "y"; then
	BUILD_ENGINES="$BUILD_ENGINES thinice"
	if test $themes = "y"; then
		BUILD_THEMES="$BUILD_THEMES ThinIce"
	fi
fi

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.8.0,,
                  AC_MSG_ERROR([GTK+-2.8 is required to compile gtk-engines]))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-2.0`
AC_SUBST(GTK_VERSION)

AC_SUBST(BUILD_ENGINES)
AC_SUBST(BUILD_THEMES)

if test $animation = "y"; then
	AC_DEFINE_UNQUOTED(HAVE_ANIMATION, 1, [Defines whether to compile with animation support])
fi

AM_CONFIG_HEADER([engines/clearlooks/src/config.h])
AM_CONFIG_HEADER([engines/crux/src/config.h])
AM_CONFIG_HEADER([engines/smooth/src/config.h])

AC_CONFIG_FILES([
Makefile
engines/Makefile
engines/support/Makefile
engines/clearlooks/Makefile
engines/crux/Makefile
engines/crux/pixmaps/Makefile
engines/hc/Makefile
engines/industrial/Makefile
engines/mist/Makefile
engines/redmond/Makefile
engines/glide/Makefile
engines/smooth/Makefile
engines/thinice/Makefile
themes/Makefile
themes/Clearlooks/Makefile
themes/Clearlooks/gtk-2.0/Makefile
themes/Crux/Makefile
themes/Crux/gtk-2.0/Makefile
themes/Industrial/Makefile
themes/Industrial/gtk-2.0/Makefile
themes/Mist/Makefile
themes/Mist/gtk-2.0/Makefile
themes/Redmond/Makefile
themes/Redmond/gtk/Makefile
themes/Redmond/gtk-2.0/Makefile
themes/ThinIce/Makefile
themes/ThinIce/gtk-2.0/Makefile
gtk-engines-2.pc
gtk-engines.spec
]) 

AC_OUTPUT

echo -e "\nEngines and Themes:"

echo -n "      ClearLooks - "
if test "$clearlooks" = "y"; then
        echo -n "Including"
        if test $animation = "y"; then
        	echo " with animation support"
        else
        	echo " without animation support"
        fi
else
        echo "Skipping"
fi

echo -n "      Crux - "
if test "$crux" = "y"; then
	if test $themes = "y"; then
		echo "Including"
	else
		echo "Including(Skipping Theme)"
	fi
else
	echo "Skipping"
fi

echo -n "      HC - "
if test "$hc" = "y"; then
	echo "Including"
else
	echo "Skipping"
fi

echo -n "      Industrial - "
if test "$industrial" = "y"; then
	if test $themes = "y"; then
		echo "Including"
	else
		echo "Including(Skipping Theme)"
	fi
else
	echo "Skipping"
fi

echo -n "      Mist - "
if test "$mist" = "y"; then
	if test $themes = "y"; then
		echo "Including"
	else
		echo "Including(Skipping Theme)"
	fi
else
	echo "Skipping"
fi

echo -n "      Redmond - "
if test "$redmond" = "y"; then
	if test $themes = "y"; then
		echo "Including"
	else
		echo "Including(Skipping Theme)"
	fi
else
	echo "Skipping"
fi

echo -n "      Glide - "
if test "$glide" = "y"; then
        echo "Including"
else
        echo "Skipping"
fi

echo -n "      Smooth - "
if test "$smooth" = "y"; then
	echo "Including"
else
	echo "Skipping"
fi

echo -n "      ThinIce - "
if test "$thinice" = "y"; then
	if test $themes = "y"; then
		echo "Including"
	else
		echo "Including(Skipping Theme)"
	fi
else
	echo "Skipping"
fi

echo ""
