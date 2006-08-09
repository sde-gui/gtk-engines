/******************************************************************************/
/* smooth_gtk_drawing.h - Shared drawing functions for all GTK+ versions      */
/******************************************************************************/
/* Smooth Theme Engine                                                        */
/* Copyright (C) 2002-2004 Andrew Johnson                                     */
/*                                                                            */
/* This library is free software; you can redistribute it and/or              */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 2.1 of the License, or (at your option) any later version.         */
/*                                                                            */
/* This library is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with this library; if not, write to the Free Software        */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  */
/*                                                                            */
/* Author(s): Andrew Johnson <ajgenius@ajgenius.us>                           */
/******************************************************************************/
/* Portions Based on the EnGradient Engine                                    */
/*   Andrew Cattau                                                            */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
			    
#define FLAT_FILL_BACKGROUND(canvas, style, state_type, part, x, y, width, height) (smooth_fill_background(canvas, style, state_type, GTK_SHADOW_NONE, part, x, y, width, height, FALSE, FALSE, FALSE,FALSE, FALSE))		
#define gradient_fill_background(canvas, style, state_type, part, x, y, width, height, invert, horizontal) (smooth_fill_background(canvas, style, state_type, GTK_SHADOW_NONE, part, x, y, width, height, TRUE, invert, horizontal,FALSE, FALSE))

GE_INTERNAL void 
smooth_fill_background(SmoothCanvas Canvas,
			GtkStyle * style,
	               GtkStateType state_type,
		       GtkShadowType shadow_type,
		       void * part,
	               gint x,
	               gint y,
	               gint width,
	               gint height,
		       gboolean use_gradient,  
	               gboolean invert,
		       gboolean horizontal,
		       gboolean arc_fill,
		       gboolean input_widget);

GE_INTERNAL void
smooth_parent_fill_background(SmoothCanvas Canvas,
				GtkStyle * style,
				GtkStateType state_type,
				GtkShadowType shadow_type,
				GtkWidget *widget,
				void * part,
				gint x,
				gint y,
				gint width,
				gint height,
				gboolean use_gradient,  
				gboolean invert,
				gboolean horizontal);

GE_INTERNAL void 
smooth_draw_grip(SmoothCanvas Canvas,

		 GtkStyle * style,
                 GtkStateType state_type,

	         SmoothInt X,
	         SmoothInt Y,
	         SmoothInt Width,
	         SmoothInt Height,
	         SmoothBool Horizontal);

GE_INTERNAL void
do_smooth_draw_shadow(SmoothCanvas Canvas,

			GtkStyle * style,
			GtkStateType state_type,
			GtkShadowType shadow_type,
			
			GtkWidget * widget,
			const gchar * detail,

			SmoothInt x,
			SmoothInt y,
			SmoothInt width,
			SmoothInt height);

GE_INTERNAL void
smooth_draw_button_default(SmoothCanvas Canvas,

				GtkStyle *style,
				GtkStateType state_type,

				GdkRectangle *button,
				GtkWidget *widget,

				gint x,
				gint y,
				gint width,
				gint height);
