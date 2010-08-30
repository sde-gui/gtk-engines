/* Redmond95 - a cairo based GTK+ engine
 * Copyright (C) 2001 Red Hat, Inc.
 * Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>
 * Copyright (C) 2006-2007 Benjamin Berg <benjamin@sipsolutions.net>
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
 * Portions of the code based on/copied from the Bluecurve theme/engine, 
 *   License: LGPL2+
 *   Copyright:
 *     Alexander Larsson <alexl@redhat.com>
 *     Garrett LeSage
 *     Owen Taylor <otaylor@redhat.com>
 *
 * Portions of the code based on/copied from the Metal theme/engine, 
 *   License: LGPL2.1+
 *   Copyright:
 *     Randy Gordon, randy@integrand.com
 *     Owen Taylor <otaylor@redhat.com>
 *
 * Portions of the code based on/copied from the Smooth Engine,
 *   License: LGPL
 *   Copyright:
 *     Andrew Johnson <ajgenius@ajgenius.us
 *
 */


#include "redmond_gtk2_engine.h"
#include "redmond_gtk2_drawing.h"
#include "redmond_gtk2_misc.h"


/***********************************************
 * redmond_draw_vline -
 *  
 *   the Function used to draw horizontal spacers.
 ***********************************************/
void
redmond_draw_hline (GtkStyle * style,
	    cairo_t * cr,
	    GtkStateType state_type,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint x1,
            gint x2, 
            gint y)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);

  CHECK_ARGS
 
  do_redmond_draw_line(cr, 
                       &redmond_style->color_cube.dark[state_type], 
                       &redmond_style->color_cube.light[state_type], 
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
	    cairo_t * cr,
	    GtkStateType state_type,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint y1, 
            gint y2, 
            gint x)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);

  if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
    return;
 
  CHECK_ARGS
 
  do_redmond_draw_line(cr, 
                       &redmond_style->color_cube.dark[state_type], 
                       &redmond_style->color_cube.light[state_type], 
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
	    cairo_t * cr,
	    GtkStateType state,
	    GtkShadowType shadow,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint x, 
            gint y, 
            gint width, 
            gint height)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);

  CHECK_ARGS
 
  if (CHECK_DETAIL (detail, "check") && /* Special case for menu items. */
     (shadow != GTK_SHADOW_ETCHED_IN))
    {
      if (shadow == GTK_SHADOW_IN)
        do_redmond_draw_check (cr, &redmond_style->color_cube.text[GTK_STATE_NORMAL],
                               x + 2, y + 2, width - 4, height - 4);
    }
  else
    {
      if (state == GTK_STATE_ACTIVE || state == GTK_STATE_INSENSITIVE)
      {
        ge_cairo_set_color(cr, &redmond_style->color_cube.bg[GTK_STATE_NORMAL]);

        cairo_rectangle(cr, x, y, width - 1, height - 1);

        cairo_fill(cr);
      }
      else
      {
        /* Draw stipples for inconsistent checkboxes. */
        if (shadow == GTK_SHADOW_ETCHED_IN)
          {
            do_redmond_draw_masked_fill (cr, &redmond_style->hatch_mask, 
                                             &redmond_style->color_cube.bg[GTK_STATE_NORMAL],
                                             &redmond_style->color_cube.light[GTK_STATE_NORMAL],
                                             x, y, width, height);
          }
        else
          {
            /* ignore PRELIGHT state ... */
            ge_cairo_set_color(cr, &redmond_style->color_cube.base[GTK_STATE_NORMAL]);

            cairo_rectangle(cr, x, y, width - 1, height - 1);

            cairo_fill(cr);
          }
      }

      if ((shadow == GTK_SHADOW_ETCHED_IN) || (state == GTK_STATE_INSENSITIVE))
        {
          /* force insensitive color for inconsistent checkboxes */
          do_redmond_draw_check (cr, &redmond_style->color_cube.fg[GTK_STATE_INSENSITIVE],
                                 x + 2, y + 2, width - 4, height - 4);
        }
      else if (shadow == GTK_SHADOW_IN)
        {
          do_redmond_draw_check (cr, &redmond_style->color_cube.fg[GTK_STATE_NORMAL],
                                 x + 2, y + 2, width - 4, height - 4);
        }
 
      redmond_draw_shadow (style, cr, GTK_STATE_NORMAL, GTK_SHADOW_IN,
			widget, detail, x, y, width, height);
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
	     cairo_t * cr,
	     GtkStateType state,
	     GtkShadowType shadow,
	     GtkWidget * widget,
	     const gchar * detail, 
             gint x,
             gint y, 
             gint width, 
             gint height)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  gdouble bullet_radius;
  gint center_x;
  gint center_y;
  gint radius;
 
  CHECK_ARGS

  center_x = x + width/2;
  center_y = y + height/2;
  radius = MAX(MIN(width, height)/2, 6);
  bullet_radius = MAX((radius - 2) * 0.50, 1);

  if (CHECK_DETAIL (detail, "check") && /* Special case for menu items. */
     (shadow != GTK_SHADOW_ETCHED_IN))
    {
      if (shadow == GTK_SHADOW_IN)
        {
          ge_cairo_set_color(cr, &redmond_style->color_cube.text[GTK_STATE_NORMAL]);
          cairo_arc(cr, center_x, center_y, bullet_radius, 0, 2 * G_PI);
          cairo_fill(cr);
        }
    }
  else
    {
      do_redmond_draw_simple_circle (cr, &redmond_style->color_cube.dark[GTK_STATE_NORMAL],
                                     &redmond_style->color_cube.light[GTK_STATE_NORMAL],
                                     center_x, center_y, radius);

      do_redmond_draw_simple_circle (cr, &redmond_style->black_border[GTK_STATE_NORMAL],
                                     &redmond_style->color_cube.bg[GTK_STATE_NORMAL],
                                     center_x, center_y, radius - 1);

      if (state == GTK_STATE_ACTIVE || state == GTK_STATE_INSENSITIVE)
      {
        ge_cairo_set_color (cr, &redmond_style->color_cube.bg[GTK_STATE_NORMAL]);

        cairo_arc  (cr, center_x, center_y,  radius - 2, 0, 2*G_PI);

        cairo_fill (cr);
      }
      else
      {
        /* Draw stipples for inconsistent checkboxes. */
        if (shadow == GTK_SHADOW_ETCHED_IN)
          {
            cairo_save (cr);
            cairo_arc  (cr, center_x, center_y,  radius - 2, 0, 2*G_PI);
            cairo_clip (cr);

            do_redmond_draw_masked_fill (cr, &redmond_style->hatch_mask, 
                                             &redmond_style->color_cube.bg[GTK_STATE_NORMAL],
                                             &redmond_style->color_cube.light[GTK_STATE_NORMAL],
                                             x, y, width, height);

            cairo_restore (cr);
          }
        else
          {
            /* ignore PRELIGHT state ... */
            ge_cairo_set_color (cr, &redmond_style->color_cube.base[GTK_STATE_NORMAL]);

            cairo_arc  (cr, center_x, center_y,  radius - 2, 0, 2*G_PI);

            cairo_fill (cr);
          }
      }

      if ((shadow == GTK_SHADOW_ETCHED_IN) || (state == GTK_STATE_INSENSITIVE))
        {
          /* force insensitive color for inconsistent checkboxes */
          ge_cairo_set_color(cr, &redmond_style->color_cube.fg[GTK_STATE_INSENSITIVE]);
          cairo_arc(cr, center_x, center_y, bullet_radius, 0, 2 * G_PI);
          cairo_fill(cr);
        }
      else if (shadow == GTK_SHADOW_IN)
        {
          ge_cairo_set_color(cr, &redmond_style->color_cube.text[GTK_STATE_NORMAL]);
          cairo_arc(cr, center_x, center_y, bullet_radius, 0, 2 * G_PI);
          cairo_fill(cr);
        }
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
            gint height)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);

  gboolean button_in = (shadow == GTK_SHADOW_IN);
  gint child_offset_x = 1, child_offset_y = 1;
 
  if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
    return;
 
  CHECK_ARGS

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
 
  if (state == GTK_STATE_INSENSITIVE)
    {
      do_redmond_draw_arrow (cr, &redmond_style->color_cube.light[state], arrow_type, TRUE, x+1, y+1, width, height);
      do_redmond_draw_arrow (cr, &redmond_style->color_cube.fg[state], arrow_type, TRUE, x, y, width, height);
    }
  else
    do_redmond_draw_arrow (cr, &redmond_style->color_cube.fg[state], arrow_type, TRUE, x, y, width, height);
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
	     cairo_t * cr,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GtkWidget * widget,
	     const gchar * detail, 
             gint x, 
             gint y, 
             gint width, 
             gint height)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
 
  CHECK_ARGS
 
  if (shadow_type == GTK_SHADOW_NONE)
    return;
  
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
           ge_cairo_stroke_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
	}
      else if (((CHECK_DETAIL (detail, "vscrollbar"))
		|| (CHECK_DETAIL (detail, "hscrollbar"))
		|| ((CHECK_DETAIL (detail, "button")) && (widget)
		    && GE_WIDGET_HAS_DEFAULT (widget))))
	{
	  /* Scrollbar steppers and Defaultable Buttons use a flat
	   * dark edge on pressed.
	   */
           cairo_set_line_width (cr, 1);
           ge_cairo_set_color(cr, &redmond_style->color_cube.dark[state_type]);	
           ge_cairo_stroke_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
	}
      else if (((CHECK_DETAIL (detail, "entry"))
		|| (CHECK_DETAIL (detail, "frame")))
	       && widget && ((GE_IS_SPIN_BUTTON (widget))
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
      else if (((GE_IS_STATUSBAR (widget)))
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
      else if (CHECK_DETAIL (detail, "menu"))
        {
          ge_cairo_simple_border (cr, &redmond_style->color_cube.bg[state_type],
                          &redmond_style->black_border[state_type], x, y,
                          width, height, FALSE);
          ge_cairo_simple_border (cr, &redmond_style->color_cube.light[state_type],
                          &redmond_style->color_cube.dark[state_type], x + 1, y + 1, width - 2,
                          height - 2, FALSE);
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
		      cairo_t * cr,
		      GtkStateType state_type,
		      GtkShadowType shadow_type,
		      GtkWidget * widget,
		      const gchar * detail,
		      gint x, 
                      gint y, 
                      gint width, 
                      gint height)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);

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
              if ((gtk_widget_get_parent (widget)))
                {
                  gtk_widget_ensure_style(gtk_widget_get_parent (widget));

                  parent_style = gtk_widget_get_style (gtk_widget_get_parent (widget));
                  parent_state = gtk_widget_get_state (gtk_widget_get_parent (widget));
                }

	      if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

              gdk_cairo_set_source_color (cr,
                                          &parent_style->base[parent_state]);
              cairo_rectangle (cr, x - 2, y, width + 2, height);
              cairo_fill (cr);
            }
          else
            gtk_paint_flat_box (style, cr, state_type,
			        GTK_SHADOW_NONE, widget, "entry_bg", x - 2,
			        y, width + 2, height);
 
          gtk_paint_shadow (style, cr, state_type, GTK_SHADOW_IN,
  		            widget, "entry", x - 2, y, width, height);
        }
      else
        {
          GtkWidget *parent = widget;
      
          if (gtk_widget_get_parent (widget))
            parent = gtk_widget_get_parent (widget);
        
          if ((parent))
            {
              gtk_widget_ensure_style(parent);

              parent_style = gtk_widget_get_style (parent);
              parent_state = gtk_widget_get_state (parent);
            }

          if (parent_state != GTK_STATE_INSENSITIVE)
            parent_state = GTK_STATE_NORMAL;

          gtk_paint_flat_box (parent_style, cr, parent_state,
			      GTK_SHADOW_NONE, parent, "entry_bg", x - 2,
			      y, width + 2, height);
          gtk_paint_shadow (parent_style, cr, parent_state, GTK_SHADOW_IN,
		   	    parent, "entry", x - 2, y, width, height);
        }

        ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
				          x, y + 2, width - 2, height - 4);

      if (shadow_type == GTK_SHADOW_IN)
        {
          gdk_cairo_set_source_color (cr, &style->dark[state_type]);
          ge_cairo_inner_rectangle (cr, x, y + 2, width - 3, height - 5);
          cairo_stroke (cr);
        }
      else
        gtk_paint_shadow (style, cr, state_type, shadow_type,
		          widget, detail, x, y + 2, width - 2, height - 4);
    } 
  else
    {
      if (ge_is_combo_box_entry (widget)
          || ge_is_combo_box (widget, TRUE))
        {
          if (!ge_is_combo_box_entry (widget))
            {
              if ((gtk_widget_get_parent (widget)))
                {
                  gtk_widget_ensure_style(gtk_widget_get_parent (widget));

                  parent_style = gtk_widget_get_style (gtk_widget_get_parent (widget));
                  parent_state = gtk_widget_get_state (gtk_widget_get_parent (widget));
                }

              if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

              gdk_cairo_set_source_color (cr,
                                          &parent_style->base[parent_state]);
              cairo_rectangle (cr, x + 2, y, width + 2, height);
              cairo_fill (cr);
            }
          else
            gtk_paint_flat_box (style, cr, state_type,
	  	                GTK_SHADOW_NONE, widget, "entry_bg", x + 2,
			        y, width + 2, height);
 
          gtk_paint_shadow (style, cr, state_type, GTK_SHADOW_IN,
	  		    widget, "entry", x + 2, y, width, height);
        }
      else
        {
          GtkWidget *parent = widget;
      
          if (gtk_widget_get_parent (widget))
            parent = gtk_widget_get_parent (widget);
        
          if ((parent))
            {
              gtk_widget_ensure_style(parent);

              parent_style = gtk_widget_get_style (parent);
              parent_state = gtk_widget_get_state (parent);
            }

          if (parent_state != GTK_STATE_INSENSITIVE)
            parent_state = GTK_STATE_NORMAL;

          gtk_paint_flat_box (parent_style, cr, parent_state,
			      GTK_SHADOW_NONE, parent, "entry_bg", x + 2,
			      y, width + 2, height);
          gtk_paint_shadow (parent_style, cr, parent_state, GTK_SHADOW_IN,
		   	    parent, "entry", x + 2, y, width, height);
        }
 
      ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
				          x + 2, y + 2, width - 2, height - 4);
 
      if (shadow_type == GTK_SHADOW_IN)
        {
          gdk_cairo_set_source_color (cr, &style->dark[state_type]);
          ge_cairo_inner_rectangle (cr, x + 2, y + 2, width - 3, height - 5);
          cairo_stroke (cr);
        }
      else
        gtk_paint_shadow (style, cr, state_type, shadow_type,
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
			 cairo_t * cr,
			 GtkStateType state_type,
			 GtkShadowType shadow_type,
			 GtkWidget * widget,
			 const gchar * detail,
			 gint x, 
                         gint y, 
                         gint width, 
                         gint height)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
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
 
  if ((!(widget)) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
    {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
          cairo_save (cr);
          gdk_cairo_rectangle (cr, &spin_area);
          cairo_clip (cr);

	  gtk_paint_flat_box (style, cr, state_type,
			      GTK_SHADOW_NONE, widget,
			      "entry_bg", x - 2, y, width + 2, height);
	  gtk_paint_shadow (style, cr, state_type, GTK_SHADOW_IN,
			    widget, detail, x - 2, y,
			    width + 2, height + 2);

          cairo_restore (cr);

	  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type), 
					      x, y + 2, width - 2, height - 2);
	  gtk_paint_shadow (style, cr, state_type, shadow_type,
			    widget, detail, x, y + 2, width - 2,
			    height - 2);
	}
      else
	{
          cairo_save (cr);
          gdk_cairo_rectangle (cr, &spin_area);
          cairo_clip (cr);

	  gtk_paint_flat_box (style, cr, state_type,
			      GTK_SHADOW_NONE, widget,
			      "entry_bg", x - 2, y, width + 2, height);
	  gtk_paint_shadow (style, cr, state_type, GTK_SHADOW_IN,
			    widget, detail, x - 2, y - 2,
			    width + 2, height + 2);

          cairo_restore (cr);

	  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
					      x, y, width - 2, height - 2);
	  gtk_paint_shadow (style, cr, state_type, shadow_type,
			    widget, detail, x, y, width - 2,
			    height - 2);
	}
    }
  else
    {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
          cairo_save (cr);
          gdk_cairo_rectangle (cr, &spin_area);
          cairo_clip (cr);

	  gtk_paint_flat_box (style, cr, state_type,
			      GTK_SHADOW_NONE, widget,
			      "entry_bg", x, y, width + 4, height);
	  gtk_paint_shadow (style, cr, state_type, GTK_SHADOW_IN,
			    widget, detail, x, y,
			    width + 4, height + 2);

          cairo_restore (cr);

	  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type), 
					      x + 2, y + 2, width - 2, height - 2);
	  gtk_paint_shadow (style, cr, state_type, shadow_type,
			    widget, detail, x + 2, y + 2, width - 2,
			    height - 2);
	}
      else
	{
          cairo_save (cr);
          gdk_cairo_rectangle (cr, &spin_area);
          cairo_clip (cr);

	  gtk_paint_flat_box (style, cr, state_type,
			      GTK_SHADOW_NONE, widget,
			      "entry_bg", x, y, width + 4, height);
	  gtk_paint_shadow (style, cr, state_type, GTK_SHADOW_IN,
			    widget, detail, x, y - 2,
			    width + 4, height + 2);

          cairo_restore (cr);

	  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type), 
					      x + 2, y, width - 2, height - 2);
	  gtk_paint_shadow (style, cr, state_type, shadow_type,
			    widget, detail, x + 2, y, width - 2,
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
	  cairo_t * cr,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GtkWidget * widget,
	  const gchar * detail, 
          gint x, 
          gint y, 
          gint width, 
          gint height)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
 
  CHECK_ARGS
 
  if (GE_IS_MENU_SHELL(widget))
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
      do_redmond_draw_masked_fill (cr, &redmond_style->hatch_mask, 
                                         &redmond_style->color_cube.bg[state_type],
                                         &redmond_style->color_cube.light[state_type],
                                         x, y, width, height);
    }
  else if ((CHECK_DETAIL (detail, "trough"))
	   && (widget && GE_IS_SCALE (widget)))
    {
      /* Scale Troughs are only 4 pixels thick
       * i.e. just a border, so make sure the background
       * is filled properly, then draw the border.
       */
      ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, GTK_STATE_NORMAL),
                                    x, y, width, height);
      if (GE_IS_HSCALE (widget))
	{
          ge_cairo_set_color(cr, &redmond_style->black_border[state_type]);
          cairo_rectangle(cr, x, y + (height / 2), width - 2, 1);
          cairo_fill(cr);

	  redmond_draw_shadow (style, cr, state_type, GTK_SHADOW_IN,
			    widget, detail, x, y + (height / 2) - 1, width,
			    4);
	}
      else
	{
          ge_cairo_set_color(cr, &redmond_style->black_border[state_type]);
          cairo_rectangle(cr, x + (width / 2), y, 1, height - 2);
          cairo_fill(cr);
 
	  redmond_draw_shadow (style, cr, state_type, GTK_SHADOW_IN,
			    widget, detail, x + (width / 2) - 1, y, 4,
			    height);
	}
    }
  else if ((CHECK_DETAIL (detail, "toolbar"))
	   || (CHECK_DETAIL (detail, "menubar"))
	   || (GE_IS_BONOBO_TOOLBAR (widget))
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
           (GE_IS_BONOBO_DOCK_ITEM(widget))) || 
          ((widget) && (ge_is_bonobo_dock_item(gtk_widget_get_parent (widget)))))
	{	  
	  GList *children = NULL, *child = NULL;
	  GtkWidget *dockitem = widget;
	  gboolean has_grip = FALSE, ltr = TRUE;
	  
	  if ((!GE_IS_BONOBO_DOCK_ITEM(widget)) && (!GE_IS_BOX(widget)))
	    dockitem = gtk_widget_get_parent (widget);
	    
	  has_grip = GE_IS_CONTAINER(dockitem);
	  
	  ltr = (!widget) || (gtk_widget_get_direction (dockitem) == GTK_TEXT_DIR_LTR);
	  
	  if (has_grip)
	  {
            has_grip = FALSE;
            
            children = gtk_container_get_children(GTK_CONTAINER(dockitem));
                          
            for (child = g_list_first(children); child; child = g_list_next(child))
              {
	        if (GE_IS_BONOBO_DOCK_ITEM_GRIP(child->data))
	          {
	            GtkAllocation allocation;
	            gtk_widget_get_allocation (GTK_WIDGET (child->data), &allocation);
                    has_grip = (gtk_widget_get_visible (child->data) && 
                                gtk_widget_get_realized(child->data) && 
                                allocation.width > 1) &&
                                (allocation.height > 1);
                  }
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
	      else if (GE_IS_BONOBO_DOCK_ITEM(dockitem))
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
      else if (GE_IS_HANDLE_BOX(widget))
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
      else if (GE_IS_HANDLE_BOX_ITEM(widget) && gtk_widget_get_realized(gtk_widget_get_parent (widget)) && gtk_widget_get_visible (gtk_widget_get_parent (widget)))
        {
	  switch (gtk_handle_box_get_handle_position
		  (GTK_HANDLE_BOX (gtk_widget_get_parent (widget))))
	    {
	      case GTK_POS_LEFT:
                left_cutoff = (!widget) || (gtk_widget_get_direction (gtk_widget_get_parent (widget)) == GTK_TEXT_DIR_LTR);
                right_cutoff= !left_cutoff;		
              break;
 
	      case GTK_POS_RIGHT:
                left_cutoff = (widget) && (gtk_widget_get_direction (gtk_widget_get_parent (widget)) == GTK_TEXT_DIR_RTL);
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
  
      ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
					  x, y, width, height);
 
      /* If this is a menu embedded in the gnome-panel, we don't
       *  draw a border since it looks cleaner without one.
       */
      if ((widget) && (gtk_widget_get_parent (widget)) &&
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
    }
  else if ((CHECK_DETAIL (detail, "menuitem")) && widget && gtk_widget_get_parent (widget)
	   && GE_IS_MENU_BAR (gtk_widget_get_parent (widget)))
    {
      /* Primary Menu Items on Menu bars are drawn with 
       * a thin inset border on select/active,
       * and a thin outset border on prelight
       */
      CairoColor *top, *bottom;
      GtkWidget *submenu = NULL;
 
      if (state_type != GTK_STATE_INSENSITIVE)
	state_type = GTK_STATE_NORMAL;
 
      ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
					  x, y, width, height);

      if (GTK_IS_MENU_ITEM (widget))
        submenu = gtk_menu_item_get_submenu (GTK_MENU_ITEM (widget));
      if ((!GE_IS_MENU(submenu)) || 
          (!(gtk_widget_get_realized (submenu) && 
             gtk_widget_get_visible (submenu))))
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

      ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, GTK_STATE_SELECTED),
                                    x, y, width, height);
    }
  else if ((CHECK_DETAIL (detail, "button")) && widget
	   && ge_is_in_combo_box (widget))
    {
      /* ComboBox Buttons are an especially complex case
       * so call our special function for them.
       */
      redmond_draw_combobox_button (style, cr, state_type, shadow_type,
			    widget, detail, x, y, width, height);
    }
  else if ((CHECK_DETAIL (detail, "spinbutton_up"))
	   || (CHECK_DETAIL (detail, "spinbutton_down")))
    {
      /* SpinButton Steppers are an especially complex case
       * so call our special function for them.
       */
      redmond_draw_spinbutton_stepper (style, cr, state_type, shadow_type,
			       widget, detail, x, y, width, height);
    }
  else if (GE_IS_TOGGLE_BUTTON (widget) && (shadow_type == GTK_SHADOW_IN))
    {
      gint pointer_x, pointer_y;
      GdkModifierType pointer_mask;
      GtkAllocation allocation;
      gtk_widget_get_allocation (widget, &allocation);

      gdk_window_get_pointer(gtk_widget_get_window (widget), &pointer_x, &pointer_y, &pointer_mask);
	    
      if ((pointer_x >= allocation.x) && 
	  (pointer_y >= allocation.y) &&
	  (pointer_x < (allocation.x + 
	                allocation.width)) && 
	  (pointer_y < (allocation.y +
	                allocation.height)))
        {
          ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                        x, y, width, height);
        }
      else 
      {
           do_redmond_draw_masked_fill (cr, &redmond_style->hatch_mask, 
                                         &redmond_style->color_cube.bg[GTK_STATE_NORMAL],
                                         &redmond_style->color_cube.light[GTK_STATE_NORMAL],
                                         x, y, width, height);
      }
      redmond_draw_shadow (style, cr, state_type, shadow_type,
			widget, detail, x, y, width, height);
    }
  else
    {
      /* default box apearance */
      ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                    x, y, width,height);
      redmond_draw_shadow (style, cr, state_type, shadow_type,
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

      do_redmond_draw_line(cr, &redmond_style->color_cube.dark[state_type], &redmond_style->color_cube.light[state_type], 
                           y + style->ythickness + 1, y + height - style->ythickness - 2, 
                           vline_x, FALSE);
 
      if ((widget) && (gtk_widget_get_direction (GTK_WIDGET (widget)) == GTK_TEXT_DIR_RTL))
         x +=  indicator_spacing.right + style->xthickness;
      else
         x += width - indicator_size.width - indicator_spacing.right - style->xthickness;
 
      y += ((height - indicator_size.height) / 2) + 1;
 
      width = indicator_size.width;
      height = indicator_size.height;
 
      redmond_draw_arrow (style, cr, state_type, shadow_type, NULL, "optionmenu", 
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
	  cairo_t * cr,
	  GtkStateType state,
	  GtkShadowType shadow,
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
	     cairo_t * cr,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
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
 
  if ((CHECK_DETAIL (detail, "hscale")) || (CHECK_DETAIL (detail, "vscale")))
    {
      ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
                                    x, y, width, height);
 
      redmond_draw_shadow (style, cr, state_type, GTK_SHADOW_OUT,
                           widget, detail, x, y, width, height);
    }
  else
    redmond_draw_box (style, cr, state_type, shadow_type,
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
		cairo_t * cr,
		GtkStateType state_type,
		GtkShadowType shadow_type,
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
  gint tab_overlap = 0;
 
  CHECK_ARGS

  if (GE_IS_NOTEBOOK (widget))
    gtk_widget_style_get (widget, "tab-overlap", &tab_overlap, NULL);

  /* If the tab_overlap is large, then decrease the size of active tabs. */
  if (state_type == GTK_STATE_ACTIVE && tab_overlap >= 4)
    {
      if (gap_side == GTK_POS_TOP || gap_side == GTK_POS_BOTTOM)
        {
          x += 2;
          width -= 4;
        }
      else
        {
          y += 2;
          height -= 4;
        }
    }

  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, GTK_STATE_NORMAL),
                                x, y, width, height);
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      /* need to destroy the cairo context */
      cairo_destroy (cr);
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
	  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
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
	  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
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
	  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
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
	  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type),
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
}
 
/***********************************************
 * redmond_draw_handle -
 *  
 *   Function which draws Handle box-
 *   aka toolbar drag grip/paned drag grip
 ***********************************************/
void
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
             GtkOrientation orientation)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);

  gboolean left_cutoff = FALSE, right_cutoff = FALSE, top_cutoff = FALSE, bottom_cutoff = FALSE;

  CHECK_ARGS
  
  ge_cairo_pattern_fill (cr, DEFAULT_BACKGROUND_PATTERN(redmond_style, state_type), 
                                x, y, width, height);
 
  if (widget && !(GE_IS_PANED (widget)))
    {
      if (GE_IS_HANDLE_BOX (widget))
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
	       && (!GE_IS_HANDLE_BOX_ITEM (widget))))
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
	      && (!GE_IS_HANDLE_BOX_ITEM (widget)))
	  && (!(GE_IS_HANDLE_BOX (widget))))
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
	  gboolean skip_shadow = FALSE;
 
          if (GE_IS_BONOBO_DOCK_ITEM_GRIP(widget))
            {
              if GE_IS_BOX(gtk_widget_get_parent (widget))
                {
                  GList *children = NULL, *child = NULL;
 
                  children = gtk_container_get_children(GTK_CONTAINER(gtk_widget_get_parent (widget)));
              
                  for (child = g_list_first(children); child; child = g_list_next(child))
                    {
	              if (GE_IS_BOX(child->data))
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
}
