/* Helpful functions when dealing with cairo in gtk engines */

#include <gtk/gtk.h>

typedef struct
{
	gdouble r;
	gdouble g;
	gdouble b;
	gdouble a;
} CairoColor;

typedef enum
{
	CR_CORNER_NONE        = 0,
	CR_CORNER_TOPLEFT     = 1,
	CR_CORNER_TOPRIGHT    = 2,
	CR_CORNER_BOTTOMLEFT  = 4,
	CR_CORNER_BOTTOMRIGHT = 8,
	CR_CORNER_ALL         = 15
} CairoCorners;


void ge_gdk_color_to_cairo (GdkColor * gc, CairoColor * cc);
void ge_shade_color (const CairoColor *a, CairoColor *b, gdouble k);
void ge_saturate_color (const CairoColor * a, CairoColor * b, gdouble k);
void ge_cairo_rounded_rectangle (cairo_t *cr, double x, double y, double w, double h, double radius, CairoCorners corners);
