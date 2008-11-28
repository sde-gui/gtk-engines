INCLUDES = -I$(top_srcdir)/engines/support \
           $(GTK_CFLAGS) $(DEVELOPMENT_CFLAGS)


enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libredmond95.la

libredmond95_la_SOURCES = \
	./src/redmond_gtk2_misc.c	\
	./src/redmond_gtk2_misc.h	\
	./src/redmond_gtk2_drawing.c	\
	./src/redmond_gtk2_drawing.h	\
	./src/redmond_gtk2_engine.c	\
	./src/redmond_gtk2_engine.h

libredmond95_la_LDFLAGS = -module -avoid-version -no-undefined
libredmond95_la_LIBADD =  $(GTK_LIBS) $(top_builddir)/engines/support/libsupport.la
