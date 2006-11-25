#include "clearlooks_draw.h"
#include "clearlooks_style.h"
#include "clearlooks_types.h"

#include "support.h"
#include <ge-support.h>

#include <cairo.h>

typedef void (*menubar_draw_proto) (cairo_t *cr,
                                    const ClearlooksColors *colors,
                                    const WidgetParameters *params,
                                    const MenuBarParameters *menubar,
                                    int x, int y, int width, int height);

static void
clearlooks_draw_inset (cairo_t *cr, int width, int height,
                       double radius, uint8 corners)
{
	double top_x1 = 0, top_x2 = width, bot_x1 = 0, bot_x2 = width;
	
	if (corners & CR_CORNER_TOPLEFT)
		top_x1 = radius-1;
	
	if (corners & CR_CORNER_TOPRIGHT)
		top_x2 = width-radius+1;
	
	if (corners & CR_CORNER_BOTTOMLEFT)
		bot_x1 = radius-1;
	
	if (corners & CR_CORNER_BOTTOMRIGHT)
		bot_x2 = width-radius+1;
	
#ifdef CAIRO_STROKE_IS_FAST
	cairo_set_line_width (cr, 1);
	cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.04);
	cairo_move_to (cr, top_x1, 0.0);
	cairo_line_to (cr, top_x2, 0.0);
	cairo_stroke (cr);
	
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.5);
	cairo_move_to (cr, bot_x1, height);
	cairo_line_to (cr, bot_x2, height);
	cairo_stroke (cr);
#else
	cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.04);
	cairo_rectangle (cr, top_x1 - 0.5, -0.5, top_x2 - top_x1 + 1, 1.0);
	cairo_fill (cr);
	
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.5);
	cairo_rectangle (cr, bot_x1 - 0.5, height - 0.5, bot_x2 - bot_x1 + 1, 1.0);
	cairo_fill (cr);
#endif
}

static void
clearlooks_draw_shadow (cairo_t *cr, int width, int height)
{
	cairo_set_line_width (cr, 1.0);
	
	cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.1);
	
	cairo_move_to (cr, width, 3.0);
	cairo_arc (cr, width-4.0, height-4.0, 4.0, 0, G_PI/2);
	cairo_line_to (cr, 3.0, height);

	cairo_stroke (cr);
}

static void
clearlooks_draw_top_left_highlight (cairo_t *cr,
                                    const WidgetParameters *params,
                                    int width, int height, gdouble radius)
{
	double light_y1 = params->ythickness-1,
	       light_y2 = height - params->ythickness - 1,
	       light_x1 = params->xthickness-1,
	       light_x2 = width - params->xthickness - 1;

	cairo_move_to         (cr, light_x1, light_y2);
	
	if (params->corners & CR_CORNER_TOPLEFT)
		cairo_arc         (cr, light_x1+radius, light_y1+radius, radius, G_PI, 270*(G_PI/180));
	else
		cairo_line_to     (cr, light_x1, light_y1);
	
	cairo_line_to         (cr, light_x2, light_y1);
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.6);
	cairo_stroke          (cr);
}

#ifdef DEVELOPMENT
#warning seems to be very slow in scrollbar_stepper
#endif

static void
clearlooks_draw_highlight_and_shade (cairo_t *cr,
                                     const ShadowParameters *params,
                                     int width, int height, gdouble radius)
{
	uint8 corners = params->corners;
	double x = 1.0;
	double y = 1.0;
	
	width  -= 3;
	height -= 3;
	
	cairo_save (cr);
	
	/* Draw top/left */
	if (corners & CR_CORNER_BOTTOMLEFT)
		cairo_move_to (cr, x, y+height-radius);
	else
		cairo_move_to (cr, x, y+height);
	
	if (corners & CR_CORNER_TOPLEFT)
		cairo_arc (cr, x+radius, y+radius, radius, G_PI, 270*(G_PI/180));
	else
		cairo_line_to (cr, x, y);
	
	if (corners & CR_CORNER_TOPRIGHT)
		cairo_line_to (cr, x+width-radius, y);
	else
		cairo_line_to (cr, x+width, y);
	
	if (params->shadow & CL_SHADOW_OUT)
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.5);
	else
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.05);
		
	cairo_stroke (cr);
	
	/* Drop bottom/right */
	
	if (corners & CR_CORNER_TOPRIGHT)
	{
		cairo_move_to (cr, x+width-radius, y);
		cairo_arc (cr, x+width-radius, y+radius, radius, G_PI + G_PI*0.5, G_PI*2);
	}
	else
		cairo_move_to (cr, x+width, y);
	
	if (corners & CR_CORNER_BOTTOMRIGHT)
		cairo_arc (cr, x+width-radius, y+height-radius, radius, 0, G_PI/2);
	else
		cairo_line_to (cr, x+width, y+height);
	
	if (corners & CR_CORNER_BOTTOMLEFT)
		cairo_arc (cr, x+radius, y+height-radius, radius, G_PI/2, G_PI);
	else
		cairo_line_to (cr, x, y+height);
	
	if (params->shadow & CL_SHADOW_OUT)
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.05);
	else
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.5);
	
	cairo_stroke (cr);
	
	cairo_restore (cr);
}

static void
clearlooks_draw_gripdots (cairo_t *cr, int x, int y,
                          int width, int height, int xr, int yr,
                          float contrast)
{
	int i, j;
	int xoff, yoff;

	for ( i = 0; i < xr; i++ ) {
		for ( j = 0; j < yr; j++ ) {
			xoff = x -(xr * 3 / 2) + 3 * i;
			yoff = y -(yr * 3 / 2) + 3 * j; 
			
			cairo_rectangle (cr, width/2+0.5+xoff, height/2+0.5+yoff, 2, 2);
			cairo_set_source_rgba (cr, 1., 1., 1., 0.8+contrast);
			cairo_fill (cr);
			cairo_rectangle (cr, width/2+0.5+xoff, height/2+0.5+yoff, 1, 1);
			cairo_set_source_rgba (cr, 0, 0, 0, 0.2+contrast);
			cairo_fill (cr);
		}
	}
}

static void
clearlooks_draw_button (cairo_t *cr,
                        const ClearlooksColors *colors,
                        const WidgetParameters *params,
                        int x, int y, int width, int height)
{
	const float RADIUS = 3.0;
	
	double xoffset = 0, yoffset = 0;
	const CairoColor *fill            = &colors->bg[params->state_type];
	const CairoColor *border_normal   = &colors->shade[7];
	const CairoColor *border_disabled = &colors->shade[4];
	
	/* Set colors and stuff */
	/* if (params->focus && !params->active)
		border_normal = &colors->spot[2]; */
	
	cairo_save (cr);
	
	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);

	if (params->xthickness == 3 || params->ythickness == 3)
	{
		cairo_translate (cr, 0.5, 0.5);
		params->style_functions->draw_inset (cr, width-1, height-1, RADIUS, params->corners);
		cairo_translate (cr, -0.5, -0.5);
		
		if (params->xthickness == 3)
			xoffset = 1;
		if (params->ythickness == 3)
			yoffset = 1;
	}
	
	ge_cairo_rounded_rectangle (cr, xoffset+1, yoffset+1,
	                                     width-(xoffset*2)-2,
	                                     height-(yoffset*2)-2,
	                                     3.0, params->corners);
	
	if (!params->active)
	{
		cairo_pattern_t *pattern;
		gdouble shade_size = ((100.0/height)*6.0)/100.0; /* 6 pixels */
		CairoColor top_shade, bottom_shade, middle_shade;
		
		ge_shade_color (fill, 1.6, &top_shade);
		ge_shade_color (fill, 0.80, &bottom_shade);
		ge_shade_color (fill, 0.96, &middle_shade);
		
		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.0, top_shade.r, top_shade.g, top_shade.b);
		cairo_pattern_add_color_stop_rgb (pattern, shade_size, fill->r, fill->g, fill->b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0-shade_size, middle_shade.r, middle_shade.g, middle_shade.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0, bottom_shade.r, bottom_shade.g, bottom_shade.b);
/*
		ge_shade_color (fill, 1.6, &top_shade); 
		ge_shade_color (fill, 0.75, &bottom_shade);
		cairo_pattern_add_color_stop_rgb (pattern, 0.0, top_shade.r, top_shade.g, top_shade.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0, bottom_shade.r, bottom_shade.g, bottom_shade.b);
*/
		cairo_set_source (cr, pattern);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}
	else
	{
		cairo_pattern_t *pattern;
		
		ge_cairo_set_color (cr, fill);
		cairo_fill_preserve (cr);

		pattern	= cairo_pattern_create_linear (0, yoffset+1, 0, 3+yoffset);
		cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0.0, 0.0, params->disabled ? 0.08 : 0.10);
		cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0.0, 0.0, 0.0);
		cairo_set_source (cr, pattern);
		cairo_fill_preserve (cr);
		cairo_pattern_destroy (pattern);

		pattern	= cairo_pattern_create_linear (xoffset+1, 0, 3+xoffset, 0);
		cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0.0, 0.0, params->disabled ? 0.08 : 0.10);
		cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0.0, 0.0, 0.0);
		cairo_set_source (cr, pattern);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}


	/* Drawing the border */

	if (!params->active && params->is_default)
	{
		const CairoColor *l = &colors->shade[4];
		const CairoColor *d = &colors->shade[4];
		ge_cairo_set_color (cr, l);
		ge_cairo_stroke_rectangle (cr, 2.5, 2.5, width-5, height-5);

		ge_cairo_set_color (cr, d);
		ge_cairo_stroke_rectangle (cr, 3.5, 3.5, width-7, height-7);
	}
	
	if (params->disabled)
		ge_cairo_set_color (cr, border_disabled);
	else
		ge_cairo_set_color (cr, border_normal);
	
	ge_cairo_rounded_rectangle (cr, xoffset + 0.5, yoffset + 0.5,
                                  width-(xoffset*2)-1, height-(yoffset*2)-1,
                                  3.0, params->corners);
	cairo_stroke (cr);
	
	/* Draw the "shadow" */
	if (!params->active)
	{
		cairo_translate (cr, 0.5, 0.5);
		/* Draw right shadow */
		cairo_move_to (cr, width-params->xthickness, params->ythickness - 1);
		cairo_line_to (cr, width-params->xthickness, height - params->ythickness - 1);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.05);
		cairo_stroke (cr);
		
		/* Draw topleft shadow */
		clearlooks_draw_top_left_highlight (cr, params, width, height, RADIUS);
	}
	cairo_restore (cr);
}

static void
clearlooks_draw_entry (cairo_t *cr,
                       const ClearlooksColors *colors,
                       const WidgetParameters *params,
                       int x, int y, int width, int height)
{
	const float RADIUS = 3.0;
	
	CairoColor *base = (CairoColor*)&colors->base[params->state_type];
	CairoColor *border;
	
	if (params->focus)
		border = (CairoColor*)&colors->spot[2];
	else
		border = (CairoColor*)&colors->shade[params->disabled ? 4 : 6];

	cairo_translate (cr, x+0.5, y+0.5);
	cairo_set_line_width (cr, 1.0);
	
	/* Fill the background (shouldn't have to) */
	cairo_rectangle (cr, -0.5, -0.5, width, height);
	ge_cairo_set_color (cr, &params->parentbg);
	cairo_fill (cr);

	/* Fill the entry's base color (why isn't is large enough by default?) */
	cairo_rectangle (cr, 1.5, 1.5, width-4, height-4);
	ge_cairo_set_color (cr, base);
	cairo_fill (cr);
	
	params->style_functions->draw_inset (cr, width-1, height-1, 2.0, params->corners);

	/* Draw the inner shadow */
	if (params->focus)
	{
		/* ge_cairo_rounded_rectangle (cr, 2, 2, width-5, height-5, RADIUS-1, params->corners); */
		ge_cairo_set_color (cr, &colors->spot[0]);
		ge_cairo_stroke_rectangle (cr, 2, 2, width-5, height-5);
	}
	else
	{
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, params->disabled ? 0.05 : 0.1);
		/*
		cairo_move_to (cr, 2, height-3);
		cairo_arc (cr, params->xthickness+RADIUS-1, params->ythickness+RADIUS-1, RADIUS, G_PI, 270*(G_PI/180));
		cairo_line_to (cr, width-3, 2);*/
		cairo_move_to (cr, 2, height-3);
		cairo_line_to (cr, 2, 2);
		cairo_line_to (cr, width-3, 2);
		cairo_stroke (cr);
	}

	/* Draw the border */
	ge_cairo_set_color (cr, border);
	ge_cairo_rounded_rectangle (cr, 1, 1, width-3, height-3, RADIUS, params->corners);
	cairo_stroke (cr);
}

static void
clearlooks_draw_spinbutton (cairo_t *cr,
                            const ClearlooksColors *colors,
                            const WidgetParameters *params,
                            int x, int y, int width, int height)
{
	params->style_functions->draw_button (cr, colors, params, x, y, width, height);

	cairo_translate (cr, x, y);
	/* Seperator */
	cairo_move_to (cr, params->xthickness + 0.5,       (height/2) + 0.5);
	cairo_line_to (cr, width-params->xthickness - 0.5, (height/2) + 0.5);
	cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.3);
	cairo_stroke (cr);

	cairo_move_to (cr, params->xthickness + 0.5,       (height/2)+1.5);
	cairo_line_to (cr, width-params->xthickness - 0.5, (height/2)+1.5);
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.7);
	cairo_stroke (cr);
}

static void
clearlooks_draw_spinbutton_down (cairo_t *cr,
                                 const ClearlooksColors *colors,
                                 const WidgetParameters *params,
                                 int x, int y, int width, int height)
{
	cairo_pattern_t *pattern;
	
	cairo_translate (cr, x+1, y+1);
	
	ge_cairo_rounded_rectangle (cr, 1, 1, width-4, height-4, 3, params->corners);
	
	ge_cairo_set_color (cr, &colors->bg[params->state_type]);
	
	cairo_fill_preserve (cr);
	
	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0.0, 0.0, 0.3);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0.0, 0.0, 0.0, 0.0);
	
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	
	cairo_pattern_destroy (pattern);
}

static void
clearlooks_scale_draw_gradient (cairo_t *cr,
                                const CairoColor *c1,
                                const CairoColor *c2,
                                const CairoColor *c3,
                                int x, int y, int width, int height,
                                boolean horizontal)
{
	cairo_pattern_t *pattern;

	pattern = cairo_pattern_create_linear (0, 0, horizontal ? 0 :  width, horizontal ? height : 0);
	cairo_pattern_add_color_stop_rgb (pattern, 0.0, c1->r, c1->g, c1->b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, c2->r, c2->g, c2->b);

	cairo_rectangle (cr, x+0.5, y+0.5, width-1, height-1);	
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);
	
	ge_cairo_set_color (cr, c3);
	ge_cairo_stroke_rectangle (cr, x, y, width, height);	
}

#define TROUGH_SIZE 6
static void
clearlooks_draw_scale_trough (cairo_t *cr,
                              const ClearlooksColors *colors,
                              const WidgetParameters *params,
                              const SliderParameters *slider,
                              int x, int y, int width, int height)
{
	int     trough_width, trough_height;
	double  translate_x, translate_y;

	if (slider->horizontal)
	{
		trough_width  = width-3;
		trough_height = TROUGH_SIZE-2;
		
		translate_x   = x + 0.5;
		translate_y   = y + 0.5 + (height/2) - (TROUGH_SIZE/2);
	}
	else
	{
		trough_width  = TROUGH_SIZE-2;
		trough_height = height-3;
		
		translate_x   = x + 0.5 + (width/2) - (TROUGH_SIZE/2);
		translate_y  = y + 0.5;
	}

	cairo_set_line_width (cr, 1.0);
	cairo_translate (cr, translate_x, translate_y);
	
	params->style_functions->draw_inset (cr, trough_width+2, trough_height+2, 0, 0);
	
	cairo_translate (cr, 1, 1);
	
	if (!slider->lower)
		clearlooks_scale_draw_gradient (cr, &colors->shade[3], /* top */
		                                    &colors->shade[2], /* bottom */
		                                    &colors->shade[6], /* border */
		                                    0, 0, trough_width, trough_height,
		                                    slider->horizontal);
	else
		clearlooks_scale_draw_gradient (cr, &colors->spot[1], /* top    */
		                                    &colors->spot[0], /* bottom */
		                                    &colors->spot[2], /* border */
		                                    0, 0, trough_width, trough_height,
		                                    slider->horizontal);
}

static void
clearlooks_draw_slider (cairo_t *cr,
                        const ClearlooksColors *colors,
                        const WidgetParameters *params,
                        int x, int y, int width, int height)
{
	const CairoColor	*border = &colors->shade[params->disabled ? 4 : 6];
	const CairoColor	*spot = &colors->spot[1];
	const CairoColor        *fill = &colors->shade[2];

	cairo_pattern_t *pattern;

	cairo_set_line_width (cr, 1.0);	
	cairo_translate      (cr, x, y);

	if (params->disabled)
		border = &colors->shade[4];
	else if (params->prelight)
		border = &colors->spot[2];
	else
		border = &colors->shade[6];

	/* fill the widget */
	cairo_rectangle (cr, 0.5, 0.5, width-2, height-2);

	/* Fake light */
	if (!params->disabled)
	{
		const CairoColor *top = &colors->shade[0];
		const CairoColor *bot = &colors->shade[2];

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.0,  top->r, top->g, top->b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0,  bot->r, bot->g, bot->b);
		cairo_set_source (cr, pattern);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}
	else
	{
		ge_cairo_set_color (cr, fill);
		cairo_rectangle    (cr, 0.5, 0.5, width-2, height-2);
		cairo_fill         (cr);
	}

	/* Set the clip */
	cairo_save (cr);
	cairo_rectangle (cr, 0.5, 0.5, 6, height-2);
	cairo_rectangle (cr, width-7.5, 0.5, 6 , height-2);
	cairo_clip_preserve (cr);

	cairo_new_path (cr);

	/* Draw the handles */
	ge_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, 3.0, params->corners);
	pattern = cairo_pattern_create_linear (0.5, 0.5, 0.5, 0.5+height);

	if ( params->prelight )
	{
		CairoColor highlight;
		ge_shade_color (spot, 1.5, &highlight);	
		cairo_pattern_add_color_stop_rgb (pattern, 0.0, highlight.r, highlight.g, highlight.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0, spot->r, spot->g, spot->b);
		cairo_set_source (cr, pattern);
	}
	else
		cairo_set_source_rgba (cr, 1, 1, 1, 0.5);

	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	cairo_restore (cr);

	/* Draw the border */
	ge_cairo_rounded_rectangle (cr, 0, 0, width-1, height-1, 3.0, params->corners);
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	/* Draw handle lines */
	if (width > 14)
	{
		cairo_move_to (cr, 6, 0.5);
		cairo_line_to (cr, 6, height-0.5);
	
		cairo_move_to (cr, width-7, 0.5);
		cairo_line_to (cr, width-7, height-0.5);
	
		cairo_set_line_width (cr, 1.0);
		cairo_set_source_rgba (cr, border->r,
		                           border->g,
		                           border->b,
	                                   0.3);
		cairo_stroke (cr);
	}
}

static void
clearlooks_draw_slider_button (cairo_t *cr,
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

	if (width > 24)
		params->style_functions->draw_gripdots (cr, 0, 0, width-2, height-2, 3, 3, 0);
}

static void
clearlooks_draw_progressbar_trough (cairo_t *cr,
                                    const ClearlooksColors *colors,
                                    const WidgetParameters *params,
                                    int x, int y, int width, int height)
{
	CairoColor      *border = (CairoColor*)&colors->shade[7];
	cairo_pattern_t *pattern;
	
	cairo_set_line_width (cr, 1.0);
	
	/* Fill with bg color */
	ge_cairo_set_color (cr, &colors->bg[params->state_type]);
	
	cairo_rectangle (cr, x, y, width, height);	
	cairo_fill (cr);

	/* Create trough box */
	cairo_rectangle (cr, x+1, y+1, width-2, height-2);
	ge_cairo_set_color (cr, &colors->shade[3]);
	cairo_fill (cr);

	/* Draw border */
	ge_cairo_rounded_rectangle (cr, x+0.5, y+0.5, width-1, height-1, 1.5, params->corners);
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);
	
	/* Top shadow */
	cairo_rectangle (cr, x+1, y+1, width-2, 4);
	pattern = cairo_pattern_create_linear (x, y, x, y+4);
	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0., 0., 0., 0.1);	
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0., 0., 0., 0.);	
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	/* Left shadow */
	cairo_rectangle (cr, x+1, y+1, 4, height-2);
	pattern = cairo_pattern_create_linear (x, y, x+4, y);
	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0., 0., 0., 0.05);	
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0., 0., 0., 0.);	
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);
}

static void
clearlooks_draw_progressbar_fill (cairo_t *cr,
                                  const ClearlooksColors *colors,
                                  const WidgetParameters *params,
                                  const ProgressBarParameters *progressbar,
                                  int x, int y, int width, int height,
                                  gint offset)
{
	boolean          is_horizontal = progressbar->orientation < 2;
	double           tile_pos = 0;
	double           stroke_width;
	int			x_step;

	cairo_pattern_t *pattern;
	CairoColor       shade1;
	
	/* This is kind of ugly, but I want to get this fixed now. -- Benjamin
	 * The problem is that the rotation should not be resetted by the save/restore. */
	cairo_rectangle (cr, x, y, width, height);

	if (!is_horizontal)
		ge_cairo_exchange_axis (cr, &x, &y, &width, &height);

	if ((progressbar->orientation == CL_ORIENTATION_RIGHT_TO_LEFT) || (progressbar->orientation == CL_ORIENTATION_BOTTOM_TO_TOP))
		ge_cairo_mirror (cr, CR_MIRROR_HORIZONTAL, &x, &y, &width, &height);

	cairo_translate (cr, x, y);


	cairo_save (cr);
	cairo_clip (cr);
	
	stroke_width = height*2;

	x_step = (((float)stroke_width/10)*offset);

	cairo_set_line_width (cr, 1.0);
	
	cairo_save (cr);
	cairo_rectangle (cr, 0, 0, width, height);
		
	/* Draw fill */
	ge_cairo_set_color (cr, &colors->spot[1]);
	cairo_fill_preserve (cr);
	
	/* Draw gradient */
	ge_shade_color (&colors->spot[1], 1.1, &shade1);
	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.0, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.6, colors->spot[1].r, colors->spot[1].g, colors->spot[1].b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, shade1.r, shade1.g, shade1.b);
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);
	
	/* Draw strokes */
	while (tile_pos <= width+x_step)
	{
		cairo_move_to (cr, stroke_width/2-x_step, 0);
		cairo_line_to (cr, stroke_width-x_step,   0);
		cairo_line_to (cr, stroke_width/2-x_step, height);
		cairo_line_to (cr, -x_step, height);
		
		cairo_translate (cr, stroke_width, 0);
		tile_pos += stroke_width;
	}

	cairo_set_source_rgba (cr, colors->spot[2].r,
	                           colors->spot[2].g,
	                           colors->spot[2].b,
	                           0.15);
	
	cairo_fill (cr);
	
	cairo_restore (cr);
	
	/* inner highlight border */
	cairo_set_source_rgba (cr, colors->spot[0].r, colors->spot[0].g, colors->spot[0].b, 0.5);

	if (progressbar->pulsing)
		ge_cairo_stroke_rectangle (cr, 1.5, 0.5, width-3, height-1);
	else
		ge_cairo_stroke_rectangle (cr, 0.5, 0.5, (progressbar->value < 1.0) ? width-2 : width-1, height-1);
	
	cairo_restore (cr);
	
	/* begin of bar line */
	if (progressbar->pulsing)
	{
		cairo_move_to (cr, 0.5, 0);
		cairo_line_to (cr, 0.5, height);
		cairo_set_source_rgba (cr, colors->spot[2].r,
		                           colors->spot[2].g,
		                           colors->spot[2].b,
								   0.5);
		cairo_stroke (cr);
		
	}

	/* Left shadow */
	cairo_translate (cr, width, 0);
	pattern = cairo_pattern_create_linear (0, 0, 3, 0);
	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0., 0., 0., 0.1);	
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0., 0., 0., 0.);	
	cairo_rectangle (cr, 0, 0, 3, height);
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);	
	
	/* End of bar line */
	if (progressbar->value < 1.0)
	{
		cairo_move_to (cr, -0.5, 0);
		cairo_line_to (cr, -0.5, height);
		cairo_set_source_rgba (cr, colors->spot[2].r,
		                           colors->spot[2].g,
		                           colors->spot[2].b,
								   0.5);
		cairo_stroke (cr);
	}
}

static void
clearlooks_draw_optionmenu (cairo_t *cr,
                            const ClearlooksColors *colors,
                            const WidgetParameters *params,
                            const OptionMenuParameters *optionmenu,
                            int x, int y, int width, int height)
{
	SeparatorParameters separator;
	int offset = params->ythickness + 1;
	
	params->style_functions->draw_button (cr, colors, params, x, y, width, height);
	
	separator.horizontal = FALSE;
	params->style_functions->draw_separator (cr, colors, params, &separator, x+optionmenu->linepos, y + offset, 2, height - offset*2);
}

static void
clearlooks_draw_menubar0 (cairo_t *cr,
                          const ClearlooksColors *colors,
                          const WidgetParameters *params,
                          const MenuBarParameters *menubar,
                          int x, int y, int width, int height)
{
	const CairoColor *light = &colors->shade[0];
	const CairoColor *dark = &colors->shade[3];

	cairo_set_line_width (cr, 1);
	cairo_translate (cr, x, y+0.5);

	cairo_move_to (cr, 0, 0);
	cairo_line_to (cr, width, 0);
	ge_cairo_set_color (cr, light);
	cairo_stroke (cr);

	cairo_move_to (cr, 0, height-1);
	cairo_line_to (cr, width, height-1);
	ge_cairo_set_color (cr, dark);
	cairo_stroke (cr);
}

static void
clearlooks_draw_menubar2 (cairo_t *cr,
                          const ClearlooksColors *colors,
                          const WidgetParameters *params,
                          const MenuBarParameters *menubar,
                          int x, int y, int width, int height)
{
	CairoColor lower;
	cairo_pattern_t *pattern;

	ge_shade_color (&colors->bg[0], 0.95, &lower);
	
	cairo_translate (cr, x, y);
	cairo_rectangle (cr, 0, 0, width, height);
	
	/* Draw the gradient */
	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.0, colors->bg[0].r,
	                                                colors->bg[0].g,
	                                                colors->bg[0].b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, lower.r,
	                                                lower.g,
	                                                lower.b);
	cairo_set_source      (cr, pattern);
	cairo_fill            (cr);
	cairo_pattern_destroy (pattern);
	
	/* Draw bottom line */
	cairo_set_line_width (cr, 1.0);
	cairo_move_to        (cr, 0, height-0.5);
	cairo_line_to        (cr, width, height-0.5);
	ge_cairo_set_color   (cr, &colors->shade[3]);
	cairo_stroke         (cr);
}

static void
clearlooks_draw_menubar1 (cairo_t *cr,
                          const ClearlooksColors *colors,
                          const WidgetParameters *params,
                          const MenuBarParameters *menubar,
                          int x, int y, int width, int height)
{
	const CairoColor *border = &colors->shade[3];

	clearlooks_draw_menubar2 (cr, colors, params, menubar,
	                          x, y, width, height);

	ge_cairo_set_color (cr, border);
	ge_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);
}


static menubar_draw_proto clearlooks_menubar_draw[3] =
	{ clearlooks_draw_menubar0, 
	  clearlooks_draw_menubar1,
	  clearlooks_draw_menubar2 };

static void
clearlooks_draw_menubar (cairo_t *cr,
                         const ClearlooksColors *colors,
                         const WidgetParameters *params,
                         const MenuBarParameters *menubar,
                         int x, int y, int width, int height)
{


	if (menubar->style < 0 || menubar->style > 3)
		return;

	clearlooks_menubar_draw[menubar->style](cr, colors, params, menubar,
	                             x, y, width, height);
}

static void
clearlooks_get_frame_gap_clip (int x, int y, int width, int height, 
                               FrameParameters     *frame,
                               ClearlooksRectangle *bevel,
                               ClearlooksRectangle *border)
{
	if (frame->gap_side == CL_GAP_TOP)
	{
		CLEARLOOKS_RECTANGLE_SET ((*bevel),  1.5 + frame->gap_x,  -0.5,
											 frame->gap_width - 3, 2.0);
		CLEARLOOKS_RECTANGLE_SET ((*border), 0.5 + frame->gap_x,  -0.5,
											 frame->gap_width - 2, 2.0);
	}
	else if (frame->gap_side == CL_GAP_BOTTOM)
	{
		CLEARLOOKS_RECTANGLE_SET ((*bevel),  1.5 + frame->gap_x,  height - 2.5,
											 frame->gap_width - 3, 2.0);
		CLEARLOOKS_RECTANGLE_SET ((*border), 0.5 + frame->gap_x,  height - 1.5,
											 frame->gap_width - 2, 2.0);		
	}
	else if (frame->gap_side == CL_GAP_LEFT)
	{
		CLEARLOOKS_RECTANGLE_SET ((*bevel),  -0.5, 1.5 + frame->gap_x,
											 2.0, frame->gap_width - 3);
		CLEARLOOKS_RECTANGLE_SET ((*border), -0.5, 0.5 + frame->gap_x,
											 1.0, frame->gap_width - 2);			
	}
	else if (frame->gap_side == CL_GAP_RIGHT)
	{
		CLEARLOOKS_RECTANGLE_SET ((*bevel),  width - 2.5, 1.5 + frame->gap_x,
											 2.0, frame->gap_width - 3);
		CLEARLOOKS_RECTANGLE_SET ((*border), width - 1.5, 0.5 + frame->gap_x,
											 1.0, frame->gap_width - 2);			
	}
}

static void
clearlooks_draw_frame            (cairo_t *cr,
                                  const ClearlooksColors     *colors,
                                  const WidgetParameters     *params,
                                  const FrameParameters      *frame,
                                  int x, int y, int width, int height)
{
	const float RADIUS = 3.0;
	CairoColor *border = frame->border;
	ClearlooksRectangle bevel_clip = {0, 0, 0, 0};
	ClearlooksRectangle frame_clip = {0, 0, 0, 0};
	
	if (frame->shadow == CL_SHADOW_NONE)
		return;
	
	if (frame->gap_x != -1)
		clearlooks_get_frame_gap_clip (x, y, width, height,
		                               (FrameParameters*)frame,
		                               &bevel_clip, &frame_clip);
	
	cairo_set_line_width (cr, 1.0);
	cairo_translate      (cr, x+0.5, y+0.5);
	
	/* save everything */
	cairo_save (cr);
	/* Set clip for the bevel */
	if (frame->gap_x != -1)
	{
		/* Set clip for gap */
		cairo_set_fill_rule  (cr, CAIRO_FILL_RULE_EVEN_ODD);
		cairo_rectangle      (cr, -0.5, -0.5, width, height);
		cairo_rectangle      (cr, bevel_clip.x, bevel_clip.y, bevel_clip.width, bevel_clip.height);
		cairo_clip           (cr);
	}
	
	/* Draw the bevel */
	if (frame->shadow == CL_SHADOW_ETCHED_IN || frame->shadow == CL_SHADOW_ETCHED_OUT)
	{
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.8 );
		if (frame->shadow == CL_SHADOW_ETCHED_IN)
			ge_cairo_stroke_rectangle (cr, 1, 1, width-2, height-2);
		else
			ge_cairo_stroke_rectangle (cr, 0, 0, width-2, height-2);
	}
	else if (frame->shadow != CL_SHADOW_NONE)
	{
		ShadowParameters shadow;
		shadow.corners = params->corners;
		shadow.shadow  = frame->shadow;
		clearlooks_draw_highlight_and_shade (cr, &shadow, width, height, RADIUS);
	}
	
	/* restore the previous clip region */
	cairo_restore    (cr);
	cairo_save       (cr);
	if (frame->gap_x != -1)
	{
		/* Set clip for gap */
		cairo_set_fill_rule  (cr, CAIRO_FILL_RULE_EVEN_ODD);
		cairo_rectangle      (cr, -0.5, -0.5, width, height);
		cairo_rectangle      (cr, frame_clip.x, frame_clip.y, frame_clip.width, frame_clip.height);
		cairo_clip           (cr);
	}

	/* Draw frame */
	if (frame->shadow == CL_SHADOW_ETCHED_IN || frame->shadow == CL_SHADOW_ETCHED_OUT)
	{
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.2 );
		if (frame->shadow == CL_SHADOW_ETCHED_IN)
			ge_cairo_stroke_rectangle (cr, 0, 0, width-2, height-2);
		else
			ge_cairo_stroke_rectangle (cr, 1, 1, width-2, height-2);
	}
	else
	{
		ge_cairo_set_color (cr, border);
		ge_cairo_stroke_rectangle (cr, 0, 0, width-1, height-1);
	}
	cairo_restore (cr);
}

static void
clearlooks_draw_tab (cairo_t *cr,
                     const ClearlooksColors *colors,
                     const WidgetParameters *params,
                     const TabParameters    *tab,
                     int x, int y, int width, int height)
{
	const float RADIUS = 3.0;
	CairoColor          *border1       = (CairoColor*)&colors->shade[6];
	CairoColor          *stripe_fill   = (CairoColor*)&colors->spot[1];
	CairoColor          *stripe_border = (CairoColor*)&colors->spot[2];
	CairoColor          *fill;

	cairo_pattern_t     *pattern;
	
	int                  corners;
	double               strip_size;
		
	/* Set clip */
	cairo_rectangle      (cr, x, y, width, height);
	cairo_clip           (cr);
	cairo_new_path       (cr);

	/* Translate and set line width */	
	cairo_set_line_width (cr, 1.0);
	cairo_translate      (cr, x+0.5, y+0.5);


	/* Make the tabs slightly bigger than they should be, to create a gap */
	/* And calculate the strip size too, while you're at it */
	if (tab->gap_side == CL_GAP_TOP || tab->gap_side == CL_GAP_BOTTOM)
	{
		height += RADIUS;
	 	strip_size = ((100.0/height)*2.0)/100.0;
		
		if (tab->gap_side == CL_GAP_TOP)
		{
			cairo_translate (cr, 0.0, -4.0); /* gap at the other side */
			corners = CR_CORNER_BOTTOMLEFT | CR_CORNER_BOTTOMRIGHT;
		}
		else
			corners = CR_CORNER_TOPLEFT | CR_CORNER_TOPRIGHT;
	}
	else
	{
		width += RADIUS;
	 	strip_size = ((100.0/width)*2.0)/100.0;
		
		if (tab->gap_side == CL_GAP_LEFT) 
		{
			cairo_translate (cr, -4.0, 0.0); /* gap at the other side */
			corners = CR_CORNER_TOPRIGHT | CR_CORNER_BOTTOMRIGHT;		
		}
		else
			corners = CR_CORNER_TOPLEFT | CR_CORNER_BOTTOMLEFT;	
	}
	
	/* Set the fill color */
	if (params->active)
		fill = (CairoColor*)&colors->bg[params->state_type];
	else
		fill = (CairoColor*)&params->parentbg;
	

	/* Set tab shape */
	ge_cairo_rounded_rectangle (cr, 0,
	                                     0,
	                                     width-1,
	                                     height-1,
	                                     RADIUS, corners);
	
	/* Draw fill */
	ge_cairo_set_color (cr, fill);
	cairo_fill   (cr);
	
	/* Draw highlight */
	if (!params->active)
	{
		ShadowParameters shadow;
		
		shadow.shadow  = CL_SHADOW_OUT;
		shadow.corners = corners;
		
		clearlooks_draw_highlight_and_shade (cr, &shadow,
		                                     width,
		                                     height, RADIUS);
	}
	

	if (params->active)
	{
		cairo_set_line_width (cr, 1);
		cairo_set_source_rgba (cr, 1, 1, 1, 0.4);
		
		if (tab->gap_side == CL_GAP_TOP || tab->gap_side == CL_GAP_BOTTOM)
			ge_cairo_stroke_rectangle (cr, 1, 1, width-3, height-3);
		else
			ge_cairo_stroke_rectangle (cr, 1, 1, width-3, height-3);
	}
	else
	{
		/* Draw shade */
		pattern = cairo_pattern_create_linear ( tab->gap_side == CL_GAP_LEFT   ? width-2  : 0,
		                                        tab->gap_side == CL_GAP_TOP    ? height-2 : 0,
		                                        tab->gap_side == CL_GAP_RIGHT  ? width    : 0,
		                                        tab->gap_side == CL_GAP_BOTTOM ? height   : 0 );
	
		ge_cairo_rounded_rectangle (cr, 0,
	                                     0,
	                                     width-1,
	                                     height-1,
	                                     RADIUS, corners);
		
		cairo_pattern_add_color_stop_rgb  (pattern, 0.0,        stripe_fill->r, stripe_fill->g, stripe_fill->b);
		cairo_pattern_add_color_stop_rgb  (pattern, strip_size, stripe_fill->r, stripe_fill->g, stripe_fill->b);
		cairo_pattern_add_color_stop_rgba (pattern, strip_size, 1.0, 1.0, 1.0, 0.5);
		cairo_pattern_add_color_stop_rgba (pattern, 0.8,        1.0, 1.0, 1.0, 0.0);
		cairo_set_source (cr, pattern);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}

	
	ge_cairo_rounded_rectangle (cr, 0,
	                                     0,
	                                     width-1,
	                                     height-1,
	                                     RADIUS, corners);
	
	
	if (params->active)
	{
		ge_cairo_set_color (cr, border1);	
		cairo_stroke (cr);
	}
	else
	{
		pattern = cairo_pattern_create_linear ( tab->gap_side == CL_GAP_LEFT   ? width-2  : 2,
		                                        tab->gap_side == CL_GAP_TOP    ? height-2 : 2,
		                                        tab->gap_side == CL_GAP_RIGHT  ? width    : 2,
		                                        tab->gap_side == CL_GAP_BOTTOM ? height   : 2 );
		
		cairo_pattern_add_color_stop_rgb (pattern, 0.0,        stripe_border->r, stripe_border->g, stripe_border->b);
		cairo_pattern_add_color_stop_rgb (pattern, strip_size, stripe_border->r, stripe_border->g, stripe_border->b);
		cairo_pattern_add_color_stop_rgb (pattern, strip_size, border1->r,       border1->g,       border1->b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0,        border1->r,       border1->g,       border1->b);
		cairo_set_source (cr, pattern);
		cairo_stroke (cr);
		cairo_pattern_destroy (pattern);
	}
}

static void
clearlooks_draw_separator (cairo_t *cr,
                           const ClearlooksColors     *colors,
                           const WidgetParameters     *widget,
                           const SeparatorParameters  *separator,
                           int x, int y, int width, int height)
{
	cairo_save (cr);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);

	if (separator->horizontal)
	{
		//cairo_reset_clip (cr);
		cairo_set_line_width  (cr, 1.0);
		cairo_translate       (cr, x, y+0.5);
		
		cairo_move_to         (cr, 0.0,   0.0);
		cairo_line_to         (cr, width, 0.0);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.2);
		cairo_stroke          (cr);
		
		cairo_move_to         (cr, 0.0,   1.0);
		cairo_line_to         (cr, width, 1.0);
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.8);
		cairo_stroke          (cr);				
	}
	else
	{
		cairo_set_line_width  (cr, 1.0);
		cairo_translate       (cr, x+0.5, y);
		
		cairo_move_to         (cr, 0.0, 0.0);
		cairo_line_to         (cr, 0.0, height);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.2);
		cairo_stroke          (cr);
		
		cairo_move_to         (cr, 1.0, 0.0);
		cairo_line_to         (cr, 1.0, height);
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.8);
		cairo_stroke          (cr);		
	}

	cairo_restore (cr);
}

static void
clearlooks_draw_list_view_header (cairo_t *cr,
                                  const ClearlooksColors          *colors,
                                  const WidgetParameters          *params,
                                  const ListViewHeaderParameters  *header,
                                  int x, int y, int width, int height)
{
	CairoColor      *border = (CairoColor*)&colors->shade[5];
	cairo_pattern_t *pattern;
	
	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);
	
	/* Draw highlight */
	if (header->order == CL_ORDER_FIRST)
	{
		cairo_move_to (cr, 0.5, height-1);
		cairo_line_to (cr, 0.5, 0.5);
	}
	else
		cairo_move_to (cr, 0.0, 0.5);
	
	cairo_line_to (cr, width, 0.5);
	
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.5);
	cairo_stroke (cr);
	
	/* Draw bottom border */
	cairo_move_to (cr, 0.0, height-0.5);
	cairo_line_to (cr, width, height-0.5);
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	/* Draw bottom shade */	
	pattern = cairo_pattern_create_linear (0.0, height-5.0, 0.0, height-1.0);
	cairo_pattern_add_color_stop_rgba     (pattern, 0.0, 0.0, 0.0, 0.0, 0.0);
	cairo_pattern_add_color_stop_rgba     (pattern, 1.0, 0.0, 0.0, 0.0, 0.1);

	cairo_rectangle       (cr, 0.0, height-5.0, width, 4.0);
	cairo_set_source      (cr, pattern);
	cairo_fill            (cr);
	cairo_pattern_destroy (pattern);
	
	/* Draw resize grip */
	if ((params->ltr && header->order != CL_ORDER_LAST) ||
	    (!params->ltr && header->order != CL_ORDER_FIRST) || header->resizable)
	{
		SeparatorParameters separator;
		separator.horizontal = FALSE;
		
		if (params->ltr)
			params->style_functions->draw_separator (cr, colors, params, &separator,
			                                         width-1.5, 4.0, 2, height-8.0);
		else
			params->style_functions->draw_separator (cr, colors, params, &separator,
			                                         1.5, 4.0, 2, height-8.0);
	}
}

/* We can't draw transparent things here, since it will be called on the same
 * surface multiple times, when placed on a handlebox_bin or dockitem_bin */
static void
clearlooks_draw_toolbar (cairo_t *cr,
                         const ClearlooksColors          *colors,
                         const WidgetParameters          *widget,
                         int x, int y, int width, int height)
{
	const CairoColor *light = &colors->shade[0];
	const CairoColor *dark  = &colors->shade[3];
	
	cairo_set_line_width (cr, 1.0);
	
	/* Draw highlight */
	cairo_translate     (cr, x, y);
	cairo_move_to       (cr, 0, 0.5);
	cairo_line_to       (cr, width-1, 0.5);
	ge_cairo_set_color  (cr, light);
	cairo_stroke        (cr);

	/* Draw shadow */
	cairo_move_to       (cr, 0, height-0.5);
	cairo_line_to       (cr, width-1, height-0.5);
	ge_cairo_set_color  (cr, dark);
	cairo_stroke        (cr);
}

static void
clearlooks_draw_menuitem (cairo_t *cr,
                          const ClearlooksColors          *colors,
                          const WidgetParameters          *widget,
                          int x, int y, int width, int height)
{
	CairoColor *fill = (CairoColor*)&colors->spot[1];
	CairoColor fill_shade;
	CairoColor *border = (CairoColor*)&colors->spot[2];
	cairo_pattern_t *pattern;
	
	ge_shade_color (fill, 0.85, &fill_shade);
	
	cairo_translate      (cr, x, y);
	cairo_set_line_width (cr, 1.0);
	
	ge_cairo_rounded_rectangle (cr, 0, 0, width, height, 3.0, widget->corners);
	
	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0,   fill->r, fill->g, fill->b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, fill_shade.r, fill_shade.g, fill_shade.b);
	
	cairo_set_source (cr, pattern);
	cairo_fill_preserve  (cr);
	cairo_pattern_destroy (pattern);
	
	ge_cairo_set_color (cr, border);
	cairo_stroke	   (cr);
}

static void
clearlooks_draw_scrollbar_trough (cairo_t *cr,
                                  const ClearlooksColors           *colors,
                                  const WidgetParameters           *widget,
                                  const ScrollBarParameters        *scrollbar,
                                  int x, int y, int width, int height)
{
	CairoColor      *bg     = (CairoColor*)&colors->shade[2];
	CairoColor      *border = (CairoColor*)&colors->shade[6];
	CairoColor       bg_shade;
	cairo_pattern_t *pattern;
	
	ge_shade_color (bg, 0.95, &bg_shade);
	
	cairo_set_line_width (cr, 1);
	/* cairo_translate (cr, x, y); */
	
	if (scrollbar->horizontal)
		ge_cairo_exchange_axis (cr, &x, &y, &width, &height);

	cairo_translate (cr, x, y);	

	/* Draw fill */
	cairo_rectangle (cr, 1, 0, width-2, height);
	ge_cairo_set_color (cr, bg);
	cairo_fill (cr);

	/* Draw shadow */
	pattern = cairo_pattern_create_linear (1, 0, 3, 0);
	cairo_pattern_add_color_stop_rgb (pattern, 0,   bg_shade.r, bg_shade.g, bg_shade.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, bg->r,      bg->g,      bg->b);	
	cairo_rectangle (cr, 1, 0, 4, height);
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);
	
	/* Draw border */
	ge_cairo_set_color (cr, border);
	ge_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);
}

static void
clearlooks_draw_scrollbar_stepper (cairo_t *cr,
                                   const ClearlooksColors           *colors,
                                   const WidgetParameters           *widget,
                                   const ScrollBarParameters        *scrollbar,
                                   const ScrollBarStepperParameters *stepper,
                                   int x, int y, int width, int height)
{
	CairoCorners corners = CR_CORNER_NONE;
	CairoColor *border = (CairoColor*)&colors->shade[6];
	CairoColor  s1, s2, s3, s4;
	cairo_pattern_t *pattern;
	ShadowParameters shadow;
	
	if (scrollbar->horizontal)
	{
		if (stepper->stepper == CL_STEPPER_A)
			corners = CR_CORNER_TOPLEFT | CR_CORNER_BOTTOMLEFT;
		else if (stepper->stepper == CL_STEPPER_D)
			corners = CR_CORNER_TOPRIGHT | CR_CORNER_BOTTOMRIGHT;
	}
	else
	{
		if (stepper->stepper == CL_STEPPER_A)
			corners = CR_CORNER_TOPLEFT | CR_CORNER_TOPRIGHT;
		else if (stepper->stepper == CL_STEPPER_D)
			corners = CR_CORNER_BOTTOMLEFT | CR_CORNER_BOTTOMRIGHT;
	}
	
	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1);
	
	cairo_rectangle (cr, 1, 1, width-2, height-2);
	
	if (scrollbar->horizontal)
		pattern = cairo_pattern_create_linear (0, 0, 0, height);
	else
		pattern = cairo_pattern_create_linear (0, 0, width, 0);
				
	s3 = colors->bg[widget->state_type];
	ge_shade_color(&s3, 1.15, &s1);
	ge_shade_color(&s3, 1.03, &s2); 
	ge_shade_color(&s3, 0.92,  &s4); 
	
	cairo_pattern_add_color_stop_rgb(pattern, 0,    s1.r, s1.g, s1.b);
	cairo_pattern_add_color_stop_rgb(pattern, 0.3, s2.r, s2.g, s2.b);
	cairo_pattern_add_color_stop_rgb(pattern, 0.7, s3.r, s3.g, s3.b);
	cairo_pattern_add_color_stop_rgb(pattern, 1.0,  s4.r, s4.g, s4.b);
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	ge_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, 3.0, corners);	
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);
	
	cairo_translate (cr, 0.5, 0.5);
	shadow.shadow  = CL_SHADOW_OUT;
	shadow.corners = corners;
	/*
	clearlooks_draw_highlight_and_shade (cr, &shadow,
	                                     width,
	                                     height, 3.0);*/
}

static void
clearlooks_draw_scrollbar_slider (cairo_t *cr,
                                   const ClearlooksColors          *colors,
                                   const WidgetParameters          *widget,
                                   const ScrollBarParameters       *scrollbar,
                                   int x, int y, int width, int height)
{
	if (scrollbar->junction & CL_JUNCTION_BEGIN)
	{
		if (scrollbar->horizontal)
		{
			x -= 1;
			width += 1;
		}
		else
		{
			y -= 1;
			height += 1;
		}
	}
	if (scrollbar->junction & CL_JUNCTION_END)
	{
		if (scrollbar->horizontal)
			width += 1;
		else
			height += 1;
	}
	
	if (!scrollbar->horizontal)
		ge_cairo_exchange_axis (cr, &x, &y, &width, &height);

	cairo_translate (cr, x, y);	

	if (scrollbar->has_color)
	{
		CairoColor *border  = (CairoColor*)&colors->shade[8];
		CairoColor  fill    = scrollbar->color;
		CairoColor  hilight;
		CairoColor  shade1, shade2, shade3, shade4;
		cairo_pattern_t *pattern;
				
		if (widget->prelight)
			ge_shade_color (&fill, 1.1, &fill);
			
		cairo_set_line_width (cr, 1);
		
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
		
		cairo_set_source_rgba (cr, hilight.r, hilight.g, hilight.b, 0.5);
		ge_cairo_stroke_rectangle (cr, 1.5, 1.5, width-3, height-3);
	
		ge_cairo_set_color (cr, border);
		ge_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);
	}
	else
	{
		CairoColor *border  = (CairoColor*)&colors->shade[6];
		CairoColor s1, s2, s3, s4;		
		cairo_pattern_t *pattern;
		int bar_x, i;
		
		CairoColor *dark  = (CairoColor*)&colors->shade[4];
		CairoColor *light = (CairoColor*)&colors->shade[0];		
		
		s3 = colors->bg[widget->state_type];
		ge_shade_color(&s3, 1.15, &s1);
		ge_shade_color(&s3, 1.03, &s2); 
		ge_shade_color(&s3, 0.92,  &s4); 
	
		pattern = cairo_pattern_create_linear(1, 1, 1, height-1);
		cairo_pattern_add_color_stop_rgb(pattern, 0,    s1.r, s1.g, s1.b);
		cairo_pattern_add_color_stop_rgb(pattern, 0.3, s2.r, s2.g, s2.b);
		cairo_pattern_add_color_stop_rgb(pattern, 0.7, s3.r, s3.g, s3.b);
		cairo_pattern_add_color_stop_rgb(pattern, 1.0,  s4.r, s4.g, s4.b);

		cairo_rectangle (cr, 1, 1, width-2, height-2);
		cairo_set_source(cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy(pattern);
		
		ge_cairo_set_color (cr, border);
		ge_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);
		
		/* draw handles */
		cairo_set_line_width (cr, 1);
		
		bar_x = width/2 - 4;
		cairo_translate(cr, 0.5, 0.5);
		for (i=0; i<3; i++)
		{
			cairo_move_to (cr, bar_x, 4);
			cairo_line_to (cr, bar_x, height-5);
			ge_cairo_set_color (cr, dark);
			cairo_stroke (cr);
			
			cairo_move_to (cr, bar_x+1, 4);
			cairo_line_to (cr, bar_x+1, height-5);
			ge_cairo_set_color (cr, light);
			cairo_stroke (cr);
			
			bar_x += 3;
		}
	}
	
}

static void
clearlooks_draw_statusbar (cairo_t *cr,
                           const ClearlooksColors          *colors,
                           const WidgetParameters          *widget,
                           int x, int y, int width, int height)
{
	cairo_set_line_width  (cr, 1);
	cairo_translate       (cr, x, y+0.5);
	cairo_move_to         (cr, 0, 0);
	cairo_line_to         (cr, width, 0);
	cairo_set_source_rgba (cr, 0, 0, 0, 0.2);
	cairo_stroke          (cr);

	cairo_translate       (cr, 0, 1);
	cairo_move_to         (cr, 0, 0);
	cairo_line_to         (cr, width, 0);
	cairo_set_source_rgba (cr, 1, 1, 1, 0.8);
	cairo_stroke          (cr);
}

void
clearlooks_draw_menu_frame (cairo_t *cr,
                            const ClearlooksColors          *colors,
                            const WidgetParameters          *widget,
                            int x, int y, int width, int height)
{
	CairoColor *border = (CairoColor*)&colors->shade[5];
	cairo_translate      (cr, x, y);
	cairo_set_line_width (cr, 1);
/*	
	cairo_set_source_rgba (cr, colors->bg[0].r, colors->bg[0].g, colors->bg[0].b, 0.9);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_paint          (cr);
*/
	ge_cairo_set_color (cr, border);
	ge_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);
}

static void
clearlooks_draw_handle (cairo_t *cr,
                        const ClearlooksColors          *colors,
                        const WidgetParameters          *params,
                        const HandleParameters          *handle,
                        int x, int y, int width, int height)
{
	const CairoColor *fill  = &colors->bg[params->state_type];
//	CairoColor *light = &colors->shade[0];
	
	int num_bars, bar_spacing;
	
	if (handle->type == CL_HANDLE_TOOLBAR)
	{
		num_bars    = 6;
		bar_spacing = 3;
	}
	else if (handle->type == CL_HANDLE_SPLITTER)
	{
		num_bars    = 16;
		bar_spacing = 3;
	}

	if (params->prelight)
	{
		cairo_rectangle (cr, x, y, width, height);
		ge_cairo_set_color (cr, fill);
		cairo_fill (cr);
	}
	
	cairo_translate (cr, x+0.5, y+0.5);
	
	cairo_set_line_width (cr, 1);
	
	if (handle->horizontal)
	{
		params->style_functions->draw_gripdots (cr, 0, 0, width, height, num_bars, 2, 0.1);
	}
	else
	{
		params->style_functions->draw_gripdots (cr, 0, 0, width, height, 2, num_bars, 0.1);
	}
}

static void
clearlooks_draw_normal_arrow (cairo_t *cr, CairoColor *color,
                              double x, double y, double width, double height)
{
	const int ARROW_WIDTH  = 10.0;
	const int ARROW_HEIGHT = 5.0;

	cairo_set_line_width (cr, 1);
	
	cairo_move_to   (cr, -ARROW_WIDTH/2, -ARROW_HEIGHT/2);
	cairo_line_to   (cr, 0, ARROW_HEIGHT/2);
	cairo_line_to   (cr, ARROW_WIDTH/2, -ARROW_HEIGHT/2);
	
	ge_cairo_set_color (cr, color);
	cairo_fill (cr);	
}

static void
clearlooks_draw_combo_arrow (cairo_t *cr, CairoColor *fill,
                             double x, double y, double width, double height)
{
	const int ARROW_WIDTH   = 7.0;
	const int ARROW_HEIGHT  = 5.0;
	const int ARROW_SPACING = 8;
	
	cairo_set_line_width (cr, 1);

	y -= ARROW_SPACING/2;
	
	cairo_move_to (cr, -ARROW_WIDTH/2, y + ARROW_HEIGHT/2);
	cairo_line_to   (cr, 0, y + -ARROW_HEIGHT/2);
	cairo_line_to   (cr, ARROW_WIDTH/2, y + ARROW_HEIGHT/2);
	ge_cairo_set_color (cr, fill);	
	cairo_fill (cr);
	
	y += ARROW_SPACING;
	
	cairo_move_to (cr, -ARROW_WIDTH/2, y + -ARROW_HEIGHT/2);
	cairo_line_to   (cr, 0, y + ARROW_HEIGHT/2);
	cairo_line_to   (cr, ARROW_WIDTH/2, y + -ARROW_HEIGHT/2);
	ge_cairo_set_color (cr, fill);	
	cairo_fill (cr);
}

static void
_clearlooks_draw_arrow (cairo_t *cr, CairoColor *color,
                        ClearlooksDirection dir, ClearlooksArrowType type,
                        double x, double y, double width, double height)
{
	double rotate;
	
	if (dir == CL_DIRECTION_LEFT)
		rotate = G_PI*1.5;
	else if (dir == CL_DIRECTION_RIGHT)
		rotate = G_PI*0.5;
	else if (dir == CL_DIRECTION_UP)
		rotate = G_PI;
	else
		rotate = 0;
	
	if (type == CL_ARROW_NORMAL)
	{
		cairo_translate (cr, x, y);
		cairo_rotate (cr, rotate);		
		clearlooks_draw_normal_arrow (cr, color, 0, 0, width, height);
	}
	else if (type == CL_ARROW_COMBO)
	{
		cairo_translate (cr, x, y);
		clearlooks_draw_combo_arrow (cr, color, 0, 0, width, height);
	}
}

static void
clearlooks_draw_arrow (cairo_t *cr,
                       const ClearlooksColors          *colors,
                       const WidgetParameters          *widget,
                       const ArrowParameters           *arrow,
                       int x, int y, int width, int height)
{
	CairoColor *color = (CairoColor*)&colors->shade[7];
	gdouble tx, ty;
	
	if (arrow->direction == CL_DIRECTION_DOWN || arrow->direction == CL_DIRECTION_UP)
	{
		tx = x + width/2;
		ty = (y + height/2) + 0.5;
	}
	else
	{
		tx = (x + width/2) + 0.5;
		ty = y + height/2;
	}
	
	if (widget->disabled)
	{
		_clearlooks_draw_arrow (cr, (CairoColor*)&colors->shade[0],
		                        arrow->direction, arrow->type,
		                        tx+0.5, ty+0.5, width, height);
		
		color = (CairoColor*)&colors->shade[4];
	}

	cairo_identity_matrix (cr);
	
	_clearlooks_draw_arrow (cr, color, arrow->direction, arrow->type,
	                        tx, ty, width, height);
}

static void
clearlooks_draw_resize_grip (cairo_t *cr,
                             const ClearlooksColors          *colors,
                             const WidgetParameters          *widget,
                             const ResizeGripParameters      *grip,
                             int x, int y, int width, int height)
{
	int lx, ly;
 
	cairo_set_line_width (cr, 1);
	
	for (ly=0; ly<4; ly++) /* vertically, four rows of dots */
	{
		for (lx=0; lx<=ly; lx++) /* horizontally */
		{
			int ny = (3.5-ly) * 3;
			int nx = lx * 3;

			cairo_set_source_rgba (cr, 1, 1, 1, 1.2);
			cairo_rectangle (cr, x+width-nx-1, y+height-ny-1, 2, 2);
			cairo_fill (cr);

			cairo_set_source_rgba (cr, 0, 0, 0, 0.4);
			cairo_rectangle (cr, x+width-nx-1, y+height-ny-1, 1, 1);
			cairo_fill (cr);
		}
	}
}

static void
clearlooks_draw_radiobutton (cairo_t *cr,
                             const ClearlooksColors  *colors,
                             const WidgetParameters  *widget,
                             const CheckboxParameters *checkbox,
                             int x, int y, int width, int height)
{
	const CairoColor *border;
	const CairoColor *dot;
	cairo_pattern_t *pt;
	gboolean inconsistent; 
	gboolean draw_bullet = (checkbox->shadow_type == GTK_SHADOW_IN);

	inconsistent = (checkbox->shadow_type == GTK_SHADOW_ETCHED_IN);
	draw_bullet |= inconsistent;

	if (widget->disabled)
	{
		border = &colors->shade[5];
		dot    = &colors->shade[6];
	}
	else
	{
		border = &colors->shade[7];
		dot    = &colors->spot[1];
	}
	pt = cairo_pattern_create_linear (0, 0, 13, 13);
	cairo_pattern_add_color_stop_rgba (pt, 0.0, 0, 0, 0, 0.1);
	cairo_pattern_add_color_stop_rgba (pt, 0.5, 0, 0, 0, 0);
	cairo_pattern_add_color_stop_rgba (pt, 0.5, 1, 1, 1, 0);
	cairo_pattern_add_color_stop_rgba (pt, 1.0, 1, 1, 1, 0.5);
	
	cairo_translate (cr, x, y);
	
	cairo_set_line_width (cr, 2);
	cairo_arc       (cr, 7, 7, 6, 0, M_PI*2);	
	cairo_set_source (cr, pt);
	cairo_stroke (cr);
	cairo_pattern_destroy (pt);

	cairo_set_line_width (cr, 1);

	cairo_arc       (cr, 7, 7, 5.5, 0, M_PI*2);	
	
	if (!widget->disabled)
	{
		ge_cairo_set_color (cr, &colors->base[0]);
		cairo_fill_preserve (cr);
	}
	
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);
	
	if (draw_bullet)
	{
		if (inconsistent)
		{
			cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
			cairo_set_line_width (cr, 4);

			cairo_move_to(cr, 5, 7);
			cairo_line_to(cr, 9, 7);

			ge_cairo_set_color (cr, dot);
			cairo_stroke (cr);

		}
		else
		{
			cairo_arc (cr, 7, 7, 3, 0, M_PI*2);
			ge_cairo_set_color (cr, dot);
			cairo_fill (cr);
		
			cairo_arc (cr, 6, 6, 1, 0, M_PI*2);
			/* cairo_set_source_rgba (cr, 1,1,1, 0.5); */
			cairo_set_source_rgb (cr, 1,1,1);
			cairo_fill (cr);
		}
	}
}

static void
clearlooks_draw_checkbox (cairo_t *cr,
                          const ClearlooksColors  *colors,
                          const WidgetParameters  *widget,
                          const CheckboxParameters *checkbox,
                          int x, int y, int width, int height)
{
	const CairoColor *border;
	const CairoColor *dot;
	gboolean inconsistent = FALSE;
	gboolean draw_bullet = (checkbox->shadow_type == GTK_SHADOW_IN);
	cairo_pattern_t *pt;

	inconsistent = (checkbox->shadow_type == GTK_SHADOW_ETCHED_IN);
	draw_bullet |= inconsistent;
	
	if (widget->disabled)
	{
		border = &colors->shade[5];
		dot    = &colors->shade[6];
	}
	else
	{
		border = &colors->shade[7];
		dot    = &colors->spot[1];
	}

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1);
	
	if (widget->xthickness > 2 && widget->ythickness > 2)
	{
		/* Draw a gradient around the box so it appears sunken. */
		pt = cairo_pattern_create_linear (0, 0, 0, 13);
		cairo_pattern_add_color_stop_rgba (pt, 0.0, 0, 0, 0, 0.04);
		cairo_pattern_add_color_stop_rgba (pt, 0.5, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pt, 0.5, 1, 1, 1, 0);
		cairo_pattern_add_color_stop_rgba (pt, 1.0, 1, 1, 1, 0.4);
		
		cairo_rectangle (cr, 0.5, 0.5, width-1, height-1);
		cairo_set_source (cr, pt);
		cairo_stroke (cr);
		cairo_pattern_destroy (pt);
		
		/* Draw the rectangle for the checkbox itself */
		cairo_rectangle (cr, 1.5, 1.5, width-3, height-3);
	}
	else
	{
		cairo_rectangle (cr, 0.5, 0.5, width-1, height-1);
	}
	
	if (!widget->disabled)
	{
		ge_cairo_set_color (cr, &colors->base[0]);
		cairo_fill_preserve (cr);
	}
	
	ge_cairo_set_color (cr, border);
	cairo_stroke (cr);

	if (draw_bullet)
	{
		if (inconsistent) /* Inconsistent */
		{
			cairo_set_line_width (cr, 2.0);
			cairo_move_to (cr, 3, height*0.5);
			cairo_line_to (cr, width-3, height*0.5);
		}
		else
		{
			cairo_set_line_width (cr, 1.7);
			cairo_move_to (cr, 0.5 + (width*0.2), (height*0.5));
			cairo_line_to (cr, 0.5 + (width*0.4), (height*0.7));
		
			cairo_curve_to (cr, 0.5 + (width*0.4), (height*0.7),
			                    0.5 + (width*0.5), (height*0.4),
			                    0.5 + (width*0.70), (height*0.25));
		}
		
		ge_cairo_set_color (cr, dot);
		cairo_stroke (cr);
	}
}



void
clearlooks_register_style_classic (ClearlooksStyleFunctions *functions)
{
	g_assert (functions);

	functions->draw_button			= clearlooks_draw_button;
	functions->draw_scale_trough		= clearlooks_draw_scale_trough;
	functions->draw_progressbar_trough	= clearlooks_draw_progressbar_trough;
	functions->draw_progressbar_fill	= clearlooks_draw_progressbar_fill;
	functions->draw_slider_button		= clearlooks_draw_slider_button;
	functions->draw_entry			= clearlooks_draw_entry;
	functions->draw_spinbutton		= clearlooks_draw_spinbutton;
	functions->draw_spinbutton_down		= clearlooks_draw_spinbutton_down;
	functions->draw_optionmenu		= clearlooks_draw_optionmenu;
	functions->draw_inset			= clearlooks_draw_inset;
	functions->draw_menubar			= clearlooks_draw_menubar;
	functions->draw_tab			= clearlooks_draw_tab;
	functions->draw_frame			= clearlooks_draw_frame;
	functions->draw_separator		= clearlooks_draw_separator;
	functions->draw_list_view_header	= clearlooks_draw_list_view_header;
	functions->draw_toolbar			= clearlooks_draw_toolbar;
	functions->draw_menuitem		= clearlooks_draw_menuitem;
	functions->draw_scrollbar_stepper	= clearlooks_draw_scrollbar_stepper;
	functions->draw_scrollbar_slider	= clearlooks_draw_scrollbar_slider;
	functions->draw_scrollbar_trough	= clearlooks_draw_scrollbar_trough;
	functions->draw_statusbar		= clearlooks_draw_statusbar;
	functions->draw_menu_frame		= clearlooks_draw_menu_frame;
	functions->draw_handle			= clearlooks_draw_handle;
	functions->draw_arrow			= clearlooks_draw_arrow;
	functions->draw_resize_grip		= clearlooks_draw_resize_grip;
	functions->draw_checkbox		= clearlooks_draw_checkbox;
	functions->draw_radiobutton		= clearlooks_draw_radiobutton;
	
	functions->draw_shadow			= clearlooks_draw_shadow;
	functions->draw_slider			= clearlooks_draw_slider;
	functions->draw_gripdots		= clearlooks_draw_gripdots;
}



