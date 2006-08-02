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

#include "hc_gtk2_engine.h"
#include "hc_gtk2_support.h"
#include "hc_gtk2_drawing.h"

#include <gtk/gtk.h>

#include <math.h>
#include <string.h>

/* Standard Border Function */
void
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
	/* Border Uses Foreground Color */
	CairoColor *foreground = &HC_STYLE(style)->color_cube.fg[state_type];

	/***********************************************/
	/* GTK Sanity Checks                           */
	/***********************************************/
	CHECK_ARGS
	SANITIZE_SIZE 


	/***********************************************/
	/* GTK Special Cases - adjust Size/Offset      */
	/***********************************************/
	#warning SHADOW LINE WIDTH SHOULD BE EXPLICITELY CONFIGURABLE
	gint line_width = floor(MIN(style->xthickness,style->ythickness)*1.5);
	gint clip_x = x, clip_y = y, clip_width = width, clip_height = height;


	/* Spin Button */
	if ((CHECK_DETAIL(detail, "spinbutton_up")) || (CHECK_DETAIL(detail, "spinbutton_down")))
	{
		/* Overdraw Height By Half The Line Width -
			Prevents Double line Between buttons */
		height += floor(line_width / 2);


		/* If Down Button Offset By Half Line Width */
		if (CHECK_DETAIL(detail, "spinbutton_down"))
		{
			y -= floor(line_width / 2);
		}


		/* Overdraw Width By Line Width -
			Prevents Double line Between Entry And Buttons */
		width += line_width;


		/* If LTR Offset X By Line Width */
		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			x -= line_width;
		}
	}


	/* Entry - Force Border To Use Foreground Matching Widget State */
	if (CHECK_DETAIL(detail, "entry") && !ge_is_combo(widget))
	{
               	gtk_widget_ensure_style(widget);

		foreground = &HC_STYLE(widget->style)->color_cube.fg[widget->state];
	}


	/* Combo Box Button's */
	if (CHECK_DETAIL(detail, "button") && ge_is_in_combo_box(widget))
	{
		/* Overdraw Width By Line Width -
			Prevents Double Line Between Entry and Button. */
		width += line_width;


		/* If LTR Offset X By Line Width */
		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			x -= line_width;
		}


		/* Force Border To Use Foreground Matching Parent State */
               	gtk_widget_ensure_style(widget->parent);
		foreground = &HC_STYLE(widget->parent->style)->color_cube.fg[widget->parent->state];
	}


	/***********************************************/
	/* Draw Border                                 */
	/***********************************************/
	cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

	/* Clip Border Too Passed Size */
	cairo_rectangle(canvas, clip_x, clip_y, clip_width, clip_height);
	cairo_clip(canvas);

	/* Set Line Style */
	ge_cairo_set_color(canvas, foreground);
	cairo_set_line_cap(canvas, CAIRO_LINE_CAP_BUTT);

	/* Pixel Alignedness -
			
		If even line width, + 1.0 
		If odd line width,  + 0.5 
	 */
	cairo_set_line_width (canvas, line_width + ((line_width%2)?1.0:0.5));

	/* Stroke Rectangle */
	cairo_rectangle (canvas, x, y, width, height);
	cairo_stroke(canvas);

	cairo_destroy(canvas);
}


/* Border Function For Frame && Notebook,
	With Gap For Text and/or Tabs
 */
void 
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
		    gint            gap_pos,
		    gint            gap_size)
{
	/* Border Uses Foreground Color */
	CairoColor *foreground = &HC_STYLE(style)->color_cube.fg[state_type];

	/***********************************************/
	/* GTK Sanity Checks                           */
	/***********************************************/
	CHECK_ARGS
	SANITIZE_SIZE 

	if (shadow_type == GTK_SHADOW_NONE)
		return;

	/***********************************************/
	/* GTK Special Cases - adjust Size/Offset      */
	/***********************************************/
	#warning SHADOW GAP LINE WIDTH SHOULD BE EXPLICITELY CONFIGURABLE
	gint line_width = floor(MIN(style->xthickness,style->ythickness)*1.5);

	if (gap_size > 0)
	{
		gap_pos += floor(line_width/2.0) + 1; 
		gap_size -= (floor(line_width/2.0) + 1)*2;
	}

	/***********************************************/
	/* Draw Border                                 */
	/***********************************************/
	cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

	/* Create And Clip Too Path To Ignore Gap */
	hc_simple_border_gap_clip(canvas, line_width, x, y, width, height, gap_side, gap_pos, gap_size);


	/* Set Line Style */
	ge_cairo_set_color(canvas, foreground);
	cairo_set_line_cap(canvas, CAIRO_LINE_CAP_BUTT);

	/* Pixel Alignedness -
			
		If even line width, + 1.0 
		If odd line width,  + 0.5 
	 */
	cairo_set_line_width (canvas, line_width + ((line_width%2)?1.0:0.5));

	/* Stroke Rectangle */
	cairo_rectangle (canvas, x, y, width, height);
	cairo_stroke(canvas);

	cairo_destroy(canvas);
}


/* Border Function For Notebooks Tabs */
void 
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
	/* Fill Uses Background Color */
	CairoColor *background = &HC_STYLE(style)->color_cube.bg[state_type];

	/* Border Uses Foreground Color */
	CairoColor *foreground = &HC_STYLE(style)->color_cube.fg[state_type];

	/***********************************************/
	/* GTK Sanity Checks                           */
	/***********************************************/
	CHECK_ARGS
	SANITIZE_SIZE 


	/***********************************************/
	/* GTK Special Cases - adjust Size/Offset      */
	/***********************************************/
	#warning EXTENSION MUST ENSURE LINE WIDTH FOLLOWS SHADOW GAP
	gint line_width = floor(MIN(style->xthickness,style->ythickness)*1.5);

	gint widget_x = 0, widget_y = 0, widget_width = 0, widget_height = 0;
	gint clip_x = x, clip_y = y, clip_width = width, clip_height = height;


	/* What all this is for -

		GTK doesn't overlap Extensions and Notebooks,
		but rather a tab is drawn with a "gap" side.

		Instead of long draw cases per gap side,
		perform a standard draw, but clipped to size,
		and overdraw edge thickness + one on gap side.
 
		To fake the apearance of overlap on edge aligned tabs
		increase clip by edge thickness on gap side. 
	 */
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
			if (GTK_CHECK_VERSION(2,10,0) || 
				((widget && GTK_IS_NOTEBOOK (widget)) && 
				((x==widget_x) || 
				((x + width) == (widget_x + widget_width)))))
			{
				clip_height += line_width;

				if (!GTK_CHECK_VERSION(2,10,0))
				{
					height -= floor(line_width/2.0);
				}
			}
			
			y -= (line_width + 1);
			height += (line_width + 1);
		break;

		case GTK_POS_LEFT:
			if (GTK_CHECK_VERSION(2,10,0) || 
				((widget && GTK_IS_NOTEBOOK (widget)) && 
				((y==widget_y) || 
				((y + height) == (widget_y + widget_height)))))
			{
				clip_width += line_width;

				if (!GTK_CHECK_VERSION(2,10,0))
				{
					x -= floor(line_width/2.0);
				}
			}

			x -= (line_width + 1);
			width += (line_width + 1);
		break;

		default:
		case GTK_POS_BOTTOM:
			height += (line_width + 1);
		break;

		case GTK_POS_RIGHT:
			width += (line_width + 1);
		break;
	}


	/***********************************************/
	/* Draw Border                                 */
	/***********************************************/
	cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

	/* Clip Too Size */
	cairo_rectangle(canvas, clip_x, clip_y, clip_width, clip_height);
	cairo_clip(canvas);


	/* Set Fill Style */
	ge_cairo_set_color(canvas, background);

	/* Fill Rectangle */
	cairo_rectangle (canvas, x, y, width, height);
	cairo_fill(canvas);


	/* Set Line Style */
	ge_cairo_set_color(canvas, foreground);
	cairo_set_line_cap(canvas, CAIRO_LINE_CAP_BUTT);

	/* Pixel Alignedness -
			
		If even line width, + 1.0 
		If odd line width,  + 0.5 
	 */
	cairo_set_line_width (canvas, line_width + ((line_width%2)?1.0:0.5));

	/* Stroke Rectangle */
	cairo_rectangle (canvas, x, y, width, height);
	cairo_stroke(canvas);

	cairo_destroy(canvas);
}


/* Draw Function For Boxes Traditionally Either Without Borders, 
		or With A Single Pixel Line */
void
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
		hc_draw_box (style, window, state_type, shadow_type, area,
				widget, detail, x, y, width, height);
	else
		hc_parent_class->draw_flat_box (style, window, state_type, shadow_type, area,
							widget, detail, x, y, width, height);
}


/* Draw Function For Standard Boxes (Most Widgets)
	Ensures Fill And Border */
void
hc_draw_box (GtkStyle	*style,
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
	/***********************************************/
	/* GTK Sanity Checks                           */
	/***********************************************/
	CHECK_ARGS
	SANITIZE_SIZE 


	/***********************************************/
	/* GTK Special Cases - adjust Size/Offset      */
	/***********************************************/

	/* Add Menu Shell Hack For Menu Bar Item Prelight */
	if (IS_MENU_SHELL(widget))
	{
		hc_gtk2_engine_hack_menu_shell_setup_signals(widget);
	}  


	/***********************************************/
	/* Draw Fill                                   */
	/***********************************************/
	gtk_style_apply_default_background (style, window,
        					widget && !GTK_WIDGET_NO_WINDOW (widget),
						state_type, area, x, y, width, height);
  

	/***********************************************/
	/* Draw Border                                 */
	/***********************************************/
	hc_draw_shadow (style, window, state_type, shadow_type, area, widget, detail, 
					x, y, width, height);    
}


/* Draw Function For Boxes With Gap 

	Primarily For Frames With Text
	And Notebooks With Tabs. 
 */
void 
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
		 gint            gap_pos,
		 gint            gap_size)
{
	/***********************************************/
	/* GTK Sanity Checks                           */
	/***********************************************/
	CHECK_ARGS
	SANITIZE_SIZE 


	/***********************************************/
	/* Draw Fill                                   */
	/***********************************************/
	gtk_style_apply_default_background (style, window,
        					widget && !GTK_WIDGET_NO_WINDOW (widget),
						state_type, area, x, y, width, height);
  

	/***********************************************/
	/* Draw Border                                 */
	/***********************************************/
	hc_draw_shadow_gap (style, window, state_type, shadow_type, area, widget, detail, 
					x, y, width, height, gap_side, gap_pos, gap_size);    
}


/* Draw Function For Boxes Commonly Needing Grips

	Primarily For -
		Paned Handles
		Toolbar Handles 
 */
void 
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
	/***********************************************/
	/* GTK Sanity Checks                           */
	/***********************************************/
	CHECK_ARGS
	SANITIZE_SIZE 


	/***********************************************/
	/* GTK Special Cases - adjust Size/Offset      */
	/***********************************************/
	gint clip_x = x, clip_y = y, clip_width = width, clip_height = height;
	gint xthick = style->xthickness, ythick = style->ythickness;

	if (CHECK_DETAIL(detail, "paned"))
	{
		/* we want to ignore the shadow border in paned widgets */
		xthick = 0;
		ythick = 0;
	}  

	clip_x = x + xthick;
	clip_y = y + ythick;
	clip_width = width - (xthick * 2);
	clip_height = height - (ythick * 2);


	/***********************************************/
	/* Draw Box                                    */
	/***********************************************/
	hc_draw_box (style, window, state_type, shadow_type, area, widget, 
			detail, x, y, width, height);
  
  
	/***********************************************/
	/* Draw Grip                                   */
	/***********************************************/
	HcStyle *hc_style = HC_STYLE (style);
	gdouble xx, yy;
	CairoColor *light, *dark;
  
	light = &hc_style->color_cube.light[state_type];
	dark = &hc_style->color_cube.dark[state_type];

	cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

	/* Clip Too Size */
	cairo_rectangle(canvas, clip_x, clip_y, clip_width, clip_height);
	cairo_clip(canvas);

	if (CHECK_DETAIL (detail, "paned"))
	{
		if (orientation == GTK_ORIENTATION_HORIZONTAL)
		{
			for (xx = x + width/2.0 - 15; xx <= x + width/2.0 + 15; xx += 5)
				do_hc_draw_dot (canvas, light, dark, xx, y + height/2.0);
		}
		else
		{
			for (yy = y + height/2 - 15; yy <= y + height/2.0 + 15; yy += 5)
			{
				do_hc_draw_dot (canvas, light, dark, x + width/2.0, yy);
			}
		}
	}
	else
	{
		for (yy = y + ythick; yy < (y + height - ythick); yy += 3)
		{
			for (xx = x + xthick; xx < (x + width - xthick); xx += 6)
			{
				do_hc_draw_dot (canvas, light, dark, xx, yy);
				do_hc_draw_dot (canvas, light, dark, xx + 3, yy + 1);
			}
		}
	}

	cairo_destroy(canvas);
}


void
hc_draw_slider (GtkStyle * style,
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
	CHECK_ARGS
	SANITIZE_SIZE
 
	cairo_t *canvas = ge_gdk_drawable_to_cairo (window, area);

	/***********************************************/
	/* Draw Box                                    */
	/***********************************************/
	#warning SHADOW LINE WIDTH SHOULD BE EXPLICITELY CONFIGURABLE
	gint line_width = floor(MIN(style->xthickness,style->ythickness)*1.5);

	hc_draw_box (style, window, state_type, shadow_type, area, widget, 
			detail, x, y, width, height);

	if (IS_SCALE(widget))
	{
		if (orientation == GTK_ORIENTATION_HORIZONTAL)
		{
			do_hc_draw_line (canvas, &HC_STYLE(style)->color_cube.fg[state_type],
						line_width /2, 
						x + ceil(width/2) + 0.5, 
						y + line_width, 
						x + ceil(width/2) + 0.5, 
						y + height - line_width);     
		}
		else
		{
			do_hc_draw_line (canvas, &HC_STYLE(style)->color_cube.fg[state_type],
						line_width /2, 
						x + line_width, 
						y + ceil(height/2) + 0.5,
						x + width - line_width, 
						y + ceil(height/2) + 0.5);     
		}
	}

	cairo_destroy(canvas);
}

/* Draw Check Buttons Check & Border */
void 
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
	gboolean invert_checkboxes = FALSE; /* TODO: read this from RC file in engine-specific data? */

	CHECK_ARGS
	SANITIZE_SIZE

	#warning - CHECK IN CELL RENDERER DOES NOT FOLLOW SIZING ALWAYS TO SMALL - FIND WORKAROUND

	if (GTK_IS_CELL_RENDERER_TOGGLE(widget) || CHECK_DETAIL("cellcheck",detail))
	{
		x -= 1;
		y -= 1;
		width += 2;
		height += 2;
	}

	gboolean inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));
	inconsistent |= (GTK_IS_CELL_RENDERER_TOGGLE(widget) && gtk_cell_renderer_toggle_get_inconsistent (widget));
	inconsistent |= (CHECK_DETAIL("cellcheck",detail) && (shadow_type == GTK_SHADOW_ETCHED_IN));

	gint line_width = floor(MIN(style->xthickness,style->ythickness)*1.5);
	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

	gtk_widget_style_get (widget,
				"focus-line-width", &line_width,
				NULL);
	
	cairo_save(cr);

	cairo_set_line_width (cr, line_width + ((line_width%2)?1:0.5));
	
	ge_cairo_set_color(cr, &hc_style->color_cube.base[state_type]);	

	cairo_rectangle (cr, x, y, width, height);

	cairo_fill(cr);

	ge_cairo_set_color(cr, &hc_style->color_cube.fg[state_type]);	
 
	cairo_rectangle (cr, x, y, width, height);

	cairo_stroke(cr);

	cairo_restore(cr);

	if ((shadow_type == GTK_SHADOW_IN) && !inconsistent)
	{
		#warning - MATH && DIAGONALS && PIXEL SNAPPING == PAIN IN MY ARSE
		ge_cairo_set_color(cr, &hc_style->color_cube.fg[state_type]);	
		
		/* The X is inside the outer line_width */
		gdouble offset = line_width;

		/* What should the line width be? not the same as border... 
			but why this?
		 */
		line_width = ceil(width/5);

		/* The X is capped square, 
				so uh. inset by line_width/2 to center line
				and.. line_width/2 space? 

				so total inner line_width?

				and + 0.5 if not (line_width - 1) % 2, pixel alignedness, 
				1 pixel center + even numbers for 45 diagonal?  

				My Head hurts. 
		 */
		offset += line_width - (((line_width - 1) % 2)?0:0.5);

		/* Backward Diagonal */
		cairo_move_to(cr, x + offset, y + offset);
		cairo_line_to(cr, x + width - offset, y + height - offset);

		/* Forward Diagonal */
		cairo_move_to(cr, x + offset, y + height - offset);
		cairo_line_to(cr, x + width - offset, y + offset);

		/* Again with the + 0.5 if not (line_width - 1) % 2 for pixel alignedness
				Sort of. I don't think this works for extra large sizes,
				which means I don't really understand the problem at all....
		 */
		cairo_set_line_width (cr, line_width + (((line_width - 1) % 2)?0:0.5));
		cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);

		cairo_stroke (cr);
	}
	else if (inconsistent)
	{
		#warning - CHECK INCONSISTENT STATE IS BROKEN-ISH, IMPROVE SIZING, SPACING, & ENSURE PIXEL ALIGNEDNESS BEFORE 2.8
		/* Rough logic, just make  thickness 1/3 height */
		int line_width = ceil((height + 1)/3);

		/* Force Thickness Even */
		line_width -= (line_width % 2);

		cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
		cairo_set_line_width (cr, line_width + ((height % 2)?1:0));

		cairo_move_to(cr, x + line_width - ((height % 2)?0.5:0), y + floor(height/2));
		cairo_line_to(cr, x + width - line_width + ((height % 2)?0.5:0), y + floor(height/2));

		ge_cairo_set_color(cr, &hc_style->color_cube.text[state_type]);	
		cairo_stroke (cr);
	}

	cairo_destroy(cr);
}

/* Draw Radio Button AKA Option Button Check & Border */
void 
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

	CHECK_ARGS
	SANITIZE_SIZE

	#warning - OPTION IN CELL RENDERER DOES NOT FOLLOW SIZING ALWAYS TO SMALL - FIND WORKAROUND
	if (GTK_IS_CELL_RENDERER_TOGGLE(widget) || CHECK_DETAIL("cellcheck",detail))
	{
		x -= 1;
		y -= 1;
		width += 2;
		height += 2;
	}

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

	gint centerX = x + floor(width/2);
	gint centerY = y + floor(height/2);
	gint radius = floor(MIN(width,height)/2.0);

	cairo_set_line_width (cr, radius*0.30);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
	
	cairo_arc(cr, centerX, centerY, radius, 0 , 2 * M_PI);
	ge_cairo_set_color(cr, &hc_style->color_cube.bg[state_type]);	
	cairo_fill (cr);

	cairo_arc(cr, centerX, centerY, radius, 0 , 2 * M_PI);
	ge_cairo_set_color(cr, &hc_style->color_cube.fg[state_type]);	
	cairo_stroke (cr);

	gboolean inconsistent = (IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)));
	inconsistent |= (GTK_IS_CELL_RENDERER_TOGGLE(widget) && gtk_cell_renderer_toggle_get_inconsistent (widget));
	inconsistent |= (CHECK_DETAIL("cellcheck",detail) && (shadow_type == GTK_SHADOW_ETCHED_IN));

	if ((shadow_type == GTK_SHADOW_IN) || inconsistent)
	{
		ge_cairo_set_color(cr, &hc_style->color_cube.text[state_type]);	

		if (inconsistent)
		{
			int line_width = ceil(radius*0.68);

			/* Force Thickness Even */
			line_width -= (line_width % 2);

			cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
			cairo_set_line_width (cr, line_width);

			cairo_move_to(cr, centerX - radius*0.38, centerY);
			cairo_line_to(cr, centerX + radius*0.38, centerY);

			cairo_stroke (cr);

		}
		else
		{
			cairo_arc(cr, centerX, centerY, radius*0.38, 0, 2 * M_PI);
			cairo_fill(cr);
			cairo_arc(cr, centerX, centerY, radius*0.38, 0, 2 * M_PI);
			cairo_stroke(cr);
		}
	}

	cairo_destroy(cr);
}

void
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
	GtkRequisition indicator_size;
	GtkBorder indicator_spacing;
	gint arrow_height;
  
	ge_option_menu_get_props (widget, &indicator_size, &indicator_spacing);
  
	indicator_size.width += 2;
	indicator_size.height += 2;

	if ((!widget) || (gtk_widget_get_direction (GTK_WIDGET (widget)) == GTK_TEXT_DIR_LTR))
	{
		x += width - indicator_size.width;
 	}

	y += ((height - indicator_size.height) / 2) + 1;
 
	width = indicator_size.width;
	height = indicator_size.height;
 
	HcStyle *hc_style = HC_STYLE (style);
	cairo_t *cr = ge_gdk_drawable_to_cairo(window, area);

	do_hc_draw_arrow (cr, &hc_style->color_cube.fg[state_type],
				GTK_ARROW_DOWN,TRUE, x, y,
				width, height);


	cairo_destroy(cr);
}

void
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

void
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
	/***********************************************/
	/* GTK Sanity Checks                           */
	/***********************************************/
	CHECK_ARGS
	SANITIZE_SIZE

	/***********************************************/
	/* GTK Arrow Special Cases - adjust Size/Offset*/
	/***********************************************/
	#warning SHADOW LINE WIDTH SHOULD BE EXPLICITELY CONFIGURABLE
	gint line_width = floor(MIN(style->xthickness,style->ythickness)*1.5);

	if (ge_is_combo_box_entry (widget))
	{
		x -= 1;

		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_LTR))
		{
			x -= (line_width/2);
		}
		else
		{
			x += (line_width/2);
		}
	}
	else if (ge_is_combo_box(widget, FALSE))
	{
		if ((widget) && (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL))
		{
			x -= 2;
		}
	}


	if (ge_is_combo(widget))
	{
		y += 1;
		width -= 2;
		height -= 2;

		if ((widget) && (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL))
		{
			x += line_width/2;
		}
		else
		{
			x -= line_width/2;
		}
	}

	if (CHECK_DETAIL(detail, "menuitem"))
	{
		x -= 1;
	}

	if (CHECK_DETAIL (detail, "arrow"))
	{
		x += (width%2)?0:1;
	}

	/***********************************************/
	/* Draw Arrow                                  */
	/***********************************************/
	HcStyle *hc_style = HC_STYLE (style);
	cairo_t *cr = ge_gdk_drawable_to_cairo(window, area);

	do_hc_draw_arrow (cr, &hc_style->color_cube.fg[state], arrow_type, TRUE,
				x, y, width+1, height+1);

	cairo_destroy(cr);
}

void
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

  do_hc_draw_line (cr, &hc_style->color_cube.fg[state_type], (CHECK_DETAIL(detail, "label"))?1:2*line_width - 1, 
			x1 + line_width + 2, y + style->ythickness/2 + 0.5, 
			x2 - line_width - 1, y + style->ythickness/2 + 0.5);     
  
  cairo_destroy(cr);
}

void
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

  do_hc_draw_line (cr, &hc_style->color_cube.fg[state_type],  (CHECK_DETAIL(detail, "label"))?1:2*line_width - 1, 
			x + style->xthickness/2 + 0.5, y1, 
			x + style->xthickness/2 + 0.5, y2);     

  cairo_destroy(cr);
}

void
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

void
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
