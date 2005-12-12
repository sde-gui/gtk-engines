/* Metal theme engine
 * Copyright (C) 2001 Red Hat, Inc.
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
 * Written by Owen Taylor <otaylor@redhat.com>
 */
 
#include "metal_gtk2_engine.h"
#include "metal_gtk2_drawing.h"
#include "metal_gtk2_misc.h"

/* Widget Type Lookups/Macros
   
   Based on/modified from functions in
   Smooth-Engine.
*/ 
gboolean
object_is_a (GtkWidget * widget, gchar * type_name)
{
  gboolean result = FALSE;
 
  if ((widget))
    {
      GType tmp = g_type_from_name (type_name);
      if (tmp)
	result = g_type_check_instance_is_a ((GTypeInstance *) widget, tmp);
    }
 
  return result;
}
 
gboolean
is_combo_box_entry (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (IS_COMBO_BOX_ENTRY (widget->parent))
	result = TRUE;
      else
	result = is_combo_box_entry (widget->parent);
    }
  return result;
}
 
gboolean
combo_box_is_using_list (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (IS_COMBO_BOX (widget->parent)))
    {
      gboolean *tmp = NULL;
 
      gtk_widget_style_get (widget, "appears-as-list", &result, NULL);
 
      if (tmp)
	result = *tmp;
    }
 
  return result;
}
 
gboolean
is_combo_box (GtkWidget * widget, gboolean as_list)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (IS_COMBO_BOX (widget->parent))
	result = ((!as_list) || combo_box_is_using_list(widget->parent));
      else
	result = is_combo_box (widget->parent, as_list);
    }
  return result;
}
 
gboolean
is_combo (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (IS_COMBO (widget->parent))
	result = TRUE;
      else
	result = is_combo (widget->parent);
    }
  return result;
}
 
gboolean
is_in_combo_box (GtkWidget * widget)
{
  return ((is_combo (widget) || is_combo_box (widget, TRUE) || is_combo_box_entry (widget)) && (!is_combo_box (widget, FALSE)));
}

/***********************************************
 * sanitize_parameters -
 *  
 *   Ensure Size and Values are valid
 *
 *   Taken from Smooth based on Metal
 ***********************************************/
gboolean
sanitize_parameters(GtkStyle * style,
  	            GdkWindow * window,
	            gint * width,
	            gint * height)
{
  if (!style) return FALSE;
  if (!window) return FALSE;
 
  if ((width) && (height)) {
    if ((*width == -1) && (*height == -1))
      gdk_drawable_get_size (window, width, height);
    else if (*width == -1)
      gdk_drawable_get_size(window, width, NULL);
    else if (*height == -1)
      gdk_drawable_get_size (window, NULL, height);  
  }
  return TRUE;  
}
 
/* This function makes up for some brokeness in gtkrange.c
 * where we never get the full arrow of the stepper button
 * and the type of button in a single drawing function.
 *
 * It doesn't work correctly when the scrollbar is squished
 * to the point we don't have room for full-sized steppers.
 */
void
reverse_engineer_stepper_box (GtkWidget    *range,
			      GtkArrowType  arrow_type,
			      gint         *x,
			      gint         *y,
			      gint         *width,
			      gint         *height)
{
  gint slider_width = 17, stepper_size = 15;
  gint box_width;
  gint box_height;
  
  if (range)
    {
      gtk_widget_style_get (range,
			    "slider_width", &slider_width,
			    "stepper_size", &stepper_size,
			    NULL);
    }
	
  if (arrow_type == GTK_ARROW_UP || arrow_type == GTK_ARROW_DOWN)
    {
      box_width = slider_width;
      box_height = stepper_size;
    }
  else
    {
      box_width = stepper_size;
      box_height = slider_width;
    }

  *x = *x - (box_width - *width) / 2;
  *y = *y - (box_height - *height) / 2;
  *width = box_width;
  *height = box_height;
}

/***********************************************
 * option_menu_get_props -
 *  
 *   Find Option Menu Size and Spacing
 *
 *   Taken from Smooth
 ***********************************************/ 
static GtkRequisition default_option_indicator_size = { 9, 5 }; 
static GtkBorder default_option_indicator_spacing = { 7, 5, 2, 2 }; 
 
void
option_menu_get_props (GtkWidget * widget,
		       GtkRequisition * indicator_size,
		       GtkBorder * indicator_spacing)
{
  GtkRequisition *tmp_size = NULL;
  GtkBorder *tmp_spacing = NULL;
 
  if ((widget) && IS_OPTION_MENU(widget))
    gtk_widget_style_get (widget,
			  "indicator_size", &tmp_size,
			  "indicator_spacing", &tmp_spacing, NULL);
 
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

void
do_metal_draw_arrow (GdkWindow * window,
               GtkWidget  * widget,
               GdkGC * gc,
               GtkArrowType arrow_type,
               gint x, 
               gint y, 
               gint width, 
               gint height)
{
  int base, span, xoffset, yoffset;
  int i;

  switch (arrow_type)
    {
    case GTK_ARROW_UP:
      base = width;
      /* if (base % 2 == 0)
        	base--;
      */
      xoffset = (width - base) / 2;
      span = (base + 1) / 2;
      yoffset = (height + span) / 2 - 1;
      for (i = 0; i < span; i++)
	{
	  gdk_draw_line (window, gc, x + xoffset + i, y + yoffset - i,
			 x + xoffset + base - 1 - i, y + yoffset - i);
	}
      break;
    case GTK_ARROW_DOWN:
      base = width;
      /* if (base % 2 == 0)
         base--;
      */
      xoffset = (width - base) / 2;
      span = (base + 1) / 2;
      yoffset = (height - span) / 2;
      for (i = 0; i < span; i++)
	{
	  gdk_draw_line (window, gc, x + xoffset + i, y + yoffset + i,
			 x + xoffset + base - 1 - i, y + yoffset + i);
	}
      break;
    case GTK_ARROW_RIGHT:
      if (GTK_CHECK_TYPE (widget, gtk_menu_item_get_type ()))
	{
	  base = 7;
	}
      else
	{
	  base = height;
	  /* if (base % 2 == 0)
	  // 	    base--;
	  */
	}
      yoffset = (height - base) / 2;
      span = (base + 1) / 2;
      xoffset = (width - span) / 2;
      for (i = 0; i < span; i++)
	{
	  gdk_draw_line (window, gc, x + xoffset + i, y + yoffset + i,
			 x + xoffset + i, y + yoffset + base - 1 - i);
	}
      break;
    case GTK_ARROW_LEFT:
      base = height;
      /*      if (base % 2 == 0)
      //	base--;
      */
      yoffset = (height - base) / 2;
      span = (base + 1) / 2;
      xoffset = (width + span) / 2 - 1;
      for (i = 0; i < span; i++)
	{
	  gdk_draw_line (window, gc, x + xoffset - i, y + yoffset + i,
			 x + xoffset - i, y + yoffset + base - 1 - i);
	}
      break;
    }
}

void
do_metal_draw_scrollbar_trough (GtkStyle     *style,
			GdkWindow    *window,
			GtkStateType  state_type,
			GtkShadowType shadow_type,
			GdkRectangle *area,
			GtkWidget    *widget,
			const char   *detail,
			gint          x,
			gint          y,
			gint          width,
			gint          height)
{
  MetalStyle *metal_style = METAL_STYLE (style);
  GdkGC *lightgc, *midgc, *darkgc, *whitegc;
  gint stepper_size = 15;

  if (widget && GTK_IS_RANGE (widget))
    gtk_widget_style_get (widget, "stepper_size", &stepper_size, NULL);

  stepper_size += 2;

  /* Get colors */
  lightgc = metal_style->light_gray_gc;
  midgc = metal_style->mid_gray_gc;
  darkgc = metal_style->dark_gray_gc;
  whitegc = style->white_gc;

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (midgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
      gdk_gc_set_clip_rectangle (whitegc, area);
    }

  /* Draw backgound */
  gdk_draw_rectangle (window, lightgc, TRUE, x, y, width, height);

  /* Draw border */
  gdk_draw_rectangle (window, darkgc, FALSE, x, y, width - 2, height - 2);
  
  /* Draw inset shadow */
  if (GTK_CHECK_TYPE (widget, gtk_hscrollbar_get_type ()))
    {
      gdk_draw_line (window, whitegc, 
		     x + 1,         y + height - 1,
		     x + width - 1, y + height - 1);
      gdk_draw_line (window, darkgc, 
		     x + stepper_size - 2, y + 2, 
		     x + stepper_size - 2, y + height - 2);      
      gdk_draw_line (window, midgc, 
		     x + stepper_size - 1,         y + 1, 
		     x + width - stepper_size - 1, y + 1);
      gdk_draw_line (window, darkgc, 
		     x + width - stepper_size, y + 2, 
		     x + width - stepper_size, y + height - 2);
      gdk_draw_line (window, whitegc, 
		     x + width - stepper_size + 1, y + 1, 
		     x + width - stepper_size + 1, y + height - 2);
      gdk_draw_line (window, midgc,
		     x + stepper_size - 1, y + 1, 
		     x + stepper_size - 1, y + height - 3);
    }
  else
    {
      gdk_draw_line (window, whitegc, 
		     x + width - 1, y + 1,
		     x + width - 1, y + height - 1);
      gdk_draw_line (window, darkgc, 
		     x + 2,         y + stepper_size - 2, 
		     x + width - 2, y + stepper_size - 2);      
      gdk_draw_line (window, midgc, 
		     x + 1, y + stepper_size - 1, 
		     x + 1, y + height - stepper_size - 1);
      gdk_draw_line (window, darkgc, 
		     x + 2,         y + height - stepper_size, 
		     x + width - 2, y + height - stepper_size);
      gdk_draw_line (window, whitegc, 
		     x + 1,         y + height - stepper_size + 1, 
		     x + width - 2, y + height - stepper_size + 1);
      gdk_draw_line (window, midgc,
		     x + 1,         y + stepper_size - 1, 
		     x + width - 3, y + stepper_size - 1);
    }

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (midgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
      gdk_gc_set_clip_rectangle (whitegc, NULL);
    }
}

void
do_metal_draw_scale_trough (GtkStyle * style,
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
  MetalStyle *metal_style = METAL_STYLE (style);
  GdkGC *lightgc, *midgc, *darkgc, *whitegc;

  /* Get colors */
  lightgc = metal_style->light_gray_gc;
  midgc = style->bg_gc[GTK_STATE_SELECTED];
  darkgc = metal_style->mid_gray_gc;
  whitegc = style->white_gc;

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (midgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
      gdk_gc_set_clip_rectangle (whitegc, area);
    }

  if (GTK_CHECK_TYPE (widget, gtk_hscale_get_type ()))
    {
      /* Draw backgound */
      gdk_draw_rectangle (window, midgc, TRUE, x, y + 3, width - 2, 9);

      /* Draw border */
      gdk_draw_rectangle (window, darkgc, FALSE, x, y + 3, width - 2, 7);
      gdk_draw_rectangle (window, whitegc, FALSE, x + 1, y + 4, width - 2, 7);
    }
  else
    {
      /* Draw backgound */
      gdk_draw_rectangle (window, midgc, TRUE, x + 3, y, 9, height - 2);

      /* Draw border */
      gdk_draw_rectangle (window, darkgc, FALSE, x + 3, y, 7, height - 2);
      gdk_draw_rectangle (window, whitegc, FALSE, x + 4, y + 1, 7, height - 2);
    }

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (midgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
      gdk_gc_set_clip_rectangle (whitegc, NULL);
    }
}

void
do_metal_draw_menu (GtkStyle * style,
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
  GdkGC *midgc, *whitegc;

  midgc = style->bg_gc[GTK_STATE_SELECTED];
  whitegc = style->white_gc;

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (midgc, area);
      gdk_gc_set_clip_rectangle (whitegc, area);
    }

  gdk_draw_rectangle (window, whitegc, FALSE, x + 1, y + 1, width - 2,
		      height - 2);
  gdk_draw_rectangle (window, midgc, FALSE, x, y, width - 1, height - 1);

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (midgc, NULL);
      gdk_gc_set_clip_rectangle (whitegc, NULL);
    }
}

void
do_metal_draw_menu_item (GtkStyle * style,
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
  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_SELECTED], area);
      gdk_gc_set_clip_rectangle (style->dark_gc[GTK_STATE_SELECTED], area);
      gdk_gc_set_clip_rectangle (style->light_gc[GTK_STATE_SELECTED], area);
    }

  gdk_draw_rectangle (window, style->bg_gc[GTK_STATE_SELECTED], TRUE, x, y,
		      width, height);
  gdk_draw_line (window, style->dark_gc[GTK_STATE_SELECTED], x, y, x +
		 width, y);
  gdk_draw_line (window, style->light_gc[GTK_STATE_SELECTED], x, y + height
		 - 1, x + width, y + height - 1);

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_SELECTED], NULL);
      gdk_gc_set_clip_rectangle (style->dark_gc[GTK_STATE_SELECTED], NULL);
      gdk_gc_set_clip_rectangle (style->light_gc[GTK_STATE_SELECTED], NULL);
    }
}

void
do_metal_draw_notebook (GtkStyle * style,
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
  MetalStyle *metal_style = METAL_STYLE (style);
  GdkGC *lightgc, *midgc, *darkgc, *whitegc;

  /* Get colors */
  if (state_type == GTK_STATE_PRELIGHT)
    {
      lightgc = style->bg_gc[GTK_STATE_PRELIGHT];
      midgc = style->bg_gc[GTK_STATE_SELECTED];
      darkgc = style->fg_gc[GTK_STATE_PRELIGHT];
      whitegc = style->white_gc;
    }
  else
    {
      lightgc = metal_style->light_gray_gc;
      midgc = metal_style->mid_gray_gc;
      darkgc = metal_style->mid_gray_gc;
      whitegc = style->white_gc;
    }

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (midgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
      gdk_gc_set_clip_rectangle (whitegc, area);
    }

  /* Draw backgound */
  gdk_draw_rectangle (window, lightgc, TRUE, x, y, width, height);

  /* Draw border */
  gdk_draw_rectangle (window, darkgc, FALSE, x, y, width - 2, height - 2);
  gdk_draw_rectangle (window, style->white_gc, FALSE, x + 1, y + 1, width -
		      2, height - 2);

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (midgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
      gdk_gc_set_clip_rectangle (whitegc, NULL);
    }
}

void
adjust_notebook_tab_size (GtkPositionType tab_pos,
			  gint           *width,
			  gint           *height)
{
  /* The default overlap is two pixels, but we only want a one pixel overlap
   */
  switch (tab_pos)
    {
    case GTK_POS_TOP:
    case GTK_POS_BOTTOM:
      *width -= 1;
      break;
    case GTK_POS_LEFT:
    case GTK_POS_RIGHT:
      *height -= 1;
      break;
    }
}

static gboolean
is_first_tab (GtkNotebook *notebook,
	      int          x,
	      int          y)
{

  GtkWidget *widget = GTK_WIDGET (notebook);
  int border_width = GTK_CONTAINER (notebook)->border_width;

  switch (notebook->tab_pos)
    {
    case GTK_POS_TOP:
    case GTK_POS_BOTTOM:
      return x == widget->allocation.x + border_width;
    case GTK_POS_LEFT:
    case GTK_POS_RIGHT:
      return y == widget->allocation.y + border_width;
    }

  return FALSE;
}

void
do_metal_draw_notebook_tab (GtkStyle * style,
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
	   gint orientation)
{
  MetalStyle *metal_style = METAL_STYLE (style);
  GdkGC *lightgc, *midgc, *darkgc, *brightgc, *bggc;
  GdkPoint points[5];
  gboolean is_first, selected;

  if (widget != NULL && GTK_IS_NOTEBOOK(widget))
     is_first = is_first_tab (GTK_NOTEBOOK (widget), x, y);
  else
     is_first = FALSE;

  selected = state_type == GTK_STATE_NORMAL;

  lightgc = metal_style->light_gray_gc;
  midgc = metal_style->mid_gray_gc;
  brightgc = style->white_gc;
  bggc = metal_style->light_gray_gc;

  if (selected)
    {
      brightgc = style->white_gc;
      darkgc = metal_style->mid_gray_gc;
    }
  else
    {
      brightgc = metal_style->light_gray_gc;
      darkgc = metal_style->dark_gray_gc;
    }

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (midgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
      gdk_gc_set_clip_rectangle (brightgc, area);
      gdk_gc_set_clip_rectangle (bggc, area);
    }

  adjust_notebook_tab_size (orientation, &width, &height);

  /* Fill area */
  gdk_draw_rectangle (window, bggc, TRUE, x + 0, y + 0, width, height);

  switch (orientation)
    {
    case GTK_POS_TOP:
      /* Draw background */
      points[0].x = x + 2;
      points[0].y = y + height;
      points[1].x = x + 2;
      points[1].y = y + 6;
      points[2].x = x + 6;
      points[2].y = y + 2;
      points[3].x = x + width - 1;
      points[3].y = y + 2;
      points[4].x = x + width - 1;
      points[4].y = y + height;
      if (selected)
	gdk_draw_polygon (window, lightgc, TRUE, points, 5);
      else
	gdk_draw_polygon (window, midgc, TRUE, points, 5);

      /* Draw border */
      if (is_first)
	gdk_draw_line (window, darkgc, x + 0, y + 6, x + 0, y + height + 1);
      else if (selected)
	gdk_draw_line (window, darkgc, x + 0, y + 6, x + 0, y + height - 1);
      gdk_draw_line (window, darkgc, x + 0, y + 6, x + 6, y + 0);
      gdk_draw_line (window, darkgc, x + 6, y + 0, x + width - 2, y + 0);
      gdk_draw_line (window, darkgc, x + width - 1, y + 1, x + width - 1, y
		     + height - 1);

      if (is_first)
	gdk_draw_line (window, brightgc, x + 1, y + 6, x + 1, y + height + 1);
      else
	gdk_draw_line (window, brightgc, x + 1, y + 6, x + 1, y + height - 1);
      gdk_draw_line (window, brightgc, x + 1, y + 6, x + 6, y + 1);
      gdk_draw_line (window, brightgc, x + 6, y + 1, x + width - 2, y + 1);
      break;
    case GTK_POS_LEFT:
      /* Draw background */
      points[0].x = x + 2;
      points[0].y = y + height;
      points[1].x = x + 2;
      points[1].y = y + 6;
      points[2].x = x + 6;
      points[2].y = y + 2;
      points[3].x = x + width - 1;
      points[3].y = y + 2;
      points[4].x = x + width - 1;
      points[4].y = y + height;
      if (selected)
	gdk_draw_polygon (window, lightgc, TRUE, points, 5);
      else
	gdk_draw_polygon (window, midgc, TRUE, points, 5);

      /* Draw border */
      gdk_draw_line (window, darkgc, x + 0, y + 6, x + 0, y + height - 1);
      gdk_draw_line (window, darkgc, x + 0, y + 6, x + 6, y + 0);
      if (is_first)
	gdk_draw_line (window, darkgc, x + 6, y + 0, x + width + 1, y + 0);
      else
	gdk_draw_line (window, darkgc, x + 6, y + 0, x + width - 1, y + 0);
      gdk_draw_line (window, darkgc, x + 0, y + height - 1, x + width - 1, y
		     + height - 1);

      gdk_draw_line (window, brightgc, x + 1, y + 6, x + 6, y + 1);
      if (is_first)
	gdk_draw_line (window, brightgc, x + 6, y + 1, x + width + 1, y + 1);
      else
	gdk_draw_line (window, brightgc, x + 6, y + 1, x + width - 1, y + 1);
      break;
    case GTK_POS_RIGHT:
      /* Draw background */
      points[0].x = x + width - 2;
      points[0].y = y + height - 1;
      points[1].x = x + width - 2;
      points[1].y = y + 6;
      points[2].x = x + width - 6;
      points[2].y = y + 2;
      points[3].x = x - 1;
      points[3].y = y + 2;
      points[4].x = x - 1;
      points[4].y = y + height - 1;
      if (selected)
	gdk_draw_polygon (window, lightgc, TRUE, points, 5);
      else
	gdk_draw_polygon (window, midgc, TRUE, points, 5);

      /* Draw border */
      gdk_draw_line (window, darkgc, x + width - 1, y + 6, x + width - 1, y
		     + height - 1);
      gdk_draw_line (window, darkgc, x + width - 1, y + 6, x + width - 7, y
		     + 0);
      if (is_first)
	gdk_draw_line (window, darkgc, x - 2, y + 0, x + width - 7, y + 0);
      else
	gdk_draw_line (window, darkgc, x - 1, y + 0, x + width - 7, y + 0);
      gdk_draw_line (window, darkgc, x - 1, y + height - 1, x + width - 1, y
		     + height - 1);

      gdk_draw_line (window, brightgc, x + width - 2, y + 6, x + width - 7, y
		     + 1);
      if (is_first)
	gdk_draw_line (window, brightgc, x + width - 7, y + 1, x - 2, y + 1);
      else
	gdk_draw_line (window, brightgc, x + width - 7, y + 1, x - 1, y + 1);
      break;
    case GTK_POS_BOTTOM:
      /* Draw background */
      points[0].x = x + 2;
      points[0].y = y + 0;
      points[1].x = x + 2;
      points[1].y = y + height - 6;
      points[2].x = x + 6;
      points[2].y = y + height - 2;
      points[3].x = x + width - 1;
      points[3].y = y + height - 2;
      points[4].x = x + width - 1;
      points[4].y = y + 0;
      if (selected)
	gdk_draw_polygon (window, lightgc, TRUE, points, 5);
      else
	gdk_draw_polygon (window, midgc, TRUE, points, 5);

      /* Draw border */
      if (is_first)
	gdk_draw_line (window, darkgc, x + 0, y + height - 6, x + 0, y - 2);
      else if (selected)
	gdk_draw_line (window, darkgc, x + 0, y + height - 6, x + 0, y - 1);
      gdk_draw_line (window, darkgc, x + 0, y + height - 6, x + 6, y + height);
      gdk_draw_line (window, darkgc, x + 5, y + height - 1, x + width - 2, y
		     + height - 1);
      gdk_draw_line (window, darkgc, x + width - 1, y + height - 1, x +
		     width - 1, y - 1);

      if (is_first)
	gdk_draw_line (window, brightgc, x + 1, y + height - 6, x + 1, y - 2);
      else
	gdk_draw_line (window, brightgc, x + 1, y + height - 6, x + 1, y - 1);
      gdk_draw_line (window, brightgc, x + 1, y + height - 6, x + 5, y +
		     height - 2);
      break;
    }

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (midgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
      gdk_gc_set_clip_rectangle (brightgc, NULL);
      gdk_gc_set_clip_rectangle (bggc, NULL);
    }
}

void
do_metal_draw_button (GtkStyle * style,
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
  MetalStyle *metal_style = METAL_STYLE (style);
  GdkGC *dark_gc, *medium_gc, *light_gc;
  gboolean active_toggle = FALSE;

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_PRELIGHT], area);
      gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_SELECTED], area);
      gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_NORMAL], area);
      gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);
      gdk_gc_set_clip_rectangle (style->light_gc[state_type], area);
      gdk_gc_set_clip_rectangle (style->dark_gc[state_type], area);
    }

  if (widget && GTK_IS_TOGGLE_BUTTON (widget) &&
      GTK_TOGGLE_BUTTON (widget)->active)
    {
      active_toggle = TRUE;
      gdk_draw_rectangle (window, metal_style->mid_gray_gc, TRUE, x, y,
			  width, height);
    }
  else if (state_type == GTK_STATE_ACTIVE)
    {
      gdk_draw_rectangle (window, metal_style->mid_gray_gc, TRUE, x, y,
			  width, height);
    }
  else
    {
      gdk_draw_rectangle (window, metal_style->light_gray_gc, TRUE, x, y,
			  width, height);
    }

  dark_gc = (state_type == GTK_STATE_INSENSITIVE) ? metal_style->mid_gray_gc : metal_style->dark_gray_gc;
  medium_gc = metal_style->light_gray_gc;
  light_gc = style->white_gc;

  gdk_draw_rectangle (window, dark_gc, FALSE, x, y, width
		      - 2, height - 2);

  if (state_type == GTK_STATE_INSENSITIVE)
    {
      /* Nothing */
    }
  else if (widget && GTK_WIDGET_HAS_DEFAULT (widget))
    {
      if (state_type == GTK_STATE_ACTIVE || active_toggle)
	{
	  if (active_toggle)
	    {
	      gdk_draw_line (window, medium_gc,
			     x + 2,     y + 2,
			     x + 2,     y + height);
	      gdk_draw_line (window, medium_gc,
			     x + 2,     y + 2,
			     x + width, y + 2);
	    }

	  gdk_draw_line (window, light_gc,
			 x + 2,     y + height,
			 x + width, y + height);
	  gdk_draw_line (window, light_gc,
			 x + width, y + 2,
			 x + width, y + height);
	}
      else
	{
	  gdk_draw_rectangle (window, light_gc, FALSE, x + 2, y + 2,
			      width - 2, height - 2);
	}
      
      gdk_draw_rectangle (window, dark_gc, FALSE, x + 1, y + 1, width
			  - 2, height - 2);
      
      gdk_draw_point (window, medium_gc, x + 2, y + height - 2);
      gdk_draw_point (window, medium_gc, x + width - 2, y + 2);
      gdk_draw_point (window, dark_gc, x, y + height - 1);
      gdk_draw_point (window, dark_gc, x + width - 1, y);
    }
  else
    {
      if (state_type == GTK_STATE_ACTIVE || active_toggle)
	{
	  if (active_toggle)
	    {
	      gdk_draw_line (window, medium_gc,
			     x + 1, y + 1,
			     x + 1, y + height - 1);
	      gdk_draw_line (window, medium_gc,
			     x + 1,         y + 1,
			     x + width - 1, y + 1);
	    }

	  gdk_draw_line (window, light_gc,
			 x + 1,     y + height - 1,
			 x + width, y + height - 1);
	  gdk_draw_line (window, light_gc,
			 x + width - 1, y + 1,
			 x + width - 1, y + height);
	}
      else
	gdk_draw_rectangle (window, light_gc, FALSE, x + 1, y + 1,
			    width - 2, height - 2);

      gdk_draw_point (window, medium_gc, x + 1, y + height - 2);
      gdk_draw_point (window, medium_gc, x + width - 2, y + 1);
    }

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_PRELIGHT], NULL);
      gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_SELECTED], NULL);
      gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_NORMAL], NULL);
      gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL); 
      gdk_gc_set_clip_rectangle (style->light_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle (style->dark_gc[state_type], NULL);
    }
}
