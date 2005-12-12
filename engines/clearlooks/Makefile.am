INCLUDES = $(GTK_CFLAGS)

EXTRA_DIST = ./src/animation.c

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libclearlooks.la

libclearlooks_la_SOURCES =		\
        ./src/clearlooks_rc_style.c	\
        ./src/clearlooks_rc_style.h	\
        ./src/clearlooks_style.c	\
        ./src/clearlooks_style.h	\
        ./src/clearlooks_theme_main.c	\
	./src/support.c			\
	./src/support.h			\
	./src/clearlooks_draw.c		\
	./src/clearlooks_draw.h		\
	./src/clearlooks_types.h

libclearlooks_la_LDFLAGS = -module -avoid-version -no-undefined
libclearlooks_la_LIBADD =  $(GTK_LIBS)

