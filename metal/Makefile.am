SUBDIRS = Theme
INCLUDES = $(GTK_CFLAGS) $(GDK_IMLIB_CFLAGS)
LIBS = $(GTK_LIBS) $(GDK_IMLIB_LIBS)
enginedir = $(libdir)/gtk/themes/engines

# replace the "metal" prefix with whatever you're using
engine_LTLIBRARIES = libmetal.la
libmetal_la_SOURCES = \
	metal_theme.h      	\
	metal_theme_draw.c	\
	metal_theme_main.c
libmetal_la_LDFLAGS = -export-dynamic
