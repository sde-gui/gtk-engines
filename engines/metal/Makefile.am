INCLUDES = $(GTK_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines

engine_LTLIBRARIES = libmetal.la

libmetal_la_SOURCES = 		\
	./src/metal_gtk2_misc.c	\
	./src/metal_gtk2_misc.h	\
	./src/metal_gtk2_drawing.c	\
	./src/metal_gtk2_drawing.h	\
	./src/metal_gtk2_engine.c	\
	./src/metal_gtk2_engine.h

libmetal_la_LDFLAGS = -module -avoid-version -no-undefined
libmetal_la_LIBADD = $(GTK_LIBS)

