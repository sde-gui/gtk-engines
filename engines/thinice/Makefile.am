INCLUDES = $(GTK_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libthinice.la

EXTRA_DIST = \
	./AUTHORS

libthinice_la_SOURCES = \
	./src/thinice_rc_style.c	\
	./src/thinice_rc_style.h	\
	./src/thinice_style.h	\
	./src/thinice_theme_draw.c	\
	./src/thinice_main.c

libthinice_la_LDFLAGS = -module -avoid-version -no-undefined
libthinice_la_LIBADD = $(GTK_LIBS)

