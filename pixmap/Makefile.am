SUBDIRS = Theme
CFLAGS = $(GTK_CFLAGS) $(GDK_IMLIB_CFLAGS)
LIBS = $(GTK_LIBS) $(GDK_IMLIB_LIBS)
enginedir = $(libdir)/gtk/themes/engines

# replace the "pixmap" prefix with whatever you're using
engine_LTLIBRARIES = libpixmap.la
libpixmap_la_SOURCES = \
	pixmap_theme_draw.c	\
	pixmap_theme_main.c
libpixmap_la_LDFLAGS = -export-dynamic
