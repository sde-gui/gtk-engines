#include "cairo-support.h"
#include <math.h>

/***********************************************
 * ge_hsb_from_color -
 *  
 *   Get HSB values from RGB values.
 *
 *   Modified from Smooth but originated in GTK+
 ***********************************************/
void
ge_hsb_from_color (const CairoColor *color, 
                        gdouble *hue, 
                        gdouble *saturation,
                        gdouble *brightness) 
{
	gdouble min, max, delta;
	gdouble red, green, blue;

	red = color->r;
	green = color->g;
	blue = color->b;
  
	if (red > green)
	{
		max = MAX(red, blue);
		min = MIN(green, blue);      
	}
	else
	{
		max = MAX(green, blue);
		min = MIN(red, blue);      
	}
  
	*brightness = (max + min) / 2;
 	
	if (max == min)
	{
		*hue = 0;
		*saturation = 0;
	}	
	else
	{
		if (*brightness <= 0.5)
			*saturation = (max - min) / (max + min);
		else
			*saturation = (max - min) / (2 - max - min);
       
		delta = max -min;
 
		if (red == max)
			*hue = (green - blue) / delta;
		else if (green == max)
			*hue = 2 + (blue - red) / delta;
		else if (blue == max)
			*hue = 4 + (red - green) / delta;
 
		*hue *= 60;
		if (*hue < 0.0)
			*hue += 360;
	}
}
 
/***********************************************
 * ge_color_from_hsb -
 *  
 *   Get RGB values from HSB values.
 *
 *   Modified from Smooth but originated in GTK+
 ***********************************************/
#define MODULA(number, divisor) (((gint)number % divisor) + (number - (gint)number))
void
ge_color_from_hsb (gdouble hue, 
                        gdouble saturation,
                        gdouble brightness, 
                        CairoColor *color)
{
	gint i;
	gdouble hue_shift[3], color_shift[3];
	gdouble m1, m2, m3;

	if (!color) return;
  	  
	if (brightness <= 0.5)
		m2 = brightness * (1 + saturation);
	else
		m2 = brightness + saturation - brightness * saturation;
 
	m1 = 2 * brightness - m2;
 
	hue_shift[0] = hue + 120;
	hue_shift[1] = hue;
	hue_shift[2] = hue - 120;
 
	color_shift[0] = color_shift[1] = color_shift[2] = brightness;	
 
	i = (saturation == 0)?3:0;
 
	for (; i < 3; i++)
	{
		m3 = hue_shift[i];
 
		if (m3 > 360)
			m3 = MODULA(m3, 360);
		else if (m3 < 0)
			m3 = 360 - MODULA(ABS(m3), 360);
 
		if (m3 < 60)
			color_shift[i] = m1 + (m2 - m1) * m3 / 60;
		else if (m3 < 180)
			color_shift[i] = m2;
		else if (m3 < 240)
			color_shift[i] = m1 + (m2 - m1) * (240 - m3) / 60;
		else
			color_shift[i] = m1;
	}	
 
	color->r = color_shift[0];
	color->g = color_shift[1];
	color->b = color_shift[2];	
	color->a = 1.0;	
}

void
ge_gdk_color_to_cairo (GdkColor *c, CairoColor *cc)
{
	gdouble r, g, b;

	g_return_if_fail (c && cc);

	r = c->red / 65536.0;
	g = c->green / 65536.0;
	b = c->blue / 65536.0;

	cc->r = r;
	cc->g = g;
	cc->b = b;
	cc->a = 1.0;
}

void
ge_cairo_color_to_gtk (CairoColor *cc, GdkColor *c)
{
	gdouble r, g, b;

	g_return_if_fail (c && cc);

	r = cc->r * 65536.0;
	g = cc->g * 65536.0;
	b = cc->b * 65536.0;

	c->red = r;
	c->green = g;
	c->blue = b;
}

void 
ge_gtk_style_to_cairo_color_cube (GtkStyle * style, CairoColorCube *cube)
{
	int i;

	g_return_if_fail (style && cube);

	for (i = 0; i < 5; i++)
	{ 
		ge_gdk_color_to_cairo (&style->bg[i], &cube->bg[i]);
		ge_gdk_color_to_cairo (&style->fg[i], &cube->fg[i]);

		ge_gdk_color_to_cairo (&style->dark[i], &cube->dark[i]);
		ge_gdk_color_to_cairo (&style->light[i], &cube->light[i]);
		ge_gdk_color_to_cairo (&style->mid[i], &cube->mid[i]);

		ge_gdk_color_to_cairo (&style->base[i], &cube->base[i]);
		ge_gdk_color_to_cairo (&style->text[i], &cube->text[i]);
		ge_gdk_color_to_cairo (&style->text_aa[i], &cube->text_aa[i]);
    	}

	cube->black.r = cube->black.g = cube->black.b = 0;
	cube->black.a = 1;

	cube->white.r = cube->white.g = cube->white.b = 1;
	cube->white.a = 1;
}

void
ge_shade_color(const CairoColor *base, gdouble shade_ratio, CairoColor *composite)
{
	gdouble hue = 0;
	gdouble saturation = 0;
	gdouble brightness = 0;
 
	g_return_if_fail (base && composite);

	ge_hsb_from_color (base, &hue, &saturation, &brightness);
 
	brightness = MIN(brightness*shade_ratio, 1.0);
	brightness = MAX(brightness, 0.0);
  
	saturation = MIN(saturation*shade_ratio, 1.0);
	saturation = MAX(saturation, 0.0);
  
	ge_color_from_hsb (hue, saturation, brightness, composite);
	composite->a = base->a;	
}

void
ge_saturate_color (const CairoColor * base, gdouble saturate_level, CairoColor *composite)
{
	gdouble hue = 0;
	gdouble saturation = 0;
	gdouble brightness = 0;
 
	g_return_if_fail (base && composite);

	ge_hsb_from_color (base, &hue, &saturation, &brightness);

	saturation = MIN(saturation*saturate_level, 1.0);
	saturation = MAX(saturation, 0.0);

	ge_color_from_hsb (hue, saturation, brightness, composite);
	composite->a = base->a;	
}

cairo_t * 
ge_gdk_drawable_to_cairo (GdkDrawable  *window, GdkRectangle *area)
{
	cairo_t *cr;

	g_return_if_fail (window != NULL);

	cr = (cairo_t*) gdk_cairo_create (window);
	cairo_set_line_width (cr, 1.0);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_SQUARE);
	cairo_set_line_join (cr, CAIRO_LINE_JOIN_MITER);

	if (area) 
	{
		cairo_rectangle (cr, area->x, area->y, area->width, area->height);
		cairo_clip_preserve (cr);
		cairo_new_path (cr);
	}

	return cr;
}

void 
ge_cairo_set_color (cairo_t *cr, CairoColor *color)
{
	g_return_if_fail (cr && color);

	cairo_set_source_rgba (cr, color->r, color->g, color->b, color->a);	
}

/* KEEP IN MIND, MIRROR TAKES PLACE BEFORE ROTATION */
/* ROTATES ANTI-CLOCKWISE, I THINK :P */
/* TODO: Do I really need THREE matrices? */
void
ge_cairo_rotate_mirror_translate (cairo_t *cr, 
					double radius, double x, double y,
					gboolean mirror_horizontally, 
					gboolean mirror_vertically)
{
	cairo_matrix_t matrix_rotate;
	cairo_matrix_t matrix_mirror;
	cairo_matrix_t matrix_result;
	
	double r_cos = cos(radius);
	double r_sin = sin(radius);
	
	cairo_matrix_init (&matrix_rotate, r_cos,
	                                   r_sin,
	                                   r_sin,
	                                   r_cos,
	                                   x, y);
	
	cairo_matrix_init (&matrix_mirror, mirror_horizontally ? -1 : 1,
	                                   0,
	                                   0,
	                                   mirror_vertically ? -1 : 1,
									   0, 0);

	cairo_matrix_multiply (&matrix_result, &matrix_mirror, &matrix_rotate);

	cairo_set_matrix (cr, &matrix_result);
}

void
ge_cairo_rounded_rectangle (cairo_t *cr,
                                 double x, double y, double w, double h,
                                 double radius, CairoCorners corners)
{
	g_return_if_fail (cr != NULL);

	if (corners & CR_CORNER_TOPLEFT)
		cairo_move_to (cr, x+radius, y);
	else
		cairo_move_to (cr, x, y);
	
	if (corners & CR_CORNER_TOPRIGHT)
		cairo_arc (cr, x+w-radius, y+radius, radius, M_PI * 1.5, M_PI * 2);
	else
		cairo_line_to (cr, x+w, y);
	
	if (corners & CR_CORNER_BOTTOMRIGHT)
		cairo_arc (cr, x+w-radius, y+h-radius, radius, 0, M_PI * 0.5);
	else
		cairo_line_to (cr, x+w, y+h);
	
	if (corners & CR_CORNER_BOTTOMLEFT)
		cairo_arc (cr, x+radius,   y+h-radius, radius, M_PI * 0.5, M_PI);
	else
		cairo_line_to (cr, x, y+h);
	
	if (corners & CR_CORNER_TOPLEFT)
		cairo_arc (cr, x+radius,   y+radius,   radius, M_PI, M_PI * 1.5);
	else
		cairo_line_to (cr, x, y);
}

/***********************************************
 * ge_cairo_simple_border -
 *  
 *   A simple routine to draw thin squared
 *   borders with a topleft and bottomright color.
 *    
 *   It originated in Smooth-Engine.
 ***********************************************/
void
ge_cairo_simple_border (cairo_t *cr,
				CairoColor * tl, CairoColor * br,
				gint x,	gint y, gint width, gint height, 
				gboolean topleft_overlap)
{
	g_return_if_fail (cr != NULL);
	g_return_if_fail (tl != NULL);
	g_return_if_fail (br != NULL);
	

	gboolean solid_color = (tl == br) || ((tl->r == br->r) && (tl->g == br->g) && (tl->b == br->b) && (tl->a == br->a));

	topleft_overlap &= !solid_color;

	cairo_save(cr);

	cairo_set_line_width (cr, 1);

	if (topleft_overlap)
	{
		ge_cairo_set_color(cr, br);	

		cairo_move_to(cr, x + 0.5, y + height - 0.5);
		cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
		cairo_line_to(cr, x + width - 0.5, y + 0.5);

		cairo_stroke(cr);
	}
 
	ge_cairo_set_color(cr, tl);	

	cairo_move_to(cr, x + 0.5, y + height - 0.5);
	cairo_line_to(cr, x + 0.5, y + 0.5);
	cairo_line_to(cr, x + width - 0.5, y + 0.5);

	if (!topleft_overlap)
	{
		if (!solid_color)
		{
			cairo_stroke(cr);
			ge_cairo_set_color(cr, br);	
		}

		cairo_move_to(cr, x + 0.5, y + height - 0.5);
		cairo_line_to(cr, x + width - 0.5, y + height - 0.5);
		cairo_line_to(cr, x + width - 0.5, y + 0.5);
	}

	cairo_stroke(cr);

	cairo_restore(cr);
}

void ge_cairo_polygon (cairo_t *cr,
				CairoColor *color,
				GdkPoint *points,
				gint npoints)
{
	int i = 0;

	cairo_save(cr);

	ge_cairo_set_color(cr, color);	
	cairo_move_to(cr, points[0].x, points[0].y);

	for (i = 1; i < npoints; i++)
	{
		if (!((points[i].x == points[i + 1].x) &&
		    (points[i].y == points[i + 1].y))) 
		{
			cairo_line_to(cr, points[i].x, points[i].y);
		}
	}
	
	if ((points[npoints-1].x != points[0].y) ||
		(points[npoints-1].y != points[0].y))
	{
		cairo_line_to(cr, points[0].x, points[0].y);
	}

	cairo_fill(cr);

	cairo_restore(cr);
}

void ge_cairo_line (cairo_t *cr,
			CairoColor *color,
			gint x1,
			gint y1,
			gint x2,
			gint y2)
{ 
	cairo_save(cr);

	ge_cairo_set_color(cr, color);	
	cairo_set_line_width (cr, 1);

	cairo_move_to(cr, x1 + 0.5, y1 + 0.5);
	cairo_line_to(cr, x2 + 0.5, y2 + 0.5);

	cairo_stroke(cr);

	cairo_restore(cr);
}
