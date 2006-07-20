#include "crux-draw.h"
#include "crux-style.h"

#include <ge-support.h>

#include <stdio.h>
#include <math.h>
#include <string.h>

#define DEBUG 0

#define DETAIL(xx) ((detail) != 0 && strcmp (xx, detail) == 0)

#define OUTLINE_GRAY 46/255.0, 52/255.0, 54/255.0

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
	ge_shade_color (&c1, 1.3, &c1);
	cairo_pattern_add_color_stop_rgb (crp, 0.35, c1.r, c1.g, c1.b);
	cairo_pattern_add_color_stop_rgb (crp, 0.28, c1.r, c1.g, c1.b);
	ge_shade_color (&c1, 0.6, &c1);
	cairo_pattern_add_color_stop_rgb (crp, 1.0, c1.r, c1.g, c1.b);
	cairo_rectangle (cr, x, y, width, height);
	cairo_set_source (cr, crp);
	cairo_fill (cr);
	cairo_pattern_destroy (crp);
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
	ge_shade_color (&c1, 0.9, &c2); /* darken */
	ge_shade_color (&c1, 1.1, &c1); /* lighten */

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
				gdk_cairo_set_source_color (cr, &style->fg [state_type]);
			else
				/* dark gray */
				cairo_set_source_rgb (cr, OUTLINE_GRAY);
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
				gdk_cairo_set_source_color (cr, &style->fg [state_type]);
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

				cairo_set_source_rgb (cr, OUTLINE_GRAY);
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
	if (shadow_type == GTK_SHADOW_OUT || shadow_type == GTK_SHADOW_ETCHED_OUT)
	{
		cairo_set_source_rgb (cr, OUTLINE_GRAY);
		cairo_stroke (cr);
	}
	else if (shadow_type == GTK_SHADOW_IN || shadow_type == GTK_SHADOW_ETCHED_IN)
	{
		/*
		cairo_pattern_add_color_stop_rgb (crp, 0.0, 186/255.0, 189/255.0, 182/255.0);
		cairo_pattern_add_color_stop_rgb (crp, 1.0, 238/255.0, 238/255.0, 236/255.0);
		*/
		crp = cairo_pattern_create_linear (x, y, x, y + height);
		cairo_pattern_add_color_stop_rgba (crp, 1.0, 1.0, 1.0, 1.0, 0.5);
		cairo_pattern_add_color_stop_rgba (crp, 0.0, .0, .0, .0, 0.2);
		cairo_set_source (cr, crp);
		cairo_stroke (cr);
		cairo_pattern_destroy (crp);
	}

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
		cairo_set_source_rgb (cr, OUTLINE_GRAY);
		cairo_stroke (cr);

		/*
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
		*/
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

	/* make sure background is initialised as shadow is only in top left */
	gdk_cairo_set_source_color (cr, &style->base[(state_type == GTK_STATE_INSENSITIVE) ? GTK_STATE_INSENSITIVE : GTK_STATE_NORMAL]);
	cairo_rectangle (cr, x + 2.0, y + 2.0, width - 4.0, height - 4.0);
	cairo_stroke (cr);

	/* draw shadow */
	cairo_move_to (cr, x + 2.0, y + height - 2.0);
	cairo_line_to (cr, x + 2.0, y + 2.0);
	cairo_line_to (cr, x + width - 2.0, y + 2.0);
	cairo_set_source_rgba (cr, 0, 0, 0, 0.12);
	cairo_stroke (cr);
}

static void
paint_scrollbar_trough (cairo_t *cr, GtkStyle *style, GtkStateType state_type, GtkOrientation orientation,
	gdouble x, gdouble y, gdouble width, gdouble height)
{
	cairo_pattern_t *crp;
	gdouble gradient_size;
	CairoColor tl, br;
	/* gradient_size is the size of the "shadow" at the ends of the trough
	 * it needs to be calculated so that it is a fixed size, since cairo takes a ratio when adding gradient stops
	 */

	#define OFFWHITE 238/255.0, 238/255.0, 238/255.0


	ge_gdk_color_to_cairo (&style->white, &br);
	ge_gdk_color_to_cairo (&style->bg[state_type], &tl);
	ge_shade_color (&tl, 1.2, &tl);
	ge_cairo_simple_border (cr, &tl, &br, x, y, width, height, FALSE);

	/* set co-ordinates for line drawing */
	x += 0.5; y += 0.5; width -= 1.0; height -= 1.0;

	x += 1.0; y += 1.0; width -= 2.0; height -= 2.0;
	cairo_rectangle (cr, x, y, width, height);
	gdk_cairo_set_source_color (cr, &style->dark[state_type]);
	cairo_fill (cr);

	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		cairo_rectangle (cr, x, y, width, height * 0.4);
	else
		cairo_rectangle (cr, x, y, width * 0.4, height);

	cairo_set_source_rgb (cr, OFFWHITE);
	cairo_fill_preserve (cr);

	cairo_set_source_rgb (cr, OUTLINE_GRAY);
	cairo_stroke (cr);


	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		cairo_rectangle (cr, x, y + height * 0.6, width, height * 0.4);
	else
		cairo_rectangle (cr, x + width * 0.6, y, width * 0.4, height);

	cairo_set_source_rgb (cr, OFFWHITE);
	cairo_fill_preserve (cr);

	cairo_set_source_rgb (cr, OUTLINE_GRAY);
	cairo_stroke (cr);

	if (orientation == GTK_ORIENTATION_HORIZONTAL)
	{
		crp = cairo_pattern_create_linear (x, y, x + width, y);
		gradient_size = 24 / width;
	}
	else
	{
		crp = cairo_pattern_create_linear (x, y, x, y + height);
		gradient_size = 24 / height;
	}


	cairo_rectangle (cr, x, y, width, height);
	cairo_pattern_add_color_stop_rgba (crp, 0.0, 0.0, 0.0, 0.0, 0.4);
	cairo_pattern_add_color_stop_rgba (crp, gradient_size, 0.0, 0.0, 0.0, 0.0);
	cairo_pattern_add_color_stop_rgba (crp, 1 - gradient_size, 0.0, 0.0, 0.0, 0.0);
	cairo_pattern_add_color_stop_rgba (crp, 1.0, 0.0, 0.0, 0.0, 0.4);

	cairo_set_source (cr, crp);
	cairo_fill (cr);
	cairo_pattern_destroy (crp);
}

static void
paint_progress_bar (cairo_t *cr, GtkStyle *style, GtkStateType state_type, GtkProgressBarOrientation orientation,
		    gdouble x, gdouble y, gdouble width, gdouble height)
{
	cairo_pattern_t *crp;
	CairoColor c1, c2;

	cairo_rectangle (cr, x, y, width, height);

	ge_gdk_color_to_cairo (&style->base[GTK_STATE_SELECTED], &c1);
	ge_shade_color (&c1, 0.8, &c2); /* darken */
	ge_shade_color (&c1, 1.4, &c1); /* lighten */
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

    CHECK_ARGS

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

    CHECK_ARGS

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
	cairo_t *cr;
	GdkRectangle area2 = {x, y, width, height};
	gboolean focused = FALSE;

	CHECK_ARGS
	SANITIZE_SIZE

	if (widget != 0 && !GTK_WIDGET_IS_SENSITIVE (widget))
		state_type = GTK_STATE_INSENSITIVE;

	debug ("draw_shadow: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	        detail, state_type, shadow_type, x, y, width, height);

	if (widget != NULL && GTK_WIDGET_HAS_FOCUS (widget))
		focused = TRUE;

	if (widget && (GTK_IS_COMBO (widget->parent) || GTK_IS_COMBO_BOX_ENTRY (widget->parent)))
	{
		width += 2;
		if (area == NULL)
			area = &area2;
	}
	cr = ge_gdk_drawable_to_cairo (window, area);

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

	CHECK_ARGS
	SANITIZE_SIZE

    if (DETAIL ("spinbutton")) {
      if (y > (height / 3)) {
	/* this is a down arrow shadow */
	y += 2;
      }
    }

    debug ("draw_box: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

	cr = ge_gdk_drawable_to_cairo (window, area);

	/* no pressed state for scollbar buttons yet... */
	if (DETAIL ("vscrollbar") || DETAIL ("hscrollbar"))
		shadow_type = GTK_SHADOW_OUT;

	if (DETAIL ("button") || DETAIL ("optionmenu"))
	{
		if (widget && (GTK_IS_COMBO (widget->parent) || GTK_IS_COMBO_BOX_ENTRY (widget->parent)))
		{
			/* Combobox buttons */
			/* TODO: need RTL support */
			if (state_type == GTK_STATE_INSENSITIVE)
				gdk_cairo_set_source_color (cr, &style->base[GTK_STATE_INSENSITIVE]);
			else
				gdk_cairo_set_source_color (cr, &style->base[GTK_STATE_NORMAL]);
			cairo_rectangle (cr, x, y, width, height);
			cairo_fill (cr);

			paint_entry_shadow (cr, style, state_type, FALSE, x - 4, y, width + 4, height);
			x += 3; y += 3;
			width -= 6; height -= 6;
		}
		if (widget && (GTK_IS_TREE_VIEW (widget->parent)))
		{
			/* Add some extra padding for treeview column buttons */
			if (state_type == GTK_STATE_INSENSITIVE)
				gdk_cairo_set_source_color (cr, &style->base[GTK_STATE_INSENSITIVE]);
			else
				gdk_cairo_set_source_color (cr, &style->base[GTK_STATE_NORMAL]);
			cairo_rectangle (cr, x, y, width, height);
			cairo_fill (cr);
					
			x += 1; y += 1;
			width -= 2; height -= 2;
		}
		paint_button (cr, style, state_type, shadow_type, x, y, width, height);
	}
	else if (DETAIL ("buttondefault"))
	{
		/* draw Default Button marking, but only when sensitive */
		if (GTK_WIDGET_SENSITIVE (widget))
		{
			gdk_cairo_set_source_color (cr, &style->bg[GTK_STATE_SELECTED]);
			cx = x + 0.5; cy = y + 0.5; cw = width - 1.0; ch = height - 1.0;
			ge_cairo_rounded_rectangle (cr, cx, cy, cw, ch, 3.0, CR_CORNER_ALL);
			cairo_stroke (cr);
		}
	}
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
			x += 1; width -= 2;
		}
		else
		{
			y += 1; height -= 2;
		}
		paint_button (cr, style, GTK_STATE_NORMAL, GTK_SHADOW_OUT, x, y, width, height);
	}
	else if (DETAIL ("trough") && GTK_IS_SCROLLBAR (widget))
	{
		if (GTK_IS_HSCROLLBAR (widget))
			paint_scrollbar_trough (cr, style, state_type, GTK_ORIENTATION_HORIZONTAL, x, y, width, height);
		else
			paint_scrollbar_trough (cr, style, state_type, GTK_ORIENTATION_VERTICAL, x, y, width, height);
	}
	else
	{
		/* fill  */
		cairo_rectangle (cr, x, y, width, height);

		if (DETAIL ("toolbar") || DETAIL ("menubar") || DETAIL ("vscrollbar") || DETAIL ("hscrollbar") || DETAIL ("handle"))
		{
			if (shadow_type == GTK_SHADOW_OUT || shadow_type == GTK_SHADOW_ETCHED_OUT)
			{
				cairo_pattern_t *crp;
				CairoColor c1, c2;

				crp = cairo_pattern_create_linear (x, y, x, y + height);
				ge_gdk_color_to_cairo (&style->bg[state_type], &c1);
				ge_shade_color (&c1, 0.9, &c2);
				ge_shade_color (&c1, 1.1, &c1);
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
		}
		else
		{
			gdk_cairo_set_source_color (cr, &style->bg[state_type]);
			cairo_fill (cr);
		}
		paint_shadow (cr, style, state_type, shadow_type, x, y, width, height);
	}

	cairo_destroy (cr);

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
    //eazel_theme_data *theme_data;

    /* FIXME GNOME2: bad hack added to make arrows draw large enough */
    width = width + 8;
    height = height + 6;
    x = x - 4;
    y = y - 3;

    CHECK_ARGS
    /*SANITIZE_SIZE*/

    //theme_data = CRUX_RC_STYLE (style->rc_style)->theme_data;
    //g_assert (theme_data != NULL);

    debug ("draw_arrow: detail=%s state=%d shadow=%d arrow_type=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, arrow_type, x, y, width, height);

    if (DETAIL ("vscrollbar") || DETAIL ("hscrollbar"))
    {
	x++;y++; width--; height--;
	    /*
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
	*/
	}
    if (DETAIL ("spinbutton"))
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
draw_check (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail, gint x, gint y, gint width, gint height)
{
	cairo_t * cr;
	cairo_pattern_t *crp;
	CairoColor c1;
	gdouble cx, cy, cw, ch; /* co-ordinates for cairo */

	CHECK_ARGS
	SANITIZE_SIZE
	
	debug ("draw_check: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

	cr = ge_gdk_drawable_to_cairo (window, NULL);

	/* set up for line drawing */
	cx = x + 0.5; cy = y + 0.5;
	ch = height - 1.0; cw = width - 1.0;


	if (!GTK_IS_MENU_ITEM (widget))
	{
		ge_cairo_rounded_rectangle (cr, cx, cy, cw, ch, 2.0, CR_CORNER_ALL);
		if (state_type == GTK_STATE_INSENSITIVE)
		{
			gdk_cairo_set_source_color (cr, &style->bg[state_type]);
			cairo_fill_preserve (cr);

			gdk_cairo_set_source_color (cr, &style->fg[state_type]);
			cairo_stroke (cr);

			/* prepare area for check mark */
			cx+= 2.0; cy+= 2.0; cw -= 4.0; ch -= 4.0;
		}
		else
		{
			/* create gradient */
			crp = cairo_pattern_create_linear (cx, cy, cx, cy + ch);

			ge_gdk_color_to_cairo (&style->bg[GTK_STATE_NORMAL], &c1);
			ge_shade_color (&c1, 0.9, &c1); /* darken */

			if (state_type == GTK_STATE_ACTIVE)
			{
				cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
				cairo_pattern_add_color_stop_rgb (crp, 1.0, 1.0, 1.0, 1.0);
			}
			else
			{
				cairo_pattern_add_color_stop_rgb (crp, 0.0, 1.0, 1.0, 1.0);
				cairo_pattern_add_color_stop_rgb (crp, 1.0, c1.r, c1.g, c1.b);
			}
			cairo_set_source (cr, crp);
			cairo_fill_preserve (cr);
			cairo_pattern_destroy (crp);

			cairo_set_source_rgb (cr, OUTLINE_GRAY);
			cairo_stroke (cr);

			/* inner hilight */
			cx+=1.0; cy+=1.0; cw -= 2.0; ch -= 2.0;
			ge_cairo_rounded_rectangle (cr, cx, cy, cw, ch, 1.0, CR_CORNER_ALL);
			cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.7);
			cairo_stroke (cr);

			/* prepare area for check mark */
			cx+=1.0; cy+=1.0; cw -= 2.0; ch -= 2.0;
		}
	}
	if (shadow_type != GTK_SHADOW_OUT)
	{

		cairo_set_line_width (cr, 2.0);

		/* draw check mark shadow first */
		if (state_type != GTK_STATE_INSENSITIVE)
		{
			cy+=1.0;
			cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.3);
			cairo_move_to (cr, cx + floor(cw / 4), cy + ch / 2);
			cairo_line_to (cr, cx + floor(cw / 2), cy + floor(ch * 0.75));
			cairo_line_to (cr, cx + cw, cy + floor(ch / 4));
			cairo_stroke (cr);
			cy-=1.0;
		}
		if (state_type == GTK_STATE_INSENSITIVE)
			gdk_cairo_set_source_color (cr, &style->fg[state_type]);
		else
			gdk_cairo_set_source_color (cr, &style->bg[GTK_STATE_SELECTED]);

		/* draw check mark */
		/* use floor() so that we still end up with a .5 value when adding to the co-ordinate! */
		cairo_move_to (cr, cx + floor(cw / 4), cy + ch / 2);
		cairo_line_to (cr, cx + floor(cw / 2), cy + floor(ch * 0.75));
		cairo_line_to (cr, cx + cw, cy + floor(ch / 4));
		cairo_stroke (cr);
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
	gdouble cx, cy, radius; /* cairo co-ordinates */
	CairoColor c1, c2;

	CHECK_ARGS
	SANITIZE_SIZE
	
	debug ("draw_option: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

	cr = ge_gdk_drawable_to_cairo (window, area);

	cx = x + (height / 2);
	cy = y + (height / 2);
	radius = (height / 2) - 0.5;

	if (!GTK_IS_MENU_ITEM (widget))
	{
		cairo_arc (cr, cx, cy, radius, 0.0, M_PI * 2);

		if (state_type == GTK_STATE_INSENSITIVE)
		{
			gdk_cairo_set_source_color (cr, &style->bg[state_type]);
			cairo_fill_preserve (cr);

			gdk_cairo_set_source_color (cr, &style->fg[state_type]);
			cairo_stroke (cr);
		}
		else
		{
			ge_gdk_color_to_cairo (&style->bg[state_type], &c1);
			ge_shade_color (&c1, 0.7, &c2);
			ge_shade_color (&c1, 9.9, &c1);

			crp = cairo_pattern_create_linear (x, y, x + height, y + height);
			if (state_type != GTK_STATE_ACTIVE)
			{
				/* out shadow */
				cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
				cairo_pattern_add_color_stop_rgb (crp, 1.0, c2.r, c2.g, c2.b);
			}
			else
			{
				/* in shadow */
				cairo_pattern_add_color_stop_rgb (crp, 0.0, c2.r, c2.g, c2.b);
				cairo_pattern_add_color_stop_rgb (crp, 1.0, c1.r, c1.g, c1.b);
			}
			cairo_set_source (cr, crp);
			cairo_fill_preserve (cr);
			cairo_pattern_destroy (crp);

			cairo_set_source_rgb (cr, OUTLINE_GRAY);
			cairo_stroke (cr);

			cairo_arc (cr, cx, cy, radius - 1.0, 0.0, M_PI * 2);
			cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.5);
			cairo_stroke (cr);
		}
		/* only make the "dot" smaller if we have drawn a border */
		radius = radius / 2.0;
	}


	if (shadow_type == GTK_SHADOW_IN)
	{
		/* draw check mark */
		cairo_arc (cr, cx, cy, radius, 0.0, M_PI * 2);

		if (state_type == GTK_STATE_INSENSITIVE)
		{
			gdk_cairo_set_source_color (cr, &style->fg[state_type]);
			cairo_fill_preserve (cr);
			cairo_stroke (cr);
		}
		else
		{
			gdk_cairo_set_source_color (cr, &style->light[GTK_STATE_SELECTED]);
			cairo_fill_preserve (cr);

			gdk_cairo_set_source_color (cr, &style->dark[GTK_STATE_SELECTED]);
			cairo_stroke (cr);
		}
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
    CHECK_ARGS
    SANITIZE_SIZE

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

    CHECK_ARGS
    SANITIZE_SIZE

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

    CHECK_ARGS
    SANITIZE_SIZE

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
    case GTK_POS_BOTTOM:
      gap_width = MIN (gap_width, width - gap_x - 1);
      break;
    case GTK_POS_LEFT:
    case GTK_POS_RIGHT:
      gap_width = MIN (gap_width, height - gap_x - 1);
      break;
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
    debug ("draw_extension: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	    detail, state_type, shadow_type, x, y, width, height);

	/* Get x relative to parent widget, not window */
	GtkWidget* parent_widget;
	gint relative_x;
	if (widget != NULL &&
	    (parent_widget = gtk_widget_get_parent(widget)) != NULL)
	{
		relative_x = x - parent_widget->allocation.x;
		if (GTK_IS_CONTAINER(widget))
			relative_x = relative_x - gtk_container_get_border_width((GtkContainer*)widget);
	}
	else
	{
		relative_x = x;
	}

	cairo_t *cr;
	cairo_pattern_t *crp;
	CairoColor c1, c2;
	CairoCorners corners;

	cr = ge_gdk_drawable_to_cairo (window, area);

	cairo_rectangle (cr, x, y, width, height);
	cairo_clip_preserve (cr);
	cairo_new_path (cr);

	ge_gdk_color_to_cairo (&style->bg[state_type], &c2);
	ge_shade_color (&c2, 1.1, &c1);

	switch (gap_side)
	{
		case GTK_POS_TOP:
			/* bottom tab */
			y--;
			y--;
			corners = CR_CORNER_BOTTOMLEFT + CR_CORNER_BOTTOMRIGHT;
			crp = cairo_pattern_create_linear (x, y + height, x, y);
			ge_shade_color (&c2, 0.8, &c1);
			break;
		case GTK_POS_BOTTOM:
			/* top tab */
			height++;
			height++;
			corners = CR_CORNER_TOPLEFT + CR_CORNER_TOPRIGHT;
			crp = cairo_pattern_create_linear (x, y, x, y + height);
			break;
		case GTK_POS_LEFT:
			/* right tab */
			x--;
			x--;
			corners = CR_CORNER_TOPRIGHT + CR_CORNER_BOTTOMRIGHT;
			crp = cairo_pattern_create_linear (x + width, y, x, y);
			break;
		case GTK_POS_RIGHT:
			/* left tab */
			width++;
			width++;
			corners = CR_CORNER_BOTTOMLEFT + CR_CORNER_TOPLEFT;
			crp = cairo_pattern_create_linear (x, y, x + width, y);
			break;
	}

	ge_cairo_rounded_rectangle (cr, x + 0.5, y + 0.5, width - 1.0, height - 1.0, 2.0, corners);

	cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
	cairo_pattern_add_color_stop_rgb (crp, 0.3, c2.r, c2.g, c2.b);
	cairo_set_source (cr, crp);
	cairo_fill_preserve (cr);
	cairo_pattern_destroy (crp);

	cairo_set_source_rgb (cr, OUTLINE_GRAY);
	cairo_stroke (cr);
	
	x++; y++; width -= 2.0; height -= 2.0;
	ge_cairo_rounded_rectangle (cr, x + 0.5, y + 0.5, width - 1.0, height - 1.0, 1.0, corners);
	gdk_cairo_set_source_color (cr, &style->light[state_type]);
	cairo_stroke (cr);
	cairo_destroy (cr);
}

static void
draw_focus (GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const gchar *detail, gint x, gint y, gint width, gint height)
{
    debug ("draw_focus: detail=%s x=%d y=%d w=%d h=%d\n",
	    detail, x, y, width, height);

	if (DETAIL ("entry"))
	{
		cairo_t *cr;
		cr = ge_gdk_drawable_to_cairo (window, area);
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
	cairo_t *cr;
	cairo_pattern_t *crp;
	CairoColor c1, c2;

	CHECK_ARGS
	SANITIZE_SIZE

	debug ("draw_slider: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n", detail, state_type, shadow_type, x, y, width, height);

	cr = ge_gdk_drawable_to_cairo (window, area);

	cairo_rectangle (cr, x + 0.5, y + 0.5, width - 1.0, height - 1.0);

	if (orientation == GTK_ORIENTATION_VERTICAL)
		crp = cairo_pattern_create_linear (x, y, x + width, y);
	else
		crp = cairo_pattern_create_linear (x, y, x, y + height);

	ge_gdk_color_to_cairo (&style->light[GTK_STATE_SELECTED], &c1);
	ge_gdk_color_to_cairo (&style->dark[GTK_STATE_SELECTED], &c2);
	cairo_pattern_add_color_stop_rgb (crp, 0.0, c1.r, c1.g, c1.b);
	cairo_pattern_add_color_stop_rgb (crp, 1.0, c2.r, c2.g, c2.b);
	cairo_set_source (cr, crp);
	cairo_fill_preserve (cr);
	cairo_pattern_destroy (crp);

	gdk_cairo_set_source_color (cr, &style->dark[GTK_STATE_SELECTED]);
	cairo_stroke (cr);
/*
	cairo_rectangle (cr, x + 1.5, y + 1.5, width - 2.0, height - 2.0);
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.3);
	cairo_stroke (cr);
*/
	/* bevel */
	cairo_rectangle (cr, x+1.5, y+1.5, width-3.0, height-3.0);
	if (orientation == GTK_ORIENTATION_VERTICAL)
		crp = cairo_pattern_create_linear (x, y, x + width, y);
	else
		crp = cairo_pattern_create_linear (x, y, x, y + height);
	cairo_pattern_add_color_stop_rgba (crp, 0.0, 1.0, 1.0, 1.0, .5);
	cairo_pattern_add_color_stop_rgba (crp, 1.0, 1.0, 1.0, 1.0, 0.2);
	cairo_set_source (cr, crp);
	cairo_stroke (cr);

	cairo_pattern_destroy (crp);


	cairo_destroy (cr);
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

    CHECK_ARGS
    SANITIZE_SIZE

    debug ("draw_handle: detail=%s state=%d shadow=%d x=%d y=%d w=%d h=%d\n",
	   detail, state_type, shadow_type, x, y, width, height);

    if (DETAIL ("dockitem") && state_type == GTK_STATE_NORMAL)
	state_type = GTK_STATE_ACTIVE;


/*
	cairo_t *cr;
	cr = ge_gdk_drawable_to_cairo (window, NULL);
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
  style_class->draw_arrow = draw_arrow;
  style_class->draw_box = draw_box;
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
