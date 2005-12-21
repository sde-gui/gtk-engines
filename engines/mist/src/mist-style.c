/* Mist theme for gtk, based on thinice, based on raster's Motif theme
 * and the Metal theme.

Mist Author: Dave Camp <dave@ximian.com>
Thinice Authors: Tim Gerla <timg@rrv.net>
                 Tomas Ögren <stric@ing.umu.se>
 */

#include <math.h>
#include <string.h>
#include <gtk/gtk.h>
#include <cairo.h>

#include "mist-style.h"
#include "mist-rc-style.h"

#define YTHICKNESS(style) (style->ythickness)
#define XTHICKNESS(style) (style->xthickness)

#define CHECK_DETAIL(detail, value) ((detail) && (!strcmp(value, detail)))

static GtkStyleClass *parent_class = NULL;

static void mist_style_init       (MistStyle      *style);
static void mist_style_class_init (MistStyleClass *klass);

#define IS_TOGGLE_BUTTON(object) ((object) && object_is_a (object, "GtkToggleButton"))
#define TOGGLE_BUTTON(object) (IS_TOGGLE_BUTTON(object)?(GtkToggleButton *)object:NULL)

/* Widget Type Lookups/Macros */
gboolean
object_is_a (gpointer object, gchar * type_name)
{
  gboolean result = FALSE;

  if ((object))
    {
      GType tmp = g_type_from_name (type_name);
      if (tmp)
	result = g_type_check_instance_is_a ((GTypeInstance * )object, tmp);
    }

  return result;
}

static void
sanitize_size (GdkWindow *window, int *width, int *height)
{
	if ((*width == -1) && (*height == -1))
		gdk_window_get_size (window, width, height);
	else if (*width == -1)
		gdk_window_get_size (window, width, NULL);
	else if (*height == -1)
		gdk_window_get_size (window, NULL, height);
}

static GtkShadowType
get_shadow_type (GtkStyle *style, const char *detail, GtkShadowType requested)
{
	GtkShadowType retval = GTK_SHADOW_NONE;
	
	if (requested != GTK_SHADOW_NONE) {
		retval = GTK_SHADOW_ETCHED_IN;
	}
	
	if (CHECK_DETAIL(detail, "dockitem")
	    || CHECK_DETAIL(detail, "handlebox_bin")) {
		retval = GTK_SHADOW_NONE;
	} else if (CHECK_DETAIL(detail, "spinbutton_up")
		   || CHECK_DETAIL(detail, "spinbutton_down")) {
		retval = GTK_SHADOW_OUT;
	} else if (CHECK_DETAIL(detail, "button") 
		   || CHECK_DETAIL(detail, "togglebutton") 
		   || CHECK_DETAIL(detail, "notebook") 
		   || CHECK_DETAIL(detail, "optionmenu")) {
		retval = requested;
	} else if (CHECK_DETAIL(detail, "menu")) {
		retval = GTK_SHADOW_ETCHED_IN;
	}
	
	return retval;
}

static cairo_t *
mist_begin_paint (GdkDrawable  *window, GdkRectangle *area)
{
    cairo_t *cr;

    cr = (cairo_t*)gdk_cairo_create (window);

    cairo_set_line_width (cr, 1);

    if (area) {
        cairo_rectangle (cr, area->x, area->y, area->width, area->height);
        cairo_clip (cr);
        cairo_new_path (cr);
    }

    return cr;
}

static void
mist_dot (cairo_t *cr,
	  GdkColor *color1,
	  GdkColor *color2,
	  int x,
	  int y)
{
	cairo_save(cr);

	cairo_set_line_width (cr, 0.5);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

	gdk_cairo_set_source_color(cr, color2);	

	cairo_rectangle (cr, x - 1, y, 0.5, 0.5);
	cairo_rectangle (cr, x - 1, y - 1, 0.5, 0.5);
	cairo_rectangle (cr, x, y - 1, 0.5, 0.5);

	cairo_stroke(cr);
	
	gdk_cairo_set_source_color(cr, color1);	

	cairo_rectangle (cr, x + 1, y, 0.5, 0.5);
	cairo_rectangle (cr, x + 1, y + 1, 0.5, 0.5);
	cairo_rectangle (cr, x, y + 1, 0.5, 0.5);

	cairo_stroke(cr);

	cairo_restore(cr);
}

static void mist_line (cairo_t *cr,
			GdkColor *color,
			int x1,
			int y1,
			int x2,
			int y2)
{ 
	cairo_save(cr);

	gdk_cairo_set_source_color(cr, color);	
	cairo_set_line_width (cr, 1);

	cairo_move_to(cr, x1 + 0.5, y1 + 0.5);
	cairo_line_to(cr, x2 + 0.5, y2 + 0.5);

	cairo_stroke(cr);

	cairo_restore(cr);
}


static void mist_polygon (cairo_t *cr,
				GdkColor *color,
				GdkPoint *points,
				gint npoints)
{
	int i = 0;

	cairo_save(cr);

	gdk_cairo_set_source_color(cr, color);	
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

static void
mist_tab (GtkStyle *style,
	  GdkWindow *window,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GdkRectangle *area,
	  GtkWidget *widget,
	  const char *detail,
	  int x,
	  int y,
	  int width,
	  int height)
{
	GtkNotebook *notebook;
	int orientation;
	
	notebook = GTK_NOTEBOOK(widget);
	orientation = notebook->tab_pos;
		
	cairo_t *cr = mist_begin_paint (window, area);

	if ((!style->bg_pixmap[state_type]) || GDK_IS_PIXMAP(window)) 
	{
		gdk_cairo_set_source_color(cr, &style->bg[state_type]);	

		cairo_rectangle(cr, x, y, width, height);
		cairo_fill(cr);
	} else {
		gtk_style_apply_default_background
			(style, window,
			 widget && !GTK_WIDGET_NO_WINDOW(widget),
			 state_type, area, x, y, width, height);
	}
	

	GdkColor *light, *dark;

	light = &style->light[state_type];
	dark = &style->dark[state_type];

	switch(orientation) {
	case GTK_POS_TOP:
		gdk_cairo_set_source_color(cr, light);	
	
		cairo_move_to (cr, x + 0.5, y + height - 0.5);
		cairo_line_to (cr, x + 0.5, y + 0.5);
		cairo_line_to (cr, x + width - 0.5, y + 0.5);
		cairo_stroke (cr);

		gdk_cairo_set_source_color(cr, dark);	
	
		cairo_move_to (cr, x + width - 0.5, y + 0.5);
		cairo_line_to (cr, x + width - 0.5, y + height - 0.5);
		cairo_stroke (cr);

		break;
	case GTK_POS_BOTTOM:
		gdk_cairo_set_source_color(cr, light);	
	
		cairo_move_to (cr, x + 0.5, y + 0.5);
		cairo_line_to (cr, x + 0.5, y + height - 0.5);
		cairo_stroke (cr);

		gdk_cairo_set_source_color(cr, dark);	
	
		cairo_move_to (cr, x + 0.5, y + height - 0.5);
		cairo_line_to (cr, x + width - 0.5, y + height - 0.5);
		cairo_line_to (cr, x + width - 0.5, y + 0.5);
		cairo_stroke (cr);
		break;
	case GTK_POS_LEFT:
		gdk_cairo_set_source_color(cr, light);	
	
		cairo_move_to (cr, x + 0.5, y + height - 0.5);
		cairo_line_to (cr, x + 0.5, y + 0.5);
		cairo_line_to (cr, x + width - 0.5, y + 0.5);
		cairo_stroke (cr);

		gdk_cairo_set_source_color(cr, dark);	
	
		cairo_move_to (cr, x + 0.5, y + height - 0.5);
		cairo_line_to (cr, x + width - 0.5, y + height - 0.5);
		cairo_stroke (cr);

		break;
	case GTK_POS_RIGHT:
		gdk_cairo_set_source_color(cr, light);	
	
		cairo_move_to (cr, x + 0.5, y + 0.5);
		cairo_line_to (cr, x + width - 0.5, y + 0.5);
		cairo_stroke (cr);

		gdk_cairo_set_source_color(cr, dark);	
	
		cairo_move_to (cr, x + width - 0.5, y + 0.5);
		cairo_line_to (cr, x + width - 0.5, y + height - 0.5);
		cairo_line_to (cr, x + 0.5, y + height - 0.5);
		cairo_stroke (cr);
		break;
	}

	cairo_destroy(cr);
}

static void
draw_rect (GtkStyle *style, 
	   cairo_t *cr, 
	   GdkColor *color1, 
	   GdkColor *color2, 
	   int x, 
	   int y,
	   int width, 
	   int height)
{
	gdk_cairo_set_source_color(cr, color2);	

	cairo_move_to (cr, x + 0.5, y + height - 0.5);
	cairo_line_to (cr, x + 0.5, y + 0.5);
	cairo_line_to (cr, x + width - 0.5, y + 0.5);

	if (color1 != color2) 
	{
		cairo_stroke (cr);

		gdk_cairo_set_source_color(cr, color1);

		cairo_move_to (cr, x + width - 0.5, y + 0.5);
	}
	
	cairo_line_to (cr, x + width - 0.5, y + height - 0.5);
	cairo_line_to (cr, x + 0.5, y + height - 0.5);

	cairo_stroke (cr);
}

static void
draw_rect_with_shadow (GtkStyle *style,
		       cairo_t *cr,
		       GtkWidget *widget,
		       GtkStateType state_type,
		       GtkShadowType shadow_type,
		       int x, 
		       int y, 
		       int width, 
		       int height)
{
	GdkColor color1;
	GdkColor color2;
	
	if (shadow_type == GTK_SHADOW_NONE) {
		return;
	}
	
	switch (shadow_type) {
	case GTK_SHADOW_ETCHED_IN :
	case GTK_SHADOW_ETCHED_OUT :
		color1 = style->dark[state_type];
		color2 = style->dark[state_type];
		break;
	case GTK_SHADOW_OUT :
		color1 = style->dark[state_type];
		color2 = style->light[state_type];
		break;
	case GTK_SHADOW_IN :
		color1 = style->light[state_type];
		color2 = style->dark[state_type];
		break;
	case GTK_SHADOW_NONE :
	default :
		color1 = style->bg[state_type];
		color2 = style->bg[state_type];
		
	}
	
	draw_rect (style, cr, &color1, &color2,
		   x, y, width, height);
	
}

static void
draw_hline(GtkStyle *style,
           GdkWindow *window,
           GtkStateType state_type,
           GdkRectangle *area,
           GtkWidget *widget,
           const char *detail,
           int x1,
           int x2,
           int y)
{
	int thickness_light;
	int thickness_dark;
	int i;
	
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	
	cairo_t *cr = mist_begin_paint (window, area);

	if (CHECK_DETAIL(detail, "hseparator") 
	    || CHECK_DETAIL(detail, "menuitem") 
	    || CHECK_DETAIL(detail, "slider")
	    || CHECK_DETAIL(detail, "vscale")) 
	{
		gdk_cairo_set_source_color(cr, &style->dark[state_type]);	

		cairo_move_to (cr, x1 + 0.5, y + 0.5);
		cairo_line_to (cr, x2 - 0.5, y + 0.5);
		cairo_stroke (cr);
	} else {
		thickness_light = YTHICKNESS(style) / 2;
		thickness_dark = YTHICKNESS(style) - thickness_light;
		
		for (i = 0; i < thickness_dark; i++) {
			gdk_cairo_set_source_color(cr, &style->light[state_type]);	
	
			cairo_move_to (cr, x2 - i - 0.5, y + i + 0.5);
			cairo_line_to (cr, x2 + 0.5, y + i + 0.5);
			cairo_stroke (cr);

			gdk_cairo_set_source_color(cr, &style->dark[state_type]);	
	
			cairo_move_to (cr, x1 + 0.5, y + i + 0.5);
			cairo_line_to (cr, x2 - i - 0.5, y + i + 0.5);
			cairo_stroke (cr);
		}
		
		y += thickness_dark;
		for (i = 0; i < thickness_light; i++) {
			gdk_cairo_set_source_color(cr, &style->dark[state_type]);	
	
			cairo_move_to (cr, x1 + 0.5, y + i + 0.5);
			cairo_line_to (cr, x1 + thickness_light - i - 0.5, y + i + 0.5);
			cairo_stroke (cr);

			gdk_cairo_set_source_color(cr, &style->light[state_type]);	
	
			cairo_move_to (cr, x1 + thickness_light - i - 0.5, y + i + 0.5);
			cairo_line_to (cr, x2 + 0.5, y + i + 0.5);
			cairo_stroke (cr);
		}
	}
	
	cairo_destroy(cr);
}

static void
draw_vline(GtkStyle *style,
           GdkWindow *window,
           GtkStateType state_type,
           GdkRectangle *area,
           GtkWidget *widget,
           const char *detail,
           int y1,
           int y2,
           int x)
{
	int thickness_light;
	int thickness_dark;
	int i;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	
	cairo_t *cr = mist_begin_paint (window, area);
	
	if (CHECK_DETAIL(detail, "vseparator") 
	    || CHECK_DETAIL(detail, "toolbar") 
	    || CHECK_DETAIL(detail, "slider") 
	    || CHECK_DETAIL(detail, "hscale")) 
	{
		gdk_cairo_set_source_color(cr, &style->dark[state_type]);	

		cairo_move_to (cr, x + 0.5, y1 + 0.5);
		cairo_line_to (cr, x + 0.5, y2 - 0.5);
		cairo_stroke (cr);
	} else {
		thickness_light = XTHICKNESS(style) / 2;
		thickness_dark = XTHICKNESS(style) - thickness_light;
		
		for (i = 0; i < thickness_dark; i++) {
			gdk_cairo_set_source_color(cr, &style->light[state_type]);	
	
			cairo_move_to (cr, x + i + 0.5, y2 - i - 0.5);
			cairo_line_to (cr, x + i + 0.5, y2 + 0.5);
			cairo_stroke (cr);

			gdk_cairo_set_source_color(cr, &style->dark[state_type]);	
	
			cairo_move_to (cr, x + i + 0.5, y1 + 0.5);
			cairo_line_to (cr, x + i + 0.5, y2 - i - 0.5);
			cairo_stroke (cr);
		}
		
		x += thickness_dark;
		for (i = 0; i < thickness_light; i++) {
			gdk_cairo_set_source_color(cr, &style->dark[state_type]);	
	
			cairo_move_to (cr, x + i + 0.5, y1 + 0.5);
			cairo_line_to (cr, x + i + 0.5, y1 + thickness_light - i - 0.5);
			cairo_stroke (cr);

			gdk_cairo_set_source_color(cr, &style->light[state_type]);	
	
			cairo_move_to (cr, x + i + 0.5, y1 + thickness_light - i - 0.5);
			cairo_line_to (cr, x + i + 0.5, y2 - 0.5);
			cairo_stroke (cr);
		}
	}
	
	cairo_destroy(cr);
}

static void
draw_shadow(GtkStyle *style,
	    GdkWindow *window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle *area,
	    GtkWidget *widget,
	    const char *detail,
	    int x,
	    int y,
	    int width,
	    int height)
{
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	
	sanitize_size (window, &width, &height);

	shadow_type = get_shadow_type (style, detail, shadow_type);

	cairo_t *cr = mist_begin_paint (window, area);

	if (state_type == GTK_STATE_INSENSITIVE && shadow_type != GTK_SHADOW_NONE) {
		shadow_type = GTK_SHADOW_ETCHED_IN;
	}
	
	if (CHECK_DETAIL(detail, "frame") && widget->parent && GTK_IS_STATUSBAR (widget->parent)) {
		if (shadow_type != GTK_SHADOW_NONE) {
			gdk_cairo_set_source_color(cr, &style->dark[GTK_STATE_NORMAL]);	
	
			cairo_move_to (cr, x + 0.5, y + 0.5);
			cairo_line_to (cr, x + width - 0.5, y + 0.5);
			cairo_stroke (cr);
		}
	} else {
		draw_rect_with_shadow (style, cr, widget, state_type,
				       shadow_type,x, y, width, height);
	}

	cairo_destroy(cr);
}

static void
draw_polygon(GtkStyle *style,
             GdkWindow *window,
             GtkStateType state_type,
             GtkShadowType shadow_type,
             GdkRectangle *area,
             GtkWidget *widget,
             const char *detail,
             GdkPoint *points,
             int npoints,
             int fill)
{
#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif /* M_PI */
#ifndef M_PI_4
#define M_PI_4  0.78539816339744830962
#endif /* M_PI_4 */
	static const gdouble pi_over_4 = M_PI_4;
	static const gdouble pi_3_over_4 = M_PI_4 * 3;
	
	GdkColor           *color1;
	GdkColor           *color2;
	GdkColor           *color3;
	GdkColor           *color4;
	gdouble            angle;
	int                xadjust;
	int                yadjust;
	int                i;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	g_return_if_fail(points != NULL);
	
	switch (shadow_type) {
	case GTK_SHADOW_IN:
		color1 = &style->light[state_type];
		color2 = &style->dark[state_type];
		color3 = &style->light[state_type];
		color4 = &style->dark[state_type];
		break;
	case GTK_SHADOW_ETCHED_IN:
		color1 = &style->light[state_type];
		color2 = &style->dark[state_type];
		color3 = &style->dark[state_type];
		color4 = &style->light[state_type];
		break;
	case GTK_SHADOW_OUT:
		color1 = &style->dark[state_type];
		color2 = &style->light[state_type];
		color3 = &style->dark[state_type];
		color4 = &style->light[state_type];
		break;
	case GTK_SHADOW_ETCHED_OUT:
		color1 = &style->dark[state_type];
		color2 = &style->light[state_type];
		color3 = &style->light[state_type];
		color4 = &style->dark[state_type];
		break;
	default:
		return;
	}

	cairo_t *cr = mist_begin_paint (window, area);
	
	if (fill)
		mist_polygon(cr, &style->bg[state_type], points, npoints);
	
	npoints--;
	
	for (i = 0; i < npoints; i++) {
		if ((points[i].x == points[i + 1].x) &&
		    (points[i].y == points[i + 1].y)) {
			angle = 0;
		} else {
			angle = atan2(points[i + 1].y - points[i].y,
				      points[i + 1].x - points[i].x);
		}
		
		if ((angle > -pi_3_over_4) && (angle < pi_over_4)) {
			if (angle > -pi_over_4) {
				xadjust = 0;
				yadjust = 1;
			} else {
				xadjust = 1;
				yadjust = 0;
			}

			mist_line(cr, color1,
				      points[i].x - xadjust, 
				      points[i].y - yadjust,
				      points[i + 1].x - xadjust, 
				      points[i + 1].y - yadjust);
			mist_line(cr, color3,
				      points[i].x, points[i].y,
				      points[i + 1].x, points[i + 1].y);
		}
		else {
			if ((angle < -pi_3_over_4) || (angle > pi_3_over_4)) {
				xadjust = 0;
				yadjust = 1;
			} else {
				xadjust = 1;
				yadjust = 0;
			}
			
			mist_line(cr, color4,
				      points[i].x + xadjust, 
				      points[i].y + yadjust,
				      points[i + 1].x + xadjust, 
				      points[i + 1].y + yadjust);
			mist_line(cr, color2,
				      points[i].x, points[i].y,
				      points[i + 1].x, points[i + 1].y);
		}
	}

	cairo_destroy(cr);
}

static void
draw_diamond(GtkStyle * style,
             GdkWindow * window,
             GtkStateType state_type,
             GtkShadowType shadow_type,
             GdkRectangle * area,
             GtkWidget * widget,
             const char *detail,
             int x,
             int y,
             int width,
             int height)
{
	int half_width;
	int half_height;
	
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);	

	sanitize_size (window, &width, &height);
	
	half_width = width / 2;
	half_height = height / 2;

	cairo_t *cr = mist_begin_paint (window, area);

	switch (shadow_type) {
	case GTK_SHADOW_IN:
		mist_line(cr, &style->light[state_type],
			      x + 2, y + half_height,
			      x + half_width, y + height - 2);
		mist_line(cr, &style->light[state_type],
			      x + half_width, y + height - 2,
			      x + width - 2, y + half_height);
		mist_line(cr, &style->light[state_type],
			      x + 1, y + half_height,
			      x + half_width, y + height - 1);
		mist_line(cr, &style->light[state_type],
			      x + half_width, y + height - 1,
			      x + width - 1, y + half_height);
		mist_line(cr, &style->light[state_type],
			      x, y + half_height,
			      x + half_width, y + height);
		mist_line(cr, &style->light[state_type],
			      x + half_width, y + height,
			      x + width, y + half_height);
		
		mist_line(cr, &style->dark[state_type],
			      x + 2, y + half_height,
			      x + half_width, y + 2);
		mist_line(cr, &style->dark[state_type],
			      x + half_width, y + 2,
			      x + width - 2, y + half_height);
		mist_line(cr, &style->dark[state_type],
			      x + 1, y + half_height,
			      x + half_width, y + 1);
		mist_line(cr, &style->dark[state_type],
			      x + half_width, y + 1,
			      x + width - 1, y + half_height);
		mist_line(cr, &style->dark[state_type],
			      x, y + half_height,
			      x + half_width, y);
		mist_line(cr, &style->dark[state_type],
			      x + half_width, y,
			      x + width, y + half_height);
		break;
	case GTK_SHADOW_OUT:
		mist_line(cr, &style->dark[state_type],
			      x + 2, y + half_height,
			      x + half_width, y + height - 2);
		mist_line(cr, &style->dark[state_type],
			      x + half_width, y + height - 2,
			      x + width - 2, y + half_height);
		mist_line(cr, &style->dark[state_type],
			      x + 1, y + half_height,
			      x + half_width, y + height - 1);
		mist_line(cr, &style->dark[state_type],
			      x + half_width, y + height - 1,
			      x + width - 1, y + half_height);
		mist_line(cr, &style->dark[state_type],
			      x, y + half_height,
			      x + half_width, y + height);
		mist_line(cr, &style->dark[state_type],
			      x + half_width, y + height,
			      x + width, y + half_height);
		
		mist_line(cr, &style->light[state_type],
			      x + 2, y + half_height,
			      x + half_width, y + 2);
		mist_line(cr, &style->light[state_type],
			      x + half_width, y + 2,
			      x + width - 2, y + half_height);
		mist_line(cr, &style->light[state_type],
			      x + 1, y + half_height,
			      x + half_width, y + 1);
		mist_line(cr, &style->light[state_type],
			      x + half_width, y + 1,
			      x + width - 1, y + half_height);
		mist_line(cr, &style->light[state_type],
			      x, y + half_height,
			      x + half_width, y);
		mist_line(cr, &style->light[state_type],
			      x + half_width, y,
			      x + width, y + half_height);
		break;
	default:
		break;
	}

	cairo_destroy(cr);
}

static void
draw_box(GtkStyle *style,
         GdkWindow *window,
         GtkStateType state_type,
         GtkShadowType shadow_type,
         GdkRectangle *area,
         GtkWidget *widget,
         const char *detail,
         int x,
         int y,
         int width,
         int height)
{
	GdkColor *light, *dark;
	GtkOrientation orientation;
	
	g_return_if_fail (style != NULL);
	g_return_if_fail (window != NULL);

	sanitize_size (window, &width, &height);

	if (CHECK_DETAIL(detail, "menuitem") && state_type == GTK_STATE_PRELIGHT) {
		state_type = GTK_STATE_SELECTED;
	}

	light = &style->light[state_type];
	dark = &style->dark[state_type];

	cairo_t *cr = mist_begin_paint (window, area);
	
	if (CHECK_DETAIL(detail, "optionmenutab")) {
		mist_line(cr, dark, x - 5, y, x - 5, y + height);
		
		gtk_paint_arrow (style, window, state_type, shadow_type, area, 
				 widget, detail, GTK_ARROW_DOWN, TRUE,
				 x + 1, y + 1, width - 2, height - 2);
	} else if (CHECK_DETAIL(detail, "trough")) {
		gdk_cairo_set_source_color(cr, &style->bg[state_type]);	
		cairo_rectangle(cr, x, y, width - 1, height - 1);
		cairo_fill(cr);

		gdk_cairo_set_source_color(cr, dark);	
		cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
		cairo_stroke(cr);
	} else if (CHECK_DETAIL(detail, "menubar")
		   || CHECK_DETAIL(detail, "dockitem_bin") 
		   || CHECK_DETAIL(detail, "dockitem") 
		   || CHECK_DETAIL(detail, "toolbar") 
		   || CHECK_DETAIL(detail, "handlebox")) {
		if (shadow_type != GTK_SHADOW_NONE) {
			mist_line (cr, 
				       &style->dark[GTK_STATE_NORMAL], 
				       x, y + height - 1, 
				       x + width - 1, y + height - 1);

		}
	} else if (CHECK_DETAIL(detail, "tab")) {
		mist_tab(style, window, state_type, 
			 shadow_type, area, widget,
			 detail, x, y, width, height);
	} else  if (CHECK_DETAIL(detail, "bar")) {
		if (width > 1 && height > 1) {
			gdk_cairo_set_source_color(cr, &style->base[GTK_STATE_SELECTED]);	
			cairo_rectangle(cr, x + 1, y + 1, width - 2, height - 2);
			cairo_fill(cr);
		}
	

		draw_rect (style, cr, 
			   &style->dark[GTK_STATE_SELECTED],
			   &style->dark[GTK_STATE_SELECTED],
			   x, y, width, height);
	} else if (CHECK_DETAIL(detail, "buttondefault")) {
		gdk_cairo_set_source_color(cr, &style->fg[GTK_STATE_NORMAL]);	
		cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);
		cairo_stroke(cr);
	} else {
		/* Make sure stepper and slider outlines "overlap" - taken from
		 * bluecurve */
		if (CHECK_DETAIL(detail, "slider") && widget && GTK_IS_RANGE (widget)) {
			GtkAdjustment *adj = GTK_RANGE (widget)->adjustment;
			if (adj->value <= adj->lower &&
			    (GTK_RANGE (widget)->has_stepper_a ||
			     GTK_RANGE (widget)->has_stepper_b)) {
				if (GTK_IS_VSCROLLBAR (widget)) {
					height += 1;
					y -= 1;
				} else if (GTK_IS_HSCROLLBAR (widget)) {
					width += 1;
					x -= 1;
				}
			}
			
			if (adj->value >= adj->upper - adj->page_size &&
			    (GTK_RANGE (widget)->has_stepper_c ||
			     GTK_RANGE (widget)->has_stepper_d)) {
				if (GTK_IS_VSCROLLBAR (widget)) {
					height += 1;
				} else if (GTK_IS_HSCROLLBAR (widget)) {
					width += 1;
				}
			}
		}
		
		gtk_style_apply_default_background(style, window,
						   widget && !GTK_WIDGET_NO_WINDOW(widget),
						   state_type, area,
						   x, y, width, height);

		shadow_type = get_shadow_type (style, detail, shadow_type);
		if (state_type == GTK_STATE_INSENSITIVE && shadow_type != GTK_SHADOW_NONE) {
			shadow_type = GTK_SHADOW_ETCHED_IN;
		}
		if (shadow_type != GTK_SHADOW_NONE) {
			draw_rect_with_shadow (style, cr, widget, 
					       state_type, 
					       shadow_type,
					       x, y, width, height);
		}
	}

	cairo_destroy(cr);
}

static void
draw_check(GtkStyle *style,
           GdkWindow *window,
           GtkStateType state_type,
           GtkShadowType shadow_type,
           GdkRectangle *area,
           GtkWidget *widget,
           const char *detail,
           int x,
           int y,
           int width,
           int height)
{
	sanitize_size (window, &width, &height);

	cairo_t *cr = mist_begin_paint (window, area);

	gdk_cairo_set_source_color(cr, &style->base[state_type == GTK_STATE_INSENSITIVE ? GTK_STATE_INSENSITIVE : GTK_STATE_NORMAL ]);	
	cairo_rectangle(cr, x + 1, y + 1, width - 3, height - 3);
	cairo_fill(cr);

	gdk_cairo_set_source_color(cr, &style->dark[state_type]);	
	cairo_rectangle(cr, x + 0.5, y + 0.5, width - 2, height - 2);
	cairo_stroke(cr);
	
	if (shadow_type == GTK_SHADOW_IN) {
		gdk_cairo_set_source_color(cr, &style->base[GTK_STATE_SELECTED]);	
		cairo_rectangle(cr, x + 2, y + 2, width - 5, height - 5);
		cairo_fill(cr);
	} else if (shadow_type == GTK_SHADOW_ETCHED_IN) { /* inconsistent */
#define gray50_width 2
#define gray50_height 2
		GdkBitmap *stipple;
		static const char gray50_bits[] = {
			0x02, 0x01
		};
		
		stipple = gdk_bitmap_create_from_data (window,
						       gray50_bits, 
						       gray50_width,
						       gray50_height);
  
		gdk_gc_set_fill (widget->style->base_gc[GTK_STATE_SELECTED],
				 GDK_STIPPLED);
		gdk_gc_set_stipple (widget->style->base_gc[GTK_STATE_SELECTED],
				    stipple);
		gdk_draw_rectangle(window,
				   widget->style->base_gc[GTK_STATE_SELECTED],
				   TRUE, x + 2, y + 2, width - 5, height - 5);
		gdk_gc_set_fill (widget->style->base_gc[GTK_STATE_SELECTED],
				 GDK_SOLID);

#undef gray50_width
#undef gray50_height 
	}

	cairo_destroy(cr);
}

static void
draw_option(GtkStyle *style,
            GdkWindow *window,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GdkRectangle *area,
            GtkWidget *widget,
            const char *detail,
            int x,
            int y,
            int width,
            int height)
{
	sanitize_size (window, &width, &height);

	cairo_t *cr = mist_begin_paint (window, area);

	cairo_arc(cr, x + floor(width/2), y + floor(height/2), floor(width/2) - 0.5, 0 , 2 * M_PI);
	gdk_cairo_set_source_color(cr, &style->dark[state_type == GTK_STATE_INSENSITIVE ? GTK_STATE_INSENSITIVE : GTK_STATE_NORMAL]);	
	cairo_stroke (cr);
	
	cairo_arc(cr, x + floor(width/2), y + floor(height/2), floor(width/2) - 0.5, 0 , 2 * M_PI);
	gdk_cairo_set_source_color(cr, &style->base[state_type == GTK_STATE_INSENSITIVE ? GTK_STATE_INSENSITIVE : GTK_STATE_NORMAL]);	
	cairo_fill (cr);

	cairo_arc(cr, x + floor(width/2), y + floor(height/2), floor(width/2) - 0.5, 0 , 2 * M_PI);
	cairo_set_source_rgba(cr, 0, 0, 0, 0.35); 
	cairo_stroke (cr);

	if ((shadow_type == GTK_SHADOW_IN) || 
		(IS_TOGGLE_BUTTON(widget) && 
		TOGGLE_BUTTON(widget)->inconsistent))
	{
		if ((IS_TOGGLE_BUTTON(widget) && 
			TOGGLE_BUTTON(widget)->inconsistent))
		{
			/* Rough logic, just make  thickness 1/3 height */
			int line_width = ceil((height + 1)/3);

			/* Force Thickness Even */
			line_width -= (line_width % 2);

			cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
			cairo_set_line_width (cr, line_width + ((height % 2)?1:0));

			cairo_move_to(cr, x + line_width - ((height % 2)?0.5:0), y + floor(height/2));
			cairo_line_to(cr, x + width - line_width + ((height % 2)?0.5:0), y + floor(height/2));

			gdk_cairo_set_source_color(cr, &style->bg[GTK_STATE_SELECTED]);	
			cairo_stroke (cr);
		}
		else
		{
			gdk_cairo_set_source_color(cr, &style->bg[GTK_STATE_SELECTED]);	
			cairo_arc(cr, x + floor(width/2), y + floor(height/2), floor((width - 7)/2) + 1, 0 , 2 * M_PI);
			cairo_fill (cr);
		}
	}

	cairo_destroy(cr);
}

static void
draw_tab(GtkStyle *style,
         GdkWindow *window,
         GtkStateType state_type,
         GtkShadowType shadow_type,
         GdkRectangle *area,
         GtkWidget *widget,
         const char *detail,
         int x,
         int y,
         int width,
         int height)
{
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	
	gtk_paint_box(style, window, state_type, shadow_type, 
		      area, widget, detail,
		      x, y, width, height);
}

static void
draw_shadow_gap(GtkStyle *style,
                GdkWindow *window,
                GtkStateType state_type,
                GtkShadowType shadow_type,
                GdkRectangle *area,
                GtkWidget *widget,
                const char *detail,
                int x,
                int y,
                int width,
                int height,
                GtkPositionType gap_side,
                int gap_x,
                int gap_width)
{
	GdkColor *color1 = NULL;
	GdkColor *color2 = NULL;
	
	g_return_if_fail (window != NULL);
	
	sanitize_size (window, &width, &height);
	shadow_type = get_shadow_type (style, detail, shadow_type);
	
	cairo_t *cr = mist_begin_paint (window, area);

	switch (shadow_type) {
	case GTK_SHADOW_NONE:
		return;
	case GTK_SHADOW_IN:
		color1 = &style->dark[state_type];
		color2 = &style->light[state_type];
		break;
	case GTK_SHADOW_OUT:
		color1 = &style->light[state_type];
		color2 = &style->dark[state_type];
		break;
	case GTK_SHADOW_ETCHED_IN:
	case GTK_SHADOW_ETCHED_OUT:
		color1 = &style->dark[state_type];
		color2 = &style->dark[state_type];
	}
	
	switch (gap_side) {
        case GTK_POS_TOP:
		if (gap_x > 0) {
			mist_line (cr, color1, 
				       x, y, 
				       x + gap_x, y);
		}
		if ((width - (gap_x + gap_width)) > 0) {
			mist_line (cr, color1, 
				       x + gap_x + gap_width - 1, y,
				       x + width - 1, y);
		}
		mist_line (cr, color1,
			       x, y, 
			       x, y + height - 1);
		mist_line (cr, color2,
			       x + width - 1, y,
			       x + width - 1, y + height - 1);
		mist_line (cr, color2,
			       x, y + height - 1,
			       x + width - 1, y + height - 1);
		break;
        case GTK_POS_BOTTOM:
		mist_line (cr, color1,
			       x, y,
			       x + width - 1, y);
		mist_line (cr, color1, 
			       x, y, 
			       x, y + height - 1);
		mist_line (cr, color2,
			       x + width - 1, y,
			       x + width - 1, y + height - 1);

		if (gap_x > 0) {
			mist_line (cr, color2, 
				       x, y + height - 1, 
				       x + gap_x, y + height - 1);
		}
		if ((width - (gap_x + gap_width)) > 0) {
			mist_line (cr, color2, 
				       x + gap_x + gap_width - 1, y + height - 1,
				       x + width - 1, y + height - 1);
		}
		
		break;
        case GTK_POS_LEFT:
		mist_line (cr, color1,
			       x, y,
			       x + width - 1, y);
		if (gap_x > 0) {
			mist_line (cr, color1, 
				       x, y,
				       x, y + gap_x);
		}
		if ((height - (gap_x + gap_width)) > 0) {
			mist_line (cr, color1, 
				       x, y + gap_x + gap_width - 1,
				       x, y + height - 1);
		}
		mist_line (cr, color2,
			       x + width - 1, y,
			       x + width - 1, y + height - 1);
		mist_line (cr, color2,
			       x, y + height - 1,
			       x + width - 1, y + height - 1);
		break;
        case GTK_POS_RIGHT:
		mist_line (cr, color1,
			       x, y,
			       x + width - 1, y);
		mist_line (cr, color1, 
			       x, y, 
			       x, y + height - 1);


		if (gap_x > 0) {
			mist_line (cr, color2, 
				       x + width - 1, y,
				       x + width - 1, y + gap_x);
		}
		if ((height - (gap_x + gap_width)) > 0) {
			mist_line (cr, color2, 
				       x + width - 1, y + gap_x + gap_width - 1,
				       x + width - 1, y + height - 1);
		}
		mist_line (cr, color2,
			       x, y + height - 1,
			       x + width - 1, y + height - 1);

	}
	
	cairo_destroy(cr);
}

static void
draw_box_gap(GtkStyle *style,
	     GdkWindow *window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle *area,
	     GtkWidget *widget,
	     const char *detail,
	     int x,
	     int y,
	     int width,
	     int height,
	     GtkPositionType gap_side,
	     int gap_x,
	     int gap_width)
{
	sanitize_size (window, &width, &height);

	gtk_style_apply_default_background(style, window,
					   widget && !GTK_WIDGET_NO_WINDOW(widget),
					   state_type, area,
					   x, y, width, height);
	draw_shadow_gap (style, window, state_type, shadow_type,
			 area, widget, detail, x, y, width, height, 
			 gap_side, gap_x, gap_width);
}

static void
draw_extension(GtkStyle *style,
               GdkWindow *window,
               GtkStateType state_type,
               GtkShadowType shadow_type,
               GdkRectangle *area,
               GtkWidget *widget,
               const char *detail,
               int x,
               int y,
               int width,
               int height,
               GtkPositionType gap_side)
{
	GdkRectangle        rect;
	
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	
	sanitize_size (window, &width, &height);

	gtk_paint_box(style, window, state_type, shadow_type, 
		      area, widget, detail,
		      x, y, width, height);
	
	switch (gap_side) {
	case GTK_POS_TOP:
		rect.x = x + XTHICKNESS(style);
		rect.y = y;
		rect.width = width - XTHICKNESS(style) * 2;
		rect.height = YTHICKNESS(style);
		break;
	case GTK_POS_BOTTOM:
		rect.x = x + XTHICKNESS(style);
		rect.y = y + height - YTHICKNESS(style);
		rect.width = width - XTHICKNESS(style) * 2;
		rect.height = YTHICKNESS(style);
		break;
	case GTK_POS_LEFT:
		rect.x = x;
		rect.y = y + YTHICKNESS(style);
		rect.width = XTHICKNESS(style);
		rect.height = height - YTHICKNESS(style) * 2;
		break;
	case GTK_POS_RIGHT:
		rect.x = x + width - XTHICKNESS(style);
		rect.y = y + YTHICKNESS(style);
		rect.width = XTHICKNESS(style);
		rect.height = height - YTHICKNESS(style) * 2;
		break;
	}
	
	gtk_style_apply_default_background(style, window,
					   widget && !GTK_WIDGET_NO_WINDOW(widget),
					   state_type, area, rect.x, rect.y, rect.width, rect.height);
}

static void
draw_handle(GtkStyle *style,
            GdkWindow *window,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GdkRectangle *area,
            GtkWidget *widget,
            const char *detail,
            int x,
            int y,
            int width,
            int height,
            GtkOrientation orientation)
{
	GdkColor *light, *dark;
	GdkRectangle dest;
	int modx, mody;
	
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	
	sanitize_size (window, &width, &height);

	gtk_paint_box(style, window, state_type, shadow_type, area, widget,
		      detail, x, y, width, height);
	
	light = &style->light[state_type];
	dark = &style->dark[state_type];
	
	orientation = GTK_ORIENTATION_HORIZONTAL;
	if (height > width)
		orientation = GTK_ORIENTATION_VERTICAL;
	
	x += XTHICKNESS(style);
	y += YTHICKNESS(style);
	dest.x = x;
	dest.y = y;
	dest.width = width - (XTHICKNESS(style) * 2);
	dest.height = height - (YTHICKNESS(style) * 2);
	
	if (orientation == GTK_ORIENTATION_HORIZONTAL) { 
		modx = 4; mody = 0; 
	} else { 
		modx = 0; mody = 4;
	}
	
	cairo_t *cr = mist_begin_paint (window, &dest);

	mist_dot(cr,
		 light, dark,
		 x + width / 2 - modx,
		 y + height / 2 - mody);
	mist_dot(cr,
		 light, dark,
		 x + width / 2,
		 y + height / 2);
	mist_dot(cr,
		 light, dark,
		 x + width / 2 + modx,
		 y + height / 2 + mody);
	
	cairo_destroy(cr);
}

static void
draw_resize_grip(GtkStyle *style,
		 GdkWindow *window,
		 GtkStateType state_type,
		 GdkRectangle *area,
		 GtkWidget *widget,
		 const char *detail,
		 GdkWindowEdge edge,
		 int x,
		 int y,
		 int width,
		 int height)
{
	GdkColor *light, *dark;
	int xi, yi;
	int max_x, max_y;
	int threshold;
	
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	
	sanitize_size (window, &width, &height);
	
	switch (edge) {
	case GDK_WINDOW_EDGE_NORTH_WEST:
		/* make it square */
		if (width < height) {
			height = width;
		}
		else if (height < width) {
			width = height;
		}
		break;
	case GDK_WINDOW_EDGE_NORTH:
		if (width < height) {
			height = width;
		}
		break;
	case GDK_WINDOW_EDGE_NORTH_EAST:
		/* make it square, aligning to top right */
		if (width < height) {
			height = width;
		} else if (height < width) {
			x += (width - height);
			width = height;
		}
		break;
	case GDK_WINDOW_EDGE_WEST:
		if (height < width) {
			width = height;
		}
		break;
	case GDK_WINDOW_EDGE_EAST:
		/* aligning to right */
		if (height < width) {
			x += (width - height);
			width = height;
		}
		break;
	case GDK_WINDOW_EDGE_SOUTH_WEST:
		/* make it square, aligning to bottom left */
		if (width < height) {
			y += (height - width);
			height = width;
		} else if (height < width) {
			width = height;
		}
		break;
	case GDK_WINDOW_EDGE_SOUTH:
		/* align to bottom */
		if (width < height) {
			y += (height - width);
			height = width;
		}
		break;
	case GDK_WINDOW_EDGE_SOUTH_EAST:
		/* make it square, aligning to bottom right */
		if (width < height) {
			y += (height - width);
			height = width;
		} else if (height < width) {
			x += (width - height);
			width = height;
		}
		break;
	default:
		g_assert_not_reached ();
	}
	
	gtk_style_apply_default_background (style, window, FALSE,
					    state_type, area,
					    x, y, width, height);

	light = &style->light[state_type];
	dark = &style->dark[state_type];

	max_x = (width - 2) / 5;
	max_y = (height - 2) / 5;
	
	threshold = max_x;
	

	cairo_t *cr = mist_begin_paint (window, area);

	for (xi = 0; xi <= max_x; xi++) {
		for (yi = 0; yi <= max_y; yi++) {
			gboolean draw_dot;
			
			switch (edge) {
			case GDK_WINDOW_EDGE_NORTH:
			case GDK_WINDOW_EDGE_WEST:
			case GDK_WINDOW_EDGE_EAST:
			case GDK_WINDOW_EDGE_SOUTH:
				draw_dot = TRUE;
				break;
			case GDK_WINDOW_EDGE_NORTH_WEST:
				draw_dot = (xi + yi <= threshold);
				break;
			case GDK_WINDOW_EDGE_NORTH_EAST:
				draw_dot = (xi >= yi);
				break;
			case GDK_WINDOW_EDGE_SOUTH_WEST:
				draw_dot = (xi <= yi);
				break;
			case GDK_WINDOW_EDGE_SOUTH_EAST:
				draw_dot = (xi + yi >= threshold);
				break;
			default:
				g_assert_not_reached ();
			}
			
			if (draw_dot) {
				mist_dot(cr,
					 light, dark,
					 x + (xi * 5) + 2,
					 y + (yi * 5) + 2);
			}
		}
	}

	cairo_destroy(cr);
}

static void
draw_string (GtkStyle      *style,
	     GdkWindow     *window,
	     GtkStateType   state_type,
	     GdkRectangle  *area,
	     GtkWidget     *widget,
	     const char    *detail,
	     int            x,
	     int            y,
	     const char    *string)
{
	g_return_if_fail (GTK_IS_STYLE (style));
	g_return_if_fail (window != NULL);
	
	if (area) {
		gdk_gc_set_clip_rectangle (style->fg_gc[state_type], area);
	}	
	
	gdk_draw_string (window,
			 gtk_style_get_font (style),
			 style->fg_gc[state_type], x, y, string);
	
	if (area) {
		gdk_gc_set_clip_rectangle (style->fg_gc[state_type], NULL);
	}
}

static void
draw_layout (GtkStyle        *style,
	     GdkWindow       *window,
	     GtkStateType     state_type,
	     gboolean         use_text,
	     GdkRectangle    *area,
	     GtkWidget       *widget,
	     const char      *detail,
	     int              x,
	     int              y,
	     PangoLayout      *layout)
{
	GdkGC *gc;
	
	g_return_if_fail (GTK_IS_STYLE (style));
	g_return_if_fail (window != NULL);
	
	gc = use_text ? style->text_gc[state_type] : style->fg_gc[state_type];
	
	if (area) {
		gdk_gc_set_clip_rectangle (gc, area);
	}
	
	gdk_draw_layout (window, gc, x, y, layout);
	
	if (area) {
		gdk_gc_set_clip_rectangle (gc, NULL);
	}
}

static GdkPixbuf *
set_transparency (const GdkPixbuf *pixbuf, gdouble alpha_percent)
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
              int width,
              int height)
{
	if (width == gdk_pixbuf_get_width (src) &&
	    height == gdk_pixbuf_get_height (src)) {
		return g_object_ref (src);
	} else {
		return gdk_pixbuf_scale_simple (src,
						width, height,
						GDK_INTERP_BILINEAR);
	}
}

static GdkPixbuf *
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
		GTK_NOTE (MULTIHEAD,
			  g_warning ("Using the default screen for gtk_default_render_icon()"));
	}
	
  
	if (size != (GtkIconSize) -1 && !gtk_icon_size_lookup_for_settings (settings, size, &width, &height)) {
		g_warning (G_STRLOC ": invalid icon size '%d'", size);
		return NULL;
	}

	/* If the size was wildcarded, and we're allowed to scale, then scale; otherwise,
	 * leave it alone.
	 */
	if (size != (GtkIconSize)-1 && gtk_icon_source_get_size_wildcarded (source))
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
	}
	else
		stated = scaled;
  
  return stated;
}

GType mist_type_style = 0;

void
mist_style_register_type (GTypeModule *module)
{
	static const GTypeInfo object_info = {
		sizeof (MistStyleClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) mist_style_class_init,
		NULL,           /* class_finalize */
		NULL,           /* class_data */
		sizeof (MistStyle),
		0,              /* n_preallocs */
		(GInstanceInitFunc) mist_style_init,
	};
	
	mist_type_style = g_type_module_register_type (module,
						       GTK_TYPE_STYLE,
						       "MistStyle",
						       &object_info, 0);
}

static void
mist_style_init (MistStyle *style)
{
}

static void
mist_style_class_init (MistStyleClass *klass)
{
	GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);
	
	parent_class = g_type_class_peek_parent (klass);
	
	style_class->draw_hline = draw_hline;
	style_class->draw_vline = draw_vline;
	style_class->draw_shadow = draw_shadow;
	style_class->draw_polygon = draw_polygon;
	style_class->draw_diamond = draw_diamond;
	style_class->draw_box = draw_box;
	style_class->draw_check = draw_check;
	style_class->draw_option = draw_option;
	style_class->draw_tab = draw_tab;
	style_class->draw_shadow_gap = draw_shadow_gap;
	style_class->draw_box_gap = draw_box_gap;
	style_class->draw_extension = draw_extension;
	style_class->draw_handle = draw_handle;
	style_class->draw_resize_grip = draw_resize_grip;
	style_class->draw_string = draw_string;
	style_class->draw_layout = draw_layout;
	style_class->render_icon = render_icon;
}
