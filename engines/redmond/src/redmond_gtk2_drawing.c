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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  cairo_t *cr;

  CHECK_ARGS
 
  cr = ge_gdk_drawable_to_cairo (window, area);

  do_redmond_draw_line(cr, 
                       &redmond_style->color_cube.dark[state_type], 
                       &redmond_style->color_cube.light[state_type], 
                       area, 
                       x1, x2, y, 
                       TRUE);

  cairo_destroy(cr);
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  cairo_t *cr;

  if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
    return;
 
  CHECK_ARGS
 
  cr = ge_gdk_drawable_to_cairo (window, area);

  do_redmond_draw_line(cr, 
                       &redmond_style->color_cube.dark[state_type], 
                       &redmond_style->color_cube.light[state_type], 
                       area, 
                       y1, y2, x, 
                       FALSE);

  cairo_destroy(cr);
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  cairo_t *cr;

  CHECK_ARGS
  SANITIZE_SIZE
 
  cr = ge_gdk_drawable_to_cairo (window, area);

  if (CHECK_DETAIL (detail, "check"))	/* Menu item */
    {
      /* check boxes in menu don't actually draw box, only the check mark,
       * which use the text color 
       */
      if (shadow == GTK_SHADOW_IN)
	{
	  do_redmond_draw_check (cr, &redmond_style->color_cube.text[state], x + 3, y + 3,
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
            do_redmond_draw_masked_fill (cr, &redmond_style->hatch_mask, 
                                              &redmond_style->color_cube.bg[GTK_STATE_NORMAL],
                                              &redmond_style->color_cube.light[GTK_STATE_NORMAL],
                                              x, y, width, height);
	  else
          {
            ge_cairo_set_color(cr, &redmond_style->color_cube.base[GTK_STATE_NORMAL]);

            cairo_rectangle(cr, x, y, width - 1, height - 1);

            cairo_fill(cr);
          }

	  break;
	default:
          ge_cairo_set_color(cr, &redmond_style->color_cube.bg[GTK_STATE_NORMAL]);

          cairo_rectangle(cr, x, y, width - 1, height - 1);

          cairo_fill(cr);

	}
 
      if ((shadow == GTK_SHADOW_IN) || 
          (IS_TOGGLE_BUTTON(widget) && 
           TOGGLE_BUTTON(widget)->inconsistent))
	{
	  if ((state == GTK_STATE_INSENSITIVE) || 
              (IS_TOGGLE_BUTTON(widget) && 
               TOGGLE_BUTTON(widget)->inconsistent))
            {
	      do_redmond_draw_check (cr, &redmond_style->color_cube.fg[GTK_STATE_INSENSITIVE],
			             x + 2, y + 2, width - 4, height - 4);
            }
	  else
            {
	      do_redmond_draw_check (cr, &redmond_style->color_cube.fg[GTK_STATE_NORMAL],
			             x + 2, y + 2, width - 4, height - 4);
            }
	}
 
      redmond_draw_shadow (style, window, GTK_STATE_NORMAL, GTK_SHADOW_IN,
			area, widget, detail, x, y, width, height);
    }
    
    cairo_destroy(cr);
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
 
  gint center_x;
  gint center_y;
  gint radius;

  cairo_t *cr;

  CHECK_ARGS
  SANITIZE_SIZE
 
  center_x = x + floor(width/2);
  center_y = y + floor(height/2);
  radius = floor(MIN(width, height)/2) + 1;

  cr = ge_gdk_drawable_to_cairo (window, area);

  if (CHECK_DETAIL (detail, "option"))	/* Menu item */
    {
      /* check boxes in menu don't actually draw box, only the mark,
       * which use the text color 
       */
      if (shadow == GTK_SHADOW_IN)
      {
        ge_cairo_set_color(cr, &redmond_style->color_cube.text[state]);	
	cairo_arc(cr, center_x, center_y, radius*0.38, 0, 2 * M_PI);
        cairo_fill(cr);
      }
    }
  else
    {
      gboolean inconsistent;

  do_redmond_draw_simple_circle (cr, &redmond_style->color_cube.dark[GTK_STATE_NORMAL],
				&redmond_style->color_cube.light[GTK_STATE_NORMAL],
					center_x, 
					center_y, 
					radius);

  do_redmond_draw_simple_circle (cr, &redmond_style->black_border[GTK_STATE_NORMAL],
				&redmond_style->color_cube.bg[GTK_STATE_NORMAL],
					center_x, 
					center_y, 
					radius - 1);

      inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));
      inconsistent |= (GE_IS_CELL_RENDERER_TOGGLE(widget) && ge_cell_renderer_toggle_get_inconsistent (widget));
      inconsistent |= (CHECK_DETAIL(detail, "cellradio") && (shadow == GTK_SHADOW_ETCHED_IN));
 
      switch (state)
	{
	case GTK_STATE_NORMAL:
	case GTK_STATE_PRELIGHT:
          if (inconsistent)
          {
             cairo_t *tmp = ge_gdk_drawable_to_cairo (window, area);

            cairo_arc(tmp, center_x, center_y,  radius - 2, 0, 2*M_PI);
            cairo_clip(tmp);

            do_redmond_draw_masked_fill (tmp, &redmond_style->hatch_mask, 
                                              &redmond_style->color_cube.bg[GTK_STATE_NORMAL],
                                              &redmond_style->color_cube.light[GTK_STATE_NORMAL],
                                              x, y, width, height);
	    cairo_destroy(tmp);
         }
          else
          {
            ge_cairo_set_color(cr, &redmond_style->color_cube.base[GTK_STATE_NORMAL]);
            cairo_arc(cr, center_x, center_y, radius - 2, 0, 2 * M_PI);
            cairo_fill(cr);
          }
	  break;
	default:
            ge_cairo_set_color(cr, &redmond_style->color_cube.bg[GTK_STATE_NORMAL]);
            cairo_arc(cr, center_x, center_y, radius - 2, 0, 2 * M_PI);
            cairo_fill(cr);
	}

      if ((shadow == GTK_SHADOW_IN) || inconsistent)
	{
           if ((state==GTK_STATE_INSENSITIVE) || inconsistent)
           {
             ge_cairo_set_color(cr, &redmond_style->color_cube.fg[GTK_STATE_INSENSITIVE]);
           }
           else
           {
             ge_cairo_set_color(cr, &redmond_style->color_cube.text[GTK_STATE_NORMAL]);	
           }

           cairo_arc(cr, center_x, center_y, radius*0.38, 0, 2 * M_PI);
           cairo_fill(cr);
	}
     }

     cairo_destroy(cr);
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);

  gboolean button_in = (shadow == GTK_SHADOW_IN);
  gint child_offset_x = 1, child_offset_y = 1;
  cairo_t *cr;
 
  if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
    return;
 
  CHECK_ARGS
  SANITIZE_SIZE

  if ((CHECK_DETAIL (detail, "spinbutton"))
      || (CHECK_DETAIL (detail, "vscrollbar"))
      || (CHECK_DETAIL (detail, "hscrollbar"))
      || (CHECK_DETAIL (detail, "optionmenu"))
      || GE_IS_SCROLLBAR(widget)
      || (ge_is_in_combo_box (widget)))
    {
      if (state != GTK_STATE_INSENSITIVE)
        state = GTK_STATE_NORMAL;

      if (CHECK_DETAIL (detail, "spinbutton") || CHECK_DETAIL (detail, "optionmenu"))
	{
	  if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
	    x -= 1;
	}
      else if (ge_is_in_combo_box (widget) && ((gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)))
	{
	    x += 1;
	}

      if (ge_is_combo_box_entry(widget))
        {
		x += 1;
        }
 
      /* Redmond prefers to have smaller arrows relative too size
       * for ComboBox/ComboBoxEntry, because its cleaner,
       * even though it seems slightly inconsistant.
       */
      if (widget && ge_is_in_combo_box (widget))
	{
	  y += 1;
	  width -= 2;
	  height -= 2;
	}
      else if (button_in)
	{
  	  /* SpinButton & Scrollbar Arrows offset down/right on button press */
	  x += child_offset_x;
	  y += child_offset_y;
	}
    }
  else
    {
      if (CHECK_DETAIL(detail, "menuitem"))
          x -= 1;
      else if (state != GTK_STATE_INSENSITIVE)
        state = GTK_STATE_NORMAL;
    }
 
  cr = ge_gdk_drawable_to_cairo (window, area);

  if (state == GTK_STATE_INSENSITIVE)
    {
      do_redmond_draw_arrow (cr, &redmond_style->color_cube.light[state], arrow_type, TRUE, x+1, y+1, width, height);
      do_redmond_draw_arrow (cr, &redmond_style->color_cube.fg[state], arrow_type, TRUE, x, y, width, height);
    }
  else
    do_redmond_draw_arrow (cr, &redmond_style->color_cube.fg[state], arrow_type, TRUE, x, y, width, height);

  cairo_destroy(cr);
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
  cairo_t *cr;
 
  CHECK_ARGS
  SANITIZE_SIZE
 
  if (shadow_type == GTK_SHADOW_NONE)
    return;

  cr = ge_gdk_drawable_to_cairo (window, area);
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      break;
    case GTK_SHADOW_ETCHED_IN:
      ge_cairo_simple_border (cr, &redmond_style->color_cube.dark[state_type],
		      &redmond_style->color_cube.light[state_type], x, y, width, height, TRUE);
      ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
		      &redmond_style->color_cube.dark[state_type], x + 1, y + 1, width - 2,
		      height - 2, TRUE);
      break;
    case GTK_SHADOW_ETCHED_OUT:
      ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
		      &redmond_style->color_cube.dark[state_type], x, y, width, height, TRUE);
      ge_cairo_simple_border (cr, &redmond_style->color_cube.dark[state_type],
		      &redmond_style->color_cube.light[state_type], x + 1, y + 1, width - 2,
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
           cairo_set_line_width (cr, 1);
           ge_cairo_set_color(cr, &redmond_style->black_border[state_type]);	
           cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
           cairo_stroke(cr);
	}
      else if (((CHECK_DETAIL (detail, "vscrollbar"))
		|| (CHECK_DETAIL (detail, "hscrollbar"))
		|| ((CHECK_DETAIL (detail, "button")) && (widget)
		    && GTK_WIDGET_HAS_DEFAULT (widget))))
	{
	  /* Scrollbar steppers and Defaultable Buttons use a flat
	   * dark edge on pressed.
	   */
           cairo_set_line_width (cr, 1);
           ge_cairo_set_color(cr, &redmond_style->color_cube.dark[state_type]);	
           cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
           cairo_stroke(cr);
	}
      else if (((CHECK_DETAIL (detail, "entry"))
		|| (CHECK_DETAIL (detail, "frame")))
	       && widget && ((IS_SPIN_BUTTON (widget))
			     || (ge_is_in_combo_box (widget))))
	{
	  /* The Combo/ComboBoxEntry button and the SpingButton Steppers should apear
	   * to be inset into the entry, as opposed to next to it, so we fake it
	   * by drawing an extra 2 pixels here to ignore the right(or left)
	   * edge, which will be drawn by the buttons.
	   */
 
	  if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
	    {
	      ge_cairo_simple_border (cr, &redmond_style->color_cube.dark[state_type],
			      &redmond_style->color_cube.light[state_type], x, y, width + 2,
			      height, FALSE);
	      ge_cairo_simple_border (cr,
			      &redmond_style->black_border[state_type],
			      &redmond_style->color_cube.bg[state_type], x + 1, y + 1, width,
			      height - 2, FALSE);
	    }
	  else
	    {
	      ge_cairo_simple_border (cr, &redmond_style->color_cube.dark[state_type],
			      &redmond_style->color_cube.light[state_type], x - 2, y,
			      width + 2, height, FALSE);
	      ge_cairo_simple_border (cr,
			      &redmond_style->black_border[state_type],
			      &redmond_style->color_cube.bg[state_type], x - 1, y + 1, width,
			      height - 2, FALSE);
	    }
	}
      else if ((CHECK_DETAIL (detail, "trough"))
	       || ((IS_STATUS_BAR (widget)))
	       || ((CHECK_DETAIL (detail, "frame")))
	       || ((CHECK_DETAIL (detail, "button"))
		   && (ge_is_toolbar_item (widget))))
	{
	  /* Toolbar Buttons, Status Bars, Frames, and Troughs are drawn with a thin border */
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.dark[state_type],
			  &redmond_style->color_cube.light[state_type], x, y, width, height,
			  FALSE);
	}
      else
	{
	  /* Normal IN Border */
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.dark[state_type],
			  &redmond_style->color_cube.light[state_type], x, y, width, height,
			  FALSE);
	  ge_cairo_simple_border (cr,
			  &redmond_style->black_border[state_type],
			  &redmond_style->color_cube.bg[state_type], x + 1, y + 1, width - 2,
			  height - 2, FALSE);
	}
      break;
    case GTK_SHADOW_OUT:
      if ((CHECK_DETAIL (detail, "vscrollbar"))
	  || (CHECK_DETAIL (detail, "hscrollbar"))
	  || (CHECK_DETAIL (detail, "slider"))
	  || (CHECK_DETAIL (detail, "spinbutton_up"))
	  || (CHECK_DETAIL (detail, "spinbutton_down"))
	  || ((CHECK_DETAIL (detail, "button")) && ge_is_in_combo_box (widget)))
	{
	  /* In the case of Scrollbars steppers, Sliders, Spinbutton steppers,
	   * and Combo/ComboBoxEntry buttons, the internal & external borders are 
	   * inverted when OUT since they are inset into another widgets IN edge.
	   */
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.bg[state_type],
			  &redmond_style->black_border[state_type], x, y,
			  width, height, FALSE);
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
			  &redmond_style->color_cube.dark[state_type], x + 1, y + 1, width - 2,
			  height - 2, FALSE);
	}
      else if ((CHECK_DETAIL (detail, "frame"))
	       || ((CHECK_DETAIL (detail, "button"))
		   && (ge_is_toolbar_item (widget))))
	{
	  /* Toolbar Buttons and Frames are drawn with a thin border */
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
			  &redmond_style->color_cube.dark[state_type], x, y, width, height,
			  FALSE);
	}
      else
	{
	  /* Normal OUT Border */
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
			  &redmond_style->black_border[state_type], x, y,
			  width, height, FALSE);
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.bg[state_type],
			  &redmond_style->color_cube.dark[state_type], x + 1, y + 1, width - 2,
			  height - 2, FALSE);
	}
      break;
    }

    cairo_destroy(cr);
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  cairo_t *cr;

  /* The Combo/ComboBoxEntry button should apear to be inset into the entry, 
   * as opposed to next to it, so we fake it by drawing an entry fill
   * then an entry border - but 2 pixels bigger so it overlaps real border
   * and so left( or right) edge doesn't actually get drawn on screen;
   * and THEN draw the button, but essentially 4 pixels smaller.
   */
  GtkStyle * parent_style = style;
  GtkStateType parent_state = state_type;

  if ((!(widget)) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
    {
      if (ge_is_combo_box_entry (widget)
          || ge_is_combo_box (widget, TRUE))
        {
          if (!ge_is_combo_box_entry (widget))
            {
              if ((widget->parent))
                {
                  gtk_widget_ensure_style(widget->parent);

                  parent_style = widget->parent->style;
                  parent_state = widget->parent->state;
                }

	      if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

	      gdk_draw_rectangle (window,
	      	                  parent_style->base_gc[parent_state],
			          TRUE, x - 2, y, width + 2, height);

            }
          else
            gtk_paint_flat_box (style, window, state_type,
			        GTK_SHADOW_NONE, area, widget, "entry_bg", x - 2,
			        y, width + 2, height);
 
          gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
  		            area, widget, "entry", x - 2, y, width, height);
        }
      else if (ge_is_combo(widget))
        {
          GtkWidget *entry = widget;

	  if (GE_IS_WIDGET(widget) && GE_IS_WIDGET(widget->parent) && GE_IS_ENTRY(GTK_COMBO (widget->parent)->entry))
            {
               entry = GTK_COMBO (widget->parent)->entry;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
             }
          else if (GE_IS_WIDGET(widget->parent))
            {
               entry = widget->parent;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
            }
 
	  if (parent_state != GTK_STATE_INSENSITIVE)
            parent_state = GTK_STATE_NORMAL;

          gtk_paint_flat_box (parent_style, window, parent_state,
	 		      GTK_SHADOW_NONE, area, entry, "entry_bg", x - 2,
			      y, width + 2, height);

          {
            GdkRectangle shadow, clip;
          
            shadow.x = x - 2;
            shadow.y = y;
            shadow.width = width + 2;
            shadow.height = height;
          
           if (area)
              gdk_rectangle_intersect(area, &shadow, &clip);
            else
              clip = shadow;

            gtk_paint_shadow (parent_style, window, parent_state, GTK_SHADOW_IN,
	 	              &clip, entry, "entry", x - 4, y, width + 2, height);
          }
        }
      else
        {
          GtkWidget *parent = widget;
      
          if (widget->parent)
            parent = widget->parent;
        
          if ((parent))
            {
              gtk_widget_ensure_style(parent);

              parent_style = parent->style;
              parent_state = parent->state;
            }

          if (parent_state != GTK_STATE_INSENSITIVE)
            parent_state = GTK_STATE_NORMAL;

          gtk_paint_flat_box (parent_style, window, parent_state,
			      GTK_SHADOW_NONE, area, parent, "entry_bg", x - 2,
			      y, width + 2, height);
          gtk_paint_shadow (parent_style, window, parent_state, GTK_SHADOW_IN,
		   	    area, parent, "entry", x - 2, y, width, height);
        }

       cr = ge_gdk_drawable_to_cairo (window, area);
 
        do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
				          x, y + 2, width - 2, height - 4);
      cairo_destroy(cr);

      if (shadow_type == GTK_SHADOW_IN)
        gdk_draw_rectangle (window, style->dark_gc[state_type], FALSE, x,
	   		    y + 2, width - 3, height - 5);
      else
        gtk_paint_shadow (style, window, state_type, shadow_type, area,
		          widget, detail, x, y + 2, width - 2, height - 4);
    } 
  else
    {
      if (ge_is_combo_box_entry (widget)
          || ge_is_combo_box (widget, TRUE))
        {
          if (!ge_is_combo_box_entry (widget))
            {
              if ((widget->parent))
                {
                  gtk_widget_ensure_style(widget->parent);

                  parent_style = widget->parent->style;
                  parent_state = widget->parent->state;
                }

              if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

	      gdk_draw_rectangle (window,
		  	          widget->parent->style->
			          base_gc[GTK_WIDGET_STATE (widget->parent)],
			          TRUE, x + 2, y, width + 2, height);
            }
          else
            gtk_paint_flat_box (style, window, state_type,
	  	                GTK_SHADOW_NONE, area, widget, "entry_bg", x + 2,
			        y, width + 2, height);
 
          gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
	  		    area, widget, "entry", x + 2, y, width, height);
        }
      else if (ge_is_combo(widget))
        {
          GtkWidget *entry = widget;

	  if (GE_IS_WIDGET(widget) && GE_IS_WIDGET(widget->parent) && GE_IS_ENTRY(GTK_COMBO (widget->parent)->entry))
            {
               entry = GTK_COMBO (widget->parent)->entry;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
             }
          else if (GE_IS_WIDGET(widget->parent))
            {
               entry = widget->parent;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
            }
  
           if (parent_state != GTK_STATE_INSENSITIVE)
             parent_state = GTK_STATE_NORMAL;

           gtk_paint_flat_box (parent_style, window, parent_state,
			       GTK_SHADOW_NONE, area, entry, "entry_bg", x + 2,
                               y, width + 2, height);
           gtk_paint_shadow (parent_style, window, parent_state, GTK_SHADOW_IN,
			     area, entry, "entry", x + 2, y, width, height);
        }
      else
        {
          GtkWidget *parent = widget;
      
          if (widget->parent)
            parent = widget->parent;
        
          if ((parent))
            {
              gtk_widget_ensure_style(parent);

              parent_style = parent->style;
              parent_state = parent->state;
            }

          if (parent_state != GTK_STATE_INSENSITIVE)
            parent_state = GTK_STATE_NORMAL;

          gtk_paint_flat_box (parent_style, window, parent_state,
			      GTK_SHADOW_NONE, area, parent, "entry_bg", x + 2,
			      y, width + 2, height);
          gtk_paint_shadow (parent_style, window, parent_state, GTK_SHADOW_IN,
		   	    area, parent, "entry", x + 2, y, width, height);
        }
 
      cr = ge_gdk_drawable_to_cairo (window, area);

      do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
				          x + 2, y + 2, width - 2, height - 4);

      cairo_destroy(cr);
 
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  GdkRectangle spin_area;
  cairo_t *cr;
 
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
 
  if ((!(widget)) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
    {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
	  gtk_paint_flat_box (style, window, state_type,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg", x - 2, y, width + 2, height);
	  gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
			    &spin_area, widget, detail, x - 2, y,
			    width + 2, height + 2);
          cr = ge_gdk_drawable_to_cairo (window, area);
	  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type), 
					      x, y + 2, width - 2, height - 2);
          cairo_destroy(cr);
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
          cr = ge_gdk_drawable_to_cairo (window, area);
	  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
					      x, y, width - 2, height - 2);
          cairo_destroy(cr);
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
          cr = ge_gdk_drawable_to_cairo (window, area);
	  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type), 
					      x + 2, y + 2, width - 2, height - 2);
          cairo_destroy(cr);
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
          cr = ge_gdk_drawable_to_cairo (window, area);
	  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type), 
					      x + 2, y, width - 2, height - 2);
          cairo_destroy(cr);
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
 *   general widgets (buttons, entries).
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
  cairo_t *cr;
 
  CHECK_ARGS
  SANITIZE_SIZE
 
  if (IS_MENU_SHELL(widget))
    {
      redmond_gtk2_engine_hack_menu_shell_setup_signals(widget);
    }  
 
  if ((CHECK_DETAIL (detail, "trough")) &&
      (widget && GE_IS_SCROLLBAR (widget)))
    {
      /* Scrollbar Troughs have a thin cross-hatch 
       * background fill. Technically its supposed
       * to change colors when actively pressed, but
       * since  thats a little hard to handle, skip it.
       */
      cr = ge_gdk_drawable_to_cairo (window, area);
      do_redmond_draw_masked_fill (cr, &redmond_style->hatch_mask, 
                                         &redmond_style->color_cube.bg[state_type],
                                         &redmond_style->color_cube.light[state_type],
                                         x, y, width, height);
 
           cairo_destroy(cr);			          

      redmond_draw_shadow (style, window, state_type, GTK_SHADOW_IN, area,
			widget, detail, x, y, width, height);
    }
  else if ((CHECK_DETAIL (detail, "trough"))
	   && (widget && GE_IS_SCALE (widget)))
    {
      /* Scale Troughs are only 4 pixels thick
       * i.e. just a border, so make sure the background
       * is filled properly, then draw the border.
       */
      cr = ge_gdk_drawable_to_cairo (window, area);

      do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                    x, y, width, height);
      if (IS_HSCALE (widget))
	{
          ge_cairo_set_color(cr, &redmond_style->black_border[state_type]);
          cairo_rectangle(cr, x, y + (height / 2), width - 2, 1);
          cairo_fill(cr);

	  redmond_draw_shadow (style, window, state_type, GTK_SHADOW_IN, area,
			    widget, detail, x, y + (height / 2) - 1, width,
			    4);
	}
      else
	{
          ge_cairo_set_color(cr, &redmond_style->black_border[state_type]);
          cairo_rectangle(cr, x + (width / 2), y, 1, height - 2);
          cairo_fill(cr);
 
	  redmond_draw_shadow (style, window, state_type, GTK_SHADOW_IN, area,
			    widget, detail, x + (width / 2) - 1, y, 4,
			    height);
	}
      cairo_destroy(cr);
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
          ((widget) && (ge_is_bonobo_dock_item(widget->parent))))
	{	  
	  GList *children = NULL, *child = NULL;
	  GtkWidget *dockitem = widget;
	  gboolean has_grip = FALSE, ltr = TRUE;
	  
	  if ((!IS_BONOBO_DOCK_ITEM(widget)) && (!IS_BOX(widget)))
	    dockitem = widget->parent;
	    
	  has_grip = GE_IS_CONTAINER(dockitem);
	  
	  ltr = (!widget) || (gtk_widget_get_direction (dockitem) == GTK_TEXT_DIR_LTR);
	  
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
 
	      if GE_IS_HBOX(dockitem)
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
                left_cutoff = (!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR);
	        right_cutoff = !left_cutoff;
              break;
 
	      case GTK_POS_RIGHT:
                left_cutoff = (widget) && (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL);
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
      else if (IS_HANDLE_BOX_ITEM(widget) && GTK_WIDGET_REALIZED(widget->parent) && GTK_WIDGET_VISIBLE(widget->parent))
        {
	  switch (gtk_handle_box_get_handle_position
		  (GTK_HANDLE_BOX (widget->parent)))
	    {
	      case GTK_POS_LEFT:
                left_cutoff = (!widget) || (gtk_widget_get_direction (widget->parent) == GTK_TEXT_DIR_LTR);
                right_cutoff= !left_cutoff;		
              break;
 
	      case GTK_POS_RIGHT:
                left_cutoff = (widget) && (gtk_widget_get_direction (widget->parent) == GTK_TEXT_DIR_RTL);
	        right_cutoff = !left_cutoff;
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

          if (left_cutoff)
            {
               x -= 2;
               width += 2;
	    }
          else
            {
               width += 2;
  	    }
        }
  
      cr = ge_gdk_drawable_to_cairo (window, area);
      do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
					  x, y, width, height);
 
      /* If this is a menu embedded in the gnome-panel, we don't
       *  draw a border since it looks cleaner without one.
       */
      if ((widget) && (widget->parent) &&
	  ((!((CHECK_DETAIL (detail, "menubar")) && 
	  ge_is_panel_widget_item (widget)))))
        {
          cairo_save(cr);

          cairo_rectangle(cr, x + 2*left_cutoff, y + 2*top_cutoff, width - 2*left_cutoff, height - 2*top_cutoff);
          
          cairo_clip(cr);			          

	  ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
			          &redmond_style->color_cube.dark[state_type], x, y, 
                                  width + 2*right_cutoff, 
                                  height + 2*bottom_cutoff,
			          FALSE);

           cairo_restore(cr);			          
        }
      cairo_destroy(cr);
    }
  else if ((CHECK_DETAIL (detail, "menuitem")) && widget && widget->parent
	   && GE_IS_MENU_BAR (widget->parent))
    {
      /* Primary Menu Items on Menu bars are drawn with 
       * a thin inset border on select/active,
       * and a thin outset border on prelight
       */
      CairoColor *top, *bottom;
 
      if (state_type != GTK_STATE_INSENSITIVE)
	state_type = GTK_STATE_NORMAL;
 
      cr = ge_gdk_drawable_to_cairo (window, area);

      do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
					  x, y, width, height);
		 			  
      if ((!GE_IS_MENU(GTK_MENU_ITEM(widget)->submenu)) || 
          (!(GTK_WIDGET_REALIZED(GTK_MENU_ITEM(widget)->submenu) && 
             GTK_WIDGET_VISIBLE(GTK_MENU_ITEM(widget)->submenu) &&
             GTK_WIDGET_REALIZED(GTK_MENU(GTK_MENU_ITEM(widget)->submenu)->toplevel) &&
             GTK_WIDGET_VISIBLE(GTK_MENU(GTK_MENU_ITEM(widget)->submenu)->toplevel))))
        {  
          top = &redmond_style->color_cube.light[state_type];
          bottom = &redmond_style->color_cube.dark[state_type];
        }
      else
        {
          top = &redmond_style->color_cube.dark[state_type];
          bottom = &redmond_style->color_cube.light[state_type];
        }
          
      ge_cairo_simple_border (cr, top, bottom,
		      x, y, width, height,
		      FALSE);

      cairo_destroy(cr);
    }
  else if ((CHECK_DETAIL (detail, "bar"))
	   && (widget && GE_IS_PROGRESS_BAR (widget)))
    {
      /* Progress bar status is always flat, and only has two states
       * NORMAL and INSENSITIVE, so check the state, and then do a 
       * default draw background.
       */
      if (state_type != GTK_STATE_INSENSITIVE)
	state_type = GTK_STATE_NORMAL;

      cr = ge_gdk_drawable_to_cairo (window, area);
      do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, GTK_STATE_SELECTED),
                                    x, y, width, height);
      cairo_destroy(cr);
    }
  else if ((CHECK_DETAIL (detail, "button")) && widget
	   && ge_is_in_combo_box (widget))
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
  else if (IS_TOGGLE_BUTTON(widget) && (TOGGLE_BUTTON(widget)->active))
    {
      gint pointer_x, pointer_y;
      GdkModifierType pointer_mask;

      gdk_window_get_pointer(widget->window, &pointer_x, &pointer_y, &pointer_mask);
	    
      cr = ge_gdk_drawable_to_cairo (window, area);
      if ((pointer_x >= widget->allocation.x) && 
	  (pointer_y >= widget->allocation.y) &&
	  (pointer_x < (widget->allocation.x + 
	                widget->allocation.width)) && 
	  (pointer_y < (widget->allocation.y +
	                widget->allocation.height)))
        {
          do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                        x, y, width, height);
        }
      else 
      {
           do_redmond_draw_masked_fill (cr, &redmond_style->hatch_mask, 
                                         &redmond_style->color_cube.bg[GTK_STATE_NORMAL],
                                         &redmond_style->color_cube.light[GTK_STATE_NORMAL],
                                         x, y, width, height);
      }

      cairo_destroy(cr);
      redmond_draw_shadow (style, window, state_type, shadow_type, area,
			widget, detail, x, y, width, height);
    }
  else
    {
      /* default box apearance */
      cr = ge_gdk_drawable_to_cairo (window, area);

      do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                    x, y, width,height);
      cairo_destroy(cr);
      redmond_draw_shadow (style, window, state_type, shadow_type, area,
			widget, detail, x, y, width, height);
    }
 
  if (CHECK_DETAIL(detail, "optionmenu") ||  (CHECK_DETAIL(detail, "button") && 
       (ge_is_combo_box(widget, FALSE)) && !(ge_is_combo_box_entry(widget))))
    {
      GtkRequisition indicator_size;
      GtkBorder indicator_spacing;
      gint vline_x;
 
      if (state_type != GTK_STATE_INSENSITIVE)
        state_type = GTK_STATE_NORMAL;
 
      ge_option_menu_get_props (widget, &indicator_size, &indicator_spacing);
 
      if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL))
	vline_x = x + indicator_size.width + indicator_spacing.left + indicator_spacing.right;
      else
	vline_x = x + width - (indicator_size.width + indicator_spacing.left + 
                               indicator_spacing.right) - style->xthickness;

      cr = ge_gdk_drawable_to_cairo (window, area);

      do_redmond_draw_line(cr, &redmond_style->color_cube.dark[state_type], &redmond_style->color_cube.light[state_type], 
                           area, y + style->ythickness + 1, y + height - style->ythickness - 2, 
                           vline_x, FALSE);
 
      cairo_destroy(cr);
 
      if ((widget) && (gtk_widget_get_direction (GTK_WIDGET (widget)) == GTK_TEXT_DIR_RTL))
         x +=  indicator_spacing.right + style->xthickness;
      else
         x += width - indicator_size.width - indicator_spacing.right - style->xthickness;
 
      y += ((height - indicator_size.height) / 2) + 1;
 
      width = indicator_size.width;
      height = indicator_size.height;
 
      redmond_draw_arrow (style, window, state_type, shadow_type, area, NULL, "optionmenu", 
	                      GTK_ARROW_DOWN, TRUE,  x,  y,  width,  height);
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);

  CHECK_ARGS
  SANITIZE_SIZE
 
  if ((CHECK_DETAIL (detail, "hscale")) || (CHECK_DETAIL (detail, "vscale")))
    {
      cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
      do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                    x, y, width, height);
 
      cairo_destroy(cr);
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
 
  CairoColor *color1 = NULL;
  CairoColor *color2 = NULL;
  CairoColor *color3 = NULL;
  CairoColor *color4 = NULL;
  cairo_t *cr;
 
  CHECK_ARGS
  SANITIZE_SIZE
 
  cr = ge_gdk_drawable_to_cairo (window, area);
  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, GTK_STATE_NORMAL),
                                x, y, width, height);
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
      color1 = &redmond_style->color_cube.dark[state_type];
      color2 = &redmond_style->black_border[state_type];
      color3 = &redmond_style->color_cube.bg[state_type];
      color4 = &redmond_style->color_cube.light[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      color1 = &redmond_style->color_cube.dark[state_type];
      color2 = &redmond_style->color_cube.light[state_type];
      color3 = &redmond_style->color_cube.dark[state_type];
      color4 = &redmond_style->color_cube.light[state_type];
      break;
    case GTK_SHADOW_OUT:
      color1 = &redmond_style->color_cube.light[state_type];
      color2 = &redmond_style->color_cube.bg[state_type];
      color3 = &redmond_style->color_cube.dark[state_type];
      color4 = &redmond_style->black_border[state_type];
      break;
    case GTK_SHADOW_ETCHED_OUT:
      color1 = &redmond_style->color_cube.light[state_type];
      color2 = &redmond_style->color_cube.dark[state_type];
      color3 = &redmond_style->color_cube.light[state_type];
      color4 = &redmond_style->color_cube.dark[state_type];
      break;
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
	  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                        x + style->xthickness, 
                                        y,
                                        width - (2 * style->xthickness),
                                        height - (style->ythickness));
 
	  ge_cairo_line (cr, color1, x, y, x, y + height - 2);
	  ge_cairo_line (cr, color2, x + 1, y, x + 1, y + height - 2);
 
	  ge_cairo_line (cr, color3,
			 x + 2, y + height - 2, x + width - 2,
			 y + height - 2);
	  ge_cairo_line (cr, color3, x + width - 2, y, x + width - 2,
			 y + height - 2);
	  ge_cairo_line (cr, color4, x + 1, y + height - 1, x + width - 2,
			 y + height - 1);
	  ge_cairo_line (cr, color4, x + width - 1, y, x + width - 1,
			 y + height - 2);
	  break;
	case GTK_POS_BOTTOM:
	  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                        x + style->xthickness, 
                                        y + style->ythickness,
                                        width - (2 * style->xthickness),
                                        height - (style->ythickness));
 
	  ge_cairo_line (cr, color1, x + 2, y, x + width - 3, y);
	  ge_cairo_line (cr, color1, x, y + 2, x, y + height - 1);
 
	  ge_cairo_line (cr, color2, x + 1, y + 1, x + width - 2, y + 1);
	  ge_cairo_line (cr, color2, x + 1, y + 1, x + 1, y + height - 1);
	  ge_cairo_line (cr, color1, x, y + 2, x + 1, y + 1);
 
	  ge_cairo_line (cr, color3,
			 x + width - 2, y + 2, x + width - 2, y + height - 1);
	  ge_cairo_line (cr, color4, x + width - 1, y + 2, x + width - 1,
			 y + height - 1);
	  ge_cairo_line (cr, color4, x + width - 1, y + 2, x + width - 2,
			 y + 1);
	  break;
	case GTK_POS_LEFT:
	  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                        x,
                                        y + style->ythickness,
                                        width - (style->xthickness),
                                        height - (2 * style->ythickness));
 
	  ge_cairo_line (cr, color1, x, y, x + width - 2, y);
	  ge_cairo_line (cr, color2, x + 1, y + 1, x + width - 2, y + 1);
 
	  ge_cairo_line (cr, color3,
			 x, y + height - 2, x + width - 2, y + height - 2);
	  ge_cairo_line (cr, color3,
			 x + width - 2, y + 2, x + width - 2, y + height - 2);
	  ge_cairo_line (cr, color4, x, y + height - 1, x + width - 2,
			 y + height - 1);
	  ge_cairo_line (cr, color4, x + width - 1, y + 1, x + width - 1,
			 y + height - 2);
	  break;
	case GTK_POS_RIGHT:
	  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                        x + style->xthickness,
                                        y + style->ythickness,
                                        width - (style->xthickness),
                                        height - (2 * style->ythickness));
 
	  ge_cairo_line (cr, color1, x + 1, y, x + width - 1, y);
	  ge_cairo_line (cr, color1, x, y + 1, x, y + height - 2);
	  ge_cairo_line (cr, color2, x + 1, y + 1, x + width - 1, y + 1);
	  ge_cairo_line (cr, color2, x + 1, y + 1, x + 1, y + height - 2);
 
	  ge_cairo_line (cr, color3,
			 x + 2, y + height - 2, x + width - 1,
			 y + height - 2);
	  ge_cairo_line (cr, color4, x + 1, y + height - 1, x + width - 1,
			 y + height - 1);
	  break;
	}
    }
 
    cairo_destroy(cr);
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
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  cairo_t *cr;

  gboolean left_cutoff = FALSE, right_cutoff = FALSE, top_cutoff = FALSE, bottom_cutoff = FALSE;

  CHECK_ARGS
  SANITIZE_SIZE
  
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
  
  cr = ge_gdk_drawable_to_cairo (window, area);
  do_redmond_draw_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type), 
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
		  if (ge_is_panel_widget_item (widget))
		    x += 2;
		}
	      else
		{
		  orientation = GTK_ORIENTATION_VERTICAL;
		  if (ge_is_panel_widget_item (widget))
		    y += 2;
		}
	    }
	}

      /* draw the drag bar */
      if (orientation == GTK_ORIENTATION_VERTICAL)
        {
          
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
			          &redmond_style->color_cube.dark[state_type],
			          x + style->xthickness + 1, y + height / 2 - 1,
			          width - style->xthickness - 3, 3, FALSE);

          bottom_cutoff = TRUE;
        }
      else
        {
          right_cutoff = (!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR);
          left_cutoff = !right_cutoff;
 
	  ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
			          &redmond_style->color_cube.dark[state_type], x + width / 2 - 1,
			          y + style->ythickness + 1, 3,
			          height - style->ythickness - 3, FALSE);
        }

      if (ge_is_panel_widget_item (widget)
	  && (CHECK_DETAIL (detail, "handlebox")
	      && (!IS_HANDLE_BOX_ITEM (widget)))
	  && (!(IS_HANDLE_BOX (widget))))
	{
	  /* If this is on a PanelWidget, we draw a line 
	   * next to it instead of drawing a border around it.
	   */
	  if (orientation == GTK_ORIENTATION_VERTICAL)
	    {
	      ge_cairo_line (cr, &redmond_style->color_cube.dark[state_type], x + 1, y + 0,
			     x + width - 2, y + 0);
	      ge_cairo_line (cr, &redmond_style->color_cube.light[state_type], x + 1,
			     y + 1, x + width - 2, y + 1);
	    }
	  else
	    {
	      ge_cairo_line (cr, &redmond_style->color_cube.dark[state_type], x + 0, y + 1,
			     x + 0, y + height - 2);
	      ge_cairo_line (cr, &redmond_style->color_cube.light[state_type], x + 1,
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
              if GE_IS_BOX(widget->parent)
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
            {
   	       ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
			               &redmond_style->color_cube.dark[state_type],
                                       x - 2*left_cutoff, 
                                       y - 2*top_cutoff, 
                                       width + 2*left_cutoff + 2*right_cutoff,
			               height + 2*top_cutoff + 2*bottom_cutoff,
			               FALSE);
            }
	}
    }
    cairo_destroy(cr);			          
}
