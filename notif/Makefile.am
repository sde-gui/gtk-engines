SUBDIRS = Theme
INCLUDES = $(GTK_CFLAGS) $(GDK_IMLIB_CFLAGS)
LIBS = $(GTK_LIBS) $(GDK_IMLIB_LIBS)
enginedir = $(libdir)/gtk/themes/engines

# replace the "notif" prefix with whatever you're using
engine_LTLIBRARIES = libnotif.la
libnotif_la_SOURCES = \
	notif_theme_draw.c	\
	notif_theme_main.c
libnotif_la_LDFLAGS = -export-dynamic
