if OS_WIN32
no_undefined = -no-undefined
gtk_libs = $(GTK_LIBS)
endif

SUBDIRS = Theme
INCLUDES = $(GTK_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

# replace the "redmond95" prefix with whatever you're using
engine_LTLIBRARIES = libredmond95.la

libredmond95_la_SOURCES = \
	redmond_rc_style.c	\
	redmond_rc_style.h	\
	redmond_style.c		\
	redmond_style.h		\
	redmond_theme_main.c

libredmond95_la_LDFLAGS = -module -avoid-version $(no_undefined)
libredmond95_la_LIBADD = $(gtk_libs)

