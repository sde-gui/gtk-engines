INCLUDES = $(GTK_CFLAGS)  $(PARANOIA_CFLAGS) \
           -I$(top_srcdir)/engines/support

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libglide.la

libglide_la_SOURCES = \
	./src/glide_gtk2_support.c	\
	./src/glide_gtk2_support.h	\
	./src/glide_gtk2_drawing.c	\
	./src/glide_gtk2_drawing.h	\
	./src/glide_gtk2_engine.c	\
	./src/glide_gtk2_engine.h

libglide_la_LDFLAGS = -module -avoid-version -no-undefined
libglide_la_LIBADD =  $(GTK_LIBS) $(top_builddir)/engines/support/libsupport.la
