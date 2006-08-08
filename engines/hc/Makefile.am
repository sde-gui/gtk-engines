INCLUDES = $(GTK_CFLAGS) $(PARANOIA_CFLAGS) \
           -I$(top_srcdir)/engines/support

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libhcengine.la

EXTRA_DIST = \
	./AUTHORS

libhcengine_la_SOURCES = \
	./src/hc_gtk2_engine.c	\
	./src/hc_gtk2_engine.h	\
        ./src/hc_gtk2_support.c  \
        ./src/hc_gtk2_support.h  \
        ./src/hc_gtk2_drawing.h  \
	./src/hc-style.c

libhcengine_la_LDFLAGS = -module -avoid-version -no-undefined
libhcengine_la_LIBADD = $(GTK_LIBS) $(top_builddir)/engines/support/libsupport.la

