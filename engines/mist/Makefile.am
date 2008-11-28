INCLUDES = -I$(top_srcdir)/engines/support \
           $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libmist.la

EXTRA_DIST = \
	./AUTHORS

libmist_la_SOURCES = \
	./src/mist-rc-style.c	\
	./src/mist-rc-style.h	\
	./src/mist-style.c	\
	./src/mist-style.h	\
	./src/mist.c

libmist_la_LDFLAGS = -module -avoid-version -no-undefined
libmist_la_LIBADD = $(GTK_LIBS) $(top_builddir)/engines/support/libsupport.la

