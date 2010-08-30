/* Redmond95 - a cairo based GTK+ engine
 * Copyright (C) 2001 Red Hat, Inc.
 * Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Project contact: <gnome-themes-list@gnome.org>
 *
 *
 * Written by Owen Taylor <otaylor@redhat.com>
 * modified by Andrew Johnson <ajgenius@ajgenius.us>
 *
 */


#include <math.h>
#include <string.h>
 
#include <cairo.h>

GE_INTERNAL void
redmond_draw_hline (GtkStyle * style,
	    cairo_t * cr,
	    GtkStateType state_type,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint x1,
            gint x2, 
            gint y);
 
GE_INTERNAL void
redmond_draw_vline (GtkStyle * style,
	    cairo_t * cr,
	    GtkStateType state_type,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint y1, 
            gint y2, 
            gint x);
 
GE_INTERNAL void
redmond_draw_check (GtkStyle * style,
	    cairo_t * cr,
	    GtkStateType state,
	    GtkShadowType shadow,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint x, 
            gint y, 
            gint width, 
            gint height);
 
GE_INTERNAL void
redmond_draw_option (GtkStyle * style,
	     cairo_t * cr,
	     GtkStateType state,
	     GtkShadowType shadow,
	     GtkWidget * widget,
	     const gchar * detail, 
             gint x,
             gint y, 
             gint width, 
             gint height);
 
GE_INTERNAL void
redmond_draw_arrow (GtkStyle * style,
	    cairo_t * cr,
	    GtkStateType state,
	    GtkShadowType shadow,
	    GtkWidget * widget,
	    const gchar * detail,
	    GtkArrowType arrow_type,
	    gboolean fill, 
            gint x, 
            gint y, 
            gint width, 
            gint height);
 
GE_INTERNAL void
redmond_draw_shadow (GtkStyle * style,
	     cairo_t * cr,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GtkWidget * widget,
	     const gchar * detail, 
             gint x, 
             gint y, 
             gint width, 
             gint height);
 
GE_INTERNAL void
redmond_draw_box (GtkStyle * style,
	  cairo_t * cr,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GtkWidget * widget,
	  const gchar * detail, 
          gint x, 
          gint y, 
          gint width, 
          gint height);
 
GE_INTERNAL void
redmond_draw_tab (GtkStyle * style,
	  cairo_t * cr,
	  GtkStateType state,
	  GtkShadowType shadow,
	  GtkWidget * widget,
	  const gchar * detail, 
          gint x, 
          gint y, 
          gint width, 
          gint height);
 
GE_INTERNAL void
redmond_draw_slider (GtkStyle * style,
	     cairo_t * cr,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GtkWidget * widget,
	     const gchar * detail,
	     gint x,
	     gint y, 
             gint width, 
             gint height, 
             GtkOrientation orientation);
 
GE_INTERNAL void
redmond_draw_extension (GtkStyle * style,
		cairo_t * cr,
		GtkStateType state_type,
		GtkShadowType shadow_type,
		GtkWidget * widget,
		const gchar * detail,
		gint x,
		gint y, 
                gint width, 
                gint height, 
                GtkPositionType gap_side);
                
GE_INTERNAL void
redmond_draw_handle (GtkStyle * style,
	     cairo_t * cr,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GtkWidget * widget,
	     const gchar * detail,
	     gint x,
	     gint y, 
             gint width, 
             gint height, 
             GtkOrientation orientation);
