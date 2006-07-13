/* Glide theme engine
 * Copyright (C) 2006 Andrew Johnson
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
 * Written by Andrew Johnson <acjgenius@earthlink.net>
 */ 

#include "glide_gtk2_engine.h"
#include "glide_gtk2_support.h"
#include "glide_gtk2_drawing.h"
  
/***********************************************
 * glide_draw_vline -
 *  
 *   the Function used to draw horizontal spacers.
 ***********************************************/
void
glide_draw_hline (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint x1,
            gint x2, 
            gint y)
{
  GlideStyle *glide_style = GLIDE_STYLE (style);
  CHECK_ARGS
 
  cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

  do_glide_draw_line(canvas, 
                       &glide_style->color_cube.dark[state_type], 
                       &glide_style->color_cube.light[state_type], 
                       area, 
                       x1, x2, y, 
                       TRUE);

  cairo_destroy(canvas);
}
 
/***********************************************
 * glide_draw_vline -
 *  
 *   the Function used to draw vertical spacers.
 ***********************************************/
void
glide_draw_vline (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail, 
            gint y1, 
            gint y2, 
            gint x)
{
  GlideStyle *glide_style = GLIDE_STYLE (style);
  if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
    return;
 
  CHECK_ARGS
 
  cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

  do_glide_draw_line(canvas, 
                       &glide_style->color_cube.dark[state_type], 
                       &glide_style->color_cube.light[state_type], 
                       area, 
                       y1, y2, x, 
                       FALSE);

  cairo_destroy(canvas);
}
  
/***********************************************
 * glide_draw_check -
 *  
 *   the Function used to draw all check boxes.
 ***********************************************/
void
glide_draw_check (GtkStyle * style,
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
  GlideStyle *glide_style = GLIDE_STYLE (style);
  CHECK_ARGS
  SANITIZE_SIZE
 
  cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

  if (GTK_IS_CHECK_MENU_ITEM(widget)) 
  {
    x -= 1;
    y -= 1;
    width += 2;
    height += 2;
  } else if (GTK_IS_CELL_RENDERER_TOGGLE(widget) || CHECK_DETAIL("cellcheck",detail))
  {
    x -= 1;
    y -= 1;
    width += 2;
    height += 2;
  }

#if 0
  if (CHECK_DETAIL (detail, "check"))	/* Menu item */
    {
      /* check boxes in menu don't actually draw box, only the check mark,
       * which use the text color 
       */
      if (shadow == GTK_SHADOW_IN)
	{
	  do_glide_draw_check (canvas, &glide_style->color_cube.text[state], x + 2, y + 2,
			 width - 4, height - 4);
	}
    }
  else
#endif
    {
      ge_cairo_set_color(canvas, &glide_style->color_cube.base[state]);

      cairo_rectangle(canvas, x, y, width - 1, height - 1);

      cairo_fill(canvas);

      gboolean inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));

      inconsistent |= (GTK_IS_CELL_RENDERER_TOGGLE(widget) && gtk_cell_renderer_toggle_get_inconsistent (widget));
      inconsistent |= (CHECK_DETAIL("cellcheck",detail) && (shadow == GTK_SHADOW_ETCHED_IN));

      if ((shadow == GTK_SHADOW_IN) || (inconsistent))
	{
		if (inconsistent)
		{
			int line_width = ceil(width*0.33);

			/* Force Thickness Even */
			line_width -= (line_width % 2);

			cairo_set_line_cap(canvas, CAIRO_LINE_CAP_SQUARE);
			cairo_set_line_width (canvas, line_width);

			cairo_move_to(canvas, x + line_width, y + floor(height/2));
			cairo_line_to(canvas, x + width - line_width, y + floor(height/2));

			ge_cairo_set_color(canvas, &glide_style->color_cube.text[state]);	
			cairo_stroke (canvas);

		}
		else
		{
	      do_glide_draw_check (canvas, &glide_style->color_cube.text[state],
			             x + 2, y + 2, width - 4, height - 4);
		}
	}
 
      glide_draw_shadow (style, window, state, GTK_SHADOW_IN,
			area, widget, detail, x, y, width, height);
    }
    
    cairo_destroy(canvas);
}
 
/***********************************************
 * glide_draw_option -
 *  
 *   the Function used to draw all Option boxes
 *  
 *   Glide Option box has essentially 3 looks -
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
glide_draw_option (GtkStyle * style,
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
  GlideStyle *glide_style = GLIDE_STYLE (style);
 
  CHECK_ARGS
  SANITIZE_SIZE 

	if (GTK_IS_CELL_RENDERER_TOGGLE(widget) || CHECK_DETAIL("cellradio",detail))
        {
		x -= 1;
		y -= 1;
		width += 2;
		height += 2;
	}

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

      cairo_set_line_width (cr, 1);

  gint centerX = x + floor(width/2);
  gint centerY = y + floor(height/2);
  gint radius = floor(MIN(width,height)/2) + 1.5;

  do_glide_draw_simple_circle (cr, &glide_style->color_cube.mid[state],
				&glide_style->color_cube.light[state],
					centerX, 
					centerY, 
					radius);

  do_glide_draw_simple_circle (cr, &glide_style->color_cube.dark[state],
				&glide_style->color_cube.mid[state],
					centerX, 
					centerY, 
					radius - 1);

      ge_cairo_set_color(cr, &glide_style->color_cube.base[state]);
      cairo_arc(cr, centerX, centerY, radius - 2, 0, 2 * M_PI);
      cairo_fill(cr);

      gboolean inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));
      inconsistent |= (GTK_IS_CELL_RENDERER_TOGGLE(widget) && gtk_cell_renderer_toggle_get_inconsistent (widget));
      inconsistent |= (CHECK_DETAIL("cellradio",detail) && (shadow == GTK_SHADOW_ETCHED_IN));

      if ((shadow == GTK_SHADOW_IN) || inconsistent)
	{
		if (inconsistent)
		{
			int line_width = ceil(radius*0.66);

			/* Force Thickness Even */
			line_width -= (line_width % 2);

			cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
			cairo_set_line_width (cr, line_width + ((height % 2)?1:0));

			cairo_move_to(cr, x + line_width - ((height % 2)?0.5:0), y + floor(height/2));
			cairo_line_to(cr, x + width - line_width + ((height % 2)?0.5:0), y + floor(height/2));

			ge_cairo_set_color(cr, &glide_style->color_cube.text[state]);	
			cairo_stroke (cr);

		}
		else
		{
			ge_cairo_set_color(cr, &glide_style->color_cube.text[state]);
			cairo_arc(cr, centerX, centerY, radius*0.38, 0, 2 * M_PI);
			cairo_fill(cr);
		}
	}
      cairo_destroy(cr);
}
 
/***********************************************
 * glide_draw_arrow -
 *  
 *   the Function used to draw all arrows
 *  
 *   Glide Arrows have 2 looks, Normal 
 *   & Insensitive 
 ***********************************************/
void
glide_draw_arrow (GtkStyle * style,
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
  GlideStyle *glide_style = GLIDE_STYLE (style);

  gboolean button_in = (shadow == GTK_SHADOW_IN);
  gint child_offset_x = 1, child_offset_y = 1;
 
  if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
    return;
 
  CHECK_ARGS
  SANITIZE_SIZE

  if ((CHECK_DETAIL (detail, "spinbutton"))
      || (CHECK_DETAIL (detail, "vscrollbar"))
      || (CHECK_DETAIL (detail, "hscrollbar"))
      || (CHECK_DETAIL (detail, "optionmenu"))
      || IS_SCROLLBAR(widget)
      || (ge_is_in_combo_box (widget)))
    {
      if ((CHECK_DETAIL (detail, "vscrollbar"))
          || (CHECK_DETAIL (detail, "hscrollbar"))
          || IS_SCROLLBAR(widget))
	{
		if (arrow_type == GTK_ARROW_LEFT)
		{
			x -= 1;
		}

		if (arrow_type == GTK_ARROW_UP)
		{
			y -= 1;
		}

		width += 1;
		height += 1;
 	}

      if (CHECK_DETAIL (detail, "spinbutton") || CHECK_DETAIL (detail, "optionmenu"))
	{
	  if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
	    x -= 1;
	}
      else if (ge_is_in_combo_box (widget) && ((gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)))
	{
	    x += 2;
	}

      if (ge_is_combo_box_entry(widget))
        {
		x += 1;
        }
 
      /* Glide prefers to have smaller arrows relative too size
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
  else if (CHECK_DETAIL(detail, "menuitem"))
      x -= 1;
  else if (CHECK_DETAIL (detail, "arrow"))
  {
      x += 2;
      y += 2;
      width -= 2;
      height -= 2;
  }

  cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

  if (state == GTK_STATE_INSENSITIVE)
    {
      do_glide_draw_arrow (canvas, &glide_style->color_cube.light[state], arrow_type, x+1, y+1, width, height);
      do_glide_draw_arrow (canvas, &glide_style->color_cube.dark[state], arrow_type, x, y, width, height);
    }
  else
    do_glide_draw_arrow (canvas, &glide_style->color_cube.fg[state], arrow_type, x, y, width, height);

  cairo_destroy(canvas);
}
 
/***********************************************
 * glide_draw_shadow -
 *  
 *   Function used to draw the Shadow/Border
 *   for most widgets/widget parts.
 *  
 *   The Glide Shadow is primarily constructed
 *   with an outside border of light/extra_dark
 *   and an internal border of bg/dark.
 ***********************************************/
void
glide_draw_shadow (GtkStyle * style,
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
  GlideStyle *glide_style = GLIDE_STYLE (style);
 
  CHECK_ARGS
  SANITIZE_SIZE
 
	if ((CHECK_DETAIL (detail, "entry")) && (widget) && (ge_is_in_combo_box (widget)))
	{
		GtkWidget *button = g_object_get_data(G_OBJECT(ge_find_combo_box_widget_parent (widget)), "button");

		if (GTK_IS_WIDGET(button))
		{
			gtk_widget_queue_draw_area(button, button->allocation.x, button->allocation.y, 
							button->allocation.width, button->allocation.height);
		}
	}

  if (shadow_type == GTK_SHADOW_NONE)
    return;

  cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      break;
    case GTK_SHADOW_ETCHED_IN:
	do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
                              GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_ETCHED,
                              x, y, width, height);
      break;
    case GTK_SHADOW_ETCHED_OUT:
	do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
                              GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_ENGRAVED,
                              x, y, width, height);
      break;
    case GTK_SHADOW_IN:
      if (CHECK_DETAIL (detail, "buttondefault"))
	{
	}
      else if (((CHECK_DETAIL (detail, "entry"))
		|| (CHECK_DETAIL (detail, "frame")))
	       && widget && ((IS_SPIN_BUTTON (widget))
			     || (ge_is_in_combo_box (widget))))
	{
	  /* The Combo/ComboBoxEntry button and the SpinButton Steppers should apear
	   * to be inset into the entry, as opposed to next to it, so we fake it
	   * by drawing an extra 2 pixels here to ignore the right(or left)
	   * edge, which will be drawn by the buttons.
	   */
 
	  if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
	    {
		do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_IN,
					x, y, width+4, height);
	    }
	  else
	    {
		do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_IN,
					x - 4, y, width + 4, height);
	    }
	}
      else if ((CHECK_DETAIL (detail, "trough"))
	       || ((IS_STATUS_BAR (widget)))
	       || ((CHECK_DETAIL (detail, "frame")))
	       || ((CHECK_DETAIL (detail, "button"))
		   && (ge_is_toolbar_item (widget))))
	{
		do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_IN,
					x, y, width+1, height);
	}
      else
	{
	  /* Normal IN Border */
		do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_IN,
					x, y, width, height);
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
		do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_OUT,
					x, y, width, height);
	}
      else if ((CHECK_DETAIL (detail, "frame"))
	       || ((CHECK_DETAIL (detail, "button"))
		   && (ge_is_toolbar_item (widget))))
	{
		do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_OUT,
					x, y, width, height);
	}
      else
	{
	  /* Normal OUT Border */
		do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_OUT,
					x, y, width, height);
	}
      break;
    }

    cairo_destroy(canvas);
}
 
/***********************************************
 * glide_draw_combobox_button-
 *   
 *   Function used to draw the ComboBox button
 *   "inset" into "entry. 
 *
 *   Slightly Evil Hack #1
 ***********************************************/
static void
glide_draw_combobox_button (GtkStyle * style,
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
  GtkStyle * parent_style = style;
  GtkStateType parent_state = state_type;

  gboolean entry_focused = FALSE;
  gint focus_line_width = 0, focus_padding = 0, focus = 0, thick = 2;
  gboolean interior_focus = TRUE;

  g_object_set_data(G_OBJECT(ge_find_combo_box_widget_parent(widget)), "button", widget);

  if (GTK_WIDGET_HAS_FOCUS(widget))
  {
	gtk_widget_style_get (widget, "focus_line_width", &focus_line_width, NULL);
	gtk_widget_style_get (widget, "focus_padding", &focus_padding, NULL);
	gtk_widget_style_get (widget, "interior_focus", &interior_focus, NULL);

	if (!interior_focus)
	{
		focus = -(focus_line_width);
		if (focus == 0)
		{
			focus = -1;
		}
		entry_focused = TRUE;
	}
  }
  else
  {
		GList *child=NULL,*children = gtk_container_get_children (GTK_CONTAINER(widget->parent));

		for (child = children; child; child = child->next)
		{
			if (GTK_IS_ENTRY(child->data) && GTK_WIDGET_HAS_FOCUS(child->data))
			{
				gtk_widget_style_get (widget, "focus_line_width", &focus_line_width, NULL);
				gtk_widget_style_get (widget, "focus_padding", &focus_padding, NULL);
				gtk_widget_style_get (widget, "interior_focus", &interior_focus, NULL);

				if (!interior_focus)
				{
					focus = focus_line_width;
					if (focus == 0)
					{
						focus = 1;
					}
					entry_focused = TRUE;
				}
				focus_padding = 0;
			}
		}

		g_list_free(children);
	}

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
  		            area, widget, "combo_box_entry", x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 + focus_padding*2, height - focus*2 + focus_padding*2);
        }
      else if (ge_is_combo(widget))
        {
          GtkWidget *entry = widget;

	  if (GTK_IS_WIDGET(widget) && GTK_IS_WIDGET(widget->parent) && GTK_IS_ENTRY(GTK_COMBO (widget->parent)->entry))
            {
               entry = GTK_COMBO (widget->parent)->entry;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
             }
          else if (GTK_IS_WIDGET(widget->parent))
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
	 	              &clip, entry, "combo_box_entry", x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 + focus_padding*2, height - focus*2 + focus_padding*2);
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
		   	    area, parent, "combo_box_entry", x - thick*2 - focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 + focus_padding*2, height - focus*2 + focus_padding*2);
        }
 
      do_glide_draw_default_fill (style, window, widget,
	  			          state_type, area, x + focus, y + thick + focus,
                             width - thick - focus*2, height - thick*2 - focus*2, FALSE, FALSE);

        gtk_paint_shadow (style, window, state_type, shadow_type, area,
		          widget, detail, x + focus, y + thick + focus, width - thick - focus*2, height - 2*thick - focus*2);

      if (entry_focused)
      {
             glide_draw_focus(style, window, state_type, area, widget, "combo_entry_button", x - 2, y, width + 2, height);
      }
    } 
  else
    {
      focus_padding = -focus_padding;

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

	      gdk_draw_rectangle (window, widget->parent->style->base_gc[parent_state],
			          TRUE, x + focus - focus_padding, y + focus - focus_padding, 
                                        width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
            }
          else
            {
              if (parent_state != GTK_STATE_INSENSITIVE)
                parent_state = GTK_STATE_NORMAL;

            gtk_paint_flat_box (style, window, state_type,
	  	                GTK_SHADOW_NONE, area, widget, "entry_bg", 
				x + focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
            }
 
          gtk_paint_shadow (style, window, state_type, GTK_SHADOW_IN,
	  		    area, widget, "combo_box_entry",
                                x + focus - focus_padding, y + focus - focus_padding, 
                                width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
        }
      else if (ge_is_combo(widget))
        {
          GtkWidget *entry = widget;

	  if (GTK_IS_WIDGET(widget) && GTK_IS_WIDGET(widget->parent) && GTK_IS_ENTRY(GTK_COMBO (widget->parent)->entry))
            {
               entry = GTK_COMBO (widget->parent)->entry;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
             }
          else if (GTK_IS_WIDGET(widget->parent))
            {
               entry = widget->parent;
               gtk_widget_ensure_style(entry);

               parent_style = entry->style;
               parent_state = entry->state;
            }
  
           if (parent_state != GTK_STATE_INSENSITIVE)
             parent_state = GTK_STATE_NORMAL;

           gtk_paint_flat_box (parent_style, window, parent_state,
			       GTK_SHADOW_NONE, area, entry, "entry_bg", 
				x + focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
           gtk_paint_shadow (parent_style, window, parent_state, GTK_SHADOW_IN,
			     area, entry, "combo_box_entry", 
                                x + focus - focus_padding, y + focus - focus_padding, 
                                width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
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
			      GTK_SHADOW_NONE, area, parent, "entry_bg", 
				x + focus - focus_padding, y + focus - focus_padding, 
				width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
          gtk_paint_shadow (parent_style, window, parent_state, GTK_SHADOW_IN,
		   	    area, parent, "combo_box_entry", 
                                x + focus - focus_padding, y + focus - focus_padding, 
                                width + thick*2 - focus + focus_padding*2, height - focus*2 + focus_padding*2);
        }
 
      do_glide_draw_default_fill (style, window, widget,
				          state_type, area, x + thick + focus, y + thick + focus,
                             width - thick - focus*2, height - 2*thick - focus*2, FALSE, FALSE);
 
        gtk_paint_shadow (style, window, state_type, shadow_type, area,
		          widget, detail, x + thick + focus, y + thick + focus, width - thick - focus*2, height - 2*thick - focus*2);

      if (entry_focused)
      {
             glide_draw_focus(style, window, state_type, area, widget, "combo_entry_button", x, y, width, height);
      }
    }
}
 
/***********************************************
 * glide_draw_spinbutton_stepper-
 *  
 *   Function used to draw the spinbutton 
 *   steppers "inset" into entry.
 *  
 *   Slightly Evil Hack #2
 ***********************************************/
static void
glide_draw_spinbutton_stepper (GtkStyle * style,
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
 
  GtkStateType parent_state = GTK_STATE_NORMAL;
  gboolean entry_focused = FALSE;
  gint focus_line_width = 0, focus_padding = 0, focus = 0;
  gboolean interior_focus = TRUE;

  if (GTK_WIDGET_HAS_FOCUS(widget))
  {
	gtk_widget_style_get (widget, "focus_line_width", &focus_line_width, NULL);
	gtk_widget_style_get (widget, "focus_padding", &focus_padding, NULL);
	gtk_widget_style_get (widget, "interior_focus", &interior_focus, NULL);

	if (!interior_focus)
	{
		focus = focus_line_width;
		if (focus == 0)
		{
			focus = 1;
		}
		entry_focused = TRUE;
	}
  }
 
  if (GTK_WIDGET_STATE(widget) == GTK_STATE_INSENSITIVE)
  {
	parent_state = GTK_STATE_INSENSITIVE;
  }

  gint thick = 2;

  if ((!(widget)) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
   {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
	  gtk_paint_flat_box (style, window, parent_state,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg", 
				x - thick, y + focus, width + thick - focus, height - focus*2);

	  gtk_paint_shadow (style, window, parent_state, GTK_SHADOW_IN, &spin_area, widget, detail, 
                                x - thick, y + focus, width + thick - focus, height + thick - focus);

	  do_glide_draw_default_fill (style, window, widget, state_type, area, 
                                 	x, y + thick + focus, width - thick - focus, height - thick - focus, FALSE, FALSE);
	  gtk_paint_shadow (style, window, state_type, shadow_type, area, widget, detail,
                                x, y + thick + focus, width - thick - focus, height - thick - focus);

          if (entry_focused)
            {
              glide_draw_focus(style, window, parent_state, area, widget, detail, x - thick, y, width + thick, height + thick);
            }
	}
      else
	{
	  gtk_paint_flat_box (style, window, parent_state,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg", x - thick, y, width + thick - focus, height - focus);

	  gtk_paint_shadow (style, window, parent_state, GTK_SHADOW_IN, &spin_area, widget, detail,  
			    x - thick, y - thick, width + thick - focus, height + thick - focus);

	  do_glide_draw_default_fill (style, window, widget, state_type, area, 
			    x, y + thick, width - thick - focus, height - thick*2 - focus, FALSE, FALSE);

	  gtk_paint_shadow (style, window, state_type, shadow_type, area, widget, detail,
			    x, y , width - thick - focus, height - thick - focus);

          if (entry_focused)
            {
              glide_draw_focus(style, window, parent_state, area, widget, detail, x - thick, y, width + thick, height);
            }
	}
    }
  else
    {
      if (CHECK_DETAIL (detail, "spinbutton_up"))
	{
	  gtk_paint_flat_box (style, window, parent_state,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg", x + focus, y + focus, width + thick*2 - focus, height - focus);
	  gtk_paint_shadow (style, window, parent_state, GTK_SHADOW_IN, &spin_area, widget, detail, 
				x + focus, y + focus, width + thick*2 - focus, height + thick - focus);
	  do_glide_draw_default_fill (style, window, widget, state_type, area, 
					x + thick + focus, y + thick + focus, width - thick - focus, height - thick - focus, FALSE, FALSE);
	  gtk_paint_shadow (style, window, state_type, shadow_type, area, widget, detail, 
				x + thick + focus, y + thick + focus , width - thick - focus, height - thick - focus);

          if (entry_focused)
            {
              glide_draw_focus(style, window, parent_state, area, widget, detail, x, y, width + thick*2, height + thick);
            }
	}
      else
	{
	  gtk_paint_flat_box (style, window, parent_state,
			      GTK_SHADOW_NONE, &spin_area, widget,
			      "entry_bg",x + focus, y, width + thick*2 - focus, height - focus);
	  gtk_paint_shadow (style, window, parent_state, GTK_SHADOW_IN, &spin_area, widget, detail, 
				x + focus , y - thick, width + thick*2 - focus, height + thick - focus);
	  do_glide_draw_default_fill (style, window, widget, state_type, area, 
				x + thick + focus, y, width - thick - focus, height - thick - focus, FALSE, FALSE);
	  gtk_paint_shadow (style, window, state_type, shadow_type, area, widget, detail, 
                                x + thick + focus, y, width - thick - focus, height - thick - focus);

          if (entry_focused)
            {
              glide_draw_focus(style, window, parent_state, area, widget, detail, x, y - thick, width + thick*2, height + thick);
            }
	}
    }
}
 
/***********************************************
 * glide_draw_box -
 *  
 *   Function used to draw the box portion of 
 *   general widgets (buttons, entries).
 *
 *  In general this is the same as calling
 *  apply background and then draw shadow,
 *  so the bulk of this routine is actually
 *  to handle drawing the peculiarities of
 *  specific widgets which don't follow this 
 *  in glide.
 ***********************************************/
void
glide_draw_box (GtkStyle * style,
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
  GlideStyle *glide_style = GLIDE_STYLE (style);
 
  CHECK_ARGS
  SANITIZE_SIZE

  if (IS_MENU_SHELL(widget))
    {
      gtk_menu_shell_setup_signals(widget);
    }  
 
  if (CHECK_DETAIL (detail, "menuitem"))
    shadow_type = GTK_SHADOW_OUT;
 
  if ((CHECK_DETAIL (detail, "vscrollbar"))  || (CHECK_DETAIL (detail, "hscrollbar")))
    {
      do_glide_draw_default_fill (style, window, widget, state_type, 
                                        area, x, y, width, height, CHECK_DETAIL (detail, "vscrollbar"), FALSE);
 
      glide_draw_shadow (style, window, state_type, shadow_type, area,
			widget, detail, x, y, width, height);
    }
  else if ((CHECK_DETAIL (detail, "trough")))
    {
      do_glide_draw_default_fill (style, window, widget, state_type, 
                                        area, x, y, width, height, IS_VSCROLLBAR(widget) || IS_VSCALE(widget), TRUE);
 
      glide_draw_shadow (style, window, state_type, GTK_SHADOW_IN, area,
			widget, detail, x, y, width-1, height);
    }
  else if ((CHECK_DETAIL (detail, "toolbar"))
	   || (CHECK_DETAIL (detail, "menubar"))
	   || (IS_BONOBO_TOOLBAR (widget))
	   || (CHECK_DETAIL (detail, "dockitem"))
	   || (CHECK_DETAIL (detail, "dockitem_bin"))
	   || (CHECK_DETAIL (detail, "handlebox_bin"))
	   || (CHECK_DETAIL (detail, "handlebox")))
    {
      if (((CHECK_DETAIL (detail, "dockitem_bin")) && 
           (IS_BONOBO_DOCK_ITEM(widget))) || 
          ((widget) && (ge_is_bonobo_dock_item(widget->parent))))
	{	  
	  GList *children = NULL, *child = NULL;
	  GtkWidget *dockitem = widget;
	  gboolean has_grip = FALSE, ltr = TRUE;
	  
	  if ((!IS_BONOBO_DOCK_ITEM(widget)) && (!IS_BOX(widget)))
	    dockitem = widget->parent;
	    
	  has_grip = IS_CONTAINER(dockitem);
	  
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
	        }
	      else
	        {
	          if (dockitem == widget)
	            {
	              y += 8;
	              height -= 8;
	            }
	        }
	    }    
	}
  
      do_glide_draw_default_fill (style, window, widget,
					  state_type, area, x, y, width,
					  height, FALSE, TRUE);
 
      /* If this is a menu embedded in the gnome-panel, we don't
       *  draw a border since it looks cleaner without one.
       */
      if ((shadow_type != GTK_SHADOW_NONE) && (widget) && (widget->parent) &&
	  ((!((CHECK_DETAIL (detail, "menubar")) && 
	  ge_is_panel_widget_item (widget)))))
        {
          GdkRectangle shadow, clip;
          
          shadow.x = x;
          shadow.y = y;
          shadow.width = width;
          shadow.height = height;
          
          if (area)
            gdk_rectangle_intersect(area, &shadow, &clip);
          else
            clip = shadow;
            
           cairo_t *canvas = ge_gdk_drawable_to_cairo (window, &clip);

		do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_OUT,
					x, y,                                   
					width, 
					height);

          
           cairo_destroy(canvas);			          
        }
    }
  else if ((CHECK_DETAIL (detail, "menuitem")))
    { 
      do_glide_draw_default_fill (style, window, widget,
					  state_type, area, x, y, width,
					  height, FALSE, FALSE);

      cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);
		 			  
	do_glide_draw_border(canvas, &glide_style->color_cube.bg[state_type],
					GLIDE_BEVEL_STYLE_FLAT, GLIDE_BORDER_TYPE_IN,
					x, y, width, height);
      cairo_destroy(canvas);
    }
  else if ((CHECK_DETAIL (detail, "bar"))
	   && (widget && IS_PROGRESS_BAR (widget)))
    { 
      do_glide_draw_default_fill (style, window, widget,
                                    GTK_STATE_SELECTED, area, x, y,
                                    width, height, FALSE, FALSE);
      cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);
		do_glide_draw_border(canvas, &glide_style->color_cube.bg[GTK_STATE_SELECTED],
					GLIDE_BEVEL_STYLE_DEFAULT, GLIDE_BORDER_TYPE_OUT,
					x, y, width, height);
      cairo_destroy(canvas);
    }
  else if ((CHECK_DETAIL (detail, "button")) && widget
	   && ge_is_in_combo_box (widget))
    {
      /* ComboBox Buttons are an especially complex case
       * so call our special function for them.
       */
      glide_draw_combobox_button (style, window, state_type, shadow_type, area,
			    widget, detail, x, y, width, height);
    }
  else if ((CHECK_DETAIL (detail, "spinbutton_up"))
	   || (CHECK_DETAIL (detail, "spinbutton_down")))
    {
      /* SpinButton Steppers are an especially complex case
       * so call our special function for them.
       */
      glide_draw_spinbutton_stepper (style, window, state_type, shadow_type, area,
			       widget, detail, x, y, width, height);
    }
  else if (IS_TOGGLE_BUTTON(widget) && (TOGGLE_BUTTON(widget)->active))
    {
      do_glide_draw_default_fill (style, window, widget, state_type, 
                                      area, x, y, width, height, FALSE, FALSE);

      glide_draw_shadow (style, window, state_type, shadow_type, area,
			widget, detail, x, y, width, height);
    }
  else
    {
      /* default box apearance */
      do_glide_draw_default_fill (style, window, widget,
                                    state_type, area, x, y, width,
                                    height, FALSE, FALSE);
      glide_draw_shadow (style, window, state_type, shadow_type, area,
			widget, detail, x, y, width, height);
    }

      /* Paint a triangle here instead of in "buttondefault"
         which is drawn _behind_ the current button */
      if ((CHECK_DETAIL (detail, "button")) && widget && GTK_WIDGET_HAS_DEFAULT (widget))
        {
      cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

          ge_cairo_set_color(canvas, &glide_style->color_cube.bg[GTK_STATE_SELECTED]);
          cairo_move_to(canvas, x+2.5, y+2.5);
          cairo_line_to(canvas, x+11.5, y+2.5);
          cairo_line_to(canvas, x+2.5, y+11.5);
          cairo_line_to(canvas, x+2.5, y+2.5);
	  cairo_fill(canvas);

          cairo_set_line_width (canvas, 1);

          ge_cairo_set_color(canvas, &glide_style->color_cube.dark[state_type]);
	  cairo_move_to(canvas, x + 2.5, y + 12);
	  cairo_line_to(canvas, x + 2.5, y + 2.5);
  	  cairo_line_to(canvas, x + 12, y + 2.5);
	  cairo_stroke(canvas);

          ge_cairo_set_color(canvas, &glide_style->color_cube.light[state_type]);
          cairo_move_to(canvas, x+12, y+2.5);
          cairo_line_to(canvas, x+2.5, y+12);
	  cairo_stroke(canvas);
	
	cairo_destroy(canvas);
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

      cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

      do_glide_draw_line(canvas, &glide_style->color_cube.dark[state_type], &glide_style->color_cube.light[state_type], 
                           area, y + style->ythickness + 1, y + height - style->ythickness - 2, 
                           vline_x, FALSE);
 
      cairo_destroy(canvas);
 
      if ((widget) && (gtk_widget_get_direction (GTK_WIDGET (widget)) == GTK_TEXT_DIR_RTL))
         x +=  indicator_spacing.right + style->xthickness;
      else
         x += width - indicator_size.width - indicator_spacing.right - style->xthickness;
 
      y += ((height - indicator_size.height) / 2) + 1;
 
      width = indicator_size.width;
      height = indicator_size.height;
 
      glide_draw_arrow (style, window, state_type, shadow_type, area, NULL, "optionmenu", 
	                      GTK_ARROW_DOWN, TRUE,  x,  y,  width,  height);
   }
}
 
/***********************************************
 * glide_draw_tab -
 *  
 *   Function pretty much only called to draw 
 *   the arrow "tab" in the option menu
 ***********************************************/
void
glide_draw_tab (GtkStyle * style,
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
 * glide_draw_slider -
 *  
 *   Function which draws scale/scrollbar slider
 ***********************************************/
void
glide_draw_slider (GtkStyle * style,
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
  GlideStyle *glide_style = GLIDE_STYLE (style);
 
  CHECK_ARGS
  SANITIZE_SIZE
 
      do_glide_draw_default_fill (style, window, widget,
                                    state_type, area, x, y, width,
                                    height, orientation == GTK_ORIENTATION_VERTICAL, FALSE);
 
      glide_draw_shadow (style, window, GTK_STATE_NORMAL, GTK_SHADOW_OUT, area,
                           widget, detail, x, y, width, height);


      cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

	do_glide_draw_grip (canvas,
			&glide_style->color_cube.light[GTK_STATE_NORMAL],
			&glide_style->color_cube.dark[GTK_STATE_NORMAL],
			x, y, width, height, orientation == GTK_ORIENTATION_VERTICAL);

	cairo_destroy(canvas);
}

void
glide_draw_box_gap (GtkStyle * style, 
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
		 gint gap_size)
{
	CHECK_ARGS
	SANITIZE_SIZE

	do_glide_draw_default_fill (style, window, widget, state_type, area, x, y, width, height, FALSE, TRUE);

	glide_draw_shadow_gap (style, window, state_type, shadow_type, area, widget, detail, 
					x, y, width, height, gap_side, gap_pos, gap_size);
}
 
void
glide_draw_shadow_gap (GtkStyle * style, 
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
		 gint gap_size)
{
	GlideStyle *glide_style = GLIDE_STYLE (style);

	CairoColor color1, color2, color3, color4;
	gboolean overlap = FALSE;

	CHECK_ARGS
	SANITIZE_SIZE

	if (shadow_type == GTK_SHADOW_NONE)
		return;

	switch (gap_side)
	{
		case GTK_POS_TOP:
		case GTK_POS_BOTTOM:
			if ((gap_pos + gap_size) == width)
			{
				gap_pos -= 1;
			}

			gap_size += 1;
		break;

		case GTK_POS_LEFT:
		case GTK_POS_RIGHT:
			if ((gap_pos + gap_size) == height)
			{
				gap_pos -= 1;
			}

			gap_size += 1;
		break;
	}

	switch (shadow_type)
	{
		case GTK_SHADOW_ETCHED_IN:
			color1 = color4 = glide_style->color_cube.dark[state_type];
			color2 = color3 = glide_style->color_cube.light[state_type];
			overlap = FALSE;
		break;

		case GTK_SHADOW_ETCHED_OUT:
			color1 = color4 = glide_style->color_cube.light[state_type];
			color2 = color3 = glide_style->color_cube.dark[state_type];
			overlap = TRUE;
		break;
      
		case GTK_SHADOW_IN:
			color1 = glide_style->color_cube.mid[state_type];
			color2 = glide_style->color_cube.dark[state_type];
			color3 = glide_style->color_cube.light[state_type];
			color4 = glide_style->color_cube.mid[state_type];
			overlap = FALSE;
		break;
	
		default:
		case GTK_SHADOW_OUT:
			color1 = glide_style->color_cube.mid[state_type];
			color2 = glide_style->color_cube.light[state_type];
			color3 = glide_style->color_cube.dark[state_type];
			color4 = glide_style->color_cube.mid[state_type];
			overlap = TRUE;
		break;
	}

	cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);
	cairo_reset_clip(canvas);
	glide_simple_border_gap_clip(canvas, x, y, width, height, gap_side, gap_pos+1, gap_size - 2);

	ge_cairo_simple_border(canvas, &color1, &color3, x, y, width, height, overlap);

	cairo_reset_clip(canvas);
	glide_simple_border_gap_clip(canvas, x, y, width, height, gap_side, gap_pos + 2, gap_size - 4);

	ge_cairo_simple_border(canvas, &color2, &color4, x+1, y+1, width-2, height-2, overlap);

	cairo_destroy(canvas);
}

/***********************************************
 * glide_draw_extension -
 *  
 *   Function which draws notebook extensions -
 *   aka, tabs.
 *
 *  This routine is so complex mostly because
 *  it tries to draw a faked 1-2 pixel rounded
 *  corner.
 ***********************************************/
void
glide_draw_extension (GtkStyle * style,
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
	gint widget_x = 0, widget_y = 0, widget_width = 0, widget_height = 0;

	CHECK_ARGS
	SANITIZE_SIZE

	GdkRectangle clip, shadow;

	shadow.x = x;
	shadow.y = y;
	shadow.width = width;
	shadow.height = height;
 
	/* GTK doesn't overlap Extensions and Notebooks,
		but rather a tab is drawn with a "gap" side.

		Instead of long draw cases per gap side,
		call standard draw, but clipped to size,
		and overdraw edge thickness + one on gap side.
 
		To fake the apearance of overlap on edge aligned tabs
		increase clip by one on gap side. */

  do_glide_draw_default_fill (style, window, widget, state_type, area, 
                                x, y, width, height+1, FALSE, state_type!=GTK_STATE_NORMAL);

	if (widget && (GTK_IS_NOTEBOOK (widget)))
	{
		widget_x = (widget->allocation.x + GTK_CONTAINER (widget)->border_width);
		widget_y = (widget->allocation.y + GTK_CONTAINER (widget)->border_width);
		widget_width = (widget->allocation.width - 2*GTK_CONTAINER (widget)->border_width);
		widget_height = (widget->allocation.height - 2*GTK_CONTAINER (widget)->border_width);
	}

	switch (gap_side)
	{
		case GTK_POS_TOP:
			if ((widget && GTK_IS_NOTEBOOK (widget)) && 
				((x==widget_x) || 
				((x + width) == (widget_x + widget_width))))
			{
				shadow.y -= 1;
				shadow.height += 1;
			}

			y -= 3;
			height += 3;
		break;

		case GTK_POS_LEFT:
			if ((widget && GTK_IS_NOTEBOOK (widget)) && 
				((y==widget_y) || 
				((y + height) == (widget_y + widget_height))))
			{
				shadow.x -= 1;
				shadow.width += 1;
			}
			x -= 3;
			width += 3;
		break;

		default:
		case GTK_POS_BOTTOM:
			if ((widget && GTK_IS_NOTEBOOK (widget)) && 
				((x==widget_x) || 
				((x + width) == (widget_x + widget_width))))
			{
				shadow.height += 1;
			}
			height += 3;
		break;

		case GTK_POS_RIGHT:
			if ((widget && GTK_IS_NOTEBOOK (widget)) && 
				((y==widget_y) || 
				((y + height) == (widget_y + widget_height))))
			{
				shadow.width += 1;
			}
			width += 3;
		break;
	}

	if (area)
		gdk_rectangle_intersect(area, &shadow, &clip);
	else
		clip = shadow;

	glide_draw_shadow (style, window, state_type, shadow_type, &clip, widget, detail, 
				x, y, width, height);

}
 
/***********************************************
 * glide_draw_handle -
 *  
 *   Function which draws Handle box-
 *   aka toolbar drag grip/paned drag grip
 ***********************************************/
void
glide_draw_handle (GtkStyle * style,
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
  GlideStyle *glide_style = GLIDE_STYLE (style);

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

      cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);
 
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
	      orientation = GTK_ORIENTATION_VERTICAL;
	      break;
 
	    case GTK_POS_TOP:
	    case GTK_POS_BOTTOM:
	      orientation = GTK_ORIENTATION_HORIZONTAL;
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

  do_glide_draw_default_fill (style, window, widget, state_type, area, 
                                x, y, width, height, orientation == (GTK_ORIENTATION_VERTICAL), FALSE);
 

      if (ge_is_panel_widget_item (widget)
	  && (CHECK_DETAIL (detail, "handlebox")
	      && (!IS_HANDLE_BOX_ITEM (widget)))
	  && (!(IS_HANDLE_BOX (widget))))
	{
      /* draw the drag bar */
      if (orientation == GTK_ORIENTATION_VERTICAL)
        {
          
	  ge_cairo_simple_border (canvas, &glide_style->color_cube.light[state_type],
			          &glide_style->color_cube.dark[state_type],
			          x + style->xthickness + 1, y + height / 2 - 1,
			          width - style->xthickness - 3, 3, FALSE);
        }
      else
        {
	  ge_cairo_simple_border (canvas, &glide_style->color_cube.light[state_type],
			          &glide_style->color_cube.dark[state_type], x + width / 2 - 1,
			          y + style->ythickness + 1, 3,
			          height - style->ythickness - 3, FALSE);
        }

	  /* If this is on a PanelWidget, we draw a line 
	   * next to it instead of drawing a border around it.
	   */
	  if (orientation == GTK_ORIENTATION_VERTICAL)
	    {
	      ge_cairo_line (canvas, &glide_style->color_cube.dark[state_type], x + 1, y + 0,
			     x + width - 2, y + 0);
	      ge_cairo_line (canvas, &glide_style->color_cube.light[state_type], x + 1,
			     y + 1, x + width - 2, y + 1);
	    }
	  else
	    {
	      ge_cairo_line (canvas, &glide_style->color_cube.dark[state_type], x + 0, y + 1,
			     x + 0, y + height - 2);
	      ge_cairo_line (canvas, &glide_style->color_cube.light[state_type], x + 1,
			     y + 1, x + 1, y + height - 2);
	    }
	}
      else
	{
	do_glide_draw_grip (canvas,
			&glide_style->color_cube.light[GTK_STATE_NORMAL],
			&glide_style->color_cube.dark[GTK_STATE_NORMAL],
			x, y, width, height, orientation == GTK_ORIENTATION_VERTICAL);

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
            {
		glide_draw_shadow (style, window, state_type, GTK_SHADOW_OUT, area,
                           widget, detail, x, y, width, height);
            }
	}
    }
           cairo_destroy(canvas);			          
}

extern GtkStyleClass *glide_parent_class;

void
glide_draw_focus(GtkStyle *style,
           GdkWindow *window,
           GtkStateType state_type,
           GdkRectangle *area,
           GtkWidget *widget,
           const gchar *detail,
           gint x,
           gint y,
           gint width,
           gint height)
{
	GlideStyle *glide_style = GLIDE_STYLE (style);

	gboolean interior_focus = FALSE;
	gint line_width = 1;

	CHECK_ARGS

	/* Fallback on GTK's Line Pattern */
	if (widget)
	{
		gtk_widget_style_get (widget, "focus-line-width", &line_width, NULL);

		gtk_widget_style_get (widget, "interior_focus", &interior_focus, NULL);
	}


	if (interior_focus)
	{
		glide_parent_class->draw_focus(style, window, state_type, area, widget, detail, x, y, width, height);
	}
	else
	{
		cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

		ge_cairo_set_color(canvas, &glide_style->color_cube.bg[GTK_STATE_SELECTED]);
		cairo_set_line_width(canvas, line_width);

	 	if (CHECK_DETAIL(detail, "button") && (ge_is_in_combo_box (widget)))
		{
			gint thick = 2;

			if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
			{
				cairo_rectangle(canvas, x - thick + line_width*2, y + thick, width - line_width*2, height - thick*2);
			}
			else
			{
				cairo_rectangle(canvas, x + thick, y + thick, width - line_width*2, height - thick*2);
			}
		}
	 	else if (CHECK_DETAIL(detail, "entry") && (IS_SPIN_BUTTON(widget)) )
		{
			if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
			{
				cairo_rectangle(canvas, x + 0.5, y + 0.5, width + line_width - 1, height - 1);
			}
			else
			{
				cairo_rectangle(canvas, x - line_width + 0.5, y + 0.5, width + line_width - 1, height - 1);
			}
		}
 		else if (CHECK_DETAIL(detail, "entry") && (ge_is_in_combo_box (widget)))
		{
			GtkWidget *button = g_object_get_data(G_OBJECT(ge_find_combo_box_widget_parent (widget)), "button");

			if (GTK_IS_WIDGET(button))
			{
				gtk_widget_queue_draw_area(button, button->allocation.x, button->allocation.y, 
								button->allocation.width, button->allocation.height);
			}

			if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
			{
				cairo_rectangle(canvas, x + 0.5, y + 0.5, width + line_width - 1, height - 1);
			}
			else
			{
				cairo_rectangle(canvas, x - line_width + 0.5, y + 0.5, width + line_width - 1, height - 1);
			}
		}
		else if (CHECK_DETAIL(detail, "spinbutton_up"))
		{
			if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
			{
				cairo_rectangle(canvas, x + line_width, y, width - line_width, height);
			}
			else
			{
				cairo_rectangle(canvas, x, y, width - line_width, height);
			}

			cairo_clip(canvas);
			cairo_rectangle(canvas, x + 0.5, y + 0.5, width - 1, height + line_width - 1);
		} 
	        else if (CHECK_DETAIL(detail, "spinbutton_down"))
		{
			if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
			{
				cairo_rectangle(canvas, x + line_width, y, width - line_width, height);
			}
			else
			{
				cairo_rectangle(canvas, x, y, width - line_width, height);
			}		

			cairo_clip(canvas);
			cairo_rectangle(canvas, x + 0.5, y - line_width  + 0.5, width - 1, height + line_width - 1);
		} 
	        else if (CHECK_DETAIL(detail, "combo_entry_button"))
		{
			if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
			{
				cairo_rectangle(canvas, x + line_width, y, width - line_width, height + 1);
				cairo_clip(canvas);

				cairo_rectangle(canvas, x + 0.5, y + 0.5, width - 1, height - 1);
			}
			else
			{
				cairo_rectangle(canvas, x, y, width, height + 1);
				cairo_clip(canvas);

				cairo_rectangle(canvas, x+0.5, y+0.5, width + line_width - 1, height - 1);
			}
		}
		else
		{
  			cairo_rectangle(canvas, x+0.5, y+0.5, width-1, height-1);
		}

		cairo_stroke(canvas);

		cairo_destroy(canvas);
	}
}
