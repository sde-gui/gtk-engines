INCLUDES = $(GTK_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libindustrial.la

libindustrial_la_SOURCES = \
	./src/industrial_rc_style.c	\
	./src/industrial_rc_style.h	\
	./src/industrial_style.c	\
	./src/industrial_style.h	\
	./src/industrial_theme_main.c

EXTRA_DIST = \
	./src/industrial_style_versioned_include.h	\
	./src/industrial_style_versioned_code.h		\
	./src/parse_rc_style.h				\
	./src/xpm/checkbox-mixed.xpm			\
	./src/xpm/menu-checkbox-on.xpm			\
	./src/xpm/radio-off.xpm				\
	./src/xpm/checkbox-off.xpm			\
	./src/xpm/menu-radio-mixed.xpm			\
	./src/xpm/radio-on.xpm				\
	./src/xpm/checkbox-on.xpm			\
	./src/xpm/menu-radio-off.xpm			\
	./src/xpm/table-checkbox-mixed.xpm		\
	./src/xpm/menu-checkbox-mixed.xpm		\
	./src/xpm/menu-radio-on.xpm			\
	./src/xpm/table-checkbox-off.xpm		\
	./src/xpm/menu-checkbox-off.xpm			\
	./src/xpm/radio-mixed.xpm			\
	./src/xpm/table-checkbox-on.xpm			\
	./AUTHORS

libindustrial_la_LDFLAGS = -module -avoid-version -no-undefined
libindustrial_la_LIBADD = $(GTK_LIBS)

