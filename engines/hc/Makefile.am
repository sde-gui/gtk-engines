INCLUDES = $(GTK_CFLAGS)	\
           -I$(top_srcdir)/engines/support

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libhcengine.la

EXTRA_DIST = \
	./AUTHORS

libhcengine_la_SOURCES = \
	./src/hc-rc-style.c	\
	./src/hc-rc-style.h	\
	./src/hc-style.c	\
	./src/hc-style.h	\
	./src/hc-main.c

libhcengine_la_LDFLAGS = -module -avoid-version -no-undefined
libhcengine_la_LIBADD = $(GTK_LIBS) $(top_builddir)/engines/support/libsupport.la

