INCLUDES = $(GTK_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = liblighthouseblue.la

liblighthouseblue_la_SOURCES = \
	./src/lighthouseblue_rc_style.c		\
	./src/lighthouseblue_rc_style.h		\
	./src/lighthouseblue_style.c		\
	./src/lighthouseblue_style.h		\
	./src/lighthouseblue_theme_main.c

EXTRA_DIST = \
	./src/util.c	\
	./AUTHORS
liblighthouseblue_la_LDFLAGS = -module -avoid-version -no-undefined
liblighthouseblue_la_LIBADD = $(GTK_LIBS)

