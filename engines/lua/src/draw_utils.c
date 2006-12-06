#include "draw_utils.h"

static void
rounded_rectangle (cairo_t *cr,
                   gdouble x, gdouble y, gdouble w, gdouble h, gdouble radius,
                   gboolean tl, gboolean tr, gboolean bl, gboolean br)
{
	if (tl)
		cairo_move_to (cr, x+radius, y);
	else
		cairo_move_to (cr, x, y);
	
	if (tr)
		cairo_arc (cr, x+w-radius, y+radius, radius, G_PI * 1.5, G_PI * 2);
	else
		cairo_line_to (cr, x+w, y);
	
	if (br)
		cairo_arc (cr, x+w-radius, y+h-radius, radius, 0, G_PI * 0.5);
	else
		cairo_line_to (cr, x+w, y+h);
	
	if (bl)
		cairo_arc (cr, x+radius,   y+h-radius, radius, G_PI * 0.5, G_PI);
	else
		cairo_line_to (cr, x, y+h);
	
	if (tl)
		cairo_arc (cr, x+radius,   y+radius,   radius, G_PI, G_PI * 1.5);
	else
		cairo_line_to (cr, x, y);
}

void
draw_rectangle (cairo_t *cr, gdouble x, gdouble y, gdouble width, gdouble height,
                gdouble radius, gboolean tl, gboolean tr, gboolean bl, gboolean br,
                gboolean filled)
{
	if (radius)
		rounded_rectangle (cr, x, y, width, height, radius, tl, tr, bl, br);
	else
		cairo_rectangle (cr, x, y, width, height);
	
	if (filled)
		cairo_fill (cr);
	else
		cairo_stroke (cr);
}
