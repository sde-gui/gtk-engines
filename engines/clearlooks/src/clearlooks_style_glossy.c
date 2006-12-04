/* Clearlooks theme engine
 * Copyright (C) 2006 Richard Stellingwerff
 * Copyright (C) 2006 Daniel Borgman
 * Copyright (C) 2006 Benjamin Berg
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
 * Written by Benjamin Berg <benjamin@sipsolutions.net>
 * Based on code by Richard Stellingwerff <remenic@gmail.com>
 *              and Daniel Borgmann <daniel.borgmann@gmail.com>
 * from the ubuntulooks engine.
 */

#include "clearlooks_draw.h"
#include "clearlooks_style.h"
#include "clearlooks_types.h"

#include "support.h"
#include <ge-support.h>

#include <cairo.h>


static void
clearlooks_draw_button_gloss (cairo_t *cr,
                              double x, double y, int width, int height,
                              const CairoColor *color, boolean disabled)
{
	CairoColor a, b, c, d, e;
	cairo_pattern_t *pt;

	ge_shade_color (color, disabled? 1.03 : 1.05, &a);
	ge_shade_color (color, disabled? 1.01 : 1.02, &b);
	ge_shade_color (color, disabled? 0.99 : 0.98, &c);
	ge_shade_color (color, 1.0, &d);
	ge_shade_color (color, disabled? 1.01 : 1.02, &e);

	pt = cairo_pattern_create_linear (x, y, x, y+height);
	cairo_pattern_add_color_stop_rgb (pt, 0.0,  a.r, a.g, a.b);
	cairo_pattern_add_color_stop_rgb (pt, 0.5,  b.r, b.g, b.b);
	cairo_pattern_add_color_stop_rgb (pt, 0.5,  c.r, c.g, c.b);
	cairo_pattern_add_color_stop_rgb (pt, 0.75, d.r, d.g, d.b);
	cairo_pattern_add_color_stop_rgb (pt, 0.85, e.r, e.g, e.b);
	cairo_pattern_add_color_stop_rgb (pt, 1.0,  d.r, d.g, d.b);

	cairo_set_source (cr, pt);
	cairo_rectangle (cr, x, y, width, height);
	cairo_fill (cr);
	
	cairo_pattern_destroy (pt);
}

static void
clearlooks_glossy_draw_button (cairo_t *cr,
                                   const ClearlooksColors *colors,
                                   const WidgetParameters *params,
                                   int x, int y, int width, int height)
{
	const float RADIUS = 3.0;
	
	double xoffset = 0, yoffset = 0;
	CairoColor fill                   = colors->bg[params->state_type];
	const CairoColor *border_normal   = &colors->shade[6];
	const CairoColor *border_disabled = &colors->shade[4];
	
	cairo_pattern_t *pattern;
	
	cairo_save (cr);
	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);

	/* Shadows and Glow */
	if (params->xthickness == 3 || params->ythickness == 3)
	{
		cairo_translate (cr, 0.5, 0.5);
		
		if (params->prelight && params->enable_glow)
		{
			const CairoColor *glow = &colors->spot[0];
			ge_cairo_rounded_rectangle (cr, 0, 0, width-1, height-1, RADIUS+1, params->corners);
			ge_cairo_set_color (cr, glow);
			cairo_stroke (cr);
		}
		
		/* Use inset as default for now - would be nice to make this an option */
		/* if (params->active || shadow->shadow == CL_SHADOW_IN) */
			params->style_functions->draw_inset (cr, width-1, height-1, RADIUS, params->corners);
		/* else
			clearlooks_draw_shadow (cr, width-1, height-1);
		*/
		cairo_translate (cr, -0.5, -0.5);
		
		if (params->xthickness == 3)
			xoffset = 1;
		if (params->ythickness == 3)
			yoffset = 1;
	}
	
	if (params->active && params->prelight)
	{
		fill = colors->bg[CL_STATE_ACTIVE];
		ge_shade_color (&fill, 1.05, &fill);
	}
	
	clearlooks_draw_button_gloss (cr, xoffset+1, yoffset+1, 
	                              width-(xoffset*2)-2, height-(yoffset*2)-2, 
	                              &fill, params->disabled);
	
	/* Pressed button shadow */
	if (params->active)
	{
		cairo_rectangle (cr, xoffset+1, yoffset+1, width-(xoffset*2)-2, 3);
	
		pattern = cairo_pattern_create_linear (xoffset+1, yoffset+1, xoffset+1, yoffset+4);
		cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0.0, 0.0, params->disabled ? 0.08 : 0.10);
		cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0.0, 0.0, 0.00);
		cairo_set_source (cr, pattern);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}
	
	/* Default button highlight */
	if (params->is_default && !params->active && !params->disabled)
	{
		const CairoColor *glow = &colors->spot[0];
		double hh = (height-5)/2.0 + 1;
		
		cairo_rectangle (cr, 3.5, 3.5, width-7, height-7);
		ge_cairo_set_color (cr, glow);
		cairo_stroke (cr);

		glow = &colors->spot[0];
		cairo_move_to (cr, 2.5, 2.5+hh); cairo_rel_line_to (cr, 0, -hh);
		cairo_rel_line_to (cr, width-5, 0); cairo_rel_line_to (cr, 0, hh);
		ge_cairo_set_color (cr, glow);
		cairo_stroke (cr);
		
		hh--;

		glow = &colors->spot[1];
		cairo_move_to (cr, 2.5, 2.5+hh); cairo_rel_line_to (cr, 0, hh);
		cairo_rel_line_to (cr, width-5, 0); cairo_rel_line_to (cr, 0, -hh);
		ge_cairo_set_color (cr, glow);
		cairo_stroke (cr);
	}
	
	/* Border */
	if (params->is_default || (params->prelight && params->enable_glow))
		border_normal = &colors->spot[2];	
	if (params->disabled)
		ge_cairo_set_color (cr, border_disabled);
	else
		ge_cairo_set_color (cr, border_normal);
	ge_cairo_rounded_rectangle (cr, xoffset + 0.5, yoffset + 0.5,
                                  width-(xoffset*2)-1, height-(yoffset*2)-1,
                                  RADIUS, params->corners);
	cairo_stroke (cr);
	cairo_restore (cr);
}

static void
clearlooks_glossy_draw_tab (cairo_t *cr,
                            const ClearlooksColors *colors,
                            const WidgetParameters *params,
                            const TabParameters    *tab,
                            int x, int y, int width, int height)
{
	const CairoColor *background;
	FrameParameters frame;
	GdkRectangle pos = { x, y, width, height };
	
	background = &colors->bg[params->state_type];
	
	cairo_save (cr);
	{
		CairoColor a, b, c, d, e;
		gint center;
		cairo_pattern_t *pt;

		/* change everything as if the gap side was at the bottom/top */
		if ((tab->gap_side == CL_GAP_LEFT) || (tab->gap_side == CL_GAP_RIGHT))
			ge_cairo_exchange_axis (cr, &x, &y, &width, &height);

		center = height / 2;
		/* draw the gloss ... */
		if ((tab->gap_side == CL_GAP_BOTTOM) || (tab->gap_side == CL_GAP_RIGHT))
		{
			ge_shade_color (background, 1.06, &a);
			ge_shade_color (background, 1.04, &b);
			ge_shade_color (background, 1.02, &c);
			ge_shade_color (background, 0.96, &d);
			ge_shade_color (background, 1.00, &e);

			pt = cairo_pattern_create_linear (x, y, x, y+height);
			cairo_pattern_add_color_stop_rgb (pt, 0.0,  a.r, a.g, a.b);
			cairo_pattern_add_color_stop_rgb (pt, 0.2,  b.r, b.g, b.b);
			cairo_pattern_add_color_stop_rgb (pt, (center-1)/((gfloat) height), c.r, c.g, c.b);
			cairo_pattern_add_color_stop_rgb (pt, (center)/((gfloat) height), d.r, d.g, d.b);
			cairo_pattern_add_color_stop_rgb (pt, 1.0,  e.r, e.g, e.b);

			cairo_set_source (cr, pt);
			ge_cairo_rounded_rectangle (cr, x, y, width, height, 3.0, CR_CORNER_TOPLEFT | CR_CORNER_TOPRIGHT);
			cairo_fill (cr);

			cairo_pattern_destroy (pt);
		}
		else
		{
			ge_shade_color (background, 1.00, &a);
			ge_shade_color (background, 1.04, &b);
			ge_shade_color (background, 1.06, &c);
			ge_shade_color (background, 0.96, &d);
			ge_shade_color (background, 0.99, &e);

			pt = cairo_pattern_create_linear (x, y, x, y+height);
			cairo_pattern_add_color_stop_rgb (pt, 0.0,  a.r, a.g, a.b);
			cairo_pattern_add_color_stop_rgb (pt, 0.2,  b.r, b.g, b.b);
			cairo_pattern_add_color_stop_rgb (pt, (center-1)/((gfloat) height), c.r, c.g, c.b);
			cairo_pattern_add_color_stop_rgb (pt, (center)/((gfloat) height), d.r, d.g, d.b);
			cairo_pattern_add_color_stop_rgb (pt, 1.0,  e.r, e.g, e.b);

			cairo_set_source (cr, pt);
			ge_cairo_rounded_rectangle (cr, x, y, width, height, 3.0, CR_CORNER_TOPLEFT | CR_CORNER_TOPRIGHT);
			cairo_fill (cr);

			cairo_pattern_destroy (pt);
		}
		
	}
	cairo_restore (cr);
	
	x = pos.x;
	y = pos.y;
	width = pos.width;
	height = pos.height;
	
	frame.shadow = CL_SHADOW_OUT;
	frame.gap_side = tab->gap_side;
	frame.gap_x = 0;
	frame.gap_width = width;
	frame.border = (CairoColor*) &colors->shade[6]; /* XXX */
	
	params->style_functions->draw_frame (cr, colors, params, &frame, x, y, width, height);
}

static void
clearlooks_glossy_draw_slider (cairo_t *cr,
                        const ClearlooksColors *colors,
                        const WidgetParameters *params,
                        int x, int y, int width, int height)
{
	CairoColor *border  = (CairoColor*)&colors->shade[8];
	CairoColor  fill;
	CairoColor  hilight;
	CairoColor  shade1, shade2, shade3, shade4;
	cairo_pattern_t *pattern;

	cairo_set_line_width (cr, 1.0);	
	cairo_translate      (cr, x, y);

	cairo_translate (cr, -0.5, -0.5);

	ge_shade_color (&colors->bg[GTK_STATE_NORMAL], 0.5, &fill);
	if (params->prelight)
		ge_shade_color (&fill, 1.1, &fill);

	ge_shade_color (&fill, 1.3, &hilight);
	ge_shade_color (&fill, 0.96, &shade1);
	ge_shade_color (&fill, 0.72, &shade2);
	ge_shade_color (&fill, 0.71, &shade3);
	ge_shade_color (&fill, 0.74, &shade4);

	pattern = cairo_pattern_create_linear (1, 1, 1, height-2);
	cairo_pattern_add_color_stop_rgb (pattern, 0,   shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.5, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0,   shade4.r, shade4.g, shade4.b);	
	cairo_pattern_add_color_stop_rgb (pattern, 0.5,   shade3.r, shade3.g, shade3.b);
	cairo_rectangle (cr, 1, 1, width-2, height-2);
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	ge_cairo_set_color (cr, border);
	ge_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, 2.5, params->corners);
	cairo_stroke (cr);

	cairo_set_source_rgba (cr, hilight.r, hilight.g, hilight.b, 0.5);
	ge_cairo_rounded_rectangle (cr, 1.5, 1.5, width-3, height-3, 2.0, params->corners);
	cairo_stroke (cr);
}

static void
clearlooks_glossy_draw_slider_button (cairo_t *cr,
                                      const ClearlooksColors *colors,
                                      const WidgetParameters *params,
                                      const SliderParameters *slider,
                                      int x, int y, int width, int height)
{
	cairo_set_line_width (cr, 1.0);
	
	if (!slider->horizontal)
		ge_cairo_exchange_axis (cr, &x, &y, &width, &height);

	cairo_translate (cr, x+0.5, y+0.5);
	
	params->style_functions->draw_shadow (cr, width-1, height-1);
	params->style_functions->draw_slider (cr, colors, params, 1, 1, width-2, height-2);
}

static void
clearlooks_glossy_draw_checkbox (cairo_t *cr,
                                 const ClearlooksColors  *colors,
                                 const WidgetParameters  *widget,
                                 const CheckboxParameters *checkbox,
                                 int x, int y, int width, int height)
{
	CairoColor border, top, bottom;
	cairo_pattern_t *pattern;
	gboolean draw_bullet = (checkbox->shadow_type == GTK_SHADOW_IN);

	/* sanitize size to be rectangular */
	if (width > height) {
		x += (width - height) / 2;
		width = height;
	} else if (height > width) {
		y += (height - width) / 2;
		height = width;
	}
	
	cairo_set_line_width (cr, 1.0);
	
	if (!checkbox->in_menu)
	{
		if ( (checkbox->shadow_type == CL_SHADOW_IN || 
		      checkbox->shadow_type == CL_SHADOW_ETCHED_IN || 
		      widget->active) && !widget->disabled && !checkbox->in_cell )
		{
			border = top = bottom = colors->spot[1];
			
			if (widget->prelight)
			{
				ge_shade_color (&top, 1.1, &top);
				ge_shade_color (&border, 0.8, &border);
			}
			
			ge_shade_color (&top, 1.2, &top);
			ge_shade_color (&bottom, 0.9, &bottom);
			ge_shade_color (&border, 0.6, &border);
		}
		else if (widget->disabled)
		{
			if (checkbox->in_cell) {
				border = top = bottom = colors->shade[3];
			} else {
				border = top = bottom = colors->shade[3];
			}
		}	
		else
		{
			top = bottom = colors->base[widget->state_type];
			
			if (widget->prelight)
			{
				ge_shade_color (&top, 1.2, &top);
				border = colors->spot[2];
			}
			else
			{
				border = colors->base[widget->state_type];
				ge_shade_color (&border, 0.4, &border);
			}
			
			ge_shade_color (&top, 0.98, &top);
			ge_shade_color (&bottom, 0.85, &bottom);
		}
		
		if (widget->state_type != GTK_STATE_INSENSITIVE)
		{
			// glow
			if ((widget->prelight || (widget->active && !draw_bullet)) && !checkbox->in_cell) 
			{
				const CairoColor *glow = &colors->spot[0];
				ge_cairo_rounded_rectangle (cr, x+1.5, y+1.5, width - 1, height - 1, 2, widget->corners);
				cairo_set_source_rgba (cr, glow->r, glow->g, glow->b, 0.5);
				cairo_stroke (cr);
			}
			
			// shadow
			if (checkbox->in_cell) {
				ge_cairo_rounded_rectangle (cr, x+0.5, y+0.5, width, height, 1, widget->corners);
			} else {
				ge_cairo_rounded_rectangle (cr, x+3.5, y+3.5, width - 3, height - 3, 1, widget->corners);
			}
			cairo_set_source_rgba (cr, 0., 0., 0., 0.2);
			cairo_stroke (cr);
		
			if (checkbox->in_cell) {
				ge_cairo_rounded_rectangle (cr, x+0.5, y+0.5, width, height, 1, widget->corners);
			} else {
				ge_cairo_rounded_rectangle (cr, x+2.5, y+2.5, width - 3, height - 3, 1, widget->corners);
			}
			pattern = cairo_pattern_create_linear (x, y, x+width, y+height);
			cairo_pattern_add_color_stop_rgb (pattern, 0.0, 1.0, 1.0, 1.0);
			cairo_pattern_add_color_stop_rgb (pattern, 0.4, top.r, top.g, top.b);
			cairo_pattern_add_color_stop_rgb (pattern, 1.0, bottom.r, bottom.g, bottom.b);
			cairo_set_source (cr, pattern);
			cairo_fill (cr);
			cairo_pattern_destroy (pattern);
		}
		
		if (checkbox->in_cell) {
			ge_cairo_rounded_rectangle (cr, x+0.5, y+0.5, width, height, 1, widget->corners);
		} else {
			ge_cairo_rounded_rectangle (cr, x+2.5, y+2.5, width - 3, height - 3, 1, widget->corners);
		}
		
		ge_cairo_set_color (cr, &border);
		cairo_stroke (cr);
		
		if (checkbox->in_cell) {
			ge_cairo_rounded_rectangle (cr, x+1.5, y+1.5, width - 2, height - 2, 1, widget->corners);
		} else {
			ge_cairo_rounded_rectangle (cr, x+3.5, y+3.5, width - 5, height - 5, 1, widget->corners);
		}
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.4);
		cairo_stroke (cr);
		
	}
	
	cairo_translate (cr, x, y);
	cairo_scale (cr, width / 13.0, height / 13.0);
	/* To offset this checkmark, one should probably use a translation */
	if (draw_bullet)
	{
		if (checkbox->in_cell)
		{
			cairo_translate (cr, -2, -2);
		}
		else if (checkbox->in_menu)
		{
			cairo_translate (cr, -2, -2);
		}
	
		cairo_move_to (cr, 4, 8);
		cairo_rel_line_to (cr,   5,   4);
		cairo_rel_curve_to (cr,  1.4,  -5,   -1,  -1,   5.7,  -12.5);
		cairo_rel_curve_to (cr, -4,   4,  -4,   4,  -6.7,    9.3);
		cairo_rel_line_to (cr,  -2.3,  -2.5);
		
		
		ge_cairo_set_color (cr, &colors->text[widget->state_type]);
		cairo_fill (cr);
	}
	else if (checkbox->shadow_type == GTK_SHADOW_ETCHED_IN)
	{
		cairo_rectangle (cr, 4.0, 6.0, 6, 2);
		ge_cairo_set_color (cr, &colors->text[widget->state_type]);
		cairo_fill(cr);
	}
}

static void
clearlooks_glossy_draw_radiobutton (cairo_t *cr,
                                    const ClearlooksColors  *colors,
                                    const WidgetParameters  *widget,
                                    const CheckboxParameters *checkbox,
                                    int x, int y, int width, int height)
{
	CairoColor border, top, bottom;
	cairo_pattern_t *pattern;
	gboolean draw_bullet = (checkbox->shadow_type == CL_SHADOW_IN);
	/* sanitize size to be rectangular */
	if (width > height) {
		x += (width - height) / 2;
		width = height;
	} else if (height > width) {
		y += (height - width) / 2;
		height = width;
	}

	cairo_set_line_width (cr, 1.0);
	
	if (checkbox->in_menu)
	{
		cairo_translate (cr, -2, -1);
	}

	if (!checkbox->in_menu)
	{
		if ( (checkbox->shadow_type == CL_SHADOW_IN || 
		      checkbox->shadow_type == CL_SHADOW_ETCHED_IN || 
		      widget->active) && !widget->disabled )
		{
			border = top = bottom = colors->spot[1];
			
			if (widget->prelight)
			{
				ge_shade_color (&top, 1.1, &top);
				ge_shade_color (&border, 0.8, &border);
			}
			
			ge_shade_color (&top, 1.2, &top);
			ge_shade_color (&bottom, 0.9, &bottom);
			ge_shade_color (&border, 0.6, &border);
		}
		else if (widget->disabled)
		{
			border = top = bottom = colors->shade[3];
		}
		else
		{		
			top = bottom = colors->base[widget->state_type];
			
			if (widget->prelight)
			{
				ge_shade_color (&top, 1.2, &top);
				border = colors->spot[2];
				
			}
			else
			{
				border = colors->base[widget->state_type];
				ge_shade_color(&border, 0.4, &border);
			}
			
			ge_shade_color (&top, 0.98, &top);
			ge_shade_color (&bottom, 0.85, &bottom);
		}
		
		if ( widget->state_type != GTK_STATE_INSENSITIVE )
		{
			// glow
			if (widget->prelight || (widget->active && !draw_bullet)) 
			{
				const CairoColor *glow = &colors->spot[0];
				cairo_arc (cr, x+width/2.0 + 0.5, y+height/2.0 + 0.5, width/2.0, 0, 2 * M_PI);
				cairo_set_source_rgba (cr, glow->r, glow->g, glow->b, 1.0);
				cairo_stroke (cr);
			}
		
			// shadow
			cairo_arc (cr, x+width/2.0+1, y+height/2.0+1, width/2.0 - 1.0, 0, 2 * M_PI);
			cairo_set_source_rgba (cr, 0., 0., 0., 0.2);
			cairo_stroke (cr);
			
			cairo_arc (cr, x+width/2., y+height/2.0, width/2.0 - 1.0, 0, 2 * M_PI);	
			pattern = cairo_pattern_create_linear (x, y, x+width, y+height);
			cairo_pattern_add_color_stop_rgb (pattern, 0.0, 1.0, 1.0, 1.0);
			cairo_pattern_add_color_stop_rgb (pattern, 0.4, top.r, top.g, top.b);
			cairo_pattern_add_color_stop_rgb (pattern, 1.0, bottom.r, bottom.g, bottom.b);
			cairo_set_source (cr, pattern);
			cairo_fill(cr);
			cairo_pattern_destroy (pattern);
		}
		cairo_translate (cr, 0.5, 0.5);
		cairo_arc (cr, x+width/2.0, y+height/2.0, width/2.0 - 1.0, 0, 2 * M_PI);
		
		ge_cairo_set_color (cr, &border);
		cairo_stroke (cr);
		
		cairo_arc (cr, x+width/2., y+height/2.0, width/2.0 - 2.0, 0, 2 * M_PI);
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.4);
		cairo_stroke (cr);
	}
	
	// draw the bullet
	cairo_translate (cr, x, y);
	cairo_scale (cr, width / 13.0, height / 13.0);
	if (draw_bullet)
	{
		cairo_arc (cr, 6.5, 6.5, 2.5, 0, 2 * M_PI);
		if (widget->disabled)
		{
			ge_cairo_set_color (cr, &colors->text[widget->state_type]);
		}
		else
		{
			pattern = cairo_pattern_create_radial (5.25, 5.25, 0.1, 5.25, 5.25, 4);
			cairo_pattern_add_color_stop_rgb (pattern, 0.0, 0.4, 0.4, 0.4);
			cairo_pattern_add_color_stop_rgb (pattern, 1.0, colors->text[widget->state_type].r,
			                                                colors->text[widget->state_type].g,
			                                                colors->text[widget->state_type].b);
			cairo_set_source (cr, pattern);
			cairo_pattern_destroy (pattern);
		}
		cairo_fill_preserve (cr);
	}
}


void
clearlooks_register_style_glossy (ClearlooksStyleFunctions *functions)
{
	functions->draw_button        = clearlooks_glossy_draw_button;
	functions->draw_tab           = clearlooks_glossy_draw_tab;
	functions->draw_slider        = clearlooks_glossy_draw_slider;
	functions->draw_slider_button = clearlooks_glossy_draw_slider_button;
	functions->draw_checkbox      = clearlooks_glossy_draw_checkbox;
	functions->draw_radiobutton   = clearlooks_glossy_draw_radiobutton;
}

