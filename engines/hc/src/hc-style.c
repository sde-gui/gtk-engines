/*
 * HighContrast GTK+ rendering engine for Gnome-Themes.
 *
 * Copyright 2003 Sun Microsystems Inc.
 *
 * Also contains code from GTK+, 
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
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
 */

#include "hc-style.h"
#include "hc-rc-style.h"

#include <gtk/gtk.h>

#include <math.h>
#include <string.h>

static GtkStyleClass *parent_class = NULL;

static const GtkRequisition default_option_indicator_size = { 7, 13 };
static const GtkBorder default_option_indicator_spacing = { 7, 5, 2, 2 };

static void
hc_draw_dot (cairo_t *cr,
	  CairoColor *light,
	  CairoColor *dark,
	  gdouble x,
	  gdouble y)
{
	cairo_save(cr);

	cairo_set_line_width (cr, 0.5);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

	ge_cairo_set_color(cr, light);	

	cairo_rectangle (cr, x - 1, y, 0.5, 0.5);
	cairo_rectangle (cr, x - 1, y - 1, 0.5, 0.5);
	cairo_rectangle (cr, x, y - 1, 0.5, 0.5);

	cairo_stroke(cr);
	
	ge_cairo_set_color(cr, dark);	

	cairo_rectangle (cr, x + 1, y, 0.5, 0.5);
	cairo_rectangle (cr, x + 1, y + 1, 0.5, 0.5);
	cairo_rectangle (cr, x, y + 1, 0.5, 0.5);

	cairo_stroke(cr);

	cairo_restore(cr);
}

static void hc_draw_line (cairo_t *cr,
			CairoColor *color,
			gdouble thickness,
			gdouble x1,
			gdouble y1,
			gdouble x2,
			gdouble y2)
{ 
	cairo_save(cr);

	ge_cairo_set_color(cr, color);	
	cairo_set_line_width (cr, thickness);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);

	cairo_stroke(cr);

	cairo_restore(cr);
}

/***********************************************
 * hc_arrow-
 *  
 *    borrowed from redmond till shareable
 ***********************************************/
void hc_arrow (cairo_t *cr,
			CairoColor *color,
			GtkArrowType arrow_type, 
			gint x, 
			gint y, 
			gint width, 
			gint height)
{
  gint i;
 
  gint steps, extra;
  gint start, increment;
  gint aw = width, ah = height;
 	
  if ((arrow_type == GTK_ARROW_UP) || (arrow_type == GTK_ARROW_DOWN))
    {
      gdouble tmp=((aw+1)/2) - ((height%2)?1:0);
       
      if (tmp > ah) 
        {
          aw = 2*ah - 1 - ((height%2)?1:0);
          ah = (aw+1)/2;
        } 
      else 
        {
          ah = (gint) tmp;
          aw = 2*ah - 1;
        }  
 
      if ((aw < 5) || (ah < 3)) 
        {
          aw = 5;
          ah = 3;
        }
 
      x += (width - aw) / 2 ;
      y += (height - ah) / 2;
      width = aw;
      height = ah;
 		
      width += width % 2 - 1;
 
      steps = 1 + width / 2;
      extra = height - steps;
 
      if (arrow_type == GTK_ARROW_DOWN)
        {
          start = y;
          increment = 1;
        }
      else
        {
          start = y + height - 1;
          increment = -1;
        }
 
      cairo_save(cr);

      ge_cairo_set_color(cr, color);	
      cairo_set_line_width (cr, 0.5);

      cairo_move_to(cr, x + 0.5, start + 0.5);
      cairo_line_to(cr, x + width - 0.5, start + 0.5);
      cairo_line_to(cr, x + ((height - 1) - extra) + 0.5, start + (height - 1)*increment + 0.5);
      cairo_line_to(cr, x + 0.5, start + 0.5);

      cairo_stroke(cr);


      cairo_move_to(cr, x + 0.5, start + 0.5);
      cairo_line_to(cr, x + width - 0.5, start + 0.5);
      cairo_line_to(cr, x + ((height - 1) - extra) + 0.5, start + (height - 1)*increment + 0.5);
      cairo_line_to(cr, x + 0.5, start + 0.5);

      cairo_fill(cr);

      cairo_restore(cr);
    }
  else
    {
      gdouble tmp=((ah+1)/2) - ((width%2)?1:0);
 
      if (tmp > aw) 
        {
          ah = 2*aw - 1 - ((width%2)?1:0);
          aw = (ah+1)/2;
        } 
      else 
        {
          aw = (gint) tmp;
          ah = 2*aw - 1;
        }  
 
      if ((ah < 5) || (aw < 3)) 
        {
          ah = 5;
          aw = 3;
        }
 
      x += (width - aw) / 2 ;
      y += (height - ah) / 2;
      width = aw;
      height = ah;
 
      height += height % 2 - 1;
 
      steps = 1 + height / 2;
      extra = width - steps;
 
      if (arrow_type == GTK_ARROW_RIGHT)
        {
          start = x;
          increment = 1;
        }
      else
        {
          start = x + width - 1;
          increment = -1;
        }
 
      cairo_save(cr);

      ge_cairo_set_color(cr, color);	
      cairo_set_line_width (cr, 0.5);

      cairo_move_to(cr, start + 0.5, y + 0.5);
      cairo_line_to(cr, start + 0.5, y + height - 0.5);
      cairo_line_to(cr, start + (width - 1)*increment + 0.5, y + ((width - 1) - extra) + 0.5);
      cairo_line_to(cr, start + 0.5, y + 0.5);

      cairo_stroke(cr);


      cairo_move_to(cr, start + 0.5, y + 0.5);
      cairo_line_to(cr, start + 0.5, y + height - 0.5);
      cairo_line_to(cr, start + (width - 1)*increment + 0.5, y + ((width - 1) - extra) + 0.5);
      cairo_line_to(cr, start + 0.5, y + 0.5);

      cairo_fill(cr);

      cairo_restore(cr);
    }
}

static void
hc_style_realize (GtkStyle *style)
{
  HcStyle *hc_style = HC_STYLE (style);

  GdkGCValues gc_values;
  GdkGCValuesMask gc_values_mask;
  
  gint i;

  for (i = 0; i < 5; i++)
    {
      style->light[i].red = MIN (65535, style->bg[i].red + 0.5 * 65535);
      style->light[i].green = MIN (65535, style->bg[i].green + 0.5 * 65535);
      style->light[i].blue = MIN (65535, style->bg[i].blue + 0.5 * 65535);

      style->dark[i].red = MAX (0, style->bg[i].red - 0.5 * 65535);
      style->dark[i].green = MAX (0, style->bg[i].green - 0.5 * 65535);
      style->dark[i].blue = MAX (0, style->bg[i].blue - 0.5 * 65535);

      style->mid[i].red = (style->light[i].red + style->dark[i].red) / 2;
      style->mid[i].green = (style->light[i].green + style->dark[i].green) / 2;
      style->mid[i].blue = (style->light[i].blue + style->dark[i].blue) / 2;

      style->text_aa[i].red = (style->text[i].red + style->base[i].red) / 2;
      style->text_aa[i].green = (style->text[i].green + style->base[i].green) / 2;
      style->text_aa[i].blue = (style->text[i].blue + style->base[i].blue) / 2;
    }

  style->black.red = 0x0000;
  style->black.green = 0x0000;
  style->black.blue = 0x0000;
  gdk_colormap_alloc_color (style->colormap, &style->black, FALSE, TRUE);

  style->white.red = 0xffff;
  style->white.green = 0xffff;
  style->white.blue = 0xffff;
  gdk_colormap_alloc_color (style->colormap, &style->white, FALSE, TRUE);

  gc_values_mask = GDK_GC_FOREGROUND;
  
  gc_values.foreground = style->black;
  style->black_gc = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
  
  gc_values.foreground = style->white;
  style->white_gc = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
  
  for (i = 0; i < 5; i++)
    {
      if (!gdk_colormap_alloc_color (style->colormap, &style->fg[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->fg[i].red, style->fg[i].green, style->fg[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->bg[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->bg[i].red, style->bg[i].green, style->bg[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->light[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->light[i].red, style->light[i].green, style->light[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->dark[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->dark[i].red, style->dark[i].green, style->dark[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->mid[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->mid[i].red, style->mid[i].green, style->mid[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->text[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->text[i].red, style->text[i].green, style->text[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->base[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->base[i].red, style->base[i].green, style->base[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->text_aa[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->text_aa[i].red, style->text_aa[i].green, style->text_aa[i].blue);
      
      gc_values.foreground = style->fg[i];
      style->fg_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->bg[i];
      style->bg_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->light[i];
      style->light_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->dark[i];
      style->dark_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->mid[i];
      style->mid_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->text[i];
      style->text_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->base[i];
      style->base_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);

      gc_values.foreground = style->text_aa[i];
      style->text_aa_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
    }

  ge_gtk_style_to_cairo_color_cube (style, &hc_style->color_cube);
}

static void
hc_draw_shadow(GtkStyle * style,
	       GdkWindow * window,
	       GtkStateType state_type,
	       GtkShadowType shadow_type,
	       GdkRectangle * area,
	       GtkWidget * widget,
	       const gchar * detail,
	       gint x,
	       gint y,
	       gint width,
	       gint height)
{
	HcStyle *hc_style = HC_STYLE (style);

	CHECK_ARGS

	SANITIZE_SIZE

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

	cairo_set_line_width (cr, MIN(style->xthickness,style->ythickness)/2);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);

	ge_cairo_set_color(cr, &hc_style->color_cube.fg[state_type]);	

	cairo_rectangle (cr, x+0.5, y+0.5, width - 1, height - 1);

	cairo_stroke(cr);

	cairo_destroy(cr);
}


static void 
hc_draw_extension (GtkStyle       *style,
		   GdkWindow      *window,
		   GtkStateType    state_type,
		   GtkShadowType   shadow_type,
		   GdkRectangle   *area,
		   GtkWidget      *widget,
		   const gchar    *detail,
		   gint            x,
		   gint            y,
		   gint            width,
		   gint            height,
		   GtkPositionType gap_side)
{
  GdkGC *gc1 = NULL;
  GdkGC *gc2 = NULL;
  GdkGC *gc3 = NULL;
  GdkGC *gc4 = NULL;
  
  CHECK_ARGS
  
  gtk_style_apply_default_background (style, window,
                                      widget && !GTK_WIDGET_NO_WINDOW (widget),
                                      GTK_STATE_NORMAL, area, x, y, width, height);
  
  SANITIZE_SIZE
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
      gc1 = style->dark_gc[state_type];
      gc2 = style->fg_gc[state_type];
      gc3 = style->bg_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_OUT:
      gc1 = style->light_gc[state_type];
      gc2 = style->bg_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->fg_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->dark_gc[state_type];
      break;
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, area);
      gdk_gc_set_clip_rectangle (gc2, area);
      gdk_gc_set_clip_rectangle (gc3, area);
      gdk_gc_set_clip_rectangle (gc4, area);
    }

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
    case GTK_SHADOW_IN:
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
      switch (gap_side)
        {
        case GTK_POS_TOP:
          gtk_style_apply_default_background (style, window,
                                              widget && !GTK_WIDGET_NO_WINDOW (widget),
                                              state_type, area,
                                              x + style->xthickness, 
                                              y, 
                                              width - (2 * style->xthickness), 
                                              height - (style->ythickness));
          gdk_draw_line (window, gc1,
                         x, y, x, y + height - 2);
          gdk_draw_line (window, gc2,
                         x + 1, y, x + 1, y + height - 2);
          
          gdk_draw_line (window, gc3,
                         x + 2, y + height - 2, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc3,
                         x + width - 2, y, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc4,
                         x + 1, y + height - 1, x + width - 2, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y, x + width - 1, y + height - 2);
          break;
        case GTK_POS_BOTTOM:
          gtk_style_apply_default_background (style, window,
                                              widget && !GTK_WIDGET_NO_WINDOW (widget),
                                              state_type, area,
                                              x + style->xthickness, 
                                              y + style->ythickness, 
                                              width - (2 * style->xthickness), 
                                              height - (style->ythickness));
          gdk_draw_line (window, gc1,
                         x + 1, y, x + width - 2, y);
          gdk_draw_line (window, gc1,
                         x, y + 1, x, y + height - 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + width - 2, y + 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + 1, y + height - 1);
          
          gdk_draw_line (window, gc3,
                         x + width - 2, y + 2, x + width - 2, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y + 1, x + width - 1, y + height - 1);
          break;
        case GTK_POS_LEFT:
          gtk_style_apply_default_background (style, window,
                                              widget && !GTK_WIDGET_NO_WINDOW (widget),
                                              state_type, area,
                                              x, 
                                              y + style->ythickness, 
                                              width - (style->xthickness), 
                                              height - (2 * style->ythickness));
          gdk_draw_line (window, gc1,
                         x, y, x + width - 2, y);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + width - 2, y + 1);
          
          gdk_draw_line (window, gc3,
                         x, y + height - 2, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc3,
                         x + width - 2, y + 2, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc4,
                         x, y + height - 1, x + width - 2, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y + 1, x + width - 1, y + height - 2);
          break;
        case GTK_POS_RIGHT:
          gtk_style_apply_default_background (style, window,
                                              widget && !GTK_WIDGET_NO_WINDOW (widget),
                                              state_type, area,
                                              x + style->xthickness, 
                                              y + style->ythickness, 
                                              width - (style->xthickness), 
                                              height - (2 * style->ythickness));
          gdk_draw_line (window, gc1,
                         x + 1, y, x + width - 1, y);
          gdk_draw_line (window, gc1,
                         x, y + 1, x, y + height - 2);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + width - 1, y + 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + 1, y + height - 2);
          
          gdk_draw_line (window, gc3,
                         x + 2, y + height - 2, x + width - 1, y + height - 2);
          gdk_draw_line (window, gc4,
                         x + 1, y + height - 1, x + width - 1, y + height - 1);
          break;
        }
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, NULL);
      gdk_gc_set_clip_rectangle (gc2, NULL);
      gdk_gc_set_clip_rectangle (gc3, NULL);
      gdk_gc_set_clip_rectangle (gc4, NULL);
    }
}

static void 
hc_draw_check (GtkStyle      *style,
	       GdkWindow     *window,
	       GtkStateType   state_type,
	       GtkShadowType  shadow_type,
	       GdkRectangle  *area,
	       GtkWidget     *widget,
	       const gchar   *detail,
	       gint           x,
	       gint           y,
	       gint           width,
	       gint           height)
{
	HcStyle *hc_style = HC_STYLE (style);
	gint line_width = 1;
	gboolean invert_checkboxes = FALSE; /* TODO: read this from RC file in engine-specific data? */

	SANITIZE_SIZE

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

	gtk_widget_style_get (widget,
				"focus-line-width", &line_width,
				NULL);

	cairo_save(cr);

	cairo_set_line_width (cr, 1.5);
	
	ge_cairo_set_color(cr, &hc_style->color_cube.base[state_type]);	

	cairo_rectangle (cr, x, y, width, height);

	cairo_fill(cr);

	ge_cairo_set_color(cr, &hc_style->color_cube.fg[state_type]);	

	cairo_rectangle (cr, x, y, width, height);

	cairo_stroke(cr);

	cairo_restore(cr);

	if (shadow_type == GTK_SHADOW_IN)
	{
		ge_cairo_set_color(cr, &hc_style->color_cube.fg[state_type]);	

		gint off_1, off_2;
		gint line_width = ceil(((width + 1)/5));
	
		if (!(line_width % 2)) 
			line_width -= 1;

		cairo_set_line_width (cr, line_width + 1);
		cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);

		off_2 = line_width + 2.5;
		off_1 = off_2;

		/* Backward Diagonal */
		cairo_move_to(cr, x+off_1, y+off_1);
		cairo_line_to(cr, x+width-off_2, y+height-off_2);
		
		cairo_stroke (cr);

		/* Forward Diagonal */
		cairo_move_to(cr, x+off_1, y+height - off_2);
		cairo_line_to(cr, x+width-off_2, y+off_1);

		cairo_stroke (cr);
	}
	else if (shadow_type == GTK_SHADOW_ETCHED_IN) /* inconsistent */
	{
		/* Rough logic, just make  thickness 1/3 height */
		int line_width = ceil((height + 1)/3);

		/* Force Thickness Even */
		line_width -= (line_width % 2);

		cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
		cairo_set_line_width (cr, line_width + ((height % 2)?1:0));

		cairo_move_to(cr, x + line_width - ((height % 2)?0.5:0), y + floor(height/2));
		cairo_line_to(cr, x + width - line_width + ((height % 2)?0.5:0), y + floor(height/2));

		ge_cairo_set_color(cr, &hc_style->color_cube.text[state_type]);	
		cairo_stroke (cr);
	}

	cairo_destroy(cr);
}

static void 
hc_draw_option (GtkStyle      *style,
		GdkWindow     *window,
		GtkStateType   state_type,
		GtkShadowType  shadow_type,
		GdkRectangle  *area,
		GtkWidget     *widget,
		const gchar   *detail,
		gint           x,
		gint           y,
		gint           width,
		gint           height)
{
	HcStyle *hc_style = HC_STYLE (style);

	SANITIZE_SIZE

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

	cairo_set_line_width (cr, 1.5);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
	
	cairo_arc(cr, x + width/2.0, y + height/2.0, width/2.0, 0 , 2 * M_PI);
	ge_cairo_set_color(cr, &hc_style->color_cube.bg[state_type]);	
	cairo_fill (cr);

	cairo_arc(cr, x + width/2.0, y + height/2.0, width/2.0, 0 , 2 * M_PI);
	ge_cairo_set_color(cr, &hc_style->color_cube.fg[state_type]);	
	cairo_stroke (cr);

	if (shadow_type == GTK_SHADOW_IN)
	{
		ge_cairo_set_color(cr, &hc_style->color_cube.fg[state_type]);	
		cairo_arc(cr, x + width/2.0, y + height/2.0, (width - 7)/2.0, 0 , 2 * M_PI);
		cairo_fill (cr);
	}
	else if (shadow_type == GTK_SHADOW_ETCHED_IN) /* inconsistent */
	{
		/* Rough logic, just make  thickness 1/3 height */
		int line_width = ceil((height + 1)/3);

		/* Force Thickness Even */
		line_width -= (line_width % 2);

		cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
		cairo_set_line_width (cr, line_width + ((height % 2)?1:0));

		cairo_move_to(cr, x + line_width - ((height % 2)?0.5:0), y + floor(height/2));
		cairo_line_to(cr, x + width - line_width + ((height % 2)?0.5:0), y + floor(height/2));

		ge_cairo_set_color(cr, &hc_style->color_cube.text[state_type]);	
		cairo_stroke (cr);
	}

	cairo_destroy(cr);
}

static void
option_menu_get_props (GtkWidget      *widget,
		       GtkRequisition *indicator_size,
		       GtkBorder      *indicator_spacing)
{
  GtkRequisition *tmp_size = NULL;
  GtkBorder *tmp_spacing = NULL;
  
  if (widget)
    gtk_widget_style_get (widget, 
			  "indicator_size", &tmp_size,
			  "indicator_spacing", &tmp_spacing,
			  NULL);

  if (tmp_size)
    {
      *indicator_size = *tmp_size;
      g_free (tmp_size);
    }
  else
    *indicator_size = default_option_indicator_size;

  if (tmp_spacing)
    {
      *indicator_spacing = *tmp_spacing;
      g_free (tmp_spacing);
    }
  else
    *indicator_spacing = default_option_indicator_spacing;
}

static void
hc_draw_tab (GtkStyle      *style,
	     GdkWindow     *window,
	     GtkStateType   state_type,
	     GtkShadowType  shadow_type,
	     GdkRectangle  *area,
	     GtkWidget     *widget,
	     const gchar   *detail,
	     gint           x,
	     gint           y,
	     gint           width,
	     gint           height)
{
#define ARROW_SPACE 4
  
  HcStyle *hc_style = HC_STYLE (style);
  cairo_t *cr = ge_gdk_drawable_to_cairo(window, area);

  GtkRequisition indicator_size;
  GtkBorder indicator_spacing;
  gint arrow_height;
  
  option_menu_get_props (widget, &indicator_size, &indicator_spacing);

  indicator_size.width += (indicator_size.width % 2) - 1;
  arrow_height = indicator_size.width / 2 + 1;

  x += (width - indicator_size.width) / 2;
  y += (height - (2 * arrow_height + ARROW_SPACE)) / 2;

  hc_arrow (cr, &hc_style->color_cube.fg[state_type],
	      GTK_ARROW_UP, x, y,
	      indicator_size.width, arrow_height);
  
  
  hc_arrow (cr, &hc_style->color_cube.fg[state_type],
	      GTK_ARROW_DOWN, x, y + arrow_height + ARROW_SPACE,
	      indicator_size.width, arrow_height);

  cairo_destroy(cr);
}

static void 
hc_draw_shadow_gap (GtkStyle       *style,
		    GdkWindow      *window,
		    GtkStateType    state_type,
		    GtkShadowType   shadow_type,
		    GdkRectangle   *area,
		    GtkWidget      *widget,
		    const gchar    *detail,
		    gint            x,
		    gint            y,
		    gint            width,
		    gint            height,
		    GtkPositionType gap_side,
		    gint            gap_x,
		    gint            gap_width)
{
  GdkGC *gc1 = NULL;
  GdkGC *gc2 = NULL;
  GdkGC *gc3 = NULL;
  GdkGC *gc4 = NULL;
  
  CHECK_ARGS
  
  SANITIZE_SIZE
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
      gc1 = style->dark_gc[state_type];
      gc2 = style->fg_gc[state_type];
      gc3 = style->bg_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_OUT:
      gc1 = style->light_gc[state_type];
      gc2 = style->bg_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->fg_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->dark_gc[state_type];
      break;
    }
  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, area);
      gdk_gc_set_clip_rectangle (gc2, area);
      gdk_gc_set_clip_rectangle (gc3, area);
      gdk_gc_set_clip_rectangle (gc4, area);
    }
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
    case GTK_SHADOW_IN:
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
      switch (gap_side)
        {
        case GTK_POS_TOP:
          gdk_draw_line (window, gc1,
                         x, y, x, y + height - 1);
          gdk_draw_line (window, gc2,
                         x + 1, y, x + 1, y + height - 2);
          
          gdk_draw_line (window, gc3,
                         x + 1, y + height - 2, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc3,
                         x + width - 2, y, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc4,
                         x, y + height - 1, x + width - 1, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              gdk_draw_line (window, gc1,
                             x, y, x + gap_x - 1, y);
              gdk_draw_line (window, gc2,
                             x + 1, y + 1, x + gap_x - 1, y + 1);
              gdk_draw_line (window, gc2,
                             x + gap_x, y, x + gap_x, y);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              gdk_draw_line (window, gc1,
                             x + gap_x + gap_width, y, x + width - 2, y);
              gdk_draw_line (window, gc2,
                             x + gap_x + gap_width, y + 1, x + width - 3, y + 1);
              gdk_draw_line (window, gc2,
                             x + gap_x + gap_width - 1, y, x + gap_x + gap_width - 1, y);
            }
          break;
        case GTK_POS_BOTTOM:
          gdk_draw_line (window, gc1,
                         x, y, x + width - 1, y);
          gdk_draw_line (window, gc1,
                         x, y, x, y + height - 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + width - 2, y + 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + 1, y + height - 1);
          
          gdk_draw_line (window, gc3,
                         x + width - 2, y + 1, x + width - 2, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              gdk_draw_line (window, gc4,
                             x, y + height - 1, x + gap_x - 1, y + height - 1);
              gdk_draw_line (window, gc3,
                             x + 1, y + height - 2, x + gap_x - 1, y + height - 2);
              gdk_draw_line (window, gc3,
                             x + gap_x, y + height - 1, x + gap_x, y + height - 1);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              gdk_draw_line (window, gc4,
                             x + gap_x + gap_width, y + height - 1, x + width - 2, y + height - 1);
              gdk_draw_line (window, gc3,
                             x + gap_x + gap_width, y + height - 2, x + width - 2, y + height - 2);
              gdk_draw_line (window, gc3,
                             x + gap_x + gap_width - 1, y + height - 1, x + gap_x + gap_width - 1, y + height - 1);
            }
          break;
        case GTK_POS_LEFT:
          gdk_draw_line (window, gc1,
                         x, y, x + width - 1, y);
          gdk_draw_line (window, gc2,
                         x, y + 1, x + width - 2, y + 1);
          
          gdk_draw_line (window, gc3,
                         x, y + height - 2, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc3,
                         x + width - 2, y + 1, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc4,
                         x, y + height - 1, x + width - 1, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              gdk_draw_line (window, gc1,
                             x, y, x, y + gap_x - 1);
              gdk_draw_line (window, gc2,
                             x + 1, y + 1, x + 1, y + gap_x - 1);
              gdk_draw_line (window, gc2,
                             x, y + gap_x, x, y + gap_x);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              gdk_draw_line (window, gc1,
                             x, y + gap_x + gap_width, x, y + height - 2);
              gdk_draw_line (window, gc2,
                             x + 1, y + gap_x + gap_width, x + 1, y + height - 2);
              gdk_draw_line (window, gc2,
                             x, y + gap_x + gap_width - 1, x, y + gap_x + gap_width - 1);
            }
          break;
        case GTK_POS_RIGHT:
          gdk_draw_line (window, gc1,
                         x, y, x + width - 1, y);
          gdk_draw_line (window, gc1,
                         x, y, x, y + height - 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + width - 1, y + 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + 1, y + height - 2);
          
          gdk_draw_line (window, gc3,
                         x + 1, y + height - 2, x + width - 1, y + height - 2);
          gdk_draw_line (window, gc4,
                         x, y + height - 1, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              gdk_draw_line (window, gc4,
                             x + width - 1, y, x + width - 1, y + gap_x - 1);
              gdk_draw_line (window, gc3,
                             x + width - 2, y + 1, x + width - 2, y + gap_x - 1);
              gdk_draw_line (window, gc3,
                             x + width - 1, y + gap_x, x + width - 1, y + gap_x);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              gdk_draw_line (window, gc4,
                             x + width - 1, y + gap_x + gap_width, x + width - 1, y + height - 2);
              gdk_draw_line (window, gc3,
                             x + width - 2, y + gap_x + gap_width, x + width - 2, y + height - 2);
              gdk_draw_line (window, gc3,
                             x + width - 1, y + gap_x + gap_width - 1, x + width - 1, y + gap_x + gap_width - 1);
            }
          break;
        }
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, NULL);
      gdk_gc_set_clip_rectangle (gc2, NULL);
      gdk_gc_set_clip_rectangle (gc3, NULL);
      gdk_gc_set_clip_rectangle (gc4, NULL);
    }
}

static void
hc_draw_layout (GtkStyle        *style,
		GdkWindow       *window,
		GtkStateType     state_type,
		gboolean         use_text,
		GdkRectangle    *area,
		GtkWidget       *widget,
		const gchar     *detail,
		gint             x,
		gint             y,
		PangoLayout     *layout)
{
  GdkGC *gc;
  
  CHECK_ARGS

  gc = use_text ? style->text_gc[state_type] : style->fg_gc[state_type];
  
  if (area)
    gdk_gc_set_clip_rectangle (gc, area);

  gdk_draw_layout (window, gc, x, y, layout);

  if (area)
    gdk_gc_set_clip_rectangle (gc, NULL);
}

static void
hc_draw_arrow (GtkStyle      *style,
	       GdkWindow     *window,
	       GtkStateType   state,
	       GtkShadowType  shadow,
	       GdkRectangle  *area,
	       GtkWidget     *widget,
	       const gchar   *detail,
	       GtkArrowType   arrow_type,
	       gboolean       fill,
	       gint           x,
	       gint           y,
	       gint           width,
	       gint           height)
{
  gint original_width, original_x;
  
  SANITIZE_SIZE

  HcStyle *hc_style = HC_STYLE (style);
  cairo_t *cr = ge_gdk_drawable_to_cairo(window, area);

  original_width = width;
  original_x = x;

  if (detail && strcmp (detail, "menuitem") == 0
      && gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR)
    x = original_x + original_width - width;

  hc_arrow (cr, &hc_style->color_cube.fg[state], arrow_type,
	      x, y, width+1, height+1);

  cairo_destroy(cr);
}


static void
hc_draw_flat_box (GtkStyle	*style,
                  GdkWindow	*window,
                  GtkStateType	 state_type,
                  GtkShadowType	 shadow_type,
                  GdkRectangle	*area,
                  GtkWidget	*widget,
                  const gchar	*detail,
                  gint		 x,
                  gint		 y,
                  gint		 width,
                  gint		 height)
{
  if (detail && !strcmp ("tooltip", detail))
    gtk_paint_box (style, window, state_type, shadow_type, area,
                   widget, detail, x, y, width, height);
  else
    parent_class->draw_flat_box (style, window, state_type, shadow_type, area,
                                 widget, detail, x, y, width, height);
}

static void 
hc_draw_box_gap (GtkStyle       *style,
		 GdkWindow      *window,
		 GtkStateType    state_type,
		 GtkShadowType   shadow_type,
		 GdkRectangle   *area,
		 GtkWidget      *widget,
		 const gchar    *detail,
		 gint            x,
		 gint            y,
		 gint            width,
		 gint            height,
		 GtkPositionType gap_side,
		 gint            gap_x,
		 gint            gap_width)
{
  GdkGC *gc1 = NULL;
  GdkGC *gc2 = NULL;
  GdkGC *gc3 = NULL;
  GdkGC *gc4 = NULL;
  
  CHECK_ARGS
  
  gtk_style_apply_default_background (style, window,
                                      widget && !GTK_WIDGET_NO_WINDOW (widget),
                                      state_type, area, x, y, width, height);
  
  SANITIZE_SIZE
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
      gc1 = style->dark_gc[state_type];
      gc2 = style->fg_gc[state_type];
      gc3 = style->bg_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_OUT:
      gc1 = style->light_gc[state_type];
      gc2 = style->bg_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->fg_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->dark_gc[state_type];
      break;
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, area);
      gdk_gc_set_clip_rectangle (gc2, area);
      gdk_gc_set_clip_rectangle (gc3, area);
      gdk_gc_set_clip_rectangle (gc4, area);
    }
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
    case GTK_SHADOW_IN:
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
      switch (gap_side)
        {
        case GTK_POS_TOP:
          gdk_draw_line (window, gc1,
                         x, y, x, y + height - 1);
          gdk_draw_line (window, gc2,
                         x + 1, y, x + 1, y + height - 2);
          
          gdk_draw_line (window, gc3,
                         x + 1, y + height - 2, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc3,
                         x + width - 2, y, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc4,
                         x, y + height - 1, x + width - 1, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              gdk_draw_line (window, gc1,
                             x, y, x + gap_x - 1, y);
              gdk_draw_line (window, gc2,
                             x + 1, y + 1, x + gap_x - 1, y + 1);
              gdk_draw_line (window, gc2,
                             x + gap_x, y, x + gap_x, y);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              gdk_draw_line (window, gc1,
                             x + gap_x + gap_width, y, x + width - 2, y);
              gdk_draw_line (window, gc2,
                             x + gap_x + gap_width, y + 1, x + width - 2, y + 1);
              gdk_draw_line (window, gc2,
                             x + gap_x + gap_width - 1, y, x + gap_x + gap_width - 1, y);
            }
          break;
        case  GTK_POS_BOTTOM:
          gdk_draw_line (window, gc1,
                         x, y, x + width - 1, y);
          gdk_draw_line (window, gc1,
                         x, y, x, y + height - 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + width - 2, y + 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + 1, y + height - 1);
          
          gdk_draw_line (window, gc3,
                         x + width - 2, y + 1, x + width - 2, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              gdk_draw_line (window, gc4,
                             x, y + height - 1, x + gap_x - 1, y + height - 1);
              gdk_draw_line (window, gc3,
                             x + 1, y + height - 2, x + gap_x - 1, y + height - 2);
              gdk_draw_line (window, gc3,
                             x + gap_x, y + height - 1, x + gap_x, y + height - 1);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              gdk_draw_line (window, gc4,
                             x + gap_x + gap_width, y + height - 1, x + width - 2, y + height - 1);
              gdk_draw_line (window, gc3,
                             x + gap_x + gap_width, y + height - 2, x + width - 2, y + height - 2);
              gdk_draw_line (window, gc3,
                             x + gap_x + gap_width - 1, y + height - 1, x + gap_x + gap_width - 1, y + height - 1);
            }
          break;
        case GTK_POS_LEFT:
          gdk_draw_line (window, gc1,
                         x, y, x + width - 1, y);
          gdk_draw_line (window, gc2,
                         x, y + 1, x + width - 2, y + 1);
          
          gdk_draw_line (window, gc3,
                         x, y + height - 2, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc3,
                         x + width - 2, y + 1, x + width - 2, y + height - 2);
          gdk_draw_line (window, gc4,
                         x, y + height - 1, x + width - 1, y + height - 1);
          gdk_draw_line (window, gc4,
                         x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              gdk_draw_line (window, gc1,
                             x, y, x, y + gap_x - 1);
              gdk_draw_line (window, gc2,
                             x + 1, y + 1, x + 1, y + gap_x - 1);
              gdk_draw_line (window, gc2,
                             x, y + gap_x, x, y + gap_x);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              gdk_draw_line (window, gc1,
                             x, y + gap_x + gap_width, x, y + height - 2);
              gdk_draw_line (window, gc2,
                             x + 1, y + gap_x + gap_width, x + 1, y + height - 2);
              gdk_draw_line (window, gc2,
                             x, y + gap_x + gap_width - 1, x, y + gap_x + gap_width - 1);
            }
          break;
        case GTK_POS_RIGHT:
          gdk_draw_line (window, gc1,
                         x, y, x + width - 1, y);
          gdk_draw_line (window, gc1,
                         x, y, x, y + height - 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + width - 1, y + 1);
          gdk_draw_line (window, gc2,
                         x + 1, y + 1, x + 1, y + height - 2);
          
          gdk_draw_line (window, gc3,
                         x + 1, y + height - 2, x + width - 1, y + height - 2);
          gdk_draw_line (window, gc4,
                         x, y + height - 1, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              gdk_draw_line (window, gc4,
                             x + width - 1, y, x + width - 1, y + gap_x - 1);
              gdk_draw_line (window, gc3,
                             x + width - 2, y + 1, x + width - 2, y + gap_x - 1);
              gdk_draw_line (window, gc3,
                             x + width - 1, y + gap_x, x + width - 1, y + gap_x);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              gdk_draw_line (window, gc4,
                             x + width - 1, y + gap_x + gap_width, x + width - 1, y + height - 2);
              gdk_draw_line (window, gc3,
                             x + width - 2, y + gap_x + gap_width, x + width - 2, y + height - 2);
              gdk_draw_line (window, gc3,
                             x + width - 1, y + gap_x + gap_width - 1, x + width - 1, y + gap_x + gap_width - 1);
            }
          break;
        }
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, NULL);
      gdk_gc_set_clip_rectangle (gc2, NULL);
      gdk_gc_set_clip_rectangle (gc3, NULL);
      gdk_gc_set_clip_rectangle (gc4, NULL);
    }
}

static void 
hc_draw_handle (GtkStyle      *style,
		GdkWindow     *window,
		GtkStateType   state_type,
		GtkShadowType  shadow_type,
		GdkRectangle  *area,
		GtkWidget     *widget,
		const gchar   *detail,
		gint           x,
		gint           y,
		gint           width,
		gint           height,
		GtkOrientation orientation)
{
  gdouble xx, yy;
  gint xthick, ythick;
  CairoColor *light, *dark;
  GdkRectangle rect;
  GdkRectangle dest;
  gint intersect;
  
  CHECK_ARGS
  
  SANITIZE_SIZE
  
  gtk_paint_box (style, window, state_type, shadow_type, area, widget, 
                 detail, x, y, width, height);
  
  
  if (!strcmp (detail, "paned"))
    {
      /* we want to ignore the shadow border in paned widgets */
      xthick = 0;
      ythick = 0;
    }
  else
    {
      xthick = style->xthickness;
      ythick = style->ythickness;
    }
  
  HcStyle *hc_style = HC_STYLE (style);

  light = &hc_style->color_cube.light[state_type];
  dark = &hc_style->color_cube.dark[state_type];

  rect.x = x + xthick;
  rect.y = y + ythick;
  rect.width = width - (xthick * 2);
  rect.height = height - (ythick * 2);

  if (area)
      intersect = gdk_rectangle_intersect (area, &rect, &dest);
  else
    {
      intersect = TRUE;
      dest = rect;
    }

  if (!intersect)
    return;

  cairo_t *cr = ge_gdk_drawable_to_cairo (window, &dest);

  if (!strcmp (detail, "paned"))
    {
      if (orientation == GTK_ORIENTATION_HORIZONTAL)
	for (xx = x + width/2.0 - 15; xx <= x + width/2.0 + 15; xx += 5)
	  hc_draw_dot (cr, light, dark, xx, y + height/2.0);
      else
	for (yy = y + height/2 - 15; yy <= y + height/2.0 + 15; yy += 5)
	  hc_draw_dot (cr, light, dark, x + width/2.0, yy);
    }
  else
    {
      for (yy = y + ythick; yy < (y + height - ythick); yy += 3)
	for (xx = x + xthick; xx < (x + width - xthick); xx += 6)
	  {
	    hc_draw_dot (cr, light, dark, xx, yy);
	    hc_draw_dot (cr, light, dark, xx + 3, yy + 1);
	  }
    }

  cairo_destroy(cr);
}

static void
hc_draw_hline (GtkStyle     *style,
	       GdkWindow    *window,
	       GtkStateType  state_type,
	       GdkRectangle  *area,
	       GtkWidget     *widget,
	       const gchar   *detail,
	       gint          x1,
	       gint          x2,
	       gint          y)
{
  HcStyle *hc_style = HC_STYLE (style);

  CHECK_ARGS
  
  cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
  
  gint line_width = style->ythickness/2;

  hc_draw_line (cr, &hc_style->color_cube.fg[state_type], (detail && !strcmp (detail, "label"))?1:2*line_width - 1, 
			x1 + line_width + 2, y + style->ythickness/2 + 0.5, 
			x2 - line_width - 1, y + style->ythickness/2 + 0.5);     
  
  cairo_destroy(cr);
}

static void
hc_draw_vline (GtkStyle     *style,
	       GdkWindow    *window,
	       GtkStateType  state_type,
	       GdkRectangle  *area,
	       GtkWidget     *widget,
	       const gchar   *detail,
	       gint          y1,
	       gint          y2,
	       gint          x)
{
  HcStyle *hc_style = HC_STYLE (style);

  CHECK_ARGS
  
  cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
  
  gint line_width = style->xthickness/2;

  hc_draw_line (cr, &hc_style->color_cube.fg[state_type],  (detail && !strcmp (detail, "label"))?1:2*line_width - 1, 
			x + style->xthickness/2 + 0.5, y1, 
			x + style->xthickness/2 + 0.5, y2);     

  cairo_destroy(cr);
}

static void
hc_draw_diamond (GtkStyle      *style,
		 GdkWindow     *window,
		 GtkStateType   state_type,
		 GtkShadowType  shadow_type,
		 GdkRectangle  *area,
		 GtkWidget     *widget,
		 const gchar   *detail,
		 gint           x,
		 gint           y,
		 gint           width,
		 gint           height)
{
  gint half_width;
  gint half_height;
  GdkGC *outer_nw = NULL;
  GdkGC *outer_ne = NULL;
  GdkGC *outer_sw = NULL;
  GdkGC *outer_se = NULL;
  GdkGC *middle_nw = NULL;
  GdkGC *middle_ne = NULL;
  GdkGC *middle_sw = NULL;
  GdkGC *middle_se = NULL;
  GdkGC *inner_nw = NULL;
  GdkGC *inner_ne = NULL;
  GdkGC *inner_sw = NULL;
  GdkGC *inner_se = NULL;
  
  CHECK_ARGS
  
  SANITIZE_SIZE
  
  half_width = width / 2;
  half_height = height / 2;
  
  if (area)
    {
      gdk_gc_set_clip_rectangle (style->light_gc[state_type], area);
      gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);
      gdk_gc_set_clip_rectangle (style->dark_gc[state_type], area);
      gdk_gc_set_clip_rectangle (style->fg_gc[state_type], area);
    }
  
  switch (shadow_type)
    {
    case GTK_SHADOW_IN:
      inner_sw = inner_se = style->bg_gc[state_type];
      middle_sw = middle_se = style->light_gc[state_type];
      outer_sw = outer_se = style->light_gc[state_type];
      inner_nw = inner_ne = style->fg_gc[state_type];
      middle_nw = middle_ne = style->dark_gc[state_type];
      outer_nw = outer_ne = style->dark_gc[state_type];
      break;
          
    case GTK_SHADOW_OUT:
      inner_sw = inner_se = style->dark_gc[state_type];
      middle_sw = middle_se = style->dark_gc[state_type];
      outer_sw = outer_se = style->fg_gc[state_type];
      inner_nw = inner_ne = style->bg_gc[state_type];
      middle_nw = middle_ne = style->light_gc[state_type];
      outer_nw = outer_ne = style->light_gc[state_type];
      break;

    case GTK_SHADOW_ETCHED_IN:
      inner_sw = inner_se = style->bg_gc[state_type];
      middle_sw = middle_se = style->dark_gc[state_type];
      outer_sw = outer_se = style->light_gc[state_type];
      inner_nw = inner_ne = style->bg_gc[state_type];
      middle_nw = middle_ne = style->light_gc[state_type];
      outer_nw = outer_ne = style->dark_gc[state_type];
      break;

    case GTK_SHADOW_ETCHED_OUT:
      inner_sw = inner_se = style->bg_gc[state_type];
      middle_sw = middle_se = style->light_gc[state_type];
      outer_sw = outer_se = style->dark_gc[state_type];
      inner_nw = inner_ne = style->bg_gc[state_type];
      middle_nw = middle_ne = style->dark_gc[state_type];
      outer_nw = outer_ne = style->light_gc[state_type];
      break;
      
    default:

      break;
    }

  if (inner_sw)
    {
      gdk_draw_line (window, inner_sw,
                     x + 2, y + half_height,
                     x + half_width, y + height - 2);
      gdk_draw_line (window, inner_se,
                     x + half_width, y + height - 2,
                     x + width - 2, y + half_height);
      gdk_draw_line (window, middle_sw,
                     x + 1, y + half_height,
                     x + half_width, y + height - 1);
      gdk_draw_line (window, middle_se,
                     x + half_width, y + height - 1,
                     x + width - 1, y + half_height);
      gdk_draw_line (window, outer_sw,
                     x, y + half_height,
                     x + half_width, y + height);
      gdk_draw_line (window, outer_se,
                     x + half_width, y + height,
                     x + width, y + half_height);
  
      gdk_draw_line (window, inner_nw,
                     x + 2, y + half_height,
                     x + half_width, y + 2);
      gdk_draw_line (window, inner_ne,
                     x + half_width, y + 2,
                     x + width - 2, y + half_height);
      gdk_draw_line (window, middle_nw,
                     x + 1, y + half_height,
                     x + half_width, y + 1);
      gdk_draw_line (window, middle_ne,
                     x + half_width, y + 1,
                     x + width - 1, y + half_height);
      gdk_draw_line (window, outer_nw,
                     x, y + half_height,
                     x + half_width, y);
      gdk_draw_line (window, outer_ne,
                     x + half_width, y,
                     x + width, y + half_height);
    }
  
  if (area)
    {
      gdk_gc_set_clip_rectangle (style->light_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle (style->dark_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle (style->fg_gc[state_type], NULL);
    }
}

static void
hc_draw_polygon (GtkStyle      *style,
		 GdkWindow     *window,
		 GtkStateType   state_type,
		 GtkShadowType  shadow_type,
		 GdkRectangle  *area,
		 GtkWidget     *widget,
		 const gchar   *detail,
		 GdkPoint      *points,
		 gint           npoints,
		 gboolean       fill)
{
  static const gdouble pi_over_4 = G_PI_4;
  static const gdouble pi_3_over_4 = G_PI_4 * 3;
  GdkGC *gc1;
  GdkGC *gc2;
  GdkGC *gc3;
  GdkGC *gc4;
  gdouble angle;
  gint xadjust;
  gint yadjust;
  gint i;
  
  CHECK_ARGS
  g_return_if_fail (points != NULL);
  
  switch (shadow_type)
    {
    case GTK_SHADOW_IN:
      gc1 = style->bg_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->fg_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_OUT:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->fg_gc[state_type];
      gc4 = style->bg_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->dark_gc[state_type];
      break;
    default:
      return;
    }
  
  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, area);
      gdk_gc_set_clip_rectangle (gc2, area);
      gdk_gc_set_clip_rectangle (gc3, area);
      gdk_gc_set_clip_rectangle (gc4, area);
    }
  
  if (fill)
    gdk_draw_polygon (window, style->bg_gc[state_type], TRUE, points, npoints);
  
  npoints--;
  
  for (i = 0; i < npoints; i++)
    {
      if ((points[i].x == points[i+1].x) &&
          (points[i].y == points[i+1].y))
        {
          angle = 0;
        }
      else
        {
          angle = atan2 (points[i+1].y - points[i].y,
                         points[i+1].x - points[i].x);
        }
      
      if ((angle > -pi_3_over_4) && (angle < pi_over_4))
        {
          if (angle > -pi_over_4)
            {
              xadjust = 0;
              yadjust = 1;
            }
          else
            {
              xadjust = 1;
              yadjust = 0;
            }
          
          gdk_draw_line (window, gc1,
                         points[i].x-xadjust, points[i].y-yadjust,
                         points[i+1].x-xadjust, points[i+1].y-yadjust);
          gdk_draw_line (window, gc3,
                         points[i].x, points[i].y,
                         points[i+1].x, points[i+1].y);
        }
      else
        {
          if ((angle < -pi_3_over_4) || (angle > pi_3_over_4))
            {
              xadjust = 0;
              yadjust = 1;
            }
          else
            {
              xadjust = 1;
              yadjust = 0;
            }
          
          gdk_draw_line (window, gc4,
                         points[i].x+xadjust, points[i].y+yadjust,
                         points[i+1].x+xadjust, points[i+1].y+yadjust);
          gdk_draw_line (window, gc2,
                         points[i].x, points[i].y,
                         points[i+1].x, points[i+1].y);
        }
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, NULL);
      gdk_gc_set_clip_rectangle (gc2, NULL);
      gdk_gc_set_clip_rectangle (gc3, NULL);
      gdk_gc_set_clip_rectangle (gc4, NULL);
    }
}

static void
hc_style_class_init (HcStyleClass *klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  style_class->draw_shadow = hc_draw_shadow;
  style_class->realize = hc_style_realize;
  style_class->draw_check = hc_draw_check;
  style_class->draw_option = hc_draw_option;
  style_class->draw_layout = hc_draw_layout;
  style_class->draw_extension = hc_draw_extension;
  style_class->draw_tab = hc_draw_tab;
  style_class->draw_shadow_gap = hc_draw_shadow_gap;
  style_class->draw_arrow = hc_draw_arrow;
  style_class->draw_flat_box = hc_draw_flat_box;
  style_class->draw_box_gap = hc_draw_box_gap;
  style_class->draw_handle = hc_draw_handle;  
  style_class->draw_hline = hc_draw_hline; 
  style_class->draw_vline = hc_draw_vline; 
  style_class->draw_diamond = hc_draw_diamond;
  style_class->draw_polygon = hc_draw_polygon;
}

GType hc_type_style = 0;

void
hc_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (HcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) hc_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (HcStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
  
  hc_type_style = g_type_module_register_type (module,
							   GTK_TYPE_STYLE,
							   "HcStyle",
							   &object_info, 0);
}
