/* Redmond Engine
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
 * modified by Andrew Johnson <ajgenius@ajgenius.us>
 */
#include "redmond_gtk2_engine.h"
#include "redmond_gtk2_drawing.h"
#include "redmond_gtk2_misc.h"
 
/***********************************************************************
 * Portions of the code based on/copied from the Bluecurve theme/engine, 
 *   License: GPL 
 *   Copyright:
 *     Alexander Larsson <alexl@redhat.com>
 *     Garrett LeSage
 *     Owen Taylor <otaylor@redhat.com>
 *
 * Portions of the code based on/copied from the Metal theme/engine, 
 *   License: GPL 
 *   Copyright:
 *     Randy Gordon, randy@integrand.com
 *     Owen Taylor <otaylor@redhat.com>
 *
 * Portions of the code based on/copied from the Smooth Engine,
 *   License: LGPL 
 *   Copyright:
 *     Andrew Johnson <ajgenius@ajgenius.us
 *
 ***********************************************************************/
 
 
/***********************************************
 * redmond_draw_vline -
 *  
 *   the Function used to draw horizontal spacers.
 ***********************************************/
void
redmond_draw_hline (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint x1,
            gint x2, 
            gint y)
{
  g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));
 
  do_redmond_draw_line(window, 
                       style->dark_gc[state_type], 
                       style->light_gc[state_type], 
                       area, 
                       x1, x2, y, 
                       TRUE);
}
 
/***********************************************
 * redmond_draw_vline -
 *  
 *   the Function used to draw vertical spacers.
 ***********************************************/
void
redmond_draw_vline (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint y1, 
            gint y2, 
            gint x)
{
  if (is_combo_box(widget, FALSE) && (!is_combo_box_entry(widget)))
    return;
 
  g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));
 
  do_redmond_draw_line(window, 
                       style->dark_gc[state_type], 
                       style->light_gc[state_type], 
                       area, 
                       y1, y2, x, 
                       FALSE);
}
  
/***********************************************
 * redmond_draw_check -
 *  
 *   the Function used to draw all check boxes.
 *  
 *   Redmond Check box has essentially 3 looks -
 *
 *   Normal/Prelight - 
 *     base[NORMAL] fill, fg[NORMAL] check
 * 
 *   Selected/Active - 
 *     bg[NORMAL] fill, fg[NORMAL] check
 * 
 *   Insensitive - 
 *     bg[NORMAL] fill, fg[INSENSITIVE] check
 * 
 *   The Shadow Always draws with a state NORMAL.
 ***********************************************/
void
redmond_draw_check (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state,
	    GtkShadowType shadow,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint x, 
            gint y, 
            gint width, 
            gint height)
{
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
 
  /* Always draw the checkbox and mark centered
   * at a fixed size.
   */
  x -= (1 + CHECK_SIZE - width) / 2;
  y -= (1 + CHECK_SIZE - height) / 2;
  width = CHECK_SIZE;
  height = CHECK_SIZE;
  
  if (CHECK_DETAIL (detail, "check"))	/* Menu item */
    {
      /* check boxes in menu don't actually draw box, only the check mark,
       * which use the text color 
       */
      if (shadow == GTK_SHADOW_IN)
	{
	  do_redmond_draw_check (window, style->text_gc[state], x + 3, y + 3,
			 width - 4, height - 4);
	}
    }
  else
    {
      switch (state)
	{
	case GTK_STATE_NORMAL:
	case GTK_STATE_PRELIGHT:
          if (IS_TOGGLE_BUTTON(widget) && TOGGLE_BUTTON(widget)->inconsistent)
	    do_redmond_draw_cross_hatch_fill (style, window, GTK_STATE_NORMAL, 
                                              area, RADIO_NONE, x, y, width, height);
	  else
	    gdk_draw_rectangle (window, style->base_gc[GTK_STATE_NORMAL],
			        TRUE, x, y, width, height);
	  break;
	default:
	  gdk_draw_rectangle (window, style->bg_gc[GTK_STATE_NORMAL], TRUE,
			      x, y, width, height);
	}
 
      if ((shadow == GTK_SHADOW_IN) || 
          (IS_TOGGLE_BUTTON(widget) && 
           TOGGLE_BUTTON(widget)->inconsistent))
	{
	  if ((state == GTK_STATE_INSENSITIVE) || 
              (IS_TOGGLE_BUTTON(widget) && 
               TOGGLE_BUTTON(widget)->inconsistent))
            {
	      do_redmond_draw_check (window, style->fg_gc[GTK_STATE_INSENSITIVE],
			             x + 2, y + 2, width - 4, height - 4);
            }
	  else
            {
	      do_redmond_draw_check (window, style->fg_gc[GTK_STATE_NORMAL],
			             x + 2, y + 2, width - 4, height - 4);
            }
	}
 
      gtk_paint_shadow (style, window, GTK_STATE_NORMAL, GTK_SHADOW_IN,
			area, widget, detail, x, y, width, height);
    }
}
 
/***********************************************
 * redmond_draw_option -
 *  
 *   the Function used to draw all Option boxes
 *  
 *   Redmond Option box has essentially 3 looks -
 *
 *   Normal/Prelight - 
 *     base[NORMAL] fill, fg[NORMAL] check
 * 
 *   Selected/Active - 
 *     bg[NORMAL] fill, fg[NORMAL] check
 * 
 *   Insensitive - 
 *     bg[NORMAL] fill, fg[INSENSITIVE] check
 * 
 *   The Shadow Always draws with a state NORMAL.
 ***********************************************/
void
redmond_draw_option (GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state,
	     GtkShadowType shadow,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail, 
             gint x,
             gint y, 
             gint width, 
             gint height)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
 
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
 
  /* Since X arcs are very primitive and don't always
   * produce circles, we use bits here. Since the bits
   * aren't really scalable, always draw the box and mark
   * centered at a fixed size.
   */
  x -= (1 + PART_SIZE - width) / 2;
  y -= (1 + PART_SIZE - height) / 2;
  width = PART_SIZE;
  height = PART_SIZE;
 
  if (CHECK_DETAIL (detail, "option"))	/* Menu item */
    {
      /* check boxes in menu don't actually draw box, only the mark,
       * which use the text color 
       */
      if (shadow == GTK_SHADOW_IN)
	redmond_draw_part (window, style->text_gc[state], area, x + 1, y + 1,
		   RADIO_TEXT);
    }
  else
    {
      switch (state)
	{
	case GTK_STATE_NORMAL:
	case GTK_STATE_PRELIGHT:
          if (IS_TOGGLE_BUTTON(widget) && TOGGLE_BUTTON(widget)->inconsistent)
            do_redmond_draw_cross_hatch_fill (style, window, GTK_STATE_NORMAL, 
                                              area, RADIO_BASE, x, y, width, height);
          else
	    redmond_draw_part (window, style->base_gc[GTK_STATE_NORMAL], area, 
                               x, y, RADIO_BASE);
	  break;
	default:
	  redmond_draw_part (window, style->bg_gc[GTK_STATE_NORMAL], area, 
		             x, y, RADIO_BASE);
	}
 
      if ((shadow == GTK_SHADOW_IN) || 
          (IS_TOGGLE_BUTTON(widget) && 
           TOGGLE_BUTTON(widget)->inconsistent))
	{
	  if ((state == GTK_STATE_INSENSITIVE) || 
              (IS_TOGGLE_BUTTON(widget) && 
               TOGGLE_BUTTON(widget)->inconsistent))
            {
	      redmond_draw_part (window, style->fg_gc[GTK_STATE_INSENSITIVE], area, 
                                 x, y, RADIO_TEXT);
            }
	  else
            {
              redmond_draw_part (window, style->fg_gc[GTK_STATE_NORMAL], area, 
                                 x, y, RADIO_TEXT);
            }
	}
        
      redmond_draw_part (window, style->dark_gc[GTK_STATE_NORMAL], area, x, y,
		 RADIO_DARK);
      redmond_draw_part (window, style->light_gc[GTK_STATE_NORMAL], area, x, y,
		 RADIO_LIGHT);
 
      redmond_draw_part (window, redmond_style->black_border_gc[GTK_STATE_NORMAL],
		 area, x, y, RADIO_BLACK);
      redmond_draw_part (window, style->bg_gc[GTK_STATE_NORMAL], area, x, y,
		 RADIO_MID);
    }
}
 
/***********************************************
 * redmond_draw_arrow -
 *  
 *   the Function used to draw all arrows
 *  
 *   Redmond Arrows have 2 looks, Normal 
 *   & Insensitive 
 ***********************************************/
void
redmond_draw_arrow (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state,
	    GtkShadowType shadow,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    GtkArrowType arrow_type,
	    gboolean fill, 
            gint x, 
            gint y, 
            gint width, 
            gint height)
{
  gboolean button_in = (shadow == GTK_SHADOW_IN);
  gint child_offset_x = 1, child_offset_y = 1;
 
  if (is_combo_box(widget, FALSE) && (!is_combo_box_entry(widget)))
    return;
 
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
 
  if ((CHECK_DETAIL (detail, "spinbutton"))
      || (CHECK_DETAIL (detail, "vscrollbar"))
      || (CHECK_DETAIL (detail, "hscrollbar"))
      || (is_in_combo_box (widget)))
    {
      if (state != GTK_STATE_INSENSITIVE)
        state = GTK_STATE_NORMAL;
 
      /* The Combo/ComboBoxEntry button and SpinButton steppers should apear
       * to be inset into the entry, as opposed to next to it, so we 
       * need to offset/resize arrows as needed to accomodate this.
       */
      if (CHECK_DETAIL (detail, "spinbutton"))
	{
	  if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR)
	    x -= 1;
	}
      else if (is_in_combo_box (widget) && (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL))
	{
	    x += 1;
	}
 
      /* Redmond prefers to have smaller arrows relative too size
       * for ComboBox/ComboBoxEntry, because its cleaner,
       * even though it seems slightly inconsistant.
       */
      if (widget && is_in_combo_box (widget))
	{
	  y += 1;
	  width -= 2;
	  height -= 2;
	}
      else
	/* SpinButton & Scrollbar Arrows offset down/right on button press */
      if (button_in)
	{
	  x += child_offset_x;
	  y += child_offset_y;
	}
 
      do_redmond_draw_arrow (window, style->fg_gc[state], arrow_type, x, y, width, height);
    }
  else
    {
      if (CHECK_DETAIL(detail, "menuitem"))
          x -= 1;
      else if (state != GTK_STATE_INSENSITIVE)
        state = GTK_STATE_NORMAL;
 
      do_redmond_draw_arrow (window, style->fg_gc[state], arrow_type, x, y, width, height);
    }
}
 
/***********************************************
 * redmond_draw_shadow -
 *  
 *   Function used to draw the Shadow/Border
 *   for most widgets/widget parts.
 *  
 *   The Redmond Shadow is primarily constructed
 *   with an outside border of light/extra_dark
 *   and an internal border of bg/dark.
 ***********************************************/
void
redmond_draw_shadow (GtkStyle * style,
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
 
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
 
  if (shadow_type == GTK_SHADOW_NONE)
    return;
 
  if (area)
    {
      gdk_gc_set_clip_rectangle (style->light_gc[state_type], area);
      gdk_gc_set_clip_rectangle (style->dark_gc[state_type], area);
      gdk_gc_set_clip_rectangle (redmond_style->black_border_gc[state_type],
 				 area);
      gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);
    }
 
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      break;
    case GTK_SHADOW_ETCHED_IN:
      do_redmond_draw_shadow (window, style->dark_gc[state_type],
		      style->light_gc[state_type], x, y, width, height, TRUE);
      do_redmond_draw_shadow (window, style->light_gc[state_type],
		      style->dark_gc[state_type], x + 1, y + 1, width - 2,
		      height - 2, TRUE);
      break;
    case GTK_SHADOW_ETCHED_OUT:
      do_redmond_draw_shadow (window, style->light_gc[state_type],
		      style->dark_gc[state_type], x, y, width, height, TRUE);
      do_redmond_draw_shadow (window, style->dark_gc[state_type],
		      style->light_gc[state_type], x + 1, y + 1, width - 2,
		      height - 2, TRUE);
      break;
    case GTK_SHADOW_IN:
      if (CHECK_DETAIL (detail, "buttondefault"))
	{
	  /* Instead of an inset outside border draw an extra black 
	   * border arround focused defaultable buttons, this really
	   * needs some logic so no matter how big the default area
	   * it only draws exactly one pixel around button.
	   */
	  gdk_draw_rectangle (window,
			      redmond_style->black_border_gc[state_type],
			      FALSE, x, y, width - 1, height - 1);
	}
      else if (((CHECK_DETAIL (detail, "vscrollbar"))
		|| (CHECK_DETAIL (detail, "hscrollbar"))
		|| ((CHECK_DETAIL (detail, "button")) && (widget)
		    && GTK_WIDGET_HAS_DEFAULT (widget))))
	{
	  /* Scrollbar steppers and Defaultable Buttons use a flat
	   * dark edge on pressed.
	   */
	  gdk_draw_rectangle (window, style->dark_gc[state_type], FALSE,
			      x, y, width - 1, height - 1);
	}
      else if (((CHECK_DETAIL (detail, "entry"))
		|| (CHECK_DETAIL (detail, "frame")))
	       && widget && ((IS_SPIN_BUTTON (widget))
			     || (is_in_combo_box (widget))))
	{
	  /* The Combo/ComboBoxEntry button and the SpingButton Steppers should apear
	   * to be inset into the entry, as opposed to next to it, so we fake it
	   * by drawing an extra 2 pixels here to ignore the right(or left)
	   * edge, which will be drawn by the buttons.
	   */
 
	  if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR)
	    {
	      do_redmond_draw_shadow (window, style->dark_gc[state_type],
			      style->light_gc[state_type], x, y, width + 2,
			      height, FALSE);
	      do_redmond_draw_shadow (window,
			      redmond_style->black_border_gc[state_type],
			      style->bg_gc[state_type], x + 1, y + 1, width,
			      height - 2, FALSE);
	    }
	  else
	    {
	      do_redmond_draw_shadow (window, style->dark_gc[state_type],
			      style->light_gc[state_type], x - 2, y,
			      width + 2, height, FALSE);
	      do_redmond_draw_shadow (window,
			      redmond_style->black_border_gc[state_type],
			      style->bg_gc[state_type], x - 1, y + 1, width,
			      height - 2, FALSE);
	    }
	}
      else if ((CHECK_DETAIL (detail, "trough"))
	       || ((IS_STATUS_BAR (widget)))
	       || ((CHECK_DETAIL (detail, "frame")))
	       || ((CHECK_DETAIL (detail, "button"))
		   && (is_toolbar_item (widget))))
	{
	  /* Toolbar Buttons, Status Bars, Frames, and Troughs are drawn with a thin border */
	  do_redmond_draw_shadow (window, style->dark_gc[state_type],
			  style->light_gc[state_type], x, y, width, height,
			  FALSE);
	}
      else
	{
	  /* Normal IN Border */
	  do_redmond_draw_shadow (window, style->dark_gc[state_type],
			  style->light_gc[state_type], x, y, width, height,
			  FALSE);
	  do_redmond_draw_shadow (window,
			  redmond_style->black_border_gc[state_type],
			  style->bg_gc[state_type], x + 1, y + 1, width - 2,
			  height - 2, FALSE);
	}
      break;
    case GTK_SHADOW_OUT:
      if ((CHECK_DETAIL (detail, "vscrollbar"))
	  || (CHECK_DETAIL (detail, "hscrollbar"))
	  || (CHECK_DETAIL (detail, "slider"))
	  || (CHECK_DETAIL (detail, "spinbutton_up"))
	  || (CHECK_DETAIL (detail, "spinbutton_down"))
	  || ((CHECK_DETAIL (detail, "button")) && is_in_combo_box (widget)))
	{
	  /* In the case of Scrollbars steppers, Sliders, Spinbutton steppers,
	   * and Combo/ComboBoxEntry buttons, the internal & external borders are 
	   * inverted when OUT since they are inset into another widgets IN edge.
	   */
	  do_redmond_draw_shadow (window, style->bg_gc[state_type],
			  redmond_style->black_border_gc[state_type], x, y,
			  width, height, FALSE);
	  do_redmond_draw_shadow (window, style->light_gc[state_type],
			  style->dark_gc[state_type], x + 1, y + 1, width - 2,
			  height - 2, FALSE);
	}
      else if ((CHECK_DETAIL (detail, "frame"))
	       || ((CHECK_DETAIL (detail, "button"))
		   && (is_toolbar_item (widget))))
	{
	  /* Toolbar Buttons and Frames are drawn with a thin border */
	  do_redmond_draw_shadow (window, style->light_gc[state_type],
			  style->dark_gc[state_type], x, y, width, height,
			  FALSE);
	}
      else
	{
	  /* Normal OUT Border */
	  do_redmond_draw_shadow (window, style->light_gc[state_type],
			  redmond_style->black_border_gc[state_type], x, y,
			  width, height, FALSE);
	  do_redmond_draw_shadow (window, style->bg_gc[state_type],
			  style->dark_gc[state_type], x + 1, y + 1, width - 2,
			  height - 2, FALSE);
	}
      break;
    }
 
  if (area)
    {
      gdk_gc_set_clip_rectangle (style->light_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle (style->dark_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle (redmond_style->black_border_gc[state_type],
				 NULL);
      gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL);
    }
}
 
/***********************************************
 * redmond_draw_combobox_button-
 *   
 *   Function used to draw the ComboBox button
 *   "inset" into "entry. 
 *
 *   Slightly Evil Hack #1
 ***********************************************/
static void
redmond_draw_combobox_button (GtkStyle * style,
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
  /* The Combo/ComboBoxEntry button should apear to be inset into the entry, 
   * as opposed to next to it, so we fake it by drawing an entry fill
   * then an entry border - but 2 pixels bigger so it overlaps real border
   * and so left( or right) edge doesn't actually get drawn on screen;
   * and THEN draw the button, but essentially 4 pixels smaller.
   */
  if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR)
    {
      if (is_combo_box_entry (widget)
          || is_combo_box (widget, TRUE))
        {
          if (!is_combo_box_entry (widget))
	    gdk_draw_rectangle (window,
	    	                widget->parent->style->
	 		        base_gc[GTK_WIDGET_STATE (widget->parent)],
			        TRUE, x - 2, y, width + 2, height);
          else
            gtk_paint_flat_box (style, window, state_type,
			        GTK_SHADOW_NONE, area, widget, "entry_bg", x - 2,
			        y, width + 2, height);
 
          gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
  		            area, widget, "entry", x - 2, y, width, height);
        }
      else if (is_combo(widget))
        {
          GtkWidget *entry = GTK_COMBO (widget->parent)->entry;
 
          gtk_paint_flat_box (entry->style, window, entry->state,
	 		      GTK_SHADOW_NONE, area, entry, "entry_bg", x - 2,
			      y, width + 2, height);
          gtk_paint_shadow (entry->style, window, entry->state, GTK_SHADOW_IN,
			    area, entry, "entry", x - 2, y, width, height);
        }
      else
        {
          GtkWidget *parent = widget;
      
          if (widget->parent)
            parent = widget->parent;
        
          gtk_paint_flat_box (parent->style, window, parent->state,
			      GTK_SHADOW_NONE, area, parent, "entry_bg", x - 2,
			      y, width + 2, height);
          gtk_paint_shadow (parent->style, window, parent->state, GTK_SHADOW_IN,
		   	    area, parent, "entry", x - 2, y, width, height);
        }
 
      do_redmond_draw_default_fill (style, window,
	  			          state_type, area, x, y + 2,
				          width - 2, height - 4);
      if (shadow_type == GTK_SHADOW_IN)
        gdk_draw_rectangle (window, style->dark_gc[state_type], FALSE, x,
	   		    y + 2, width - 3, height - 5);
      else
        gtk_paint_shadow (style, window, state_type, shadow_type, area,
		          widget, detail, x, y + 2, width - 2, height - 4);
    } 
  else
    {
      if (is_combo_box_entry (widget)
          || is_combo_box (widget, TRUE))
        {
          if (!is_combo_box_entry (widget))
	    gdk_draw_rectangle (window,
			        widget->parent->style->
			        base_gc[GTK_WIDGET_STATE (widget->parent)],
			        TRUE, x + 2, y, width + 2, height);
          else
            gtk_paint_flat_box (style, window, state_type,
	  	                GTK_SHADOW_NONE, area, widget, "entry_bg", x + 2,
			        y, width + 2, height);
 
          gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
	  		    area, widget, "entry", x + 2, y, width, height);
        }
      else if (is_combo(widget))
        {
           GtkWidget *entry = GTK_COMBO (widget->parent)->entry;
 
           gtk_paint_flat_box (entry->style, window, entry->state,
			       GTK_SHADOW_NONE, area, entry, "entry_bg", x + 2,
                               y, width + 2, height);
           gtk_paint_shadow (entry->style, window, entry->state, GTK_SHADOW_IN,
			     area, entry, "entry", x + 2, y, width, height);
        }
      else
        {
          GtkWidget *parent = widget;
       
          if (widget->parent)
            parent = widget->parent;
           
          gtk_paint_flat_box (parent->style, window, parent->state,
			      GTK_SHADOW_NONE, area, parent, "entry_bg", x + 2,
			      y, width + 2, height);
          gtk_paint_shadow (parent->style, window, parent->state, GTK_SHADOW_IN,
		   	    area, parent, "entry", x + 2, y, width, height);
        }
 
      do_redmond_draw_default_fill (style, window,
				          state_type, area, x + 2, y + 2,
				          width - 2, height - 4);
 
      if (shadow_type == GTK_SHADOW_IN)
        gdk_draw_rectangle (window, style->dark_gc[state_type], FALSE, x + 2,
			    y + 2, width - 3, height - 5);
      else
        gtk_paint_shadow (style, window, state_type, shadow_type, area,
		          widget, detail, x + 2, y + 2, width - 2, height - 4);
    }
}
 
/***********************************************
 * redmond_draw_spinbutton_stepper-
 *  
 *   Function used to draw the spinbutton 
 *   steppers "inset" into entry.
 *  
 *   Slightly Evil Hack #2
 ***********************************************/
static void
redmond_draw_spinbutton_stepper (GtkStyle * style,
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
  GdkRectangle spin_area;
 
  /* The SpinButton Steppers should apear to be inset into the entry, 
   * as opposed to next to it, so we fake it by drawing en entry fill
   * and then entry border - but 2 pixels bigger so it overlaps real border
   * and so left( or right) edge doesn't actually get drawn on screen -
   * and THEN draw the Stepper. Since we have two steppers this is slightly
   * more complicated because we can only do half for each stepper.
   */
 
  spin_area.x = x;
  spin_area.y = y;
  spin_area.width = width;
  spin_area.height = height;
 
  if (state_type != GTK_STATE_INSENSITIVE)
    state_type = GTK_STATE_NORMAL;
 
  if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR)
    {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
	  gtk_paint_flat_box (style, window, state_type,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg", x - 2, y, width + 2, height);
	  gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
			    &spin_area, widget, detail, x - 2, y,
			    width + 2, height + 2);
	  do_redmond_draw_default_fill (style, window,
					      state_type, area, x,
					      y + 2, width - 2, height - 2);
	  gtk_paint_shadow (style, window, state_type, shadow_type,
			    area, widget, detail, x, y + 2, width - 2,
			    height - 2);
	}
      else
	{
	  gtk_paint_flat_box (style, window, state_type,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg", x - 2, y, width + 2, height);
	  gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
			    &spin_area, widget, detail, x - 2, y - 2,
			    width + 2, height + 2);
	  do_redmond_draw_default_fill (style, window,
					      state_type, area, x, y,
					      width - 2, height - 2);
	  gtk_paint_shadow (style, window, state_type, shadow_type,
			    area, widget, detail, x, y, width - 2,
			    height - 2);
	}
    }
  else
    {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
	  gtk_paint_flat_box (style, window, state_type,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg", x, y, width + 4, height);
	  gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
			    &spin_area, widget, detail, x, y,
			    width + 4, height + 2);
	  do_redmond_draw_default_fill (style, window,
					      state_type, area, x + 2,
					      y + 2, width - 2, height - 2);
	  gtk_paint_shadow (style, window, state_type, shadow_type,
			    area, widget, detail, x + 2, y + 2, width - 2,
			    height - 2);
	}
      else
	{
	  gtk_paint_flat_box (style, window, state_type,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg", x, y, width + 4, height);
	  gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
			    &spin_area, widget, detail, x, y - 2,
			    width + 4, height + 2);
	  do_redmond_draw_default_fill (style, window,
					      state_type, area, x + 2, y,
					      width - 2, height - 2);
	  gtk_paint_shadow (style, window, state_type, shadow_type,
			    area, widget, detail, x + 2, y, width - 2,
			    height - 2);
	}
    }
}
 
/***********************************************
 * redmond_draw_box -
 *  
 *   Function used to draw the box portion of 
 *   general widgets(buttons, entries.
 *
 *  In general this is the same as calling
 *  apply background and then draw shadow,
 *  so the bulk of this routine is actually
 *  to handle drawing the peculiarities of
 *  specific widgets which don't follow this 
 *  in redmond.
 ***********************************************/
void
redmond_draw_box (GtkStyle * style,
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
 
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
 
  if (IS_MENU_SHELL(widget))
    {
      gtk_menu_shell_setup_signals(widget);
    }  
 
  if (CHECK_DETAIL (detail, "menuitem"))
    shadow_type = GTK_SHADOW_NONE;
 
  if ((CHECK_DETAIL (detail, "trough")) &&
      (widget && IS_SCROLLBAR (widget)))
    {
      /* Scrollbar Troughs have a thin cross-hatch 
       * background fill. Technically its supposed
       * to change colors when actively pressed, but
       * since  thats a little hard to handle, skip it.
       */
      do_redmond_draw_cross_hatch_fill (style, window, GTK_STATE_NORMAL, 
                                        area, RADIO_NONE, x, y, width, height);
 
      redmond_draw_shadow (style, window, state_type, GTK_SHADOW_IN, area,
			widget, detail, x, y, width, height);
    }
  else if ((CHECK_DETAIL (detail, "trough"))
	   && (widget && IS_SCALE (widget)))
    {
      /* Scale Troughs are only 4 pixels thick
       * i.e. just a border, so make sure the background
       * is filled properly, then draw the border.
       */
      do_redmond_draw_default_fill (style, window,
                                    state_type, area, x,
                                    y, width, height);
      if (IS_HSCALE (widget))
	{
	  gdk_draw_rectangle (window,
			      redmond_style->black_border_gc[state_type],
			      TRUE, x, y + (height / 2), width - 2, 1);
 
	  redmond_draw_shadow (style, window, state_type, GTK_SHADOW_IN, area,
			    widget, detail, x, y + (height / 2) - 1, width,
			    4);
	}
      else
	{
	  gdk_draw_rectangle (window,
			      redmond_style->black_border_gc[state_type],
			      TRUE, x + (width / 2), y, 1, height - 2);
 
	  redmond_draw_shadow (style, window, state_type, GTK_SHADOW_IN, area,
			    widget, detail, x + (width / 2) - 1, y, 4,
			    height);
	}
    }
  else if ((CHECK_DETAIL (detail, "toolbar"))
	   || (CHECK_DETAIL (detail, "menubar"))
	   || (IS_BONOBO_TOOLBAR (widget))
	   || (CHECK_DETAIL (detail, "dockitem"))
	   || (CHECK_DETAIL (detail, "dockitem_bin"))
	   || (CHECK_DETAIL (detail, "handlebox_bin"))
	   || (CHECK_DETAIL (detail, "handlebox")))
    {
      /* Toolbars have a simple thin border, so custom 
       * case to handle the various widgets used as 
       * toolbars.
       */
       gboolean left_cutoff = FALSE, right_cutoff = FALSE, 
                top_cutoff = FALSE, bottom_cutoff = FALSE;
 
      if (((CHECK_DETAIL (detail, "dockitem_bin")) && 
           (IS_BONOBO_DOCK_ITEM(widget))) || 
          ((widget) && (is_bonobo_dock_item(widget->parent))))
	{	  
	  GList *children = NULL, *child = NULL;
	  GtkWidget *dockitem = widget;
	  gboolean has_grip = FALSE, ltr = TRUE;
	  
	  if ((!IS_BONOBO_DOCK_ITEM(widget)) && (!IS_BOX(widget)))
	    dockitem = widget->parent;
	    
	  has_grip = IS_CONTAINER(dockitem);
	  
	  ltr = (gtk_widget_get_direction (dockitem) == GTK_TEXT_DIR_LTR);
	  
	  if (has_grip)
	  {
            has_grip = FALSE;
            
            children = gtk_container_get_children(GTK_CONTAINER(dockitem));
                          
            for (child = g_list_first(children); child; child = g_list_next(child))
              {
	        if (IS_BONOBO_DOCK_ITEM_GRIP(child->data))
                  has_grip = (GTK_WIDGET_VISIBLE(child->data) && 
                              GTK_WIDGET_REALIZED(child->data) && 
                              GTK_WIDGET(child->data)->allocation.width > 1) &&
                              (GTK_WIDGET(child->data)->allocation.height > 1);
              }	            
 
            if (children)   
  	      g_list_free(children);
          }
          
          if (has_grip)
            {
	      /* bonobo dock items draw the handle bar grip different from
	       * other toolbars - namely it draws the bin the whole size
	       * and then the handle bar on top, which make the edge overlap 
	       * look wrong, especially relative to bonobo toolbars and
	       * gtk handle bars etc. luckily the handle bar size
	       * is fixed at 10, and the orientation can be grabbed
	       * via gobject, so we just make sure the bin is drawn
	       * NEXT to the handle bar, not beneath it
	       */
	       
   	      GtkOrientation tmp = GTK_ORIENTATION_HORIZONTAL;
 
	      if IS_HBOX(dockitem)
	        tmp = GTK_ORIENTATION_VERTICAL;
	      else if (IS_BONOBO_DOCK_ITEM(dockitem))
	        g_object_get (dockitem, "orientation", &tmp, NULL);
 
	      if (tmp == GTK_ORIENTATION_HORIZONTAL)
	        {
	          if (dockitem == widget)
	            {
	              x += 8*ltr;
	              width -= 8;
	            }
	          else
	            {
	              x -= 2*ltr;
	              width += 2;
	            }
	            
	          left_cutoff = ltr;
	          right_cutoff = !left_cutoff;
	        }
	      else
	        {
	          if (dockitem == widget)
	            {
	              y += 8;
	              height -= 8;
	            }
	          else
	            {
	              y -= 2;
	              height += 2;
	            }
	          top_cutoff = TRUE;
	        }
	    }    
	}
      else if (IS_HANDLE_BOX(widget))
        {
	  switch (gtk_handle_box_get_handle_position
		  (GTK_HANDLE_BOX (widget)))
	    {
	      case GTK_POS_LEFT:
                left_cutoff = (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR);
	        right_cutoff = !left_cutoff;
              break;
 
	      case GTK_POS_RIGHT:
                left_cutoff = (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL);
	        right_cutoff = !left_cutoff;
	      break;
 
	      case GTK_POS_TOP:
                top_cutoff = TRUE;
	      break;
	      
              case GTK_POS_BOTTOM:
	        bottom_cutoff = TRUE;
	      break;
	    }      
        }
      else if (IS_HANDLE_BOX_ITEM(widget))
        {
	  switch (gtk_handle_box_get_handle_position
		  (GTK_HANDLE_BOX (widget->parent)))
	    {
	      case GTK_POS_LEFT:
                left_cutoff = (gtk_widget_get_direction (widget->parent) == GTK_TEXT_DIR_LTR);
                right_cutoff= !left_cutoff;
                x -= 2;
                width += 2;
              break;
 
	      case GTK_POS_RIGHT:
                left_cutoff = (gtk_widget_get_direction (widget->parent) == GTK_TEXT_DIR_RTL);
	        right_cutoff = !left_cutoff;
                width += 2;
	      break;
 
	      case GTK_POS_TOP:
                top_cutoff = TRUE;
                y -= 2;
                height += 2;
	      break;
	      
              case GTK_POS_BOTTOM:
	        bottom_cutoff = TRUE;
                height += 2;
	      break;
	    }      
        }
  
      do_redmond_draw_default_fill (style, window,
					  state_type, area, x, y, width,
					  height);
 
      /* If this is a menu embedded in the gnome-panel, we don't
       *  draw a border since it looks cleaner without one.
       */
      if ((widget) && (widget->parent) &&
	  ((!((CHECK_DETAIL (detail, "menubar")) && 
	  is_panel_widget_item (widget)))))
        {
          GdkRectangle shadow, clip;
          
          shadow.x = x + 2*left_cutoff;
          shadow.y = y + 2*top_cutoff;
          shadow.width = width - 2*left_cutoff;
          shadow.height = height - 2*top_cutoff;
          
          if (area)
            gdk_rectangle_intersect(area, &shadow, &clip);
          else
            clip = shadow;
            
          gdk_gc_set_clip_rectangle(style->light_gc[state_type], &clip);
          gdk_gc_set_clip_rectangle(style->dark_gc[state_type], &clip);
          
	  do_redmond_draw_shadow (window, style->light_gc[state_type],
			          style->dark_gc[state_type], x, y, 
                                  width + 2*right_cutoff, 
                                  height + 2*bottom_cutoff,
			          FALSE);
			          
          gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
          gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
        }
    }
  else if ((CHECK_DETAIL (detail, "menuitem")) && widget && widget->parent
	   && IS_MENU_BAR (widget->parent))
    {
      /* Primary Menu Items on Menu bars are drawn with 
       * a thin inset border on select/active, technically
       * they should have a thin outset border on prelight,
       * but at this point that would require a dynamic 
       * runtime function change hack - so skip it.
       */
      GdkGC *top, *bottom;
      gboolean pressed = FALSE;
 
      if (state_type != GTK_STATE_INSENSITIVE)
	state_type = GTK_STATE_NORMAL;
 
      do_redmond_draw_default_fill (style, window,
					  state_type, area, x, y, width,
					  height);
		 			  
      if ((!GTK_MENU_ITEM(widget)->submenu) || 
          (!(GTK_WIDGET_REALIZED(GTK_MENU_ITEM(widget)->submenu) && 
             GTK_WIDGET_VISIBLE(GTK_MENU_ITEM(widget)->submenu))))
        { 
          top = style->light_gc[state_type];
          bottom = style->dark_gc[state_type];
        }
      else
        {
          top = style->dark_gc[state_type];
          bottom = style->light_gc[state_type];
        }
      do_redmond_draw_shadow (window, top, bottom,
		      x, y, width, height,
		      FALSE);
    }
  else if ((CHECK_DETAIL (detail, "bar"))
	   && (widget && IS_PROGRESS_BAR (widget)))
    {
      /* Progress bar status is always flat, and only has two states
       * NORMAL and INSENSITIVE, so check the state, and then do a 
       * default draw background.
       */
      if (state_type != GTK_STATE_INSENSITIVE)
	state_type = GTK_STATE_NORMAL;
 
      do_redmond_draw_default_fill (style, window,
                                    GTK_STATE_SELECTED, area, x, y,
                                    width, height);
    }
  else if ((CHECK_DETAIL (detail, "button")) && widget
	   && is_in_combo_box (widget))
    {
      /* ComboBox Buttons are an especially complex case
       * so call our special function for them.
       */
      redmond_draw_combobox_button (style, window, state_type, shadow_type, area,
			    widget, detail, x, y, width, height);
    }
  else if ((CHECK_DETAIL (detail, "spinbutton_up"))
	   || (CHECK_DETAIL (detail, "spinbutton_down")))
    {
      /* SpinButton Steppers are an especially complex case
       * so call our special function for them.
       */
      redmond_draw_spinbutton_stepper (style, window, state_type, shadow_type, area,
			       widget, detail, x, y, width, height);
    }
  else
    {
      /* default box apearance */
      do_redmond_draw_default_fill (style, window,
                                    state_type, area, x, y, width,
                                    height);
      redmond_draw_shadow (style, window, state_type, shadow_type, area,
			widget, detail, x, y, width, height);
    }
 
  if (CHECK_DETAIL(detail, "optionmenu") ||  (CHECK_DETAIL(detail, "button") && 
       (is_combo_box(widget, FALSE)) && !(is_combo_box_entry(widget))))
    {
      GtkRequisition indicator_size;
      GtkBorder indicator_spacing;
      gint vline_x;
 
      if (state_type != GTK_STATE_INSENSITIVE)
        state_type = GTK_STATE_NORMAL;
 
      option_menu_get_props (widget, &indicator_size, &indicator_spacing);
 
      if (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
	vline_x = x + indicator_size.width + indicator_spacing.left + indicator_spacing.right;
      else
	vline_x = x + width - (indicator_size.width + indicator_spacing.left + 
                               indicator_spacing.right) - style->xthickness;
 
      do_redmond_draw_line(window, style->dark_gc[state_type], style->light_gc[state_type], 
                           area, y + style->ythickness + 1, y + height - style->ythickness - 2, 
                           vline_x, FALSE);
 
      if (gtk_widget_get_direction (GTK_WIDGET (widget)) == GTK_TEXT_DIR_RTL) 
         x +=  indicator_spacing.right + style->xthickness;
      else
         x += width - indicator_size.width - indicator_spacing.right - style->xthickness;
 
      y += ((height - indicator_size.height) / 2) + 1;
 
      width = indicator_size.width;
      height = indicator_size.height;
 
      do_redmond_draw_arrow (window, style->fg_gc[state_type], GTK_ARROW_DOWN, 
                             x, y, width, height);
   }
}
 
/***********************************************
 * redmond_draw_tab -
 *  
 *   Function pretty much only called to draw 
 *   the arrow "tab" in the option menu
 ***********************************************/
void
redmond_draw_tab (GtkStyle * style,
	  GdkWindow * window,
	  GtkStateType state,
	  GtkShadowType shadow,
	  GdkRectangle * area,
	  GtkWidget * widget,
	  const gchar * detail, 
          gint x, 
          gint y, 
          gint width, 
          gint height)
{
}
 
/***********************************************
 * redmond_draw_slider -
 *  
 *   Function which draws scale/scrollbar slider
 ***********************************************/
void
redmond_draw_slider (GtkStyle * style,
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
             GtkOrientation orientation)
{
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
 
  if ((CHECK_DETAIL (detail, "hscale")) || (CHECK_DETAIL (detail, "vscale")))
    {
      do_redmond_draw_default_fill (style, window,
                                    state_type, area, x, y, width,
                                    height);
 
      redmond_draw_shadow (style, window, state_type, GTK_SHADOW_OUT, area,
                           widget, detail, x, y, width, height);
    }
  else
    redmond_draw_box (style, window, state_type, shadow_type, area,
                      widget, detail, x, y, width, height);
}
  
/***********************************************
 * redmond_draw_extension -
 *  
 *   Function which draws notebook extensions -
 *   aka, tabs.
 *
 *  This routine is so complex mostly because
 *  it tries to draw a faked 1-2 pixel rounded
 *  corner.
 ***********************************************/
void
redmond_draw_extension (GtkStyle * style,
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
                GtkPositionType gap_side)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
 
  GdkGC *gc1 = NULL;
  GdkGC *gc2 = NULL;
  GdkGC *gc3 = NULL;
  GdkGC *gc4 = NULL;
 
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
 
  do_redmond_draw_default_fill (style, window, GTK_STATE_NORMAL, area, 
                                x, y, width, height);
 
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
      gc1 = style->dark_gc[state_type];
      gc2 = redmond_style->black_border_gc[state_type];
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
      gc4 = redmond_style->black_border_gc[state_type];
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
	  do_redmond_draw_default_fill (style, window, state_type, NULL,
                                        x + style->xthickness, 
                                        y,
                                        width - (2 * style->xthickness),
                                        height - (style->ythickness));
 
	  gdk_draw_line (window, gc1, x, y, x, y + height - 2);
	  gdk_draw_line (window, gc2, x + 1, y, x + 1, y + height - 2);
 
	  gdk_draw_line (window, gc3,
			 x + 2, y + height - 2, x + width - 2,
			 y + height - 2);
	  gdk_draw_line (window, gc3, x + width - 2, y, x + width - 2,
			 y + height - 2);
	  gdk_draw_line (window, gc4, x + 1, y + height - 1, x + width - 2,
			 y + height - 1);
	  gdk_draw_line (window, gc4, x + width - 1, y, x + width - 1,
			 y + height - 2);
	  break;
	case GTK_POS_BOTTOM:
	  do_redmond_draw_default_fill (style, window, state_type, NULL,
                                        x + style->xthickness, 
                                        y + style->ythickness,
                                        width - (2 * style->xthickness),
                                        height - (style->ythickness));
 
	  gdk_draw_line (window, gc1, x + 2, y, x + width - 3, y);
	  gdk_draw_line (window, gc1, x, y + 2, x, y + height - 1);
 
	  gdk_draw_line (window, gc2, x + 1, y + 1, x + width - 2, y + 1);
	  gdk_draw_line (window, gc2, x + 1, y + 1, x + 1, y + height - 1);
	  gdk_draw_line (window, gc1, x, y + 2, x + 1, y + 1);
 
	  gdk_draw_line (window, gc3,
			 x + width - 2, y + 2, x + width - 2, y + height - 1);
	  gdk_draw_line (window, gc4, x + width - 1, y + 2, x + width - 1,
			 y + height - 1);
	  gdk_draw_line (window, gc4, x + width - 1, y + 2, x + width - 2,
			 y + 1);
	  break;
	case GTK_POS_LEFT:
	  do_redmond_draw_default_fill (style, window, state_type, area, 
                                        x,
                                        y + style->ythickness,
                                        width - (style->xthickness),
                                        height - (2 * style->ythickness));
 
	  gdk_draw_line (window, gc1, x, y, x + width - 2, y);
	  gdk_draw_line (window, gc2, x + 1, y + 1, x + width - 2, y + 1);
 
	  gdk_draw_line (window, gc3,
			 x, y + height - 2, x + width - 2, y + height - 2);
	  gdk_draw_line (window, gc3,
			 x + width - 2, y + 2, x + width - 2, y + height - 2);
	  gdk_draw_line (window, gc4, x, y + height - 1, x + width - 2,
			 y + height - 1);
	  gdk_draw_line (window, gc4, x + width - 1, y + 1, x + width - 1,
			 y + height - 2);
	  break;
	case GTK_POS_RIGHT:
	  do_redmond_draw_default_fill (style, window, state_type, NULL,
                                        x + style->xthickness,
                                        y + style->ythickness,
                                        width - (style->xthickness),
                                        height - (2 * style->ythickness));
 
	  gdk_draw_line (window, gc1, x + 1, y, x + width - 1, y);
	  gdk_draw_line (window, gc1, x, y + 1, x, y + height - 2);
	  gdk_draw_line (window, gc2, x + 1, y + 1, x + width - 1, y + 1);
	  gdk_draw_line (window, gc2, x + 1, y + 1, x + 1, y + height - 2);
 
	  gdk_draw_line (window, gc3,
			 x + 2, y + height - 2, x + width - 1,
			 y + height - 2);
	  gdk_draw_line (window, gc4, x + 1, y + height - 1, x + width - 1,
			 y + height - 1);
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
 
/***********************************************
 * redmond_draw_handle -
 *  
 *   Function which draws Handle box-
 *   aka toolbar drag grip/paned drag grip
 ***********************************************/
void
redmond_draw_handle (GtkStyle * style,
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
             GtkOrientation orientation)
{
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
  
  gboolean left_cutoff = FALSE, right_cutoff = FALSE, top_cutoff = FALSE, bottom_cutoff = FALSE;
  
  if (IS_BONOBO_DOCK_ITEM_GRIP(widget) && 
     (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) && 
      orientation == (GTK_ORIENTATION_HORIZONTAL))
  {
    x = widget->parent->allocation.width - widget->allocation.width;
    y = widget->parent->allocation.height - widget->allocation.height;
    width = widget->allocation.width;
    height = widget->allocation.height;
    
    area = NULL;
  }
  
  do_redmond_draw_default_fill (style, window, state_type, area, 
                                x, y, width, height);
 
  if (widget && !(IS_PANED (widget)))
    {
      if (IS_HANDLE_BOX (widget))
	{
	  /* handle box apears to be broken in that
	   * it doesn't pass the orientation properly,
	   * but always passes as vertical, so we look
	   * it up ourselves.
	   */
	  switch (gtk_handle_box_get_handle_position
		  (GTK_HANDLE_BOX (widget)))
	    {
	    case GTK_POS_LEFT:
	    case GTK_POS_RIGHT:
	      orientation = GTK_ORIENTATION_HORIZONTAL;
	      break;
 
	    case GTK_POS_TOP:
	    case GTK_POS_BOTTOM:
	      orientation = GTK_ORIENTATION_VERTICAL;
	      break;
	    }
	}
      else
	{
	  if ((CHECK_DETAIL (detail, "handlebox")
	       && (!IS_HANDLE_BOX_ITEM (widget))))
	    {
	      /* panel_applet_frame used by the panel is broken,
	       * always passes orientation == horizontal 
	       * since we don't know anything about the panel
	       * here, we just determine orientation by size.
	       *
	       * If this is on a PanelWidget, we also offset the 
	       * actual drag bar by 2 pixels, since we draw
	       * a line next to it instead of drawing a border
	       * around it.
	       */
	      if (width < height)
		{
		  orientation = GTK_ORIENTATION_HORIZONTAL;
		  if (is_panel_widget_item (widget))
		    x += 2;
		}
	      else
		{
		  orientation = GTK_ORIENTATION_VERTICAL;
		  if (is_panel_widget_item (widget))
		    y += 2;
		}
	    }
	}
 
      /* draw the drag bar */
      if (orientation == GTK_ORIENTATION_VERTICAL)
        {
  	  do_redmond_draw_shadow (window, style->light_gc[state_type],
			          style->dark_gc[state_type],
			          x + style->xthickness + 1, y + height / 2 - 1,
			          width - style->xthickness - 3, 3, FALSE);
 	
          bottom_cutoff = TRUE;
        }
      else
        {
          right_cutoff = (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR);
          left_cutoff = !right_cutoff;
 
	  do_redmond_draw_shadow (window, style->light_gc[state_type],
			          style->dark_gc[state_type], x + width / 2 - 1,
			          y + style->ythickness + 1, 3,
			          height - style->ythickness - 3, FALSE);
			          
        }
      if (is_panel_widget_item (widget)
	  && (CHECK_DETAIL (detail, "handlebox")
	      && (!IS_HANDLE_BOX_ITEM (widget)))
	  && (!(IS_HANDLE_BOX (widget))))
	{
	  /* If this is on a PanelWidget, we draw a line 
	   * next to it instead of drawing a border around it.
	   */
	  if (orientation == GTK_ORIENTATION_VERTICAL)
	    {
	      gdk_draw_line (window, style->dark_gc[state_type], x + 1, y + 0,
			     x + width - 2, y + 0);
	      gdk_draw_line (window, style->light_gc[state_type], x + 1,
			     y + 1, x + width - 2, y + 1);
	    }
	  else
	    {
	      gdk_draw_line (window, style->dark_gc[state_type], x + 0, y + 1,
			     x + 0, y + height - 2);
	      gdk_draw_line (window, style->light_gc[state_type], x + 1,
			     y + 1, x + 1, y + height - 2);
	    }
	}
      else
	{
	  GdkRectangle shadow, clip;
	  gboolean skip_shadow = FALSE;
 
	  shadow.x = x;
          shadow.y = y;
          shadow.width = width;
          shadow.height = height;
 
          if (area)
            gdk_rectangle_intersect(area, &shadow, &clip);
          else
            clip = shadow;
 
          if (IS_BONOBO_DOCK_ITEM_GRIP(widget))
            {
              if IS_BOX(widget->parent)
                {
                  GList *children = NULL, *child = NULL;
 
                  children = gtk_container_get_children(GTK_CONTAINER(widget->parent));
              
                  for (child = g_list_first(children); child; child = g_list_next(child))
                    {
	              if (IS_BOX(child->data))
	                {
	                  skip_shadow = TRUE;
	                  child = NULL;
	                }
                    }	 
                             
                  if (children)   
  	            g_list_free(children);
                }
            }  
             
          if (!skip_shadow)
	    gtk_paint_shadow (style, window, state_type, GTK_SHADOW_ETCHED_IN,
			      &clip, widget, detail, 
                              x - 1 - 2*left_cutoff, 
                              y - 1 - 2*top_cutoff, 
                              width + 2 + 2*right_cutoff,
			      height + 2 + 2*bottom_cutoff);
	}
    }
}