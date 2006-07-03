/* Redmond theme engine
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
 
static char radio_base_bits[] = {
  0x00,0x00,0x00,0x00,0xf0,0x01,0xf8,0x03,0xfc,0x07,0xfc,0x07,0xfc,0x07,0xfc,
  0x07,0xfc,0x07,0xf8,0x03,0xf0,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
static char radio_black_bits[] = {
  0x00,0x00,0xf0,0x01,0x0c,0x02,0x04,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,
  0x00,0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0d,0x08};
static char radio_dark_bits[] = {
  0xf0,0x00,0x0c,0x02,0x02,0x00,0x02,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
  0x00,0x00,0x00,0x02,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static char radio_light_bits[] = {
  0x00,0x00,0x00,0x04,0x00,0x08,0x00,0x08,0x00,0x10,0x00,0x10,0x00,0x10,0x00,
  0x10,0x00,0x10,0x00,0x08,0x00,0x08,0x08,0x06,0xf0,0x01,0x00,0x00};
static char radio_mid_bits[] = {
  0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x04,0x00,0x08,0x00,0x08,0x00,0x08,0x00,
  0x08,0x00,0x08,0x00,0x04,0x00,0x06,0xf0,0x01,0x00,0x00,0x00,0x00};
static char radio_text_bits[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x00,0xf0,0x01,0xf0,0x01,0xf0,
  0x01,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
 
static struct
{
  char *bits;
  GdkBitmap *bmap;
} parts[] =
{
  {
  radio_base_bits, NULL},
  {
  radio_black_bits, NULL},
  {
  radio_dark_bits, NULL},
  {
  radio_light_bits, NULL},
  {
  radio_mid_bits, NULL},
  {
  radio_text_bits, NULL}
};
 
 
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
        {
          if (as_list)
            result = (combo_box_is_using_list(widget));
          else
            result = (!combo_box_is_using_list(widget));
        }
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
  return ((is_combo (widget) || is_combo_box (widget, TRUE) || is_combo_box_entry (widget)));
}
 
gboolean
is_toolbar_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent)) {
    if ((IS_BONOBO_TOOLBAR (widget->parent))
	|| (IS_BONOBO_DOCK_ITEM (widget->parent))
	|| (IS_EGG_TOOLBAR (widget->parent))
	|| (IS_TOOLBAR (widget->parent))
	|| (IS_HANDLE_BOX (widget->parent)))
      result = TRUE;
    else
      result = is_toolbar_item (widget->parent);
  }
  return result;
}
 
gboolean
is_panel_widget_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (IS_PANEL_WIDGET (widget->parent))
	result = TRUE;
      else
	result = is_panel_widget_item (widget->parent);
    }
  return result;
}
 
gboolean 
is_bonobo_dock_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget))
    {
      if (IS_BONOBO_DOCK_ITEM(widget) || IS_BONOBO_DOCK_ITEM (widget->parent))
	result = TRUE;
      else if (IS_BOX(widget) || IS_BOX(widget->parent))
        {
          GtkContainer *box = IS_BOX(widget)?GTK_CONTAINER(widget):GTK_CONTAINER(widget->parent);
          GList *children = NULL, *child = NULL;
 
          children = gtk_container_get_children(box);
              
          for (child = g_list_first(children); child; child = g_list_next(child))
            {
	      if (IS_BONOBO_DOCK_ITEM_GRIP(child->data))
	        {
	          result = TRUE;
	          child = NULL;
	        }
            }	            
         
          if (children)   
  	    g_list_free(children);
	}
    }
  return result;
}
 
/***********************************************/
/* Widget Sizing                               */
/***********************************************/
 
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
  
/***********************************************
 * redmond_draw_part -
 *  
 *   draw a bits generated pixmap part with a
 *   specific gc.
 ***********************************************/
void
redmond_draw_part (GdkDrawable * drawable,
	   GdkGC * gc, 
           GdkRectangle * area, 
           gint x, 
           gint y, 
           Part part)
{
  if (area)
    gdk_gc_set_clip_rectangle (gc, area);
 
  if (!parts[part].bmap)
    parts[part].bmap = gdk_bitmap_create_from_data (drawable,
						    parts[part].bits,
						    PART_SIZE, PART_SIZE);
 
  gdk_gc_set_ts_origin (gc, x, y);
  gdk_gc_set_stipple (gc, parts[part].bmap);
  gdk_gc_set_fill (gc, GDK_STIPPLED);
 
  gdk_draw_rectangle (drawable, gc, TRUE, x, y, PART_SIZE, PART_SIZE);
 
  gdk_gc_set_fill (gc, GDK_SOLID);
 
  if (area)
    gdk_gc_set_clip_rectangle (gc, NULL);
}
 
/***********************************************
 * do_redmond_draw_fill -
 *  
 *   A simple routine to fill with the bg_gc
 ***********************************************/
void
do_redmond_draw_default_fill (GtkStyle *style, 
                              GdkWindow *window, 
                              GtkStateType state_type, 
                              GdkRectangle *area, 
                              gint x, 
                              gint y, 
                              gint width, 
                              gint height)
{
      if (area)
        gdk_gc_set_clip_rectangle(style->bg_gc[state_type], area);
        
      gdk_draw_rectangle (window, style->bg_gc[state_type], TRUE,
                          x, y,width, height);
 
      if (area)
        gdk_gc_set_clip_rectangle(style->bg_gc[state_type], NULL);
}
 
/***********************************************
 * do_redmond_draw_cross_hatch_fill -
 *  
 *   A simple routine to draw a thin cross-hatch 
 *   background fill. This is use for Scrollbar
 *   Trough's and greyed Checkbox's
 *    
 *   It originated in Smooth-Engine.
 ***********************************************/
void
do_redmond_draw_cross_hatch_fill (GtkStyle * style,
                                  GdkWindow * window,
                                  GtkStateType state,
                                  GdkRectangle * area,
                                  Part part,
                                  gint x,
                                  gint y, 
                                  gint width, 
                                  gint height)
{
  GdkGCValues gc_values;
  GdkGC *gc;
  GdkPixmap *pixmap;
 
  pixmap = gdk_pixmap_new (window, 2, 2, -1);
 
  gdk_draw_point (pixmap, style->bg_gc[state], 0, 0);
  gdk_draw_point (pixmap, style->bg_gc[state], 1, 1);
  gdk_draw_point (pixmap, style->light_gc[state], 1, 0);
  gdk_draw_point (pixmap, style->light_gc[state], 0, 1);
 
  gc_values.fill = GDK_TILED;
  gc_values.tile = pixmap;
  gc_values.ts_x_origin = x;
  gc_values.ts_y_origin = y;
  gc = gdk_gc_new_with_values (window, &gc_values,
                               GDK_GC_TS_X_ORIGIN | GDK_GC_TS_Y_ORIGIN |
                               GDK_GC_FILL | GDK_GC_TILE);
 
  if (part != RADIO_NONE)
  {
    if (!parts[part].bmap)
      parts[part].bmap = gdk_bitmap_create_from_data (window,
  						      parts[part].bits,
 						      PART_SIZE, PART_SIZE);
    gdk_gc_set_clip_origin (gc, x, y);	
    gdk_gc_set_clip_mask(gc, parts[part].bmap);
  }
  else if (area)
    gdk_gc_set_clip_rectangle (gc, area);
 
  gdk_draw_rectangle (window, gc, TRUE, x, y, width, height);
 
  g_object_unref (gc);
  g_object_unref (pixmap);
}
  
/***********************************************
 * do_redmond_draw_check -
 *  
 *   A simple routine to draw a redmond style
 *   check mark using the passed GC.
 *  
 *   It originated in Smooth-Engine.
 ***********************************************/
void
do_redmond_draw_check (cairo_t *cr,
                       CairoColor * color,
                       gint x, 
                       gint y, 
                       gint width, 
                       gint height)
{ 
  double left, top;
  int scale;

  scale = MIN(width, height);

  if (!(scale % 2))
    scale -= 1;

  if (scale <= 11)
    scale = 9;

  left = x + floor((width - scale) / 2) + 0.5;
  top = y + floor((height - scale) / 2) + 0.5;

  cairo_save(cr);

  ge_cairo_set_color(cr, color);	
  cairo_set_line_width(cr, 0.5);
/*
    0   1   2   3   4   5   6   7   8
  +---+---+---+---+---+---+---+---+---+
0 |   |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+---+
1 |   |   |   |   |   |   |   | x |   |
  +---+---+---+---+---+---+---+---+---+
2 |   |   |   |   |   |   | x | x |   |
  +---+---+---+---+---+---+---+---+---+
3 |   | x |   |   |   | x | x | x |   |
  +---+---+---+---+---+---+---+---+---+
4 |   | x | x |   | x | x | x |   |   |
  +---+---+---+---+---+---+---+---+---+
5 |   | x | x | x | x | x |   |   |   |
  +---+---+---+---+---+---+---+---+---+
6 |   |   | x | x | x |   |   |   |   |
  +---+---+---+---+---+---+---+---+---+
7 |   |   |   | x |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+---+
8 |   |   |   |   |   |   |   |   |   |
  +---+---+---+---+---+---+---+---+---+

*/

  cairo_move_to(cr, left + floor((1*scale)/9), top + floor((3*scale)/9)); //(1,3)
  cairo_line_to(cr, left + floor((1*scale)/9), top + floor((5*scale)/9)); //(1,5)
  cairo_line_to(cr, left + floor((3*scale)/9), top + floor((7*scale)/9)); //(3,7)
  cairo_line_to(cr, left + floor((7*scale)/9), top + floor((3*scale)/9)); //(7,3)
  cairo_line_to(cr, left + floor((7*scale)/9), top + floor((1*scale)/9)); //(7,1)
  cairo_line_to(cr, left + floor((3*scale)/9), top + floor((5*scale)/9)); //(3,5)
  cairo_line_to(cr, left + floor((1*scale)/9), top + floor((3*scale)/9)); //(1,3)

  cairo_fill(cr);

  cairo_move_to(cr, left + floor((1*scale)/9), top + floor((3*scale)/9)); //(1,3)
  cairo_line_to(cr, left + floor((1*scale)/9), top + floor((5*scale)/9)); //(1,5)
  cairo_line_to(cr, left + floor((3*scale)/9), top + floor((7*scale)/9)); //(3,7)
  cairo_line_to(cr, left + floor((7*scale)/9), top + floor((3*scale)/9)); //(7,3)
  cairo_line_to(cr, left + floor((7*scale)/9), top + floor((1*scale)/9)); //(7,1)
  cairo_line_to(cr, left + floor((3*scale)/9), top + floor((5*scale)/9)); //(3,5)
  cairo_line_to(cr, left + floor((1*scale)/9), top + floor((3*scale)/9)); //(1,3)
  cairo_stroke(cr);

  cairo_restore(cr);

#if 0
  x += 1;
  y += 1;
  width -= 2;
  height -= 2;

  /* short diagonal */
  gdk_draw_line (window, gc, x + 0, y + height - 5, x + 0, y + height - 3);	/* Left Line */
  gdk_draw_line (window, gc, x + 1, y + height - 4, x + 1, y + height - 2);	/* Right Line */
 
  /* Long Diagonal */
  gdk_draw_line (window, pxp?gc, x + 2, y + height - 3, x + width - 1, y + 0);	/* Top Line */
  gdk_draw_line (window, gc, x + 2, y + height - 2, x + width - 1, y + 1);	/* Center Line */
  gdk_draw_line (window, gc, x + 2, y + height - 1, x + width - 1, y + 2);	/* Bottom Line */

#endif
}
 
/***********************************************
 * do_redmond_draw_arrow -
 *  
 *   A simple routine to draw a redmond style
 *   arrow using the passed GC.
 *  
 *   Taken in part from smooth, it was based on 
 *   XFCE's & CleanIce draw arrow routines, 
 *   both which  were based on ThinIce's.
 ***********************************************/
void
do_redmond_draw_arrow (cairo_t *cr,
               CairoColor * color,
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
 
/***********************************************/
/* MenuShell/MenuBar Item Prelight Workaround  */
/***********************************************/
 
/***********************************************
 * do_redmond_draw_line -
 *  
 *   A simple routine to draw a redmond style
 *   spacer line.
 ***********************************************/
void
do_redmond_draw_line(cairo_t *cr,
             CairoColor * dark,
             CairoColor * light,
             GdkRectangle * area,
             gint start,
             gint end,
             gint base,
             gboolean horizontal)
{  
  cairo_set_line_width (cr, 1);

  if (horizontal) 
    {
      ge_cairo_set_color(cr, dark);	
      cairo_move_to(cr, start + 1.5, base + 0.5);
      cairo_line_to(cr, end - 1.5, base + 0.5);
      cairo_stroke(cr);

      ge_cairo_set_color(cr, light);	
      cairo_move_to(cr, start + 1.5, base + 1.5);
      cairo_line_to(cr, end - 1.5, base + 1.5);
      cairo_stroke(cr);
    } 
  else 
    {
      ge_cairo_set_color(cr, dark);	
      cairo_move_to(cr, base + 0.5, start + 1.5);
      cairo_line_to(cr, base + 0.5, end - 1.5);
      cairo_stroke(cr);

      ge_cairo_set_color(cr, light);	
      cairo_move_to(cr, base + 1.5, start + 1.5);
      cairo_line_to(cr, base + 1.5, end - 1.5);
      cairo_stroke(cr);
    }
}
 
/***********************************************
 * gtk_menu_shell_style_set -
 *  
 *   Style set signal to ensure menushell signals
 *   get cleaned up if the theme changes
 ***********************************************/
static gboolean 
gtk_menu_shell_style_set(GtkWidget *widget,
                         GtkStyle *previous_style,
                         gpointer user_data)
{
  gtk_menu_shell_cleanup_signals(widget);
  
  return FALSE;
}
 
/***********************************************
 * gtk_menu_shell_destroy -
 *  
 *   Destroy signal to ensure menushell signals
 *   get cleaned if it is destroyed
 ***********************************************/
static gboolean    
gtk_menu_shell_destroy(GtkWidget *widget,
                       GdkEvent *event,
                       gpointer user_data)
{
  gtk_menu_shell_cleanup_signals(widget);
  
  return FALSE;
}
 
/***********************************************
 * gtk_menu_shell_motion -
 *  
 *   Motion signal to ensure menushell items
 *   prelight state changes on mouse move.
 ***********************************************/
static gboolean 
gtk_menu_shell_motion(GtkWidget *widget, 
                      GdkEventMotion *event, 
                      gpointer user_data)
{
  if (IS_MENU_SHELL(widget))
    {
      gint pointer_x, pointer_y;
      GdkModifierType pointer_mask;
      GList *children = NULL, *child = NULL;
     
      gdk_window_get_pointer(widget->window, &pointer_x, &pointer_y, &pointer_mask);
	    
      if (IS_CONTAINER(widget))
        {
          children = gtk_container_get_children(GTK_CONTAINER(widget));
              
          for (child = g_list_first(children); child; child = g_list_next(child))
            {
	      if ((child->data) && IS_WIDGET(child->data) && 
                  (GTK_WIDGET_STATE(GTK_WIDGET(child->data)) != GTK_STATE_INSENSITIVE))
	        {
	          if ((pointer_x >= GTK_WIDGET(child->data)->allocation.x) && 
	              (pointer_y >= GTK_WIDGET(child->data)->allocation.y) &&
	              (pointer_x < (GTK_WIDGET(child->data)->allocation.x + 
	                              GTK_WIDGET(child->data)->allocation.width)) && 
	              (pointer_y < (GTK_WIDGET(child->data)->allocation.y +
	                              GTK_WIDGET(child->data)->allocation.height)))
	            {
                      gtk_widget_set_state (GTK_WIDGET(child->data), GTK_STATE_PRELIGHT);
	            }
	          else
                    {
                      gtk_widget_set_state (GTK_WIDGET(child->data), GTK_STATE_NORMAL);
                    }
                 }
             }	            
         
           if (children)   
             g_list_free(children);
        }
    }
 
  return FALSE;
}
 
/***********************************************
 * gtk_menu_shell_leave -
 *  
 *   Leave signal to ensure menushell items
 *   normal state on mouse leave.
 ***********************************************/
static gboolean 
gtk_menu_shell_leave(GtkWidget *widget, 
                      GdkEventCrossing *event,
                      gpointer user_data)
{
  if (IS_MENU_SHELL(widget))
    {
      GList *children = NULL, *child = NULL;
 
      if (IS_CONTAINER(widget))
        {
          children = gtk_container_get_children(GTK_CONTAINER(widget));
              
          for (child = g_list_first(children); child; child = g_list_next(child))
            {
	      if ((child->data) && IS_MENU_ITEM(child->data) && 
                  (GTK_WIDGET_STATE(GTK_WIDGET(child->data)) != GTK_STATE_INSENSITIVE))
	        {
                  if ((!GTK_IS_MENU(GTK_MENU_ITEM(child->data)->submenu)) || 
                      (!(GTK_WIDGET_REALIZED(GTK_MENU_ITEM(child->data)->submenu) && 
                         GTK_WIDGET_VISIBLE(GTK_MENU_ITEM(child->data)->submenu) &&
                         GTK_WIDGET_REALIZED(GTK_MENU(GTK_MENU_ITEM(child->data)->submenu)->toplevel) &&
                         GTK_WIDGET_VISIBLE(GTK_MENU(GTK_MENU_ITEM(child->data)->submenu)->toplevel))))
	          {
                    gtk_widget_set_state (GTK_WIDGET(child->data), GTK_STATE_NORMAL);
                  }
                }	            
            }         
            
          if (children)   
  	    g_list_free(children);
        }
    }
 
  return FALSE;
}
 
/***********************************************
 * gtk_menu_shell_setup_signals -
 *  
 *   Setup Menu Shell with signals to ensure
 *   prelight works on items
 ***********************************************/
void
gtk_menu_shell_setup_signals(GtkWidget *widget)
{
  if (IS_MENU_BAR(widget))
    {
      gint id = 0;
 
      if (!g_object_get_data(G_OBJECT(widget), "REDMOND_MENU_SHELL_HACK_SET"))
      {
        id = g_signal_connect(G_OBJECT(widget), "motion-notify-event",
                                             (GtkSignalFunc)gtk_menu_shell_motion,
                                             NULL);
                                  
        g_object_set_data(G_OBJECT(widget), "REDMOND_MENU_SHELL_MOTION_ID", (gpointer)id);
        
        id = g_signal_connect(G_OBJECT(widget), "leave-notify-event",
                                             (GtkSignalFunc)gtk_menu_shell_leave,
                                             NULL);
        g_object_set_data(G_OBJECT(widget), "REDMOND_MENU_SHELL_LEAVE_ID", (gpointer)id);
                                             
        id = g_signal_connect(G_OBJECT(widget), "destroy-event",
                                             (GtkSignalFunc)gtk_menu_shell_destroy,
                                             NULL);
        g_object_set_data(G_OBJECT(widget), "REDMOND_MENU_SHELL_DESTROY_ID", (gpointer)id);
 
        g_object_set_data(G_OBJECT(widget), "REDMOND_MENU_SHELL_HACK_SET", (gpointer)1);
        
        id = g_signal_connect(G_OBJECT(widget), "style-set",
                                             (GtkSignalFunc)gtk_menu_shell_style_set,
                                             NULL);
        g_object_set_data(G_OBJECT(widget), "REDMOND_MENU_SHELL_STYLE_SET_ID", (gpointer)id);
      }
    }  
}
 
/***********************************************
 * gtk_menu_shell_cleanuo_signals -
 *  
 *   Cleanup/remove Menu Shell signals
 ***********************************************/
void
gtk_menu_shell_cleanup_signals(GtkWidget *widget)
{
  if (IS_MENU_BAR(widget))
    {
      gint id = 0;
 
      id = (gint)g_object_steal_data (G_OBJECT(widget), "REDMOND_MENU_SHELL_MOTION_ID");
      g_signal_handler_disconnect(G_OBJECT(widget), id);
                                             
      id = (gint)g_object_steal_data (G_OBJECT(widget), "REDMOND_MENU_SHELL_LEAVE_ID");
      g_signal_handler_disconnect(G_OBJECT(widget), id);
     
      id = (gint)g_object_steal_data (G_OBJECT(widget), "REDMOND_MENU_SHELL_DESTROY_ID");
      g_signal_handler_disconnect(G_OBJECT(widget), id);
       
      id = (gint)g_object_steal_data (G_OBJECT(widget), "REDMOND_MENU_SHELL_STYLE_SET_ID");
      g_signal_handler_disconnect(G_OBJECT(widget), id);
       
      g_object_steal_data (G_OBJECT(widget), "REDMOND_MENU_SHELL_HACK_SET");      
    }
}
