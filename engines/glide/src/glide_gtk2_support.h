/* Glide theme engine
 * Copyright (C) 2006 Andrew Johnson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Written by Andrew Johnson <acjgenius@earthlink.net>
 */  

#include <math.h>

void 
glide_simple_border_gap_clip(cairo_t *canvas,
				gint x,
				gint y,
				gint width,
				gint height,

				GtkPositionType gap_side,
				gint gap_pos,
				gint gap_size);
void ge_cairo_pattern_add_shade_color_stop(cairo_pattern_t *pattern, gdouble offset, CairoColor *color, gdouble shade);
void ge_cairo_pattern_add_color_stop(cairo_pattern_t *pattern, gdouble offset, CairoColor *color);

void
do_glide_draw_default_fill (GtkStyle *style,
                              GdkWindow *window,
                              GtkWidget *widget,
                              GtkStateType state_type, 
                              GdkRectangle *area, 
                              gint x, 
                              gint y, 
                              gint width, 
                              gint height,
                              gboolean vertical,
                              gboolean flat);

typedef enum {
	GLIDE_BEVEL_STYLE_SMOOTH = 1,

	GLIDE_BEVEL_STYLE_THINICE,

	GLIDE_BEVEL_STYLE_REDMOND,

	GLIDE_BEVEL_STYLE_FLAT,

	GLIDE_BEVEL_STYLE_NONE,

	/* Default */
	GLIDE_BEVEL_STYLE_DEFAULT = GLIDE_BEVEL_STYLE_SMOOTH
} GlideBevelStyle;

typedef enum {
	GLIDE_BORDER_TYPE_IN, 
	GLIDE_BORDER_TYPE_OUT, 
	GLIDE_BORDER_TYPE_ETCHED, 
	GLIDE_BORDER_TYPE_ENGRAVED, 
	GLIDE_BORDER_TYPE_NONE
} GlideBorderType;

void do_glide_draw_border(cairo_t *canvas,
                              CairoColor *base,
                              GlideBevelStyle border_style,
                              GlideBorderType border_type,
                              gint x,
                              gint y,
                              gint width,
                              gint height);

void do_glide_draw_check (cairo_t *cr,
                            CairoColor *color, 
                            gint x, 
                            gint y, 
                            gint width, 
                            gint height);
                
void
do_glide_draw_simple_circle (cairo_t *canvas,
                     	  		CairoColor * tl,
                       			CairoColor * br,
					gint center_x, 
					gint center_y, 
					gint radius);
            
void do_glide_draw_arrow (cairo_t *cr,
                            CairoColor *color,
                            GtkArrowType arrow_type, 
                            gint x, 
                            gint y, 
                            gint width, 
                            gint height);
                            
void do_glide_draw_line(cairo_t *cr, 
                          CairoColor * dark, 
                          CairoColor * light, 
                          GdkRectangle * area, 
                          gint start, 
                          gint end, 
                          gint base, 
                          gboolean horizontal);
 
void
do_glide_draw_grip (cairo_t *canvas,
			CairoColor * light,
			CairoColor * dark,
			gint x,
			gint y,
			gint width,
			gint height,
			gboolean vertical);

void gtk_menu_shell_setup_signals(GtkWidget *widget);
void gtk_menu_shell_cleanup_signals(GtkWidget *widget);
