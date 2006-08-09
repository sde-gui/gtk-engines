/******************************************************************************/
/* smooth_gtk2_drawing.h - Drawing Functions for GTK2 Engine                  */
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
#include <math.h>
#include <string.h>

GE_INTERNAL void
smooth_draw_shadow(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height);
	    
GE_INTERNAL void
smooth_draw_hline(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   gint x1,
	   gint x2,
	   gint y);
	   
GE_INTERNAL void
smooth_draw_vline(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   gint y1,
	   gint y2,
	   gint x);

GE_INTERNAL void
smooth_draw_polygon(GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail,
	     GdkPoint * points,
	     gint npoints,
	     gint fill);

GE_INTERNAL void
smooth_draw_arrow(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GtkShadowType shadow_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   GtkArrowType arrow_type,
	   gint fill,
	   gint x,
	   gint y,
	   gint width,
	   gint height);

GE_INTERNAL void
smooth_draw_focus(GtkStyle *style,
           GdkWindow *window,
           GtkStateType state_type,
           GdkRectangle *area,
           GtkWidget *widget,
           const gchar *detail,
           gint x,
           gint y,
           gint width,
           gint height);

GE_INTERNAL void
smooth_draw_slider(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height,
	    GtkOrientation orientation);

GE_INTERNAL void
smooth_draw_extension(GtkStyle * style,
	       GdkWindow * window,
	       GtkStateType state_type,
	       GtkShadowType shadow_type,
	       GdkRectangle * area,
	       GtkWidget * widget,
	       const gchar * detail,
	       gint x,
	       gint y,
	       gint width,
	       gint height,
	       GtkPositionType gap_side);

GE_INTERNAL void
smooth_draw_box(GtkStyle * style,
	 GdkWindow * window,
	 GtkStateType state_type,
	 GtkShadowType shadow_type,
	 GdkRectangle * area,
	 GtkWidget * widget,
	 const gchar * detail,
	 gint x,
	 gint y,
	 gint width,
	 gint height);

GE_INTERNAL void
smooth_draw_box_gap (GtkStyle * style, 
              GdkWindow * window, 
	      GtkStateType state_type, 
	      GtkShadowType shadow_type, 
	      GdkRectangle * area, 
	      GtkWidget * widget, 
	      const gchar * detail, 
	      gint x, 
	      gint y, 
	      gint width, 
	      gint height, 
	      GtkPositionType gap_side, 
	      gint gap_pos, 
	      gint gap_width);

GE_INTERNAL void
smooth_draw_shadow_gap (GtkStyle * style, 
                 GdkWindow * window, 
		 GtkStateType state_type, 
		 GtkShadowType shadow_type, 
		 GdkRectangle * area, 
		 GtkWidget * widget, 
		 const gchar * detail, 
		 gint x, 
		 gint y, 
		 gint width, 
		 gint height, 
		 GtkPositionType gap_side, 
		 gint gap_pos, 
		 gint gap_width);

GE_INTERNAL void
smooth_draw_option(GtkStyle * style,
            GdkWindow * window,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GdkRectangle * area,
            GtkWidget * widget,
            const gchar *detail,
            gint X,
            gint Y,
            gint Width,
            gint Height);

GE_INTERNAL void
smooth_draw_check(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GtkShadowType shadow_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   gint x,
	   gint y,
	   gint width,
	   gint height);

GE_INTERNAL void
smooth_draw_diamond(GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail,
	     gint x,
	     gint y,
	     gint width,
	     gint height);

GE_INTERNAL void
smooth_draw_handle(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height,
	    GtkOrientation orientation);
   
GE_INTERNAL void
smooth_draw_resize_grip (GtkStyle       *style,
                              GdkWindow      *window,
                              GtkStateType    state_type,
                              GdkRectangle   *area,
                              GtkWidget      *widget,
                              const gchar    *detail,
                              GdkWindowEdge   edge,
                              gint            x,
                              gint            y,
                              gint            width,
                              gint            height);

GE_INTERNAL void
smooth_draw_tab (GtkStyle      *style,
		      GdkWindow     *window,
		      GtkStateType   state_type,
		      GtkShadowType  shadow_type,
		      GdkRectangle  *area,
		      GtkWidget     *widget,
		      const gchar   *detail,
		      gint           x,
		      gint           y,
		      gint           width,
		      gint           height);

GE_INTERNAL void 
smooth_draw_flat_box (GtkStyle * style,
               GdkWindow * window,
	       GtkStateType state_type,
	       GtkShadowType shadow_type,
	       GdkRectangle * area,
	       GtkWidget * widget,
	       const gchar * detail,
	       gint x, gint y, gint width, gint height);                                        
