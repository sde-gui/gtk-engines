/**************************************************************************
* GTK Industrial Theme
* Designed by Tigert and Jimmac
* Implemented by Christopher James Lahey
*
* Based on GTK Bluecurve Theme
* Designed by Garrett LeSage
* Implemented by Alexander Larsson
*
* Parts of the code based on Metal theme, Copyright:
* Randy Gordon, randy@integrand.com
* Owen Taylor <otaylor@redhat.com>
*
* Ported to cairo by:
* Benjamin Berg <benjamin@sipsolutions.net>
*
* License: GPL (Gnu Public License)
*
**************************************************************************/

/* The port to cairo does not currently support the contrast options.
 * This is because I did not see the option, I'll need to investigate
 * later on what it does exactly and how to implement it.
 */

#include "industrial_style_versioned_include.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <ge-support.h>
#include <cairo.h>
#include <math.h>

#define WIDGET_TYPE_NAME(xx) (widget && !strcmp (G_OBJECT_TYPE_NAME (widget), (xx)))
/* #define DEBUG 1 */

#define DEFAULT_SHADOW_SIZE 3

#define LINE_OPACITY 0.4
#define HANDLE_OPACITY 0.38
#define BUTTON_DEPRESSED_SHADOW_OPACITY_1 0.14
#define BUTTON_DEPRESSED_SHADOW_OPACITY_2 0.07
#define DEFAULT_SHADOW_OPACITY 0.26
#define MENUITEM_BORDER_OPACITY 0.8
#define STANDARD_BORDER_OPACITY 0.38
#define BUTTON_BORDER_OPACITY 0.38
#define TROUGH_BG_OPACITY 0.15
#define TROUGH_BORDER_OPACITY 0.21
#define MENUBAR_BORDER_OPACITY 0.07

static GtkStyleClass *parent_class;

static void
draw_rounded_gradient (cairo_t    *cr,
		       gint        x,
		       gint	   y,
		       gint        width,
		       gint        height,
		       gfloat      inner_radius,
		       gfloat      outer_radius,
		       CairoColor *inner_color,
		       CairoColor *outer_color)
{
	gfloat gradient_width = outer_radius - inner_radius;
	cairo_pattern_t *pattern;
	cairo_matrix_t matrix;
	cairo_save (cr);
	cairo_translate (cr, x, y);

	/* setup a clip region for the gradient area */
	cairo_new_path (cr);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);

	ge_cairo_rounded_rectangle (cr, 0, 0, width, height, outer_radius, CR_CORNER_ALL);
	ge_cairo_rounded_rectangle (cr, gradient_width, gradient_width,
				    width - 2 * gradient_width,
				    height - 2 * gradient_width,
				    inner_radius, CR_CORNER_ALL);

	cairo_clip (cr);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);

	/* create radial gradient */
	pattern = cairo_pattern_create_radial (0, 0, 0, 0, 0, outer_radius);

	/* XXX: Shouldn't it work to set the first radius in create_radial
	 * to inner_radius instead of this hack? */
	ge_cairo_pattern_add_color_stop_color (pattern, 0, inner_color);
	ge_cairo_pattern_add_color_stop_color (pattern,
					       inner_radius / outer_radius,
					       inner_color);
	ge_cairo_pattern_add_color_stop_color (pattern, 1, outer_color);
	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	/* move to each corner, and draw it. (as everything is clipped, this is OK to do) */
	cairo_save (cr);
	cairo_rectangle (cr, 0, 0, outer_radius, outer_radius);
	cairo_matrix_init_translate (&matrix, -outer_radius, -outer_radius);
	cairo_pattern_set_matrix (pattern, &matrix);
	cairo_fill (cr);
	cairo_restore (cr);

	cairo_save (cr);
	cairo_rectangle (cr, width - outer_radius, 0, outer_radius, outer_radius);
	cairo_matrix_init_translate (&matrix, outer_radius - width, -outer_radius);
	cairo_pattern_set_matrix (pattern, &matrix);
	cairo_fill (cr);
	cairo_restore (cr);

	cairo_save (cr);
	cairo_rectangle (cr, width - outer_radius, height - outer_radius,
			 outer_radius, outer_radius);
	cairo_matrix_init_translate (&matrix, outer_radius - width,
				     outer_radius - height);
	cairo_pattern_set_matrix (pattern, &matrix);
	cairo_fill (cr);
	cairo_restore (cr);

	cairo_save (cr);
	cairo_rectangle (cr, 0, height - outer_radius, outer_radius, outer_radius);
	cairo_matrix_init_translate (&matrix, -outer_radius, outer_radius - height);
	cairo_pattern_set_matrix (pattern, &matrix);
	cairo_fill (cr);
	cairo_restore (cr);

	/* create linear gradient */
	pattern = cairo_pattern_create_linear (0, 0, 0, gradient_width);
	ge_cairo_pattern_add_color_stop_color (pattern, 0, inner_color);
	ge_cairo_pattern_add_color_stop_color (pattern, 1, outer_color);
	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	/* draw those gradients */
	cairo_save (cr);
	cairo_rectangle (cr, outer_radius, 0, width - 2 * outer_radius,
			 gradient_width);
	cairo_matrix_init_rotate (&matrix, M_PI);
	cairo_matrix_translate (&matrix, 0, -gradient_width);
	cairo_pattern_set_matrix (pattern, &matrix);
	cairo_fill (cr);
	cairo_restore (cr);

	cairo_save (cr);
	cairo_rectangle (cr, width - outer_radius, outer_radius, outer_radius,
			 height - 2 * outer_radius);
	cairo_matrix_init_rotate (&matrix, M_PI_2);
	cairo_matrix_translate (&matrix, -width + gradient_width, -gradient_width);
	cairo_pattern_set_matrix (pattern, &matrix);
	cairo_fill (cr);
	cairo_restore (cr);

	cairo_save (cr);
	cairo_rectangle (cr, outer_radius, height - gradient_width,
			 width - 2 * outer_radius, gradient_width);
	cairo_matrix_init_rotate (&matrix, 0);
	cairo_matrix_translate (&matrix, 0, -height + gradient_width);
	cairo_pattern_set_matrix (pattern, &matrix);
	cairo_fill (cr);
	cairo_restore (cr);

	cairo_save (cr);
	cairo_rectangle (cr, 0, outer_radius, gradient_width,
			 height - 2 * outer_radius);
	cairo_matrix_init_rotate (&matrix, -M_PI_2);
	cairo_matrix_translate (&matrix, -gradient_width, 0);
	cairo_pattern_set_matrix (pattern, &matrix);
	cairo_fill (cr);
	cairo_restore (cr);

	cairo_restore (cr);
}

/* The fill will only be drawn if fill != NULL.
 * radius should be 1.5/2.5 for 1px/2px corners. */
static void
draw_rounded_rect_cairo (cairo_t    *cr,
			 gint        x,
			 gint        y,
			 gint        width,
			 gint        height,
			 gdouble     radius,
			 CairoColor *bevel,
			 CairoColor *fill)
{
	if (fill) {
		ge_cairo_rounded_rectangle (cr, x, y, width, height, radius,
					    CR_CORNER_ALL);

		ge_cairo_set_color (cr, fill);
		cairo_fill (cr);
	}

	ge_cairo_set_color (cr, bevel);
	ge_cairo_rounded_rectangle (cr, x + 0.5, y + 0.5, width - 1, height - 1,
				    radius, CR_CORNER_ALL);
	cairo_stroke (cr);
}

/* #define names assume vertical */
#define HANDLE_HEIGHT (4 * 4 + 1 + 2)
#define HANDLE_WIDTH (4 + 1 + 2)

static void
draw_grid_cairo (cairo_t    *cr,
		 CairoColor *color,
		 gint        x,
		 gint        y,
		 gint        width,
		 gint        height)
{
	gint xpos, ypos;
	gboolean indented = FALSE;
	cairo_save (cr);

	ge_cairo_set_color (cr, color);

	/* walk trough the rows */
	for (ypos = y + 1; ypos < y + height; ypos += 2) {
		xpos = x;
		if (!indented)
			xpos += 1;
		else
			xpos += 3;

		for (; xpos < x + width; xpos += 4) {
			/* This one would fill in the center pixel exactly, but
			 * the dots don't look as good then.
			 * cairo_arc (cr, xpos + 0.5, ypos + 0.5, sqrt (0.5), 0, 2 * M_PI); */
			cairo_arc (cr, xpos + 0.5, ypos + 0.5, sqrt (0.7), 0, 2 * M_PI);
			cairo_fill (cr);
		}

		indented = !indented;
	}

	cairo_restore (cr);
}

/**************************************************************************/

static void
draw_hline (GtkStyle     *style,
	    GdkWindow    *window,
	    GtkStateType  state_type,
	    GdkRectangle *area,
	    GtkWidget    *widget,
	    const gchar  *detail,
	    gint          x1,
	    gint          x2,
	    gint          y)
{
	CairoColor color;
	cairo_t *cr;

#ifdef DEBUG
	printf ("draw_hline: %p %p %s %i %i %i\n", widget, window, detail, x1,
		x2, y);
#endif

	CHECK_ARGS
	
	/* Move the seperator line in menus down by 1 pixel. */
	if (CHECK_DETAIL (detail, "menuitem"))
		y++;

	ge_gdk_color_to_cairo (&style->fg[state_type], &color);
	color.a = LINE_OPACITY;

	cr = ge_gdk_drawable_to_cairo (window, area);
	ge_cairo_set_color (cr, &color);

	cairo_move_to (cr, x1 + 0.5, y + 0.5);
	cairo_line_to (cr, x2 + 0.5, y + 0.5);

	cairo_stroke (cr);

	cairo_destroy (cr);
}

/**************************************************************************/

static void
draw_handle (GtkStyle      *style,
	     GdkWindow     *window,
	     GtkStateType   state_type,
	     GtkShadowType  shadow_type,
	     GdkRectangle  *area,
	     GtkWidget     *widget,
	     const gchar   *detail,
	     gint           x,
	     gint           y,
	     gint           width,
	     gint           height,
	     GtkOrientation orientation)
{
	int handle_width, handle_height, maxwidth, maxheight;
	CairoColor color;
	cairo_t *cr;

	CHECK_ARGS
	SANITIZE_SIZE
	
#ifdef DEBUG
	 printf ("%s: %p %p %s %i %i %i %i  %i\n", __FUNCTION__, widget,
		 window, detail, x, y, width, height, orientation);
#endif

	gtk_paint_box (style, window, state_type, shadow_type, area, widget,
		       detail, x, y, width, height);

	if (!CHECK_DETAIL (detail, "paned")) {
		x += 2;
		y += 2;
		width -= 4;
		height -= 4;
	}

	if (shadow_type == GTK_SHADOW_NONE) {
		maxwidth = width;
		maxheight = height;
	} else {
		maxwidth = width - 2;
		maxheight = height - 2;
	}

	if (orientation == GTK_ORIENTATION_HORIZONTAL) {
		handle_width = MIN (maxwidth, HANDLE_HEIGHT);
		handle_height = MIN (maxheight, HANDLE_WIDTH);
	} else {
		handle_width = MIN (maxwidth, HANDLE_WIDTH);
		handle_height = MIN (maxheight, HANDLE_HEIGHT);
	}

	if (handle_width <= 0 || handle_height <= 0)
		return;

	x += (width - handle_width) / 2;
	y += (height - handle_height) / 2;

	ge_gdk_color_to_cairo (&style->fg[state_type], &color);

	color.a = HANDLE_OPACITY;

	cr = ge_gdk_drawable_to_cairo (window, area);

	draw_grid_cairo (cr, &color, x, y, handle_width, handle_height);

	cairo_destroy (cr);
}

/**************************************************************************/

static void
draw_vline (GtkStyle     *style,
	    GdkWindow    *window,
	    GtkStateType  state_type,
	    GdkRectangle *area,
	    GtkWidget    *widget,
	    const gchar  *detail,
	    gint          y1,
	    gint          y2,
	    gint          x)
{
	CairoColor color;
	cairo_t *cr;

#ifdef DEBUG
	printf ("%s: %p %p %s %i %i %i\n", __FUNCTION__, widget, window, detail,
		y1, y2, x);
#endif

	CHECK_ARGS
	
	ge_gdk_color_to_cairo (&style->fg[state_type], &color);
	color.a = LINE_OPACITY;

	cr = ge_gdk_drawable_to_cairo (window, area);
	ge_cairo_set_color (cr, &color);

	cairo_move_to (cr, x + 0.5, y1 + 0.5);
	cairo_line_to (cr, x + 0.5, y2 + 0.5);

	cairo_stroke (cr);

	cairo_destroy (cr);
}

/**************************************************************************/
static void
draw_slider (GtkStyle      *style,
	     GdkWindow     *window,
	     GtkStateType   state_type,
	     GtkShadowType  shadow_type,
	     GdkRectangle  *area,
	     GtkWidget     *widget,
	     const gchar   *detail,
	     gint           x,
	     gint           y,
	     gint           width,
	     gint           height,
	     GtkOrientation orientation)
{
	int handle_width, handle_height;
	CairoColor color;
	cairo_t *cr;

#ifdef DEBUG
	printf ("%s: %p %p %s %i %i %i %i  %i\n", __FUNCTION__, widget, window,
		detail, x, y, width, height, orientation);
#endif

	CHECK_ARGS
	SANITIZE_SIZE
	
	/* This makes the slider overlay the stepper buttons at the sides. XXX */
	if (widget && GTK_IS_SCROLLBAR (widget)) {
		GtkAdjustment *adjustment = GTK_RANGE (widget)->adjustment;
		/* If all of these values are 0, don't do anything.  This fixes mozilla/galeon scrollbars. */
		if (adjustment->value ||
		    adjustment->lower ||
		    adjustment->upper ||
		    adjustment->step_increment ||
		    adjustment->page_increment ||
		    adjustment->page_size) {
			if (adjustment->value <= adjustment->lower) {
				if (orientation == GTK_ORIENTATION_VERTICAL) {
					y--;
					height++;
				} else {
					x--;
					width++;
				}
			}
			if (adjustment->value >= adjustment->upper - adjustment->page_size) {
				if (orientation == GTK_ORIENTATION_VERTICAL) {
					height++;
				} else {
					width++;
				}
			}
		}
	}

	/* XXX? */
	gtk_paint_box (style, window, state_type, shadow_type,
		       area, widget, detail, x, y, width, height);

	if (orientation == GTK_ORIENTATION_HORIZONTAL) {
		handle_width = MIN (width - 2, HANDLE_HEIGHT);
		handle_height = MIN (height - 2, HANDLE_WIDTH);
	} else {
		handle_width = MIN (width - 2, HANDLE_WIDTH);
		handle_height = MIN (height - 2, HANDLE_HEIGHT);
	}

	x += (width - handle_width) / 2;
	y += (height - handle_height) / 2;

	ge_gdk_color_to_cairo (&style->fg[state_type], &color);
	color.a = HANDLE_OPACITY;
	cr = ge_gdk_drawable_to_cairo (window, area);

	draw_grid_cairo (cr, &color, x, y, handle_width, handle_height);

	cairo_destroy (cr);
}

static void
real_draw_box (GtkStyle      *style,
	       cairo_t       *cr,
	       GdkWindow     *window,
	       GtkStateType   state_type,
	       GtkShadowType  shadow_type,
	       GdkRectangle  *area,
	       GtkWidget     *widget,
	       const gchar   *detail,
	       gint           x,
	       gint           y,
	       gint           width,
	       gint           height,
	       gboolean       fill)
{
	/* None of the drawing routines below are set up to handle SHADOW_TYPE_NONE,
	 * so just fill in a blank box here.  Ideally everything in the theme
	 * would be reworked to handle SHADOW_TYPE_NONE correctly */
	if (shadow_type == GTK_SHADOW_NONE)
		return;

	cairo_save (cr);

	if (CHECK_DETAIL (detail, "button") ||
	    CHECK_DETAIL (detail, "togglebutton") ||
	    CHECK_DETAIL (detail, "optionmenu")) {
		/* Draw the button border, except if this is a "default" button. */
		CairoColor bg;
		CairoColor fg;

		ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
		ge_gdk_color_to_cairo (&style->fg[state_type], &fg);
		fg.a = BUTTON_BORDER_OPACITY;

		if (GET_ROUNDED_BUTTONS (style))
			ge_cairo_rounded_rectangle (cr, x + 0.5, y + 0.5,
						    width - 1, height - 1, 2.5,
						    CR_CORNER_ALL);
		else
			cairo_rectangle (cr, x + 0.5, y + 0.5,
			                 width - 1, height - 1);

		ge_cairo_set_color (cr, &bg);
		cairo_fill_preserve (cr);

		cairo_set_line_width (cr, 1);
		ge_cairo_set_color (cr, &fg);
		cairo_stroke (cr);

		if (shadow_type == GTK_SHADOW_IN) {
			gint slope = MIN (width / 2, height / 2);
			CairoColor inner;
			CairoColor outer;

			ge_gdk_color_to_cairo (&style->fg[state_type], &inner);
			ge_gdk_color_to_cairo (&style->fg[state_type], &outer);
			/* XXX: some way to calculate these would be nice. */
			inner.a = BUTTON_DEPRESSED_SHADOW_OPACITY_2;
			outer.a = BUTTON_DEPRESSED_SHADOW_OPACITY_1;

			cairo_save (cr);
			cairo_move_to (cr, x, y + height);
			cairo_line_to (cr, x + slope, y + height - slope);
			cairo_line_to (cr, x + width - slope, y + slope);
			cairo_line_to (cr, x + width, y);
			cairo_line_to (cr, x, y);
			cairo_close_path (cr);
			cairo_clip (cr);

			if (GET_ROUNDED_BUTTONS (style))
				draw_rounded_gradient (cr, x + 1, y + 1,
						       width - 2, height - 2,
						       0.5, 2.5, &inner,
						       &outer);
			else
				draw_rounded_gradient (cr, x + 1, y + 1,
						       width - 2, height - 2,
						       0.0, 2.0, &inner,
						       &outer);

			cairo_restore (cr);
		}

		/* Only draw shadow if button is large enough to fit it. */
		if (width > 18 && height > 18) {
			gint slope = MIN (width / 2, height / 2);
			CairoColor inner;
			CairoColor outer;

			/* Offset shadow if depressed. */
			if (shadow_type == GTK_SHADOW_IN) {
				x++;
				y++;
			}

			ge_gdk_color_to_cairo (&style->fg[state_type], &inner);
			ge_gdk_color_to_cairo (&style->fg[state_type], &outer);
			inner.a = 0;
			outer.a = 0.1;	/* XXX: #define this? */

			cairo_save (cr);
			cairo_move_to (cr, x, y + height);
			cairo_line_to (cr, x + slope, y + height - slope);
			cairo_line_to (cr, x + width - slope, y + slope);
			cairo_line_to (cr, x + width, y);
			cairo_line_to (cr, x + width, y + height);
			cairo_close_path (cr);
			cairo_clip (cr);

			/* XXX: This results in a few pixel that stay blank.
			 * Need to rethink this one. */
			if (GET_ROUNDED_BUTTONS (style))
				draw_rounded_gradient (cr, x + 1, y + 1,
						       width - 2, height - 2, 1,
						       5, &inner, &outer);
			else
				draw_rounded_gradient (cr, x + 1, y + 1,
						       width - 2, height - 2, 0,
						       4, &inner, &outer);
			cairo_restore (cr);
		}

		/* fake a line inside the optionmenu */
		if (CHECK_DETAIL (detail, "optionmenu")) {
			gint vline_x;
			gint spacing;
			GtkRequisition indicator_size;
			GtkBorder indicator_spacing;

			ge_option_menu_get_props (widget, &indicator_size,
						  &indicator_spacing);
			spacing = indicator_size.width;
			spacing +=
			    indicator_spacing.left + indicator_spacing.right;
			spacing += style->xthickness;
			/* seems to be 2px ... where do those come from? */
			spacing -= 2;

			if (ge_widget_is_ltr (widget))
				vline_x = x + width - spacing;
			else
				vline_x = x + spacing;

			gtk_paint_vline (style, window, state_type, area,
					 widget, detail,
					 y + style->ythickness + 4,
					 y + height - style->ythickness - 4,
					 vline_x);
		}
	} else if (CHECK_DETAIL (detail, "buttondefault") ||
		   CHECK_DETAIL (detail, "togglebuttondefault")) {
		CairoColor shadow_inner;
		CairoColor shadow_outer;
		CairoColor fg;
		gfloat inner_radius;

		inner_radius = GET_ROUNDED_BUTTONS (style) ? 2.5 : 0;

		ge_gdk_color_to_cairo (&style->fg[state_type], &fg);

		ge_cairo_rounded_rectangle (cr, x + DEFAULT_SHADOW_SIZE + 0.5,
					    y + DEFAULT_SHADOW_SIZE + 0.5,
					    width - 2 * DEFAULT_SHADOW_SIZE - 1,
					    height - 2 * DEFAULT_SHADOW_SIZE -
					    1, inner_radius, CR_CORNER_ALL);

		ge_cairo_set_color (cr, &fg);
		cairo_stroke (cr);

		/* draw the shadow around this. */
		shadow_outer = fg;
		shadow_inner = fg;
		shadow_outer.a = 0;
		shadow_inner.a = DEFAULT_SHADOW_OPACITY;

		draw_rounded_gradient (cr, x, y, width, height, inner_radius,
				       DEFAULT_SHADOW_SIZE + inner_radius,
				       &shadow_inner, &shadow_outer);
	} else if (CHECK_DETAIL (detail, "menuitem")) {
		CairoColor bevel;
		CairoColor bg;

		ge_gdk_color_to_cairo (&style->bg[state_type], &bg);

		/* for some reason, industrial always uses GTK_STATE_NORMAL here, can't change that
		 * without breaking compatibility. */
		ge_gdk_color_to_cairo (&style->fg[GTK_STATE_NORMAL], &bevel);
		bevel.a = MENUITEM_BORDER_OPACITY;

		if (fill)
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, &bg);
		else
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, NULL);
	} else if (CHECK_DETAIL (detail, "trough")
		   || CHECK_DETAIL (detail, "menu")
		   || CHECK_DETAIL (detail, "dockitem_bin")
		   || CHECK_DETAIL (detail, "handlebox_bin")
		   || CHECK_DETAIL (detail, "toolbar")
		   || CHECK_DETAIL (detail, "dockitem")
		   || WIDGET_TYPE_NAME ("PanelAppletFrame")) {
		CairoColor bevel;
		CairoColor bg;

		if (fill) {
			/* XXX: should also handle the trough invert style property ...
			 * but what do I want to do in that case? */
			if (CHECK_DETAIL (detail, "trough")) {
				/* Troughs cannot be transparent!
				 * This is a hack to fix this by filling with a solid color first. */
				ge_gdk_color_to_cairo (&style->bg[GTK_STATE_NORMAL], &bg);
				ge_cairo_set_color (cr, &bg);
				cairo_rectangle (cr, x, y, width, height);
				cairo_fill (cr);

				/* Troughs always draw with state ACTIVE or INSENSITIVE,
				 * so this sets the state to NORMAL to get the correct colors. */
				if (state_type == GTK_STATE_ACTIVE)
					state_type = GTK_STATE_NORMAL;

				ge_gdk_color_to_cairo (&style->fg[state_type], &bg);
				bg.a = TROUGH_BG_OPACITY;
				ge_gdk_color_to_cairo (&style->fg[state_type],  &bevel);
				bevel.a = TROUGH_BORDER_OPACITY;
			} else {
				ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
				ge_gdk_color_to_cairo (&style->fg[state_type], &bevel);
				bevel.a = TROUGH_BORDER_OPACITY;
			}
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, &bg);
		}
	} else if (CHECK_DETAIL (detail, "menubar")) {
		CairoColor bg;
		CairoColor bevel;

		ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
		ge_gdk_color_to_cairo (&style->fg[state_type], &bevel);
		bevel.a = MENUBAR_BORDER_OPACITY;

		if (fill)
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, &bg);
		else
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, NULL);
	} else if (CHECK_DETAIL (detail, "spinbutton_up")
		   || CHECK_DETAIL (detail, "spinbutton_down")) {
		if (CHECK_DETAIL (detail, "spinbutton_up")) {
			x += 1;
			y += 1;
			width -= 2;
			height -= 1;
		} else {
			x += 1;
			width -= 2;
			height -= 1;
		}

		parent_class->draw_flat_box (style, window, state_type,
					     shadow_type, area, widget, detail,
					     x, y, width, height);
	} else if (CHECK_DETAIL (detail, "slider")
		   || CHECK_DETAIL (detail, "stepper")
		   || CHECK_DETAIL (detail, "vscrollbar")
		   || CHECK_DETAIL (detail, "hscrollbar")) {
		CairoColor bevel, bg;
		/* The original had some weird stuff here, which would draw the bottom/right
		 * lines different then the top/left ones. I just dropped this. */

		ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
		ge_gdk_color_to_cairo (&style->fg[state_type], &bevel);
		bevel.a = 0.5; /* XXX: #define this? */

		/* XXX: fill can never be FALSE, right? */
		if (fill)
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, &bg);
		else
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, NULL);
	} else {
		CairoColor bevel, bg;

		/* XXX: the "spinbutton" should not have rounded corners on the inside ... */
		if (IS_SPIN_BUTTON (widget) && CHECK_DETAIL (detail, "entry")) {
			/* effectively cut one side off. */
			width += 2;
			if (!ge_widget_is_ltr (widget))
				x -= 2;
		}

		ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
		ge_gdk_color_to_cairo (&style->fg[state_type], &bevel);
		bevel.a = STANDARD_BORDER_OPACITY;

		if (fill)
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, &bg);
		else
			draw_rounded_rect_cairo (cr, x, y, width, height, 1.5,
						 &bevel, NULL);
	}

	cairo_restore (cr);
}

static void
draw_box (GtkStyle      *style,
	  GdkWindow     *window,
	  GtkStateType   state_type,
	  GtkShadowType  shadow_type,
	  GdkRectangle  *area,
	  GtkWidget     *widget,
	  const gchar   *detail,
	  gint           x,
	  gint           y,
	  gint           width,
	  gint           height)
{
	cairo_t *cr;
#ifdef DEBUG
	printf ("draw_box: %p %p %s %i %i %i %i\n", widget, window, detail, x,
		y, width, height);
#endif
	CHECK_ARGS SANITIZE_SIZE cr = ge_gdk_drawable_to_cairo (window, area);

	real_draw_box (style, cr, window, state_type, shadow_type, area, widget,
		       detail, x, y, width, height, TRUE);

	cairo_destroy (cr);
}

static void
draw_shadow (GtkStyle      *style,
	     GdkWindow     *window,
	     GtkStateType   state_type,
	     GtkShadowType  shadow_type,
	     GdkRectangle  *area,
	     GtkWidget     *widget,
	     const gchar   *detail,
	     gint           x,
	     gint           y,
	     gint           width,
	     gint           height)
{
	cairo_t *cr;
#ifdef DEBUG
	printf ("draw_shadow: %p %p %s %i %i %i %i\n", widget, window, detail,
		x, y, width, height);
#endif
	CHECK_ARGS SANITIZE_SIZE cr = ge_gdk_drawable_to_cairo (window, area);

	real_draw_box (style, cr, window, state_type, shadow_type, area, widget,
		       detail, x, y, width, height, FALSE);

	cairo_destroy (cr);
}

/**************************************************************************/

static void
real_draw_box_gap (GtkStyle       *style,
		   cairo_t        *cr,
		   GdkWindow      *window,
		   GtkStateType    state_type,
		   GtkShadowType   shadow_type,
		   GdkRectangle   *area,
		   GtkWidget      *widget,
		   const gchar    *detail,
		   gint            x,
		   gint            y,
		   gint            width,
		   gint            height,
		   GtkPositionType gap_side,
		   gint            gap_x,
		   gint            gap_width,
		   gboolean        fill)
{
	CairoColor bevel, bg;
	gint gap_maxwidth = 0;

	cairo_save (cr);

	switch (gap_side) {
	case GTK_POS_TOP:
	case GTK_POS_BOTTOM:
		gap_maxwidth = width;
		break;
	case GTK_POS_LEFT:
	case GTK_POS_RIGHT:
		gap_maxwidth = height;
		break;
	default:
		g_assert_not_reached();
	}

	gap_x -= 1;
	gap_width += 2;

	if (gap_x < 0) {
		gap_width += gap_x;
		gap_x = 0;
	}
	if (gap_x + gap_width > gap_maxwidth) {
		gap_width = gap_maxwidth - gap_x;
	}

	ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
	ge_gdk_color_to_cairo (&style->fg[state_type], &bevel);
	bevel.a = STANDARD_BORDER_OPACITY;

	/* clip the gap area */
	cairo_save (cr);
	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_rectangle (cr, x, y, width, height);

	switch (gap_side) {
	case GTK_POS_TOP:
		cairo_rectangle (cr, x + gap_x, y, gap_width, 1);
		break;
	case GTK_POS_BOTTOM:
		cairo_rectangle (cr, x + gap_x, y + height - 1, gap_width, 1);
		break;
	case GTK_POS_LEFT:
		cairo_rectangle (cr, x, y + gap_x, 1, gap_width);
		break;
	case GTK_POS_RIGHT:
		cairo_rectangle (cr, x + width - 1, y + gap_x, 1, gap_width);
		break;
	}

	ge_cairo_set_color (cr, &bg);
	if (fill)
		cairo_fill_preserve (cr);
	cairo_clip (cr);

	/* reset fill rule */
	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);

	if (fill)
		draw_rounded_rect_cairo (cr, x, y, width, height, 1.5, &bevel, &bg);
	else
		draw_rounded_rect_cairo (cr, x, y, width, height, 1.5, &bevel, NULL);
	cairo_restore (cr);

	/* and draw two dots on both sides of the gap. */
	switch (gap_side) {
	case GTK_POS_TOP:
		cairo_rectangle (cr, x + gap_x, y, 1, 1);
		cairo_rectangle (cr, x + gap_x + gap_width - 1, y, 1, 1);
		break;
	case GTK_POS_BOTTOM:
		cairo_rectangle (cr, x + gap_x, y + height - 1, 1, 1);
		cairo_rectangle (cr, x + gap_x + gap_width - 1, y + height - 1, 1, 1);
		break;
	case GTK_POS_LEFT:
		cairo_rectangle (cr, x, y + gap_x, 1, 1);
		cairo_rectangle (cr, x, y + gap_x + gap_width - 1, 1, 1);
		break;
	case GTK_POS_RIGHT:
		cairo_rectangle (cr, x + width - 1, y + gap_x, 1, 1);
		cairo_rectangle (cr, x + width - 1, y + gap_x + gap_width - 1, 1, 1);
		break;
	}
	ge_cairo_set_color (cr, &bevel);
	cairo_fill (cr);

	cairo_restore (cr);
}

static void
draw_box_gap (GtkStyle      *style,
	      GdkWindow     *window,
	      GtkStateType   state_type,
	      GtkShadowType  shadow_type,
	      GdkRectangle  *area,
	      GtkWidget     *widget,
	      const gchar   *detail,
	      gint            x,
	      gint            y,
	      gint            width,
	      gint            height,
	      GtkPositionType gap_side,
	      gint            gap_x,
	      gint            gap_width)
{
	cairo_t *cr;

#ifdef DEBUG
	printf ("%s: %p %p %s %i %i %i %i  %i\n", __FUNCTION__, widget, window,
		detail, x, y, width, height, gap_side);
#endif

	if (shadow_type == GTK_SHADOW_NONE)
		return;

	CHECK_ARGS
	SANITIZE_SIZE
	
	cr = ge_gdk_drawable_to_cairo (window, area);

	real_draw_box_gap (style, cr, window, state_type, shadow_type, area,
			   widget, detail, x, y, width, height, gap_side, gap_x,
			   gap_width, TRUE);

	cairo_destroy (cr);
}

static void
draw_shadow_gap (GtkStyle       *style,
		 GdkWindow      *window,
		 GtkStateType    state_type,
		 GtkShadowType   shadow_type,
		 GdkRectangle   *area,
		 GtkWidget      *widget,
		 const gchar    *detail,
		 gint            x,
		 gint            y,
		 gint            width,
		 gint            height,
		 GtkPositionType gap_side,
		 gint            gap_x,
		 gint            gap_width)
{
	cairo_t *cr;

#ifdef DEBUG
	printf ("%s: %p %p %s %i %i %i %i  %i\n", __FUNCTION__, widget, window,
		detail, x, y, width, height, gap_side);
#endif

	if (shadow_type == GTK_SHADOW_NONE)
		return;

	cr = ge_gdk_drawable_to_cairo (window, area);

	real_draw_box_gap (style, cr, window, state_type, shadow_type, area,
			   widget, detail, x, y, width, height, gap_side, gap_x,
			   gap_width, FALSE);

	cairo_destroy (cr);
}

static void
draw_extension (GtkStyle       *style,
		GdkWindow      *window,
		GtkStateType    state_type,
		GtkShadowType   shadow_type,
		GdkRectangle   *area,
		GtkWidget      *widget,
		const gchar    *detail,
		gint            x,
		gint            y,
		gint            width,
		gint            height,
		GtkPositionType gap_side)
{
	cairo_t *cr;
	cairo_pattern_t *pattern = NULL;
	CairoColor bevel;
	CairoColor bg;

#ifdef DEBUG
	printf ("%s: %p %p %s %i %i %i %i  %i\n", __FUNCTION__, widget, window,
		detail, x, y, width, height, gap_side);
#endif

	if (shadow_type == GTK_SHADOW_NONE)
		return;

	cr = ge_gdk_drawable_to_cairo (window, area);
	cairo_rectangle (cr, x, y, width, height);
	cairo_clip (cr);

	ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
	ge_gdk_color_to_cairo (&style->fg[state_type], &bevel);
	bevel.a = STANDARD_BORDER_OPACITY;

	switch (gap_side) {
	case GTK_POS_TOP:
		draw_rounded_rect_cairo (cr, x, y - 3, width, height + 3, 1.5, &bevel, &bg);
		pattern = cairo_pattern_create_linear (x, y, x, y + 4);
		cairo_rectangle (cr, x, y, width, 4);
		break;
	case GTK_POS_BOTTOM:
		draw_rounded_rect_cairo (cr, x, y, width, height + 3, 1.5, &bevel, &bg);
		pattern = cairo_pattern_create_linear (x, y + height, x, y + height - 4);
		cairo_rectangle (cr, x, y + height - 4, width, 4);
		break;
	case GTK_POS_LEFT:
		draw_rounded_rect_cairo (cr, x - 3, y, width + 3, height, 1.5, &bevel, &bg);
		pattern = cairo_pattern_create_linear (x, y, x + 4, y);
		cairo_rectangle (cr, x, y, 4, height);
		break;
	case GTK_POS_RIGHT:
		draw_rounded_rect_cairo (cr, x, y, width + 3, height, 1.5, &bevel, &bg);
		pattern = cairo_pattern_create_linear (x + width, y, x + width - 4, y);
		cairo_rectangle (cr, x + width - 4, y, 4, height);
		break;
	}

	if (state_type != GTK_STATE_NORMAL) {
		bevel.a = 0.15;
		ge_cairo_pattern_add_color_stop_color (pattern, 0, &bevel);
		bevel.a = 0;
		ge_cairo_pattern_add_color_stop_color (pattern, 1, &bevel);

		cairo_set_source (cr, pattern);
		cairo_fill (cr);
	}

	cairo_pattern_destroy (pattern);
	cairo_destroy (cr);
}

static void
draw_check (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail, gint x, gint y, gint width, gint height)
{
	gint check_size;
	cairo_t *cr;
	CairoColor fg, bg;

#ifdef DEBUG
	printf ("%s: %p %p %s %i %i %i %i\n", __FUNCTION__, widget, window,
		detail, x, y, width, height);
#endif

	cr = ge_gdk_drawable_to_cairo (window, area);
	if (state_type == GTK_STATE_NORMAL) {
		ge_gdk_color_to_cairo (&style->text[state_type], &fg);
		ge_gdk_color_to_cairo (&style->base[state_type], &bg);
	} else {
		ge_gdk_color_to_cairo (&style->fg[state_type], &fg);
		ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
	}

	check_size = width > height ? height : width;
	/* center the check */
	x += (width - check_size) / 2;
	y += (height - check_size) / 2;

	width = check_size;
	height = check_size;

	fg.a = DEFAULT_SHADOW_OPACITY;
	draw_rounded_rect_cairo (cr, x, y, width, height, 1.5, &fg, &bg);

	/* XXX: following stuff need rework ... ;-) */
	cairo_translate (cr, x + 2.0, y + 2.0);

	cairo_scale (cr, (check_size - 4) / 7., (check_size - 4) / 7.);

	fg.a = 1.0;
	ge_cairo_set_color (cr, &fg);
	/* draw the interior */
	if (shadow_type == GTK_SHADOW_IN) {
		/* copied from GTK+, but it should be more bold ... */
		cairo_move_to (cr, 7.0, 0.0);
		cairo_line_to (cr, 7.5, 1.0);
		cairo_curve_to (cr, 5.3, 2.0, 4.3, 4.0, 3.5, 7.0);
		cairo_curve_to (cr, 3.0, 5.7, 1.3, 4.7, 0.0, 4.7);
		cairo_line_to (cr, 0.2, 3.5);
		cairo_curve_to (cr, 1.1, 3.5, 2.3, 4.3, 3.0, 5.0);
		cairo_curve_to (cr, 1.0, 3.9, 2.4, 4.1, 3.2, 4.9);
		cairo_curve_to (cr, 3.5, 3.1, 5.2, 2.0, 7.0, 0.0);

		cairo_fill (cr);
	} else if (shadow_type == GTK_SHADOW_ETCHED_IN) {
		cairo_set_line_width (cr, 2.0);
		cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
		cairo_move_to (cr, 0.0, 3.5);
		cairo_line_to (cr, 7.0, 3.5);
		cairo_stroke (cr);
	}

	cairo_destroy (cr);
}

static void
draw_option (GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail, gint x, gint y, gint width, gint height)
{
	cairo_t *cr;
	CairoColor fg, bg;
	gfloat radius;
	gfloat xc, yc;

#ifdef DEBUG
	printf ("%s: %p %p %s %i %i %i %i\n", __FUNCTION__, widget, window,
		detail, x, y, width, height);
#endif

	cr = ge_gdk_drawable_to_cairo (window, area);
	if (state_type == GTK_STATE_NORMAL) {
		ge_gdk_color_to_cairo (&style->base[state_type], &bg);
		ge_gdk_color_to_cairo (&style->text[state_type], &fg);
	} else {
		ge_gdk_color_to_cairo (&style->bg[state_type], &bg);
		ge_gdk_color_to_cairo (&style->fg[state_type], &fg);
	}

	radius = MIN (width, height) / 2.0;

	xc = x + width / 2.0;
	yc = y + height / 2.0;
	cairo_arc (cr, xc, yc, radius - 0.5, 0, 2 * M_PI);

	ge_cairo_set_color (cr, &bg);
	cairo_fill (cr);

	fg.a = 0.5;		/* XXX: #define this? */
	ge_cairo_set_color (cr, &fg);

	/* XXX: There should be another shadow ... */
	cairo_arc (cr, xc, yc, radius - 0.5, -M_PI, M_PI);
	/* cairo_arc (cr, xc + 0.5, yc + 0.3, radius - 0.3, M_PI - M_PI_4 / 2.0, 3 * M_PI_2 + M_PI_4 / 2.0); */

	cairo_stroke (cr);

	if (shadow_type == GTK_SHADOW_IN) {
		cairo_pattern_t *pattern;
		fg.a = 1.0;

		ge_cairo_set_color (cr, &fg);
		cairo_arc (cr, xc, yc, radius - 3, 0, 2 * M_PI);
		cairo_fill (cr);

		pattern =
		    cairo_pattern_create_radial (xc, yc, 0, xc, yc, radius - 4);

		bg.a = 0;
		ge_cairo_pattern_add_color_stop_color (pattern, 0, &bg);
		bg.a = 0.7;	/* XXX: #define this? */
		ge_cairo_pattern_add_color_stop_color (pattern, 1, &bg);

		cairo_set_source (cr, pattern);
		cairo_pattern_destroy (pattern);

		cairo_move_to (cr, xc, yc);
		cairo_arc (cr, xc, yc, radius - 4, M_PI - M_PI_4,
			   3 * M_PI_2 + M_PI_4);
		cairo_close_path (cr);
		cairo_fill (cr);
	}
	/* XXX: inconsistent state missing */
}

static GdkPixbuf*
set_transparency (const GdkPixbuf *pixbuf,
		  gdouble          alpha_percent)
{
	GdkPixbuf *target;
	guchar *data, *current;
	guint x, y, rowstride, height, width;

	g_return_val_if_fail (pixbuf != NULL, NULL);
	g_return_val_if_fail (GDK_IS_PIXBUF (pixbuf), NULL);

	/* Returns a copy of pixbuf with it's non-completely-transparent pixels to
	   have an alpha level "alpha_percent" of their original value. */

	target = gdk_pixbuf_add_alpha (pixbuf, FALSE, 0, 0, 0);

	if (alpha_percent == 1.0)
		return target;
	width = gdk_pixbuf_get_width (target);
	height = gdk_pixbuf_get_height (target);
	rowstride = gdk_pixbuf_get_rowstride (target);
	data = gdk_pixbuf_get_pixels (target);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			/* The "4" is the number of chars per pixel, in this case, RGBA,
			   the 3 means "skip to the alpha" */
			current = data + (y * rowstride) + (x * 4) + 3;
			*(current) = (guchar) (*(current) * alpha_percent);
		}
	}

	return target;
}

static GdkPixbuf*
scale_or_ref (GdkPixbuf *src,
              gint       width,
              gint       height)
{
	if (width == gdk_pixbuf_get_width (src) &&
	    height == gdk_pixbuf_get_height (src)) {
		return g_object_ref (src);
	} else {
		return gdk_pixbuf_scale_simple (src, width, height, GDK_INTERP_BILINEAR);
	}
}

static GdkPixbuf*
render_icon (GtkStyle            *style,
	     const GtkIconSource *source,
	     GtkTextDirection     direction,
	     GtkStateType         state,
	     GtkIconSize          size,
	     GtkWidget           *widget,
	     const char          *detail)
{
	int width = 1;
	int height = 1;
	GdkPixbuf *scaled;
	GdkPixbuf *stated;
	GdkPixbuf *base_pixbuf;
	GdkScreen *screen;
	GtkSettings *settings;

	/* Oddly, style can be NULL in this function, because
	 * GtkIconSet can be used without a style and if so
	 * it uses this function.
	 */

	base_pixbuf = gtk_icon_source_get_pixbuf (source);

	g_return_val_if_fail (base_pixbuf != NULL, NULL);

	if (widget && gtk_widget_has_screen (widget)) {
		screen = gtk_widget_get_screen (widget);
		settings = gtk_settings_get_for_screen (screen);
	} else if (style->colormap) {
		screen = gdk_colormap_get_screen (style->colormap);
		settings = gtk_settings_get_for_screen (screen);
	} else {
		settings = gtk_settings_get_default ();
		GTK_NOTE (MULTIHEAD, g_warning ("Using the default screen for gtk_default_render_icon()"));
	}

	if (size != (GtkIconSize) - 1
	    && !gtk_icon_size_lookup_for_settings (settings, size, &width, &height)) {
		g_warning (G_STRLOC ": invalid icon size '%d'", size);
		return NULL;
	}

	/* If the size was wildcarded, and we're allowed to scale, then scale; otherwise,
	 * leave it alone.
	 */
	if (size != (GtkIconSize) - 1
	    && gtk_icon_source_get_size_wildcarded (source))
		scaled = scale_or_ref (base_pixbuf, width, height);
	else
		scaled = g_object_ref (base_pixbuf);

	/* If the state was wildcarded, then generate a state. */
	if (gtk_icon_source_get_state_wildcarded (source)) {
		if (state == GTK_STATE_INSENSITIVE) {
			stated = set_transparency (scaled, 0.3);
			gdk_pixbuf_saturate_and_pixelate (stated, stated,
							  0.1, FALSE);

			g_object_unref (scaled);
		} else if (state == GTK_STATE_PRELIGHT) {
			stated = gdk_pixbuf_copy (scaled);

			gdk_pixbuf_saturate_and_pixelate (scaled, stated,
							  1.2, FALSE);

			g_object_unref (scaled);
		} else {
			stated = scaled;
		}
	} else
		stated = scaled;

	return stated;
}

#include "industrial_style_versioned_code.h"
