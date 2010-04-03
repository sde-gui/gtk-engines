/* Clearlooks - a cairo based GTK+ engine
 * Copyright (C) 2006 Richard Stellingwerff <remenic@gmail.com>
 * Copyright (C) 2006 Daniel Borgman <daniel.borgmann@gmail.com>
 * Copyright (C) 2007 Benjamin Berg <benjamin@sipsolutions.net>
 * Copyright (C) 2007-2008 Andrea Cimitan <andrea.cimitan@gmail.com>
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
 */


#include "clearlooks_draw.h"
#include "clearlooks_style.h"
#include "clearlooks_types.h"

#include "support.h"
#include <ge-support.h>
#include <math.h>

#include <cairo.h>

static void
clearlooks_gnome3_draw_entry (cairo_t *cr,
	                      const ClearlooksColors *colors,
	                      const WidgetParameters *params,
	                      const FocusParameters  *focus,
	                      int x, int y, int width, int height)
{
	const CairoColor *fg_color = &colors->text[params->state_type];
	const CairoColor *bg_color = &colors->base[params->state_type];
	CairoColor top_color;

	if (params->focus) {
		fg_color = &colors->base[GTK_STATE_SELECTED];
	}
	top_color = *fg_color;	

	cairo_save (cr);

	cairo_set_line_width (cr, 1.0);
	ge_cairo_rounded_rectangle (cr, x + 0.5, y + 0.5, width - 1,
	                            height - 1, MAX(0, params->radius - 0.5),
	                            params->corners);
	ge_cairo_set_color (cr, bg_color);
	cairo_fill_preserve (cr);

	if (params->focus || params->disabled || params->prelight || params->active) {
		ge_cairo_set_color (cr, fg_color);
		cairo_stroke (cr);
	} else {
		cairo_pattern_t *pattern;
		CairoColor bottom_color = colors->text[GTK_STATE_INSENSITIVE];

		ge_mix_color (&top_color, &bottom_color, 0.86, &top_color);

		/* Top/bottom pixel lines are outside of the gradient, there is
		 * no point. */
		pattern = cairo_pattern_create_linear (x, y + 1, x, height - 1);
		cairo_pattern_add_color_stop_rgb (pattern, 0.0, top_color.r, top_color.g, top_color.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0, bottom_color.r, bottom_color.g, bottom_color.b);
		cairo_set_source (cr, pattern);
		cairo_stroke (cr);

		cairo_pattern_destroy (pattern);
	}

	if (!params->disabled) {
		ge_cairo_rounded_rectangle (cr, x + 1.0, y + 1.0, width - 2,
		                            height - 2, MAX(0, params->radius - 1.0),
		                            params->corners);
		cairo_clip (cr);
		cairo_move_to (cr, x, y + 1.5);
		cairo_line_to (cr, x + width, y + 1.5);
		top_color.a = 0.1;
		ge_cairo_set_color (cr, &top_color);
		cairo_stroke (cr);
	}

	cairo_restore (cr);
}

static void
clearlooks_gnome3_draw_button (cairo_t *cr,
                        const ClearlooksColors *colors,
                        const WidgetParameters *params,
                        int x, int y, int width, int height)
{
	CairoColor bg_color = colors->bg[params->state_type];
	CairoColor fg_color = colors->fg[params->state_type];
	CairoColor stroke_top_color, stroke_bottom_color;
	CairoColor fill_top_color, fill_bottom_color;
	CairoColor border_color;
	cairo_pattern_t *pattern;

	ge_mix_color (&bg_color, &fg_color, 0.6, &border_color);

	cairo_save (cr);

	if (params->disabled) {
		stroke_top_color = stroke_bottom_color = bg_color;
		fill_top_color = fill_bottom_color = bg_color;
	} else if (params->active) {
		ge_shade_color (&bg_color, 0.93, &stroke_top_color);
		ge_shade_color (&bg_color, 1.12, &stroke_bottom_color);

		ge_shade_color (&bg_color, 0.98, &fill_top_color);
		ge_shade_color (&bg_color, 1.09, &fill_bottom_color);
	} else {
		ge_shade_color (&bg_color, 1.09, &stroke_top_color);
		ge_shade_color (&bg_color, 0.95, &stroke_bottom_color);

		ge_shade_color (&bg_color, 1.04, &fill_top_color);
		ge_shade_color (&bg_color, 0.95, &fill_bottom_color);
	}

	/* Border around everything */	
	ge_cairo_inner_rounded_rectangle (cr, x, y, width, height, params->radius, params->corners);
	ge_cairo_set_color (cr, &border_color);
	cairo_stroke (cr);

	ge_cairo_inner_rounded_rectangle (cr, x + 1, y + 1, width - 2, height - 2, MAX(0, params->radius - 1), params->corners);
	pattern = cairo_pattern_create_linear (x, y + 2, x, y + height - 4);
	ge_cairo_pattern_add_color_stop_color (pattern, 0.0, &stroke_top_color);
	ge_cairo_pattern_add_color_stop_color (pattern, 1.0, &stroke_bottom_color);
	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);
	cairo_stroke (cr);

	ge_cairo_rounded_rectangle (cr, x + 2, y + 2, width - 4, height - 4, MAX(0, params->radius - 2), params->corners);
	pattern = cairo_pattern_create_linear (x, y + 3, x, y + height - 6);
	ge_cairo_pattern_add_color_stop_color (pattern, 0.0, &fill_top_color);
	ge_cairo_pattern_add_color_stop_color (pattern, 1.0, &fill_bottom_color);
	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);
	cairo_fill (cr);

	cairo_restore (cr);
}

static void
clearlooks_gnome3_draw_arrow (cairo_t *cr,
                       const ClearlooksColors *colors,
                       const WidgetParameters *widget,
                       const ArrowParameters  *arrow,
                       int x, int y, int width, int height)
{
	gdouble w = width, h = height;
	gdouble rotate;
	cairo_matrix_t matrix;

	switch (arrow->direction) {
	case CL_DIRECTION_LEFT:
		rotate = G_PI*1.5;
		break;
	case CL_DIRECTION_RIGHT:
		rotate = G_PI*0.5;
		break;
	case CL_DIRECTION_UP:
		rotate = G_PI;
		break;
	case CL_DIRECTION_DOWN:
		rotate = 0;
		break;
	default:
		return;
	}

	cairo_save (cr);

	cairo_translate (cr, x + width / 2.0, y + height / 2.0);
	cairo_rotate (cr, -rotate);
	cairo_get_matrix (cr, &matrix);
	cairo_matrix_transform_distance (&matrix, &w, &h);

	h = MAX(h / 2 - 2, w - 2);
	w = h * 2;
	cairo_move_to (cr, -w / 2 + 1, -h / 2 + 1);
	cairo_line_to (cr, 0, h / 2 - 1);
	cairo_line_to (cr, +w / 2 + 1, -h / 2 + 1);
	cairo_set_line_width (cr, 2.0);

	ge_cairo_set_color (cr, &colors->fg[widget->state_type]);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void
clearlooks_gnome3_draw_scrollbar_trough (cairo_t *cr,
                                         const ClearlooksColors           *colors,
                                         const WidgetParameters           *widget,
                                         const ScrollBarParameters        *scrollbar,
                                         int x, int y, int width, int height)
{
	const CairoColor *bg_color = &colors->bg[GTK_STATE_ACTIVE];
	cairo_save (cr);

	ge_cairo_set_color (cr, bg_color);
	cairo_rectangle (cr, x, y, width, height);
	cairo_fill (cr);

	cairo_restore (cr);
}

static void
clearlooks_gnome3_draw_scrollbar_button (cairo_t *cr,
                                         const ClearlooksColors           *colors,
                                         const WidgetParameters           *params,
                                         const ScrollBarParameters        *scrollbar,
                                         int x, int y,
                                         int width, int height)
{
	const CairoColor *bg_color = &colors->bg[params->state_type];
	const CairoColor *fg_color = &colors->fg[params->state_type];

	cairo_save (cr);
	ge_cairo_inner_rounded_rectangle (cr, x, y, width, height, params->radius, CR_CORNER_ALL);
	ge_cairo_set_color (cr, bg_color);
	cairo_fill_preserve (cr);
	ge_cairo_set_color (cr, fg_color);
	cairo_stroke (cr);
	cairo_restore (cr);
}

static void
clearlooks_gnome3_draw_scrollbar_stepper (cairo_t *cr,
                                          const ClearlooksColors           *colors,
                                          const WidgetParameters           *params,
                                          const ScrollBarParameters        *scrollbar,
                                          const ScrollBarStepperParameters *stepper,
                                          int x, int y, int width, int height)
{
	clearlooks_gnome3_draw_scrollbar_button (cr, colors, params, scrollbar, x + 1, y + 1, width - 2, height - 2);
}

static void
clearlooks_gnome3_draw_scrollbar_slider (cairo_t *cr,
                                         const ClearlooksColors          *colors,
                                         const WidgetParameters          *params,
                                         const ScrollBarParameters       *scrollbar,
                                         int x, int y, int width, int height)
{
	if (scrollbar->horizontal) {
		clearlooks_gnome3_draw_scrollbar_button (cr, colors, params, scrollbar, x, y + 1, width, height - 2);
	} else {
		clearlooks_gnome3_draw_scrollbar_button (cr, colors, params, scrollbar, x + 1, y, width - 2, height);
	}
}

void
clearlooks_register_style_gnome3 (ClearlooksStyleFunctions *functions, ClearlooksStyleConstants *constants)
{
	g_assert (functions);

	functions->draw_entry               = clearlooks_gnome3_draw_entry;
	functions->draw_button              = clearlooks_gnome3_draw_button;
	functions->draw_scrollbar_stepper   = clearlooks_gnome3_draw_scrollbar_stepper;
	functions->draw_scrollbar_slider    = clearlooks_gnome3_draw_scrollbar_slider;
	functions->draw_scrollbar_trough    = clearlooks_gnome3_draw_scrollbar_trough;
/*	functions->draw_arrow               = clearlooks_gnome3_draw_arrow;
	functions->draw_top_left_highlight  = clearlooks_draw_top_left_highlight;
	functions->draw_scale_trough        = clearlooks_draw_scale_trough;
	functions->draw_progressbar_trough  = clearlooks_draw_progressbar_trough;
	functions->draw_progressbar_fill    = clearlooks_draw_progressbar_fill;
	functions->draw_slider_button       = clearlooks_draw_slider_button;
	functions->draw_entry_progress      = clearlooks_draw_entry_progress;
	functions->draw_spinbutton          = clearlooks_draw_spinbutton;
	functions->draw_spinbutton_down     = clearlooks_draw_spinbutton_down;
	functions->draw_optionmenu          = clearlooks_draw_optionmenu;
	functions->draw_inset               = clearlooks_draw_inset;
	functions->draw_menubar	            = clearlooks_draw_menubar;
	functions->draw_tab                 = clearlooks_draw_tab;
	functions->draw_frame               = clearlooks_draw_frame;
	functions->draw_separator           = clearlooks_draw_separator;
	functions->draw_menu_item_separator = clearlooks_draw_menu_item_separator;
	functions->draw_list_view_header    = clearlooks_draw_list_view_header;
	functions->draw_toolbar             = clearlooks_draw_toolbar;
	functions->draw_menuitem            = clearlooks_draw_menuitem;
	functions->draw_menubaritem         = clearlooks_draw_menubaritem;
	functions->draw_selected_cell       = clearlooks_draw_selected_cell;
	functions->draw_statusbar           = clearlooks_draw_statusbar;
	functions->draw_menu_frame          = clearlooks_draw_menu_frame;
	functions->draw_tooltip             = clearlooks_draw_tooltip;
	functions->draw_icon_view_item      = clearlooks_draw_icon_view_item;
	functions->draw_handle              = clearlooks_draw_handle;
	functions->draw_resize_grip         = clearlooks_draw_resize_grip;
	functions->draw_focus               = clearlooks_draw_focus;
	functions->draw_checkbox            = clearlooks_draw_checkbox;
	functions->draw_radiobutton         = clearlooks_draw_radiobutton;
	functions->draw_shadow              = clearlooks_draw_shadow;
	functions->draw_slider              = clearlooks_draw_slider;
	functions->draw_gripdots            = clearlooks_draw_gripdots;*/
}

