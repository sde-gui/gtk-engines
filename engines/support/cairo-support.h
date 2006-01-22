/* Helpful functions when dealing with cairo in gtk engines */

#include <gtk/gtk.h>

typedef struct
{
	gdouble r;
	gdouble g;
	gdouble b;
	gdouble a;
} CairoColor;

void ge_gdk_color_to_cairo (GdkColor * gc, CairoColor * cc);
void ge_shade_color (const CairoColor *a, CairoColor *b, gdouble k);
