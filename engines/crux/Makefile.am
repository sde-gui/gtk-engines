SUBDIRS = pixmaps

INCLUDES = $(GTK_CFLAGS)				\
           -DDATADIR=\"$(datadir)/themes/Crux/pixmaps\"	\
           -I$(top_srcdir)/engines/support

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libcrux-engine.la

EXTRA_DIST = \
	./AUTHORS

libcrux_engine_la_SOURCES = \
	./src/crux-common.h           \
	./src/crux.h                  \
	./src/crux-draw.c             \
	./src/crux-draw.h             \
	./src/crux-main.c             \
	./src/crux-pixmaps.c          \
	./src/crux-pixmaps.h          \
	./src/crux-rc-parser.c        \
	./src/crux-rc-parser.h        \
	./src/crux-rc-style.c         \
	./src/crux-rc-style.h         \
	./src/crux-style.c            \
	./src/crux-style.h            \
	./src/debug.c                 \
	./src/debug.h                 \
	./src/crux-gradient.c         \
	./src/crux-gradient.h         \
	./src/eazel-theme.h

libcrux_engine_la_LDFLAGS = -module -avoid-version -no-undefined
libcrux_engine_la_LIBADD = $(GTK_LIBS) $(top_builddir)/engines/support/libsupport.la

