INCLUDES = 								\
	-I$(top_srcdir)/engines/smooth/src				\
	$(GTK_CFLAGS)							\
	-DINTERFACE_DEFINES="<interfaces/gdk2/gdk2_type_defines.h>" 	\
	-DENGINE_HEADER="<engine/smooth_gtk2_engine.h>"			\
	-DENGINE_RC_HEADER="<engine/smooth_gtk2_rc.h>"			\
	-DENGINE_MISC_HEADER="<engine/smooth_gtk2_misc.h>"

AM_CFLAGS =								\
	$(GTK_CFLAGS) 							\
	-DG_DISABLE_DEPRECATED 						\
	-DGDK_DISABLE_DEPRECATED 					\
	-DGDK_PIXBUF_DISABLE_DEPRECATED					\
	-DGTK_DISABLE_DEPRECATED

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libsmooth.la

EXTRA_DIST =								\
	./src/interfaces/drawing_interface.h				\
	./src/interfaces/abstract/abstract_drawing_interface.h		\
	./src/interfaces/abstract/abstract_type_defines.h		\
	./src/interfaces/abstract/src/abstract_gradient_routines.c	\
	./src/interfaces/abstract/src/abstract_gradient_routines.h	\
	./src/interfaces/abstract/src/abstract_tile_routines.c		\
	./src/interfaces/abstract/src/abstract_tile_routines.h		\
	./src/interfaces/shared/gdk/gdk_drawing_interface.c		\
	./src/interfaces/shared/gdk/gdk_drawing_interface.h		\
	./src/interfaces/shared/gdk/gdk_private_drawing_interface.h	\
	./src/interfaces/shared/gdk/gdk_type_defines.h			\
	./src/interfaces/gdk2/gdk2_drawing_interface.h			\
	./src/interfaces/gdk2/gdk2_private_drawing_interface.h		\
	./src/interfaces/gdk2/gdk2_type_defines.h			\
	./src/interfaces/gdk2/src/gdk2_gradient_routines.c		\
	./src/interfaces/gdk2/src/gdk2_gradient_routines.h		\
	./src/utils/calc_colors.h					\
	./src/utils/draw_fill.h						\
	./src/utils/draw_border.h					\
	./src/utils/draw_grips.h					\
	./src/utils/draw_arrows.h					\
	./src/utils/draw_checkmark.h					\
        ./src/utils/smooth_parts.h					\
	./src/engine/shared/gtk/smooth_gtk_misc.h			\
	./src/engine/shared/gtk/smooth_gtk_drawing.h			\
	./src/engine/shared/gtk/smooth_gtk_rc.h				\
	./src/engine/smooth_gtk2_misc.h					\
	./src/engine/smooth_gtk2_drawing.h				\
	./src/engine/smooth_gtk2_engine.h				\
	./src/engine/smooth_gtk2_rc.h					\
	./src/config.h.in						\
	./AUTHORS

libsmooth_la_SOURCES =							\
	./src/interfaces/drawing_interface.c				\
	./src/interfaces/abstract/abstract_drawing_interface.c		\
	./src/interfaces/shared/gdk/gdk_drawing_interface.c		\
        ./src/interfaces/gdk2/gdk2_drawing_interface.c			\
	./src/interfaces/gdk2/gdk2_drawing_interface.h			\
	./src/utils/calc_colors.c					\
	./src/utils/draw_fill.c						\
	./src/utils/draw_border.c					\
	./src/utils/draw_grips.c					\
	./src/utils/draw_arrows.c					\
	./src/utils/draw_checkmark.c					\
	./src/engine/shared/gtk/smooth_gtk_misc.c			\
	./src/engine/shared/gtk/smooth_gtk_drawing.c			\
	./src/engine/shared/gtk/smooth_gtk_rc.c				\
	./src/engine/smooth_gtk2_misc.c					\
	./src/engine/smooth_gtk2_drawing.c				\
	./src/engine/smooth_gtk2_engine.c				\
	./src/engine/smooth_gtk2_rc.c

libsmooth_la_LDFLAGS= -module -avoid-version -no-undefined
libsmooth_la_LIBADD = $(GTK_LIBS)
