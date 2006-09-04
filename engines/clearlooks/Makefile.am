INCLUDES = $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS) \
           -I$(top_srcdir)/engines/support

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libclearlooks.la

EXTRA_DIST = ./src/config.h.in


libclearlooks_la_SOURCES =		\
        ./src/clearlooks_rc_style.c	\
        ./src/clearlooks_rc_style.h	\
        ./src/clearlooks_style.c	\
        ./src/clearlooks_style.h	\
        ./src/clearlooks_theme_main.c	\
	./src/support.c			\
	./src/animation.c		\
	./src/animation.h		\
	./src/support.h			\
	./src/clearlooks_draw.c		\
	./src/clearlooks_draw.h		\
	./src/clearlooks_types.h	\
	./src/config.h

libclearlooks_la_LDFLAGS = -module -avoid-version -no-undefined
libclearlooks_la_LIBADD =  $(GTK_LIBS) $(top_builddir)/engines/support/libsupport.la

