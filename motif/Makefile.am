SUBDIRS = Theme
INCLUDES = $(GTK_CFLAGS) $(GDK_IMLIB_CFLAGS)
LIBS = $(GTK_LIBS) $(GDK_IMLIB_LIBS)
enginedir = $(libdir)/gtk/themes/engines

# replace the "motif" prefix with whatever you're using
engine_LTLIBRARIES = libmotif.la
libmotif_la_SOURCES = \
	motif_theme_draw.c	\
	motif_theme_main.c
libmotif_la_LDFLAGS = -export-dynamic
