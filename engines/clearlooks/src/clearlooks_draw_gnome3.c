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

	if (params->focus) {
		fg_color = &colors->base[GTK_STATE_SELECTED];
	}

	cairo_set_line_width (cr, 1.0);
	ge_cairo_rounded_rectangle (cr, x + 0.5, y + 0.5, width - 1,
	                            height - 1, params->radius,
	                            params->corners);
	ge_cairo_set_color (cr, bg_color);
	cairo_fill_preserve (cr);
	ge_cairo_set_color (cr, fg_color);
	cairo_stroke (cr);
}

void
clearlooks_register_style_gnome3 (ClearlooksStyleFunctions *functions, ClearlooksStyleConstants *constants)
{
	g_assert (functions);

/*	functions->draw_top_left_highlight  = clearlooks_draw_top_left_highlight;
	functions->draw_button              = clearlooks_draw_button;
	functions->draw_scale_trough        = clearlooks_draw_scale_trough;
	functions->draw_progressbar_trough  = clearlooks_draw_progressbar_trough;
	functions->draw_progressbar_fill    = clearlooks_draw_progressbar_fill;
	functions->draw_slider_button       = clearlooks_draw_slider_button;*/
	functions->draw_entry               = clearlooks_gnome3_draw_entry;
/*	functions->draw_entry_progress      = clearlooks_draw_entry_progress;
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
	functions->draw_scrollbar_stepper   = clearlooks_draw_scrollbar_stepper;
	functions->draw_scrollbar_slider    = clearlooks_draw_scrollbar_slider;
	functions->draw_scrollbar_trough    = clearlooks_draw_scrollbar_trough;
	functions->draw_statusbar           = clearlooks_draw_statusbar;
	functions->draw_menu_frame          = clearlooks_draw_menu_frame;
	functions->draw_tooltip             = clearlooks_draw_tooltip;
	functions->draw_icon_view_item      = clearlooks_draw_icon_view_item;
	functions->draw_handle              = clearlooks_draw_handle;
	functions->draw_resize_grip         = clearlooks_draw_resize_grip;
	functions->draw_arrow               = clearlooks_draw_arrow;
	functions->draw_focus               = clearlooks_draw_focus;
	functions->draw_checkbox            = clearlooks_draw_checkbox;
	functions->draw_radiobutton         = clearlooks_draw_radiobutton;
	functions->draw_shadow              = clearlooks_draw_shadow;
	functions->draw_slider              = clearlooks_draw_slider;
	functions->draw_gripdots            = clearlooks_draw_gripdots;*/
}

