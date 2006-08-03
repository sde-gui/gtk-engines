/******************************************************************************/
/* smooth_gtk2_misc.c - Misc Functions for GTK2 Engine                        */
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
/* Portions Based on GTK+                                                     */
/*   Peter Mattis <petm@xcf.berkeley.edu>                                     */
/*   Spencer Kimball <spencer@xcf.berkeley.edu>                               */
/*   Josh MacDonald <jmacd@xcf.berkeley.edu>                                  */
/*                                                                            */
/* Portions Based on the EnGradient Engine                                    */
/*   Andrew Cattau                                                            */
/*                                                                            */
/* Portions Based on the Metal Engine                                         */
/*  Randy Gordon <randy@integrand.com>                                        */
/*  Owen Taylor <otaylor@redhat.com>                                          */
/*                                                                            */
/* Portions Based on the ThinIce Engine                                       */
/*   Tim Gerla <timg@rrv.net>                                                 */
/*   Tomas Ögren <stric@ing.umu.se>                                           */
/*   Richard Hult <rhult@codefactory.se>                                      */
/*                                                                            */
/* Portions Based on the Wonderland Engine                                    */
/*   Garrett LeSage                                                           */
/*   Alexander Larsson                                                        */
/*   Owen Taylor <otaylor@redhat.com>                                         */
/*                                                                            */
/* Portions Based on the XFCE Engine                                          */
/*   Olivier Fourdan <fourdan@xfce.org>                                       */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#include "smooth_gtk2_engine.h"
#include "smooth_gtk2_rc.h"
#include "smooth_gtk2_drawing.h"
#include "smooth_gtk2_misc.h"

/* Misc Functions */
GdkBitmap *
smooth_arc_clip_mask(gint width,
	      gint height)
{
  GdkBitmap *result;
  GdkGC *gc;
  GdkColor color;

  result = (GdkBitmap *)gdk_pixmap_new(NULL, width, height, 1);
  gc = gdk_gc_new(result);
	
  color.pixel = 0;
  gdk_gc_set_foreground(gc, &color);

  gdk_draw_rectangle(result, gc, TRUE, 0, 0, width, height);
  gdk_draw_rectangle(result, gc, FALSE, 0, 0, width, height);

  color.pixel = 1;
  gdk_gc_set_foreground(gc, &color);

  gdk_draw_arc(result, gc, TRUE, 0, 0, width + 1, height + 1, 0, 360*64);
  gdk_draw_arc(result, gc, FALSE, 0, 0, width, height, 0, 360*64);

  g_object_unref(gc);

  return result;
}

/*
GtkWidget *
get_notebook_page(GtkWidget * widget)
{
  g_return_val_if_fail(GTK_IS_WIDGET(widget), NULL);

  while (widget && widget->parent && !g_type_is_a(GTK_WIDGET_TYPE(widget->parent), GTK_TYPE_NOTEBOOK))
    widget = widget->parent;

  if (!(widget && widget->parent && g_type_is_a(GTK_WIDGET_TYPE(widget->parent), GTK_TYPE_NOTEBOOK)))
    return NULL;
      

  return widget;
}

GtkWidget *
get_tab_label(GtkWidget *page, GtkWidget * widget)
{
  g_return_val_if_fail(GTK_IS_WIDGET(widget), NULL);

  widget = gtk_notebook_get_menu_label(GTK_NOTEBOOK(page->parent), page);
  if (!(widget)) 
     widget = gtk_notebook_get_tab_label(GTK_NOTEBOOK(page->parent), page);
     
  return widget;
}

gboolean 
widget_is_tab_label(GtkWidget *page, GtkWidget * widget)
{
  GtkWidget * real_widget = widget;
  g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);

  widget = gtk_notebook_get_menu_label(GTK_NOTEBOOK(page->parent), page);
  if (!(widget)) 
     widget = gtk_notebook_get_tab_label(GTK_NOTEBOOK(page->parent), page);
     
  if ((real_widget) && (widget) && (real_widget != widget) && (!(gtk_widget_is_ancestor(real_widget, widget))))
    return FALSE;

  return TRUE;
}

gboolean 
tab_label_is_current_page(GtkWidget *page, GtkWidget * widget)
{
  gint current_num = 0;
  GtkWidget *current_page = NULL; 
  GtkWidget *current_label = NULL;

  current_num = gtk_notebook_get_current_page(GTK_NOTEBOOK(page->parent));
  if (current_num ==-1)
    return FALSE;

  current_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(page->parent), current_num); 

  if (!(current_page))
    return FALSE;

  current_label = gtk_notebook_get_tab_label(GTK_NOTEBOOK(page->parent), current_page);

  if ((current_label == NULL) || ((current_label != widget) && (!gtk_widget_is_ancestor(widget, current_label))))
    return FALSE;

  return TRUE;
}
*/
/* From GTK-Engines Metal 2.0:
 * 
 * This function makes up for some brokeness in gtkrange.c
 * where we never get the full arrow of the stepper button
 * and the type of button in a single drawing function.
 *
 * It doesn't work correctly when the scrollbar is squished
 * to the point we don't have room for full-sized steppers.
 */
void
smooth_reverse_engineer_stepper_box (GtkWidget    *range,
			      GtkArrowType  arrow_type,
			      gint         *x,
			      gint         *y,
			      gint         *width,
			      gint         *height)
{
  gint slider_width = 15, stepper_size = 15;
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

  *x = (*x - (box_width - *width) / 2) + 2;
  *y = (*y - (box_height - *height) / 2) + 2;
  *width = box_width - 3;
  *height = box_height - 3;
}

/* This function is based on smooth_reverse_engineer_stepper_box
 * (and gtk2 sources) except it is for getting spin button 
 * size instead. It is not always right, and only returns 
 * a (hopefully more accurate) arrow box, not the entire
 * button box, as the button box is passed correctly
 * to paint_box and so only paint_arrow needs this.
 */
void
smooth_reverse_engineer_spin_button (GtkWidget    *widget,
			      GtkArrowType  arrow_type,
			      gint         *x,
			      gint         *y,
			      gint         *width,
			      gint         *height)
{
  gint size = pango_font_description_get_size (widget->style->font_desc);
  gint realheight, realwidth;

  realwidth = MIN(PANGO_PIXELS (size), 30);

  realwidth -= realwidth % 2; /* force even */
  
  realwidth -= 2 * xthickness(widget->style);
  
  realheight = ((widget->requisition.height) - 2 * ythickness(widget->style)) / 2;
      
  realheight -= 1;
  realwidth += 1;

  *x += ((*width - realwidth) / 2);
  *y += ((*height - realheight) / 2) + (arrow_type==GTK_ARROW_DOWN?(ENTRY_BUTTON_EMBED(widget->style)?0:1):(ENTRY_BUTTON_EMBED(widget->style)?0:-1));
  *width = realwidth;
  *height = realheight;

  if (ENTRY_BUTTON_EMBED(widget->style))
    {
      GtkStyle *style = widget->style;
      smooth_part_style *part = THEME_PART(STEPPER_PART(widget->style));
      gint thick = 2;

      thick = EDGE_LINE_THICKNESS(style, part);

      if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
        *x -= (gint)thick/2;
    }
}

/* This function is a home-grown (probably flawed) function 
 * inspired by the metal, ThinIce, & Wonderland GTK2 engines
 */
void
smooth_reverse_engineer_arrow_box (GtkWidget    *widget,
                            const gchar * detail,
			    GtkArrowType  arrow_type,
			    gint         *x,
			    gint         *y,
			    gint         *width,
			    gint         *height)
{
  if (CHECK_DETAIL(detail, "hscrollbar") || CHECK_DETAIL(detail, "vscrollbar"))
    {
      smooth_reverse_engineer_stepper_box (widget, arrow_type,
				    x, y, width, height);
 
    } else if (CHECK_DETAIL(detail, "spinbutton")) {
      smooth_reverse_engineer_spin_button (widget, arrow_type,
				    x, y, width, height);
   }
  else if (CHECK_DETAIL(detail, "menuitem"))
    {
      *width += 2;
      *height += 2;
      *x -= 1;
    }
  else if ((ge_is_in_combo_box (widget) || 
               IS_SPIN_BUTTON (widget)) && 
             ENTRY_BUTTON_EMBED(widget->style))
    {
      GtkStyle *style = widget->style;
      smooth_part_style *part = smooth_button_part(style, FALSE);
      gint thick = 2;

      thick = EDGE_LINE_THICKNESS(style, part);

      if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
        *x -= (gint)thick/2;
      else
        *x += (gint)thick/2;
    }
  else if (CHECK_DETAIL(detail, "arrow")) {
      *width += 2;
      *height += 2;
      *x -= 1;
      *y -= 1;
  }  
}

void
smooth_gtk_button_get_props (GtkWidget *widget,
		      GtkBorder *default_border,
		      GtkBorder *default_outside_border,
		      gboolean  *interior_focus)
{
  GtkBorder *tmp_border;

  if (default_border)
    {
      if( GTK_IS_BUTTON(widget) )
        {
          gtk_widget_style_get (widget, "default_border", &tmp_border, NULL);
        }
      else
        tmp_border = NULL;

      if (tmp_border)
	{
	  *default_border = *tmp_border;
	  g_free (tmp_border);
	}
      else
	*default_border = (GtkBorder){ 1, 1, 1, 1 };
    }

  if (default_outside_border)
    {
      if( GTK_IS_BUTTON(widget) )
        {
          gtk_widget_style_get (widget, "default_outside_border", &tmp_border, NULL);
        }
      else
        tmp_border = NULL;

      if (tmp_border)
	{
	  *default_outside_border = *tmp_border;
	  g_free (tmp_border);
	}
      else
	*default_outside_border = (GtkBorder){ 0, 0, 0, 0 };
    }

  if (interior_focus)
    gtk_widget_style_get (widget, "interior_focus", interior_focus, NULL);
}

void
smooth_gtk_widget_get_focus_props (GtkWidget *widget,
		      gint *focus_line_width,
		      gint *focus_padding,
		      gboolean  *interior_focus)
{
  if (focus_line_width)
    gtk_widget_style_get (widget, "focus_line_width", focus_line_width, NULL);

  if (focus_padding)
    gtk_widget_style_get (widget, "focus_padding", focus_padding, NULL);

  if (interior_focus)
    gtk_widget_style_get (widget, "interior_focus", interior_focus, NULL);
}

gboolean
gtk_cell_renderer_toggle_get_inconsistent (GtkWidget *widget)
{
  gboolean result = FALSE;

  g_object_get (widget, "inconsistent", result, NULL);

  return result;
}
