#include "crux-draw.h"
#include "crux-style.h"
#include "crux-rc-style.h"
#include "crux-common.h"
#include "crux-pixmaps.h"

#include "cairo-support.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

#define DEBUG 0

#define DETAIL(xx) ((detail) != 0 && strcmp (xx, detail) == 0)

GtkStyleClass *parent_style_class;

static void
draw_hline (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail,
	    gint x1, gint x2, gint y);

static void
draw_vline (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GdkRectangle *area,
	    GtkWidget *widget, const gchar *detail, gint y1, gint y2, gint x);

static void
draw_shadow (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail, gint x, gint y, gint width, gint height);

static void
draw_box (GtkStyle *style,
	  GdkWindow *window,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GdkRectangle *area,
	  GtkWidget *widget,
	  const gchar *detail, gint x, gint y, gint width, gint height);

static void
draw_polygon (GtkStyle *style,
	      GdkWindow *window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle *area,
	      GtkWidget *widget,
	      const gchar *detail, GdkPoint *points, gint npoints, gint fill);

static void
draw_arrow (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail,
	    GtkArrowType arrow_type,
	    gint fill, gint x, gint y, gint width, gint height);

static void
draw_diamond (GtkStyle *style,
	      GdkWindow *window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle *area,
	      GtkWidget *widget,
	      const gchar *detail, gint x, gint y, gint width, gint height);

static void
draw_string (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail, gint x, gint y, const gchar *string);
#if 0
static void
draw_flat_box (GtkStyle *style,
	       GdkWindow *window,
	       GtkStateType state_type,
	       GtkShadowType shadow_type,
	       GdkRectangle *area,
	       GtkWidget *widget,
	       const gchar *detail, gint x, gint y, gint width, gint height);
#endif

static void
draw_check (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail, gint x, gint y, gint width, gint height);
static void
draw_option (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail, gint x, gint y, gint width, gint height);

static void
draw_tab (GtkStyle *style,
	  GdkWindow *window,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GdkRectangle *area,
	  GtkWidget *widget,
	  const gchar *detail, gint x, gint y, gint width, gint height);

static void
draw_shadow_gap (GtkStyle *style,
		 GdkWindow *window,
		 GtkStateType state_type,
		 GtkShadowType shadow_type,
		 GdkRectangle *area,
		 GtkWidget *widget,
		 const gchar *detail,
		 gint x,
		 gint y,
		 gint width,
		 gint height,
		 GtkPositionType gap_side, gint gap_x, gint gap_width);

static void
draw_box_gap (GtkStyle *style,
	      GdkWindow *window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle *area,
	      GtkWidget *widget,
	      const gchar *detail,
	      gint x,
	      gint y,
	      gint width,
	      gint height,
	      GtkPositionType gap_side, gint gap_x, gint gap_width);

static void
draw_extension (GtkStyle *style,
		GdkWindow *window,
		GtkStateType state_type,
		GtkShadowType shadow_type,
		GdkRectangle *area,
		GtkWidget *widget,
		const gchar *detail,
		gint x,
		gint y, gint width, gint height, GtkPositionType gap_side);

static void
draw_focus (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height);

static void
draw_slider (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail,
	     gint x,
	     gint y, gint width, gint height, GtkOrientation orientation);

static void
draw_handle (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail,
	     gint x,
	     gint y, gint width, gint height, GtkOrientation orientation);


static void
paint_shadow (cairo_t *cr, GtkStyle *style,
		    GtkStateType state_type,
		    GtkShadowType shadow_type,
		    gdouble x, gdouble y, gdouble width, gdouble height);


/* utility functions */

static inline void
debug (const char *fmt, ...)
{
    if (DEBUG)
    {
	va_list args;
	va_start (args, fmt);
	vfprintf (stderr, fmt, args);
	va_end (args);
    }
}

/* cairo functions */

static cairo_t *
crux_begin_paint (GdkDrawable *window, GdkRectangle *area)
{
	cairo_t *cr;

	cr = (cairo_t*) gdk_cairo_create (window);

	cairo_set_line_width (cr, 1.0);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_SQUARE);

	if (area)
	{
		cairo_rectangle (cr, area->x, area->y, area->width, area->height);
		cairo_clip (cr);
		cairo_new_path (cr);
	}

	return cr;
}

static void
paint_menuitem (cairo_t *cr, GtkStyle *style, GtkStateType state_type,
	gdouble x, gdouble y, gdouble width, gdouble height)
{
	cairo_pattern_t * crp;
	CairoColor c1;

	/* TODO: this is wasteful, since paint_shadow () adds an extra shadow that is
	 * drawn over by the gradient
	 */
	paint_shadow (cr, style, state_type, GTK_SHADOW_IN, x, y, width, height);

	x += 2.0; y += 2.0; width -= 4.0; height -= 4.0;

	ge_gdk_color_to_cairo (&style->bg[state_type], &c1);
	crp = cairo_pattern_create_linear (x, y, x, y + height);
	cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
	ge_shade_color (&c1, &c1, 1.3);
	cairo_pattern_add_color_stop_rgb (crp, 0.35, c1.r, c1.g, c1.b);
	cairo_pattern_add_color_stop_rgb (crp, 0.28, c1.r, c1.g, c1.b);
	ge_shade_color (&c1, &c1, 0.6);
	cairo_pattern_add_color_stop_rgb (crp, 1.0, c1.r, c1.g, c1.b);
	cairo_rectangle (cr, x, y, width, height);
	cairo_set_source (cr, crp);
	cairo_fill (cr);
	cairo_pattern_destroy (crp);
}

static void
crux_paint_gradient (cairo_t * cr, GdkColor * c, eazel_engine_gradient_direction gradient,
			gdouble x, gdouble y, gdouble width, gdouble height)
{
	cairo_set_line_width (cr, 0.0);
	cairo_pattern_t * crp;
	gdouble r, g, b;

	gdouble lighten, darken;

	lighten = 1.2;
	darken = 0.8;

	r = c->red / 65536.0;
	g = c->green / 65536.0;
	b = c->blue / 65536.0;
	switch (gradient)
	{
		case GRADIENT_VERTICAL:
			crp = cairo_pattern_create_linear (x, y, x, y + height);
			break;
		case GRADIENT_HORIZONTAL:
			crp = cairo_pattern_create_linear (x, y, x + width, y);
			break;
		default: return;
	}

	cairo_pattern_add_color_stop_rgb (crp, 0.0, r*lighten, g*lighten, b*lighten);
	cairo_pattern_add_color_stop_rgb (crp, 1.0, r*darken, g*darken, b*darken);
	cairo_rectangle (cr, x, y, width, height);
	cairo_set_source (cr, crp);
	cairo_fill (cr);
	cairo_pattern_destroy (crp);
}



/* adapted from nautilus-gdk-extensions.c */
static void
interpolate_color (GdkColor *dest, gdouble ratio,
		   GdkColor *start, GdkColor *end)
{
	g_return_if_fail (ratio >= 0.0);
	g_return_if_fail (ratio <= 1.0);

	dest->red = start->red * (1.0 - ratio) + (end->red & 0xFF) * ratio;
	dest->green = start->green * (1.0 - ratio) + end->green * ratio;
	dest->blue = start->blue * (1.0 - ratio) + end->blue * ratio;
}

static void
paint_background_area (GtkStyle *style,
		       GdkWindow *window,
		       GtkStateType state_type,
		       GdkRectangle *area,
		       int x, int y, int width, int height)
{
    GdkGC *gc = style->bg_gc[GTK_STATE_NORMAL];

    if (area != 0)
	gdk_gc_set_clip_rectangle (gc, area);

    gdk_draw_rectangle (window, gc, TRUE, x, y, width, height);

    if (area != 0)
	gdk_gc_set_clip_rectangle (gc, NULL);
}

static void
paint_stock_image (eazel_theme_data *theme_data,
		   eazel_engine_stock_image type,
		   gboolean scaled, gboolean setbg,
		   GtkStyle *style,
		   GdkWindow *window,
		   GtkStateType state_type,
		   GdkRectangle *area,
		   GtkWidget *widget,
		   gint x, gint y, gint width, gint height)
{
    GdkPixmap *pixmap, *mask;

    if (width == -1 || height == -1)
    {
	eazel_engine_stock_get_size (theme_data->stock, type,
				     width == -1 ? &width : NULL,
				     height == -1 ? &height : NULL);
    }

    if (scaled)
    {
	eazel_engine_stock_pixmap_and_mask_scaled (theme_data->stock, type,
						   width, height,
						   gdk_drawable_get_screen (window),
						   &pixmap, &mask);
    }
    else
	eazel_engine_stock_pixmap_and_mask (theme_data->stock, type,
					    gdk_drawable_get_screen (window),
					    &pixmap, &mask);

    /* FIXME GNOME2
       if (gdk_window_get_type (window) == GDK_WINDOW_PIXMAP)
	setbg = FALSE;
    */

    /* FIXME GNOME2 this is probably bad */
    setbg = FALSE;

    if (setbg)
    {
	gdk_draw_pixmap (window, style->fg_gc[state_type], pixmap,
			 0, 0, x, y, width, height);
	if (mask != 0)
	    gdk_window_shape_combine_mask (window, mask, 0, 0);
    }
    else
    {
	int xsrc = 0, ysrc = 0;

	/* Install the mask before clipping.. */
	if (mask != 0)
	{
	    gdk_gc_set_clip_mask (style->fg_gc[state_type], mask);
	    gdk_gc_set_clip_origin (style->fg_gc[state_type], x, y);
	}

	if (area != 0)
	{
	    /* clip by hand to leave gc's clipping for the possible mask */
	    GdkRectangle src = { x, y, width, height }, dest;

	    if (!gdk_rectangle_intersect (&src, area, &dest))
		return;

	    xsrc -= x - dest.x;
	    ysrc -= y - dest.y;
	    x = dest.x;
	    y = dest.y;
	    width = dest.width;
	    height = dest.height;

	}

	if (width > 0 && height > 0)
	{
	    gdk_draw_pixmap (window, style->fg_gc[state_type], pixmap,
			     xsrc, ysrc, x, y, width, height);
	}

	if (mask != 0)
	{
	    gdk_gc_set_clip_mask (style->fg_gc[state_type], NULL);
	    gdk_gc_set_clip_origin (style->fg_gc[state_type], 0, 0);
	}
   }

    eazel_engine_stock_free_pixmaps (theme_data->stock, type, pixmap, mask);
}

static void
paint_button (cairo_t *cr, GtkStyle *style, GtkStateType state_type, GtkShadowType shadow_type,
	gdouble x, gdouble y, gdouble width, gdouble height)
{
	gdouble radius = 2.0;
	cairo_pattern_t *crp;
	CairoColor c1, c2;


	if (state_type == GTK_STATE_INSENSITIVE)
	{
		/* solid fill for insensitive */
		ge_cairo_rounded_rectangle (cr, x, y, width, height, radius, CR_CORNER_ALL);
		gdk_cairo_set_source_color (cr, &style->bg[state_type]);
		cairo_fill (cr);

		/* reduce height so that a shadow appears beneath the button */
		height -= 1.0;
	}

	/* optimise for line drawing */
	x += 0.5; y += 0.5;
	width -= 1.0; height -= 1.0;

	/* calculate start and end colours for the gradient */
	ge_gdk_color_to_cairo (&style->bg[state_type], &c1);
	ge_shade_color (&c1, &c2, 0.9); /* darken */
	ge_shade_color (&c1, &c1, 1.1); /* lighten */

	switch (shadow_type)
	{
		case GTK_SHADOW_NONE:
			return;
		case GTK_SHADOW_IN:
		case GTK_SHADOW_ETCHED_IN:
			ge_cairo_rounded_rectangle (cr, x, y, width, height, radius, CR_CORNER_ALL);
			if (state_type != GTK_STATE_INSENSITIVE)
			{
				/* gradient fill */
				crp = cairo_pattern_create_linear (x, y, x, y +height);
				/*
				 * cairo_pattern_add_color_stop_rgb (crp, 0.0, 186/255.0, 189/255.0, 182/255.0);
				 * cairo_pattern_add_color_stop_rgb (crp, 1.0, 136/255.0, 138/255.0, 133/255.0);
				 */

				cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
				cairo_pattern_add_color_stop_rgb (crp, 1.0, c2.r, c2.g, c2.b);
				cairo_set_source (cr, crp);
				cairo_fill_preserve (cr);
				cairo_pattern_destroy (crp);
			}

			/* outline */
			if (state_type == GTK_STATE_INSENSITIVE)
				/* disabled gray stroke */
				cairo_set_source_rgb (cr, 136/255.0, 138/255.0, 133/255.0);
			else
				/* dark gray */
				cairo_set_source_rgb (cr, 46/255.0, 52/255.0, 54/255.0);
			cairo_stroke (cr);

			/* inside shadow */
			radius = 1.0;
			cairo_move_to (cr, x + 1.0, y + height - 1.0);
			cairo_arc (cr, x + radius + 1.0, y + radius + 1.0, radius, M_PI, M_PI * 1.5);
			cairo_line_to (cr, x + width - 1.0, y + 1.0);
			cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.22);
			cairo_stroke (cr);

			cairo_move_to (cr, x + 2.0, y + height - 1.0);
			cairo_line_to (cr, x + 2.0, y + 2.0);
			cairo_line_to (cr, x + width - 1.0, y + 2.0);
			cairo_line_to (cr, x + width - 1.0, y + height - 1.0);
			cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.12);
			cairo_stroke (cr);

			cairo_rectangle (cr, x + 3.0, y + 3.0, width - 5.0, height - 4.0);
			cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.03);
			cairo_stroke (cr);
			break;
		case GTK_SHADOW_OUT:
		case GTK_SHADOW_ETCHED_OUT:
			if (state_type == GTK_STATE_INSENSITIVE)
			{
				ge_cairo_rounded_rectangle (cr, x, y, width, height , radius, CR_CORNER_ALL);
				/* disabled gray stroke */
				cairo_set_source_rgb (cr, 136/255.0, 138/255.0, 133/255.0);
				cairo_stroke (cr);
			}
			else
			{
				ge_cairo_rounded_rectangle (cr, x, y, width, height, radius, CR_CORNER_ALL);

				crp = cairo_pattern_create_linear (x, y+2.0, x, y+height-2.0);

				cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
				cairo_pattern_add_color_stop_rgb (crp, 1.0, c2.r, c2.g, c2.b);
				/*
				cairo_pattern_add_color_stop_rgb (crp, 0.0, 238/255.0, 238/255.0, 236/255.0);
				cairo_pattern_add_color_stop_rgb (crp, 1.0, 186/255.0, 189/255.0, 182/255.0);
				*/
				cairo_set_source (cr, crp);
				cairo_fill_preserve (cr);
				cairo_pattern_destroy (crp);

				/* dark gray */
				cairo_set_source_rgb (cr, 46/255.0, 52/255.0, 54/255.0);
				cairo_stroke (cr);

				ge_cairo_rounded_rectangle (cr, x+1.0, y+1.0, width-2.0, height-2.0, radius, CR_CORNER_ALL);
				crp = cairo_pattern_create_linear (x, y, x, y + height);
				cairo_pattern_add_color_stop_rgb (crp, 0.0, 1.0, 1.0, 1.0);
				cairo_pattern_add_color_stop_rgba (crp, 1.0, 1.0, 1.0, 1.0, 100/255.0);
				cairo_set_source (cr, crp);
				cairo_stroke (cr);
				cairo_pattern_destroy (crp);
			}
			break;
	}
}

static void
paint_shadow (cairo_t *cr, GtkStyle *style,
		    GtkStateType state_type,
		    GtkShadowType shadow_type,
		    gdouble x, gdouble y, gdouble width, gdouble height)
{
	cairo_pattern_t *crp;

	x += 0.5; y += 0.5;
	width -= 1.0; height -= 1.0;

	/* outer rectangle */
	cairo_rectangle (cr, x, y, width, height);
	crp = cairo_pattern_create_linear (x, y, x, y + height);
	if (shadow_type == GTK_SHADOW_OUT || shadow_type == GTK_SHADOW_ETCHED_OUT)
	{
		CairoColor c1, c2;
		if (state_type == GTK_STATE_SELECTED)
		{
			ge_gdk_color_to_cairo (&style->bg[state_type], &c1);
			ge_saturate_color (&c1, &c2, 1.2); /* darken */
			//ge_saturate_color (&c1, &c1, 0.8); /* darken */
			cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
			cairo_pattern_add_color_stop_rgb (crp, 1.0, c2.r, c2.g, c2.b);
		}
		else
		{
			// TODO: Find a way to calculate these values
			cairo_pattern_add_color_stop_rgb (crp, 0.0, 85/255.0, 87/255.0, 83/255.0);
			cairo_pattern_add_color_stop_rgb (crp, 1.0, 46/255.0, 52/255.0, 54/255.0);
		}
	}
	else if (shadow_type == GTK_SHADOW_IN || shadow_type == GTK_SHADOW_ETCHED_IN)
	{
		/*
		cairo_pattern_add_color_stop_rgb (crp, 0.0, 186/255.0, 189/255.0, 182/255.0);
		cairo_pattern_add_color_stop_rgb (crp, 1.0, 238/255.0, 238/255.0, 236/255.0);
		*/
		cairo_pattern_add_color_stop_rgba (crp, 1.0, 1.0, 1.0, 1.0, 0.5);
		cairo_pattern_add_color_stop_rgba (crp, 0.0, .0, .0, .0, 0.2);
	}
	cairo_set_source (cr, crp);
	cairo_stroke (cr);
	cairo_pattern_destroy (crp);

	/* inner rectangle */
	x += 1.0; y += 1.0;
	width -= 2.0; height -= 2.0;

	cairo_rectangle (cr, x, y, width, height);

	/* stroke */
	if (shadow_type == GTK_SHADOW_OUT || shadow_type == GTK_SHADOW_ETCHED_OUT)
	{
		crp = cairo_pattern_create_linear (x, y, x, y + height);
		/*
		cairo_pattern_add_color_stop_rgb (crp, 0.0, 1.0, 1.0, 1.0);
		cairo_pattern_add_color_stop_rgb (crp, 1.0, 136/255.0, 138/255.0, 133/255.0);
		*/
		cairo_pattern_add_color_stop_rgba (crp, 0.0, 1.0, 1.0, 1.0, 0.5);
		cairo_pattern_add_color_stop_rgba (crp, 1.0, .0, .0, .0, 0.2);
		cairo_set_source (cr, crp);
		cairo_stroke (cr);
		cairo_pattern_destroy (crp);
	}
	else if (shadow_type == GTK_SHADOW_IN || shadow_type == GTK_SHADOW_ETCHED_IN)
	{
		// TODO: Find a way to calculate this value (same as shadow out outer line)
		cairo_set_source_rgb (cr, 46/255.0, 52/255.0, 54/255.0);
		cairo_stroke (cr);

		// three lines of alpha: 0.22, 0.12, 0.03
		cairo_move_to (cr, x + 1.0, y + height);
		cairo_line_to (cr, x + 1.0, y + 1.0);
		cairo_line_to (cr, x + width - 1.0, y + 1.0);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.22);
		cairo_stroke (cr);

		cairo_move_to (cr, x + 2.0, y + height);
		cairo_line_to (cr, x + 2.0, y + 2.0);
		cairo_line_to (cr, x + width - 1.0, y + 2.0);
		cairo_line_to (cr, x + width - 1.0, y + height - 1.0);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.12);
		cairo_stroke (cr);

		cairo_rectangle (cr, x + 3.0, y + 3.0, width - 5.0, height - 4.0);
		cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.03);
		cairo_stroke (cr);
	}


}

static void
paint_entry_shadow (cairo_t *cr, GtkStyle *style,
		    GtkStateType state_type,
		    gboolean focused,
		    gdouble x, gdouble y, gdouble width, gdouble height)
{

	cairo_pattern_t *crp;

	x += 0.5; y += 0.5;
	width -= 1.0; height -= 1.0;


	/* Outer Line */
	cairo_rectangle (cr, x, y, width, height);
	if (focused)
	{
		gdk_cairo_set_source_color (cr, &style->light[GTK_STATE_SELECTED]);
		cairo_stroke (cr);
	}
	else
	{
		crp = cairo_pattern_create_linear (x, y, x, y + height);
		/* TODO: Find a way to calculate these values */
		cairo_pattern_add_color_stop_rgb (crp, 0.0, 185/255.0, 189/255.0, 182/255.0);
		cairo_pattern_add_color_stop_rgb (crp, 1.0, 238/255.0, 238/255.0, 236/255.0);
		cairo_set_source (cr, crp);
		cairo_stroke (cr);
		cairo_pattern_destroy (crp);
	}

	/* inner Line */
	cairo_rectangle (cr, x + 1.0, y + 1.0, width - 2.0, height - 2.0);
	/* TODO: Find a way to calculate these values */
	cairo_set_source_rgb (cr, 54/255.0, 52/255.0, 54/255.0);
	cairo_stroke (cr);

	/* inner shadow */
	cairo_move_to (cr, x + 2.0, y + height - 4.0);
	cairo_line_to (cr, x + 2.0, y + 2.0);
	cairo_line_to (cr, x + width - 4.0, y + 2.0);
	cairo_set_source_rgba (cr, 0, 0, 0, 0.12);
	cairo_stroke (cr);
}

static void
paint_progress_bar (cairo_t *cr, GtkStyle *style, GtkStateType state_type, GtkProgressBarOrientation orientation,
		    gdouble x, gdouble y, gdouble width, gdouble height)
{
	cairo_pattern_t *crp;
	CairoColor c1, c2;

	cairo_rectangle (cr, x, y, width, height);

	ge_gdk_color_to_cairo (&style->base[GTK_STATE_SELECTED], &c1);
	ge_shade_color (&c1, &c2, 0.8); /* darken */
	ge_shade_color (&c1, &c1, 1.4); /* lighten */
	if (orientation == GTK_PROGRESS_LEFT_TO_RIGHT || orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
		crp = cairo_pattern_create_linear (x, y, x, y + height);
	else
		crp = cairo_pattern_create_linear (x, y, x + width, y);

	cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
	cairo_pattern_add_color_stop_rgb (crp, 1.0, c2.r, c2.g, c2.b);

	cairo_set_source (cr, crp);
	cairo_fill (cr);
	cairo_pattern_destroy (crp);

	/* set co-ordinates for line drawing */
	x += 0.5; y += 0.5; width -= 1.0; height -= 1.0;

	/* bevel */
	cairo_rectangle (cr, x, y, width, height);
	crp = cairo_pattern_create_linear (x, y, x, y + height);
	cairo_pattern_add_color_stop_rgba (crp, 0.0, 1.0, 1.0, 1.0, .3);
	cairo_pattern_add_color_stop_rgba (crp, 1.0, 1.0, 1.0, 1.0, 0.1);
	cairo_set_source (cr, crp);
	cairo_stroke (cr);

	cairo_pattern_destroy (crp);


	/* end cap */

	cairo_set_source_rgb (cr, c2.r, c2.g, c2.b);
	switch (orientation)
	{
		case GTK_PROGRESS_LEFT_TO_RIGHT:
			cairo_move_to (cr, x + width + 1.0, y);
			cairo_line_to (cr, x + width + 1.0, y + height);
			break;
		case GTK_PROGRESS_RIGHT_TO_LEFT:
			cairo_move_to (cr, x - 1.0, y);
			cairo_line_to (cr, x - 1.0, y + height);
			break;
		case GTK_PROGRESS_TOP_TO_BOTTOM:
			cairo_move_to (cr, x, y + height + 1.0);
			cairo_line_to (cr, x + width, y + height + 1.0);
			break;
		case GTK_PROGRESS_BOTTOM_TO_TOP:
			cairo_move_to (cr, x, y - 1.0);
			cairo_line_to (cr, x + width, y - 1.0);
			break;
	}


	cairo_stroke (cr);


	/* draw the end cap shadow
	 * TODO: create shadows for all progress bars (currently only works for left to right)
	 *
	 * three lines of alpha: 0.22, 0.12, 0.03
	 * TODO: this doesn't work quite right, as the top part of the shadow
	 *       is already drawn by draw_box (), so the corner looks too dark
	 */

	if (orientation == GTK_PROGRESS_LEFT_TO_RIGHT)
	{
		cairo_set_source_rgba (cr, 0, 0, 0, .22);
		cairo_move_to (cr, x + width + 2.0, y);
		cairo_line_to (cr, x + width + 2.0, y + height);
		cairo_stroke (cr);

		cairo_set_source_rgba (cr, 0, 0, 0, .12);
		cairo_move_to (cr, x + width + 3.0, y);
		cairo_line_to (cr, x + width + 3.0, y + height);
		cairo_stroke (cr);

		cairo_set_source_rgba (cr, 0, 0, 0, .03);
		cairo_move_to (cr, x + width + 4.0, y);
		cairo_line_to (cr, x + width + 4.0, y + height);
		cairo_stroke (cr);
	}
}

static void
paint_arrow (GdkWindow *window, GdkGC *gc, GtkArrowType arrow_type,
	     int x, int y, int width, int height)
{
    int half_width, half_height;
    int center_x, center_y;

    if ((width & 1) == 0)
	width = width - 1;
    if ((height & 1) == 0)
	height = height - 1;

    half_width = width / 2;
    half_height = height / 2;

    center_x = x + half_width;
    center_y = y + half_height;

    switch (arrow_type)
    {
	int i;
	static int offset[5] = { 0, -1, -2, -3, -4 };
	static int length[5] = { 0, 2, 4, 6, 8 };
	static const int size = 4;

    case GTK_ARROW_UP:
	for (i = 0; i < size; i++)
	{
	    gdk_draw_line (window, gc,
			   center_x + offset[i],
			   center_y - 2 + i,
			   center_x + offset[i] + length[i],
			   center_y - 2 + i);
	}
	break;

    case GTK_ARROW_DOWN:
	for (i = 0; i < size; i++)
	{
	    gdk_draw_line (window, gc,
			   center_x + offset[i],
			   center_y + 2 - i,
			   center_x + offset[i] + length[i],
			   center_y + 2 - i);
	}
	break;

    case GTK_ARROW_LEFT:
	for (i = 0; i < size; i++)
	{
	    gdk_draw_line (window, gc,
			   center_x - 2 + i,
			   center_y + offset[i],
			   center_x - 2 + i,
			   center_y + offset[i] + length[i]);
	}
	break;

    case GTK_ARROW_RIGHT:
	for (i = 0; i < size; i++)
	{
	    gdk_draw_line (window, gc,
			   center_x + 2 - i,
			   center_y + offset[i],
			   center_x + 2 - i,
			   center_y + offset[i] + length[i]);
	}
	break;
    }
}

/* style functions */

static void
draw_hline (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail,
	    gint x1, gint x2, gint y)
{
    gint thickness_light;
    gint thickness_dark;
    gint i;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    debug ("draw_hline: detail=%s state=%d x1=%d x2=%d y=%d\n",
	    detail, state_type, x1, x2, y);

    thickness_light = style->ythickness / 2;
    thickness_dark = style->ythickness - thickness_light;

    if (area)
    {
	gdk_gc_set_clip_rectangle (style->light_gc[state_type], area);
	gdk_gc_set_clip_rectangle (style->dark_gc[state_type], area);
    }
    for (i = 0; i < thickness_dark; i++)
    {
	gdk_draw_line (window, style->light_gc[state_type], x2 - i - 1,
		       y + i, x2, y + i);
	gdk_draw_line (window, style->dark_gc[state_type], x1, y + i,
		       x2 - i - 1, y + i);
    }

    y += thickness_dark;
    for (i = 0; i < thickness_light; i++)
    {
	gdk_draw_line (window, style->dark_gc[state_type], x1, y + i,
		       x1 + thickness_light - i - 1, y + i);
	gdk_draw_line (window, style->light_gc[state_type],
		       x1 + thickness_light - i - 1, y + i, x2, y + i);
    }
    if (area)
    {
	gdk_gc_set_clip_rectangle (style->light_gc[state_type], NULL);
	gdk_gc_set_clip_rectangle (style->dark_gc[state_type], NULL);
    }
}

static void
draw_vline (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GdkRectangle *area,
	    GtkWidget *widget, const gchar *detail, gint y1, gint y2, gint x)
{
    gint thickness_light;
    gint thickness_dark;
    gint i;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    debug ("draw_vline: detail=%s state=%d x=%d y1=%d y2=%d\n",
	    detail, state_type, x, y1, y2);

    thickness_light = style->xthickness / 2;
    thickness_dark = style->xthickness - thickness_light;

    if (area)
    {
	gdk_gc_set_clip_rectangle (style->light_gc[state_type], area);
	gdk_gc_set_clip_rectangle (style->dark_gc[state_type], area);
    }
    for (i = 0; i < thickness_dark; i++)
    {
	gdk_draw_line (window, style->light_gc[state_type], x + i,
		       y2 - i - 1, x + i, y2);
	gdk_draw_line (window, style->dark_gc[state_type], x + i, y1, x + i,
		       y2 - i - 1);
    }

    x += thickness_dark;
    for (i = 0; i < thickness_light; i++)
    {
	gdk_draw_line (window, style->dark_gc[state_type], x + i, y1, x + i,
		       y1 + thickness_light - i);
	gdk_draw_line (window, style->light_gc[state_type], x + i,
		       y1 + thickness_light - i, x + i, y2);
    }
    if (area)
    {
	gdk_gc_set_clip_rectangle (style->light_gc[state_type], NULL);
	gdk_gc_set_clip_rectangle (style->dark_gc[state_type], NULL);
    }
}

static void
draw_shadow (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail, gint x, gint y, gint width, gint height)
{
    GdkGC *gc1 = NULL;
    GdkGC *gc2 = NULL;
    GdkGC *gc3 = NULL;
    GdkGC *gc4 = NULL;
    GdkGC *gc_a, *gc_b, *gc_c, *gc_d;
    gint thickness_light;
    gint thickness_dark;
    gboolean rounded = FALSE;
    gboolean outline = TRUE;
    gboolean focused = FALSE;
    gint i;

    eazel_theme_data *theme_data;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    theme_data = CRUX_RC_STYLE(style->rc_style)->theme_data;
    g_assert (theme_data != NULL);

    outline = theme_data->outline_shadow;

    if (widget != 0 && !GTK_WIDGET_IS_SENSITIVE (widget))
	state_type = GTK_STATE_INSENSITIVE;

    debug ("draw_shadow: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

    /* Protection against broken GTK+ widgets */
    g_return_if_fail (width < 32768);
    g_return_if_fail (height < 32768);
    g_return_if_fail (width >= -1);
    g_return_if_fail (height >= -1);

    if ((width == -1) && (height == -1))
	gdk_window_get_size (window, &width, &height);
    else if (width == -1)
	gdk_window_get_size (window, &width, NULL);
    else if (height == -1)
	gdk_window_get_size (window, NULL, &height);


	if (widget != NULL && GTK_WIDGET_HAS_FOCUS (widget))
		focused = TRUE;

	cairo_t *cr;
	cr = crux_begin_paint (window, area);

	if (DETAIL ("entry"))
		paint_entry_shadow (cr, style, state_type, focused, x, y, width, height);
	else
		paint_shadow (cr, style, state_type, shadow_type, x, y, width, height);
	cairo_destroy (cr);
}

static void
draw_box (GtkStyle *style,
	  GdkWindow *window,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GdkRectangle *area,
	  GtkWidget *widget,
	  const gchar *detail, gint x, gint y, gint width, gint height)
{

	cairo_t *cr;
	gdouble cx, cy, cw, ch;

	g_return_if_fail (style != NULL);
	g_return_if_fail (window != NULL);

    /* Protection against broken GTK+ widgets */
    g_return_if_fail (width < 32768);
    g_return_if_fail (height < 32768);
    g_return_if_fail (width >= -1);
    g_return_if_fail (height >= -1);

    if ((width == -1) && (height == -1))
	gdk_window_get_size (window, &width, &height);
    else if (width == -1)
	gdk_window_get_size (window, &width, NULL);
    else if (height == -1)
	gdk_window_get_size (window, NULL, &height);

    if (DETAIL ("spinbutton")) {
      if (y > (height / 3)) {
	/* this is a down arrow shadow */
	y += 2;
      }
    }

    debug ("draw_box: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

	cr = crux_begin_paint (window, area);

	if (DETAIL ("button") || DETAIL ("optionmenu"))
		paint_button (cr, style, state_type, shadow_type, x, y, width, height);
	else if (DETAIL ("menuitem"))
		paint_menuitem (cr, style, state_type, x, y, width, height);
	else if (DETAIL ("menu"))
	{

		cx = x + 0.5; cy = y + 0.5; cw = width - 1.0; ch = height - 1.0;
		cairo_rectangle (cr, cx, cy, cw, ch);
		cairo_set_source_rgb (cr, 54/255.0, 52/255.0, 54/255.0);
		cairo_stroke (cr);

		cx++; cy++; cw -= 2.0; ch -= 2.0;

		gdk_cairo_set_source_color (cr, &style->light[state_type]);
		cairo_move_to (cr, cx, cy + ch);
		cairo_line_to (cr, cx, cy);
		cairo_line_to (cr, cx + cw, cy);
		cairo_stroke (cr);

		gdk_cairo_set_source_color (cr, &style->dark[state_type]);
		cairo_move_to (cr, cx + cw, cy);
		cairo_line_to (cr, cx + cw, cy + ch);
		cairo_line_to (cr, cx, cy + ch);
		cairo_stroke (cr);
	}
	else if (DETAIL ("bar"))
	{
		GtkProgressBarOrientation orientation;
		if (widget && GTK_IS_PROGRESS_BAR (widget))
			orientation = gtk_progress_bar_get_orientation (GTK_PROGRESS_BAR (widget));
		else
			orientation = GTK_PROGRESS_LEFT_TO_RIGHT;
		paint_progress_bar (cr, style, state_type, orientation, x, y, width, height);
	}
	else if (DETAIL ("trough") && GTK_IS_SCALE (widget))
	{
		if (GTK_IS_VSCALE (widget))
		{
			x++; width-=2;
		}
		else
		{
			y += 1;
			height-=2;
		}
		paint_button (cr, style, GTK_STATE_NORMAL, GTK_SHADOW_OUT, x, y, width, height);
	}
	else
	{
		/* fill  */
		cairo_rectangle (cr, x, y, width, height);
		if (shadow_type == GTK_SHADOW_OUT || shadow_type == GTK_SHADOW_ETCHED_OUT)
		{
			cairo_pattern_t *crp;
			CairoColor c1, c2;

			crp = cairo_pattern_create_linear (x, y, x, y + height);
			ge_gdk_color_to_cairo (&style->bg[state_type], &c1);
			ge_shade_color (&c1, &c2, 0.9);
			ge_shade_color (&c1, &c1, 1.1);
			/*
			cairo_pattern_add_color_stop_rgb (crp, 0.0, 238/255.0, 238/255.0, 236/255.0);
			cairo_pattern_add_color_stop_rgb (crp, 1.0, 185/255.0, 189/255.0, 182/255.0);
			*/
			cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
			cairo_pattern_add_color_stop_rgb (crp, 1.0, c2.r, c2.g, c2.b);
			cairo_set_source (cr, crp);
			cairo_fill (cr);
			cairo_pattern_destroy (crp);
		}
		else if (shadow_type == GTK_SHADOW_IN || shadow_type == GTK_SHADOW_ETCHED_IN)
		{
			/* cairo_set_source_rgb (cr, 196/255.0, 198/255.0, 192/255.0); */
			gdk_cairo_set_source_color (cr, &style->bg[state_type]);
			cairo_fill (cr);
		}

		paint_shadow (cr, style, state_type, shadow_type, x, y, width, height);
	}

	cairo_destroy (cr);

}

static void
draw_polygon (GtkStyle *style,
	      GdkWindow *window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle *area,
	      GtkWidget *widget,
	      const gchar *detail, GdkPoint *points, gint npoints, gint fill)
{
#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif /*
        * M_PI
        */
#ifndef M_PI_4
#define M_PI_4  0.78539816339744830962
#endif /*
        * M_PI_4
        */

    static const gdouble pi_over_4 = M_PI_4;
    static const gdouble pi_3_over_4 = M_PI_4 * 3;

    GdkGC *gc1;
    GdkGC *gc2;
    GdkGC *gc3;
    GdkGC *gc4;
    gdouble angle;
    gint xadjust;
    gint yadjust;
    gint i;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);
    g_return_if_fail (points != NULL);

    debug ("draw_polygon: detail=%s state=%d shadow=%d\n",
	    detail, state_type, shadow_type);

    switch (shadow_type)
    {
    case GTK_SHADOW_IN:
	gc1 = style->light_gc[state_type];
	gc2 = style->dark_gc[state_type];
	gc3 = style->light_gc[state_type];
	gc4 = style->dark_gc[state_type];
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
	gc3 = style->dark_gc[state_type];
	gc4 = style->light_gc[state_type];
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
	gdk_draw_polygon (window, style->bg_gc[state_type], TRUE, points,
			  npoints);

    npoints--;

    for (i = 0; i < npoints; i++)
    {
	if ((points[i].x == points[i + 1].x) &&
	    (points[i].y == points[i + 1].y))
	{
	    angle = 0;
	}
	else
	{
	    angle = atan2 (points[i + 1].y - points[i].y,
			   points[i + 1].x - points[i].x);
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
			   points[i].x - xadjust, points[i].y - yadjust,
			   points[i + 1].x - xadjust,
			   points[i + 1].y - yadjust);
	    gdk_draw_line (window, gc3, points[i].x, points[i].y,
			   points[i + 1].x, points[i + 1].y);
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
			   points[i].x + xadjust, points[i].y + yadjust,
			   points[i + 1].x + xadjust,
			   points[i + 1].y + yadjust);
	    gdk_draw_line (window, gc2, points[i].x, points[i].y,
			   points[i + 1].x, points[i + 1].y);
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

static void
draw_arrow (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail,
	    GtkArrowType arrow_type,
	    gint fill, gint x, gint y, gint width, gint height)
{
    eazel_theme_data *theme_data;

    /* FIXME GNOME2: bad hack added to make arrows draw large enough */
    width = width + 8;
    height = height + 6;
    x = x - 4;
    y = y - 3;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);
    g_return_if_fail (width >= -1);
    g_return_if_fail (height >= -1);

    theme_data = CRUX_RC_STYLE (style->rc_style)->theme_data;
    g_assert (theme_data != NULL);

    if ((width == -1) && (height == -1))
	gdk_window_get_size (window, &width, &height);
    else if (width == -1)
	gdk_window_get_size (window, &width, NULL);
    else if (height == -1)
	gdk_window_get_size (window, NULL, &height);

    debug ("draw_arrow: detail=%s state=%d shadow=%d arrow_type=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, arrow_type, x, y, width, height);

    if (DETAIL ("vscrollbar") || DETAIL ("hscrollbar"))
    {
	int type = 0;
	switch (arrow_type)
	{
	case GTK_ARROW_UP:
	    type = EAZEL_ENGINE_ARROW_UP;
	    break;

	case GTK_ARROW_DOWN:
	    type = EAZEL_ENGINE_ARROW_DOWN;
	    break;

	case GTK_ARROW_LEFT:
	    type = EAZEL_ENGINE_ARROW_LEFT;
	    break;

	case GTK_ARROW_RIGHT:
	    type = EAZEL_ENGINE_ARROW_RIGHT;
	    break;
	}

	type += (state_type == GTK_STATE_ACTIVE ? 2
		 : state_type == GTK_STATE_PRELIGHT ? 1 : 0);

	paint_stock_image (theme_data, type, TRUE, FALSE,
			   style, window, state_type,
			   area, widget, x, y, width, height);
    }
    else if (DETAIL ("spinbutton"))
    {
	int window_width, window_height;
	int tem_x, tem_y;

	if (widget != 0 && !GTK_WIDGET_IS_SENSITIVE (widget))
	    state_type = GTK_STATE_INSENSITIVE;

	gdk_window_get_size (window, &window_width, &window_height);

	if (state_type != GTK_STATE_INSENSITIVE)
	{
	    draw_box (style, window, state_type, shadow_type,
		      area, widget, detail,
		      x, y - (arrow_type == GTK_ARROW_DOWN),
		      width, height + 1);
	}

	/*
	  else if (arrow_type == GTK_ARROW_UP)
	  {
	  XXX A hack, assumes that up arrow is drawn before
	  XXX down arrow. (Currently it is)

	  draw_shadow (style, window, GTK_STATE_NORMAL, GTK_SHADOW_OUT,
	  NULL, widget, "entry", x - 2, 0,
	  width + 4, window_height);
	  }
	*/

	tem_x = x + (width / 2);
	tem_y = y + (height / 2);
	if (arrow_type == GTK_ARROW_UP)
	{
	    int i;
	    tem_y--;
	    for (i = 0; i < 4; i++)
	    {
		gdk_draw_line (window, style->fg_gc[state_type],
			       tem_x - i, tem_y + i,
			       tem_x + i, tem_y + i);
	    }
	}
	else
	{
	    int i;
	    tem_y -= 2;
	    for (i = 0; i < 4; i++)
	    {
		gdk_draw_line (window, style->fg_gc[state_type],
			       tem_x - i, tem_y + (4 - i),
			       tem_x + i, tem_y + (4 - i));
	    }
	}
	if (state_type != GTK_STATE_INSENSITIVE)
	{
	  draw_shadow (style, window, GTK_STATE_NORMAL, GTK_SHADOW_OUT,
		       NULL, widget, "entry", x - 2, 0,
		       width + 4, window_height);
	}
    }
    else
    {
	if (widget != 0 && !GTK_WIDGET_IS_SENSITIVE (widget))
	    state_type = GTK_STATE_INSENSITIVE;

	paint_arrow (window, style->fg_gc[state_type],
		     arrow_type, x, y, width, height);
    }
}

static void
draw_diamond (GtkStyle *style,
	      GdkWindow *window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle *area,
	      GtkWidget *widget,
	      const gchar *detail, gint x, gint y, gint width, gint height)
{
    gint half_width;
    gint half_height;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    debug ("draw_diamond: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

    /* Protection against broken GTK+ widgets */
    g_return_if_fail (width < 32768);
    g_return_if_fail (height < 32768);
    g_return_if_fail (width >= -1);
    g_return_if_fail (height >= -1);

    if ((width == -1) && (height == -1))
	gdk_window_get_size (window, &width, &height);
    else if (width == -1)
	gdk_window_get_size (window, &width, NULL);
    else if (height == -1)
	gdk_window_get_size (window, NULL, &height);

    half_width = width / 2;
    half_height = height / 2;

    if (area)
    {
	gdk_gc_set_clip_rectangle (style->light_gc[state_type], area);
	gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);
	gdk_gc_set_clip_rectangle (style->dark_gc[state_type], area);
	gdk_gc_set_clip_rectangle (style->black_gc, area);
    }
    switch (shadow_type)
    {
    case GTK_SHADOW_IN:
	gdk_draw_line (window, style->light_gc[state_type],
		       x + 2, y + half_height,
		       x + half_width, y + height - 2);
	gdk_draw_line (window, style->light_gc[state_type],
		       x + half_width, y + height - 2,
		       x + width - 2, y + half_height);
	gdk_draw_line (window, style->light_gc[state_type],
		       x + 1, y + half_height,
		       x + half_width, y + height - 1);
	gdk_draw_line (window, style->light_gc[state_type],
		       x + half_width, y + height - 1,
		       x + width - 1, y + half_height);
	gdk_draw_line (window, style->light_gc[state_type],
		       x, y + half_height, x + half_width, y + height);
	gdk_draw_line (window, style->light_gc[state_type],
		       x + half_width, y + height,
		       x + width, y + half_height);

	gdk_draw_line (window, style->dark_gc[state_type],
		       x + 2, y + half_height, x + half_width, y + 2);
	gdk_draw_line (window, style->dark_gc[state_type],
		       x + half_width, y + 2, x + width - 2, y + half_height);
	gdk_draw_line (window, style->dark_gc[state_type], x + 1,
		       y + half_height, x + half_width, y + 1);
	gdk_draw_line (window, style->dark_gc[state_type], x + half_width,
		       y + 1, x + width - 1, y + half_height);
	gdk_draw_line (window, style->dark_gc[state_type], x,
		       y + half_height, x + half_width, y);
	gdk_draw_line (window, style->dark_gc[state_type], x + half_width,
		       y, x + width, y + half_height);
	break;
    case GTK_SHADOW_OUT:
	gdk_draw_line (window, style->dark_gc[state_type],
		       x + 2, y + half_height,
		       x + half_width, y + height - 2);
	gdk_draw_line (window, style->dark_gc[state_type],
		       x + half_width, y + height - 2,
		       x + width - 2, y + half_height);
	gdk_draw_line (window, style->dark_gc[state_type],
		       x + 1, y + half_height,
		       x + half_width, y + height - 1);
	gdk_draw_line (window, style->dark_gc[state_type],
		       x + half_width, y + height - 1,
		       x + width - 1, y + half_height);
	gdk_draw_line (window, style->dark_gc[state_type],
		       x, y + half_height, x + half_width, y + height);
	gdk_draw_line (window, style->dark_gc[state_type],
		       x + half_width, y + height,
		       x + width, y + half_height);

	gdk_draw_line (window, style->light_gc[state_type],
		       x + 2, y + half_height, x + half_width, y + 2);
	gdk_draw_line (window, style->light_gc[state_type],
		       x + half_width, y + 2, x + width - 2, y + half_height);
	gdk_draw_line (window, style->light_gc[state_type], x + 1,
		       y + half_height, x + half_width, y + 1);
	gdk_draw_line (window, style->light_gc[state_type], x + half_width,
		       y + 1, x + width - 1, y + half_height);
	gdk_draw_line (window, style->light_gc[state_type], x,
		       y + half_height, x + half_width, y);
	gdk_draw_line (window, style->light_gc[state_type], x + half_width,
		       y, x + width, y + half_height);
	break;
    default:
	break;
    }
    if (area)
    {
	gdk_gc_set_clip_rectangle (style->light_gc[state_type], NULL);
	gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL);
	gdk_gc_set_clip_rectangle (style->dark_gc[state_type], NULL);
	gdk_gc_set_clip_rectangle (style->black_gc, NULL);
    }
}

static void
draw_string (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail, gint x, gint y, const gchar *string)
{
    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    debug ("draw_string: detail=%s state=%d x=%d y=%d\n",
	    detail, state_type, x, y);

    if (area)
    {
	gdk_gc_set_clip_rectangle (style->white_gc, area);
	gdk_gc_set_clip_rectangle (style->fg_gc[state_type], area);
    }

    /* FIXME GNOME2
    if (state_type == GTK_STATE_INSENSITIVE)
	gdk_draw_string (window, style->font, style->white_gc, x + 1, y + 1,
			 string);
    gdk_draw_string (window, style->font, style->fg_gc[state_type], x, y,
		     string);
    */
    if (area)
    {
	gdk_gc_set_clip_rectangle (style->white_gc, NULL);
	gdk_gc_set_clip_rectangle (style->fg_gc[state_type], NULL);
    }
}

#if 0
static void
draw_flat_box (GtkStyle *style,
	       GdkWindow *window,
	       GtkStateType state_type,
	       GtkShadowType shadow_type,
	       GdkRectangle *area,
	       GtkWidget *widget,
	       const gchar *detail, gint x, gint y, gint width, gint height)
{
    GdkGC *gc1;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    /* Protection against broken GTK+ widgets */
    g_return_if_fail (width < 32768);
    g_return_if_fail (height < 32768);
    g_return_if_fail (width >= -1);
    g_return_if_fail (height >= -1);

    if ((width == -1) && (height == -1))
	gdk_window_get_size (window, &width, &height);
    else if (width == -1)
	gdk_window_get_size (window, &width, NULL);
    else if (height == -1)
	gdk_window_get_size (window, NULL, &height);

    debug ("draw_flat_box: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

    gc1 = style->bg_gc[state_type];

    if (DETAIL ("text") && (state_type == GTK_STATE_SELECTED))
	gc1 = style->bg_gc[GTK_STATE_SELECTED];
    else if (DETAIL ("viewportbin"))
	gc1 = style->bg_gc[GTK_STATE_NORMAL];
    else if (DETAIL ("entry_bg"))
	gc1 = style->white_gc;

    if ((!style->bg_pixmap[state_type])
	|| (gc1 != style->bg_gc[state_type])
	/* FIXME GNOME2 || (gdk_window_get_type (window) == GDK_WINDOW_PIXMAP)*/ )
    {
	if (area)
	{
	    gdk_gc_set_clip_rectangle (gc1, area);
	}
	gdk_draw_rectangle (window, gc1, TRUE, x, y, width, height);
	if ((detail) && (!strcmp ("tooltip", detail)))
	    gdk_draw_rectangle (window, style->black_gc, FALSE,
				x, y, width - 1, height - 1);
	if (area)
	{
	    gdk_gc_set_clip_rectangle (gc1, NULL);
	}
    }
    else
	gtk_style_apply_default_pixmap (style, window, state_type, area, x,
					y, width, height);
}
#endif

static void
draw_check (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail, gint x, gint y, gint width, gint height)
{
	cairo_t * cr;
	gdouble cx, cy; /* co-ordinates for cairo */

	cr = crux_begin_paint (window, NULL);

	paint_button (cr, style, state_type, shadow_type, x, y, width, height);

	if (shadow_type != GTK_SHADOW_OUT)
	{
		/* draw tick mark */
		gdk_cairo_set_source_color (cr, &style->fg[state_type]);
		cairo_set_line_width (cr, 2.0);
		cx = x + 0.5; width--;
		cy = y + 0.5; height--;

		/* TODO: draw drop shadow (if state is not insensitive) */

		cairo_move_to (cr, cx + 3.0, cy + height * 0.5);
		cairo_line_to (cr, cx + width * 0.5, cy + height * 0.7);
		cairo_line_to (cr, cx + width - 2.0, cy + 3.0);
		cairo_stroke (cr);
		cairo_fill (cr);
	}

	cairo_destroy (cr);
}

static void
draw_option (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail, gint x, gint y, gint width, gint height)
{
	cairo_t *cr;
	cairo_pattern_t *crp;
	GdkRectangle rect;
	gdouble cx, cy, radius; /* cairo co-ordinates */
	CairoColor c1, c2;


	cx = x + (height / 2) + 0.5;
	cy = y + (height / 2) + 0.5;
	radius = (height / 2) - 1.0;

	rect.x = x; rect.y = y;
	rect.height = rect.width = height;

	cr = crux_begin_paint (window, area);

	cairo_set_line_width (cr, 1.0);
	cairo_arc (cr, cx, cy, radius, 0.0, M_PI * 2);


	if (state_type != GTK_STATE_INSENSITIVE)
	{
		ge_gdk_color_to_cairo (&style->white, &c1);
		ge_shade_color (&c1, &c2, 0.2);

		crp = cairo_pattern_create_linear (x + 0.5, y + 0.5, x + height - 1.0, y + height - 1.0);
		if (shadow_type == GTK_SHADOW_OUT && state_type != GTK_STATE_ACTIVE)
		{
			cairo_pattern_add_color_stop_rgb (crp, 0.3, c1.r, c1.g, c1.b);
			cairo_pattern_add_color_stop_rgb (crp, 1.2, c2.r, c2.g, c2.b);
		}
		else
		{
			cairo_pattern_add_color_stop_rgb (crp, 0.3, c2.r, c2.g, c2.b);
			cairo_pattern_add_color_stop_rgb (crp, 1.2, c1.r, c1.g, c1.b);
		}
		cairo_set_source (cr, crp);
		cairo_fill_preserve (cr);
		cairo_pattern_destroy (crp);
	}

	gdk_cairo_set_source_color (cr, &style->fg[state_type]);
	cairo_stroke (cr);

	if (shadow_type == GTK_SHADOW_IN)
	{
		cairo_set_line_width (cr, 0.0);
		cairo_arc (cr, cx, cy, radius - 2.5, 0.0, M_PI * 2);
		cairo_fill (cr);
	}

	cairo_destroy (cr);
}

static void
draw_tab (GtkStyle *style,
	  GdkWindow *window,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GdkRectangle *area,
	  GtkWidget *widget,
	  const gchar *detail, gint x, gint y, gint width, gint height)
{
	g_return_if_fail (style != NULL);
	g_return_if_fail (window != NULL);

	/* Draws an option menu tab (the up and down pointing arrows)
	 * TODO: Make this look neater
	 */

	draw_arrow (style, window, state_type, shadow_type, area, widget, detail,
		    GTK_ARROW_UP, FALSE, x, y - 2, width, height);

	draw_arrow (style, window, state_type, shadow_type, area, widget, detail,
		    GTK_ARROW_DOWN, FALSE, x, y + 3, width, height);
}

static void
draw_shadow_gap (GtkStyle *style,
		 GdkWindow *window,
		 GtkStateType state_type,
		 GtkShadowType shadow_type,
		 GdkRectangle *area,
		 GtkWidget *widget,
		 const gchar *detail,
		 gint x,
		 gint y,
		 gint width,
		 gint height,
		 GtkPositionType gap_side, gint gap_x, gint gap_width)
{
    GdkRectangle rect;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    debug ("draw_shadow_gap: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

    gtk_paint_shadow (style, window, state_type, shadow_type, area, widget,
		      detail, x, y, width, height);

    switch (gap_side)
    {
    case GTK_POS_TOP:
	rect.x = x + gap_x;
	rect.y = y;
	rect.width = gap_width;
	rect.height = 2;
	break;
    case GTK_POS_BOTTOM:
	rect.x = x + gap_x;
	rect.y = y + height - 2;
	rect.width = gap_width;
	rect.height = 2;
	break;
    case GTK_POS_LEFT:
	rect.x = x;
	rect.y = y + gap_x;
	rect.width = 2;
	rect.height = gap_width;
	break;
    case GTK_POS_RIGHT:
	rect.x = x + width - 2;
	rect.y = y + gap_x;
	rect.width = 2;
	rect.height = gap_width;
	break;
    }

    gtk_style_apply_default_pixmap (style, window, state_type, area,
				    rect.x, rect.y, rect.width, rect.height);
}

static void
draw_box_gap (GtkStyle *style,
	      GdkWindow *window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle *area,
	      GtkWidget *widget,
	      const gchar *detail,
	      gint x,
	      gint y,
	      gint width,
	      gint height,
	      GtkPositionType gap_side, gint gap_x, gint gap_width)
{
    GdkRectangle rect;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    debug ("draw_box_gap: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

    gtk_paint_box (style, window, state_type, shadow_type, area, widget,
		   detail, x, y, width, height);

    /* XXX Eavel hack to prevent a hole being draw when the
       XXX active tab is on the far left */
    if (gap_x < 1)
    {
	gap_width -= (1 - gap_x);
	gap_x = 1;
    }

    switch (gap_side)
    {
    case GTK_POS_TOP:
	rect.x = x + gap_x;
	rect.y = y;
	rect.width = gap_width;
	rect.height = 2;
	break;
    case GTK_POS_BOTTOM:
	rect.x = x + gap_x;
	rect.y = y + height - 2;
	rect.width = gap_width;
	rect.height = 2;
	break;
    case GTK_POS_LEFT:
	rect.x = x;
	rect.y = y + gap_x;
	rect.width = 2;
	rect.height = gap_width;
	break;
    case GTK_POS_RIGHT:
	rect.x = x + width - 2;
	rect.y = y + gap_x;
	rect.width = 2;
	rect.height = gap_width;
	break;
    }

    gtk_style_apply_default_pixmap (style, window, state_type, area,
				    rect.x, rect.y, rect.width, rect.height);
}

static void
draw_extension (GtkStyle *style,
		GdkWindow *window,
		GtkStateType state_type,
		GtkShadowType shadow_type,
		GdkRectangle *area,
		GtkWidget *widget,
		const gchar *detail,
		gint x,
		gint y, gint width, gint height, GtkPositionType gap_side)
{
    GdkRectangle rect;

    eazel_theme_data *theme_data;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    theme_data = CRUX_RC_STYLE (style->rc_style)->theme_data;
    g_assert (theme_data != NULL);

    debug ("draw_extension: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

	/* Get x relative to parent widget, not window */
	GtkWidget* parent_widget;
	gint relative_x;
	if (widget != NULL)
	{
		parent_widget = gtk_widget_get_parent(widget);
		relative_x = x - parent_widget->allocation.x;
		if (GTK_IS_CONTAINER(widget))
			relative_x = relative_x - gtk_container_get_border_width((GtkContainer*)widget);
	}
	else
	{
		relative_x = x;
	}


    if (DETAIL ("tab"))
    {
	eazel_engine_stock_image type = 0;
	switch (gap_side)
	{
	case GTK_POS_TOP:
	    type = ((state_type != GTK_STATE_ACTIVE)
		    ? EAZEL_ENGINE_TAB_BOTTOM_ACTIVE
		    : (relative_x < style->xthickness*2) ? EAZEL_ENGINE_TAB_BOTTOM_LEFT
		    : EAZEL_ENGINE_TAB_BOTTOM);
	    break;

	case GTK_POS_BOTTOM:
	    type = ((state_type != GTK_STATE_ACTIVE)
		    ? EAZEL_ENGINE_TAB_TOP_ACTIVE
		    : (relative_x < style->xthickness*2) ? EAZEL_ENGINE_TAB_TOP_LEFT
		    : EAZEL_ENGINE_TAB_TOP);
	    break;

	default:			/* gcc drugging */
	  ;
	}
	paint_background_area (style, window, state_type, area,
			       x, y, width, height);
	if (type != 0)
	{
	    paint_stock_image (theme_data, type, TRUE, FALSE, style, window,
			       state_type, area, widget, x, y, width, height);
	    return;
	}
    }

    gtk_paint_box (style, window, state_type, shadow_type, area, widget,
		   detail, x, y, width, height);

    switch (gap_side)
    {
    case GTK_POS_TOP:
	rect.x = x + style->xthickness;
	rect.y = y;
	rect.width = width - style->xthickness * 2;
	rect.height = style->ythickness;
	break;
    case GTK_POS_BOTTOM:
	rect.x = x + style->xthickness;
	rect.y = y + height - style->ythickness;
	rect.width = width - style->xthickness * 2;
	rect.height = style->ythickness;
	break;
    case GTK_POS_LEFT:
	rect.x = x;
	rect.y = y + style->ythickness;
	rect.width = style->xthickness;
	rect.height = height - style->ythickness * 2;
	break;
    case GTK_POS_RIGHT:
	rect.x = x + width - style->xthickness;
	rect.y = y + style->ythickness;
	rect.width = style->xthickness;
	rect.height = height - style->ythickness * 2;
	break;
    }

    gtk_style_apply_default_pixmap (style, window, state_type, area,
				    rect.x, rect.y, rect.width, rect.height);
}

static void
draw_focus (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail, gint x, gint y, gint width, gint height)
{
    eazel_theme_data *theme_data;
    gboolean rounded = TRUE, rounded_inner = TRUE;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    theme_data = CRUX_RC_STYLE (style->rc_style)->theme_data;
    g_assert (theme_data != NULL);

    debug ("draw_focus: detail=%s x=%d y=%d w=%d h=%d\n",
	    detail, x, y, width, height);

    /* Protection against broken GTK+ widgets */
    g_return_if_fail (width < 32768);
    g_return_if_fail (height < 32768);
    g_return_if_fail (width >= -1);
    g_return_if_fail (height >= -1);

    if ((width == -1) && (height == -1))
	gdk_window_get_size (window, &width, &height);
    else if (width == -1)
	gdk_window_get_size (window, &width, NULL);
    else if (height == -1)
	gdk_window_get_size (window, NULL, &height);

	if (DETAIL ("entry"))
	{
		cairo_t *cr;
		cr = crux_begin_paint (window, area);
		cairo_rectangle (cr, x + 0.5, y + 0.5, width - 1.0, height - 1.0);
		gdk_cairo_set_source_color (cr, &style->base[GTK_STATE_SELECTED]);
		cairo_stroke (cr);
		cairo_destroy (cr);
		return;
	}
	/* Make sure no widget is without a focus indicator! */
	parent_style_class->draw_focus(style, window, state_type, area, widget,
	                               detail, x, y, width, height);
}

static void
draw_slider (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail,
	     gint x,
	     gint y, gint width, gint height, GtkOrientation orientation)
{
    eazel_theme_data *theme_data;
    gboolean focused;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    theme_data = CRUX_RC_STYLE (style->rc_style)->theme_data;
    g_assert (theme_data != NULL);

    debug ("draw_slider: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

    /* Protection against broken GTK+ widgets */
    g_return_if_fail (width < 32768);
    g_return_if_fail (height < 32768);
    g_return_if_fail (width >= -1);
    g_return_if_fail (height >= -1);

    if ((width == -1) && (height == -1))
	gdk_window_get_size (window, &width, &height);
    else if (width == -1)
	gdk_window_get_size (window, &width, NULL);
    else if (height == -1)
	gdk_window_get_size (window, NULL, &height);

    if (area)
	gdk_gc_set_clip_rectangle (style->black_gc, area);

    focused = (widget != 0);

    if (DETAIL ("slider")) {
	{
	    int thumb_x, thumb_y;

	    focused = 1;

	    /* XXX could be a gradient? */
	    paint_stock_image (theme_data,
			       width > height
			       ? (state_type == GTK_STATE_PRELIGHT
				  ? EAZEL_ENGINE_H_SCROLLBAR_HI
				  : !focused
				  ? EAZEL_ENGINE_H_SCROLLBAR_INACTIVE
				  : EAZEL_ENGINE_H_SCROLLBAR)
			       : (state_type == GTK_STATE_PRELIGHT
				  ? EAZEL_ENGINE_V_SCROLLBAR_HI
				  : !focused
				  ? EAZEL_ENGINE_V_SCROLLBAR_INACTIVE
				  : EAZEL_ENGINE_V_SCROLLBAR),
			       TRUE, FALSE, style, window, state_type, area,
			       widget, x, y, width, height);
	    if (width > height)
	      {
	      /* XXX `4' is 1/2 width of thumb */
	      thumb_x = x + width / 2 - 4;
		thumb_y = y;
	    }
	    else
	      {
	      thumb_x = x;
		/* XXX `4' is 1/2 height of thumb */
		thumb_y = y + height / 2 - 4;
	    }

	    paint_stock_image (theme_data,
			       width > height
			       ? (state_type == GTK_STATE_PRELIGHT
				  ? EAZEL_ENGINE_H_SCROLLBAR_THUMB_HI
				  : !focused
				  ? EAZEL_ENGINE_H_SCROLLBAR_THUMB_INACTIVE
				  : EAZEL_ENGINE_H_SCROLLBAR_THUMB)
			       : (state_type == GTK_STATE_PRELIGHT
				  ? EAZEL_ENGINE_V_SCROLLBAR_THUMB_HI
				  : !focused
				  ? EAZEL_ENGINE_V_SCROLLBAR_THUMB_INACTIVE
				  : EAZEL_ENGINE_V_SCROLLBAR_THUMB),
			       FALSE, FALSE, style, window, state_type, area,
			       widget, thumb_x, thumb_y, -1, -1);
	}
    } else {
      paint_stock_image (theme_data,
			 orientation == GTK_ORIENTATION_HORIZONTAL
			 ? (focused ? EAZEL_ENGINE_H_SLIDER_THUMB
			    : EAZEL_ENGINE_H_SLIDER_THUMB_INACTIVE)
			 : (focused ? EAZEL_ENGINE_V_SLIDER_THUMB
			    : EAZEL_ENGINE_V_SLIDER_THUMB_INACTIVE),
			 TRUE, TRUE, style, window, state_type,
			 area, widget, x, y, width, height);
    }
    if (area)
	gdk_gc_set_clip_rectangle (style->black_gc, NULL);
}

static void
draw_handle (GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const gchar *detail,
	     gint x,
	     gint y, gint width, gint height, GtkOrientation orientation)
{
    gint i, yy, xx;
    gint xthick, ythick;
    GdkGC *light_gc, *dark_gc;
    GdkRectangle dest;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    debug ("draw_handle: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	   detail, state_type, shadow_type, x, y, width, height);

    /* Protection against broken GTK+ widgets */
    g_return_if_fail (width < 32768);
    g_return_if_fail (height < 32768);
    g_return_if_fail (width >= -1);
    g_return_if_fail (height >= -1);

    if ((width == -1) && (height == -1))
	gdk_window_get_size (window, &width, &height);
    else if (width == -1)
	gdk_window_get_size (window, &width, NULL);
    else if (height == -1)
	gdk_window_get_size (window, NULL, &height);

    if (DETAIL ("dockitem") && state_type == GTK_STATE_NORMAL)
	state_type = GTK_STATE_ACTIVE;


/*
	cairo_t *cr;
	cr = crux_begin_paint (window, NULL);
	paint_shadow (cr, style, state_type, GTK_SHADOW_OUT,x, y, width, height);
	cairo_destroy (cr);
	*/


	draw_box (style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);

    light_gc = style->light_gc[state_type];
    dark_gc = style->dark_gc[state_type];

    xthick = style->xthickness;
    ythick = style->ythickness;

    dest.x = x + xthick;
    dest.y = y + ythick;
    dest.width = width - (xthick * 2);
    dest.height = height - (ythick * 2);

    gdk_gc_set_clip_rectangle (light_gc, &dest);
    gdk_gc_set_clip_rectangle (dark_gc, &dest);

    /* ORIENTATION parameters is unreliable */
    if (height > width)
    {
	yy = y + height / 2 - 4;
	for (i = 0; i < 9; i += 2)
	{
	    gdk_draw_line (window, dark_gc, xthick, yy + i,
			   x + width - xthick, yy + i);
	    gdk_draw_line (window, light_gc, xthick, yy + i + 1,
			   x + width - xthick, yy + i + 1);
	}
    }
    else
    {
	xx = x + width / 2 - 5;
	for (i = 0; i < 10; i += 4)
	{
	    gdk_draw_line (window, dark_gc, xx + i, ythick,
			   xx + i, y + height - ythick);
	    gdk_draw_line (window, light_gc, xx + i + 1, ythick,
			   xx + i + 1, y + height - ythick);
	}
    }

    gdk_gc_set_clip_rectangle (light_gc, NULL);
    gdk_gc_set_clip_rectangle (dark_gc, NULL);
}

void
crux_draw_style_class_init (GtkStyleClass *style_class)
{

  parent_style_class = g_type_class_peek_parent(style_class);

  style_class->draw_hline = draw_hline;
  style_class->draw_vline = draw_vline;
  style_class->draw_shadow = draw_shadow;
  style_class->draw_polygon = draw_polygon;
  style_class->draw_arrow = draw_arrow;
  style_class->draw_diamond = draw_diamond;
  style_class->draw_string = draw_string;
  style_class->draw_box = draw_box;
  //style_class->draw_flat_box = draw_flat_box;
  style_class->draw_check = draw_check;
  style_class->draw_option = draw_option;
  style_class->draw_tab = draw_tab;
  style_class->draw_shadow_gap = draw_shadow_gap;
  style_class->draw_box_gap = draw_box_gap;
  style_class->draw_extension = draw_extension;
  style_class->draw_focus = draw_focus;
  style_class->draw_slider = draw_slider;
  style_class->draw_handle = draw_handle;
}
