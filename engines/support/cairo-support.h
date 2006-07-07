/* Helpful functions when dealing with cairo in gtk engines */

#include <gtk/gtk.h>

typedef struct
{
	gdouble r;
	gdouble g;
	gdouble b;
	gdouble a;
} CairoColor;

typedef struct
{
	CairoColor bg[5];
	CairoColor fg[5];

	CairoColor dark[5];
	CairoColor light[5];
	CairoColor mid[5];

	CairoColor base[5];
	CairoColor text[5];
	CairoColor text_aa[5];

	CairoColor black;
	CairoColor white;
} CairoColorCube;

typedef enum
{
	CR_CORNER_NONE        = 0,
	CR_CORNER_TOPLEFT     = 1,
	CR_CORNER_TOPRIGHT    = 2,
	CR_CORNER_BOTTOMLEFT  = 4,
	CR_CORNER_BOTTOMRIGHT = 8,
	CR_CORNER_ALL         = 15
} CairoCorners;

void ge_hsb_from_color (const CairoColor *color, gdouble *hue, gdouble *saturation, gdouble *brightness);
void ge_color_from_hsb (gdouble hue, gdouble saturation, gdouble brightness, CairoColor *color);

void ge_gdk_color_to_cairo (GdkColor * gc, CairoColor * cc) G_GNUC_INTERNAL;
void ge_cairo_color_to_gtk (CairoColor *cc, GdkColor *c) G_GNUC_INTERNAL;
void ge_gtk_style_to_cairo_color_cube (GtkStyle * style, CairoColorCube *cube) G_GNUC_INTERNAL;

void ge_shade_color(const CairoColor *base, gdouble shade_ratio, CairoColor *composite) G_GNUC_INTERNAL;
void ge_saturate_color (const CairoColor * base, gdouble saturate_level, CairoColor *composite) G_GNUC_INTERNAL;

cairo_t * ge_gdk_drawable_to_cairo (GdkDrawable  *window, GdkRectangle *area) G_GNUC_INTERNAL;
void ge_cairo_set_color (cairo_t *cr, CairoColor *color) G_GNUC_INTERNAL;

void ge_cairo_rounded_rectangle (cairo_t *cr, double x, double y, double w, double h, double radius, CairoCorners corners) G_GNUC_INTERNAL;

void ge_cairo_simple_border (cairo_t *cr, CairoColor * tl, CairoColor * br, gint x, gint y, gint width, gint height, gboolean topleft_overlap) G_GNUC_INTERNAL;

void ge_cairo_line (cairo_t *cr, CairoColor *color, gint x1, gint y1, gint x2, gint y2) G_GNUC_INTERNAL;
void ge_cairo_polygon (cairo_t *cr, CairoColor *color, GdkPoint *points, gint npoints) G_GNUC_INTERNAL;
