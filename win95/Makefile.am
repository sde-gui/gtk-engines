SUBDIRS = Theme
INCLUDES = $(GTK_CFLAGS) $(GDK_IMLIB_CFLAGS)
LIBS = $(GTK_LIBS) $(GDK_IMLIB_LIBS)
enginedir = $(libdir)/gtk/themes/engines

# replace the "win95" prefix with whatever you're using
engine_LTLIBRARIES = libwin95.la
libwin95_la_SOURCES = \
	win95_theme_draw.c	\
	win95_theme_main.c
libwin95_la_LDFLAGS = -export-dynamic
