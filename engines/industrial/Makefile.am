INCLUDES = -I$(top_srcdir)/engines/support \
           $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libindustrial.la

libindustrial_la_SOURCES = \
	./src/industrial_rc_style.c	\
	./src/industrial_rc_style.h	\
	./src/industrial_style.c	\
	./src/industrial_style.h	\
	./src/industrial_theme_main.c

EXTRA_DIST = \
	./src/parse_rc_style.h				\
	./AUTHORS

libindustrial_la_LDFLAGS = -module -avoid-version -no-undefined
libindustrial_la_LIBADD = $(GTK_LIBS) $(top_builddir)/engines/support/libsupport.la

