/* ThinIce theme for gtk, based on raster's Motif theme and the Metal theme.
Authors: Tim Gerla <timg@rrv.net>
Tomas �gren <stric@ing.umu.se>
 */ 
/* #define DEBUG 1 */

#include <math.h>
#include <string.h>
#include <gtk/gtknotebook.h>

#include "thinice_style.h"
#include "thinice_rc_style.h"
#include "thinice_misc.h"

#ifndef max
#define max(x,y) ((x)>=(y)?(x):(y))
#endif
#ifndef min
#define min(x,y) ((x)<=(y)?(x):(y))
#endif

#define DRAW_ARGS    GtkStyle       *style, \
                     GdkWindow      *window, \
                     GtkStateType    state_type, \
                     GtkShadowType   shadow_type, \
                     GdkRectangle   *area, \
                     GtkWidget      *widget, \
                     const gchar    *detail, \
                     gint            x, \
                     gint            y, \
                     gint            width, \
                     gint            height

#define DRAW_VARS    style, window, state_type, shadow_type, area, widget, detail, x, y, width, height

static void thinice_style_init       (ThiniceStyle      *style);
static void thinice_style_class_init (ThiniceStyleClass *klass);

static void
thinice_tab(GtkStyle * style,
            GdkWindow * window,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GdkRectangle * area,
            GtkWidget * widget,
            const gchar *detail,
            gint x,
            gint y,
            gint width,
            gint height,
	    gint orientation);

static GtkStyleClass *parent_class = NULL;

static void
draw_hline(GtkStyle * style,
           GdkWindow * window,
           GtkStateType state_type,
           GdkRectangle * area,
           GtkWidget * widget,
           const gchar *detail,
           gint x1,
           gint x2,
           gint y)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);
	
	CHECK_ARGS

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
	
	thinice_draw_separator(cr, TRUE, x1, y, x2-x1, 2);
	cairo_destroy (cr);
}

static void
draw_vline(GtkStyle * style,
           GdkWindow * window,
           GtkStateType state_type,
           GdkRectangle * area,
           GtkWidget * widget,
           const gchar *detail,
           gint y1,
           gint y2,
           gint x)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

	CHECK_ARGS

	if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
		return;

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
	
	thinice_draw_separator(cr, FALSE, x, y1, 2, y2-y1);

	cairo_destroy (cr);
}

static void
draw_shadow(GtkStyle     *style,
	    GdkWindow    *window,
	    GtkStateType  state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle *area,
	    GtkWidget    *widget,
	    const gchar  *detail,
	    gint          x,
	    gint          y,
	    gint          width,
	    gint          height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

  CairoColor *color1 = NULL;
  CairoColor *color2 = NULL;

  CHECK_ARGS

  SANITIZE_SIZE

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
      if (((x == 1) || (y == 1)) && (CHECK_DETAIL (detail, "entry") || CHECK_DETAIL (detail, "text")))
        {
    			color1 = color2 = &thinice_style->color_cube.base[state_type];
          break;
        }
    case GTK_SHADOW_ETCHED_IN:
			color1 = &thinice_style->color_cube.light[state_type];
			color2 = &thinice_style->color_cube.dark[state_type];
      break;
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_OUT:
			color1 = &thinice_style->color_cube.dark[state_type];
			color2 = &thinice_style->color_cube.light[state_type];
      break;
    }

  cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      break;

    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
	  ge_cairo_set_color(cr, color1);	
   	  cairo_rectangle(cr, x + 1.5, y + 1.5, width - 3, height - 3);  
	  cairo_stroke(cr);

	  ge_cairo_set_color(cr, color2);	
   	  cairo_rectangle(cr, x + 0.5, y + 0.5, width - 3, height - 3);  
	  cairo_stroke(cr);
      break;

    case GTK_SHADOW_IN:
    case GTK_SHADOW_OUT:
	ge_cairo_simple_border (cr, color2, color1, x, y, width, height, FALSE);
      break;
    }

   cairo_destroy(cr);
}

static void
draw_polygon(GtkStyle * style,
             GdkWindow * window,
             GtkStateType state_type,
             GtkShadowType shadow_type,
             GdkRectangle * area,
             GtkWidget * widget,
             const gchar *detail,
             GdkPoint * points,
             gint npoints,
             gint fill)
{
#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif /* M_PI */
#ifndef M_PI_4
#define M_PI_4  0.78539816339744830962
#endif /* M_PI_4 */

  static const gdouble pi_over_4 = M_PI_4;
  static const gdouble pi_3_over_4 = M_PI_4 * 3;

  ThiniceStyle *thinice_style = THINICE_STYLE (style);

  CairoColor *color1 = NULL;
  CairoColor *color2 = NULL;
  CairoColor *color3 = NULL;
  CairoColor *color4 = NULL;

  gdouble             angle;
  gint                xadjust;
  gint                yadjust;
  gint                i;

  CHECK_ARGS
  g_return_if_fail(points != NULL);

  switch (shadow_type)
    {
    case GTK_SHADOW_IN:
			color1 = &thinice_style->color_cube.light[state_type];
			color2 = &thinice_style->color_cube.dark[state_type];
			color3 = &thinice_style->color_cube.light[state_type];
			color4 = &thinice_style->color_cube.dark[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
			color1 = &thinice_style->color_cube.light[state_type];
			color2 = &thinice_style->color_cube.dark[state_type];
			color3 = &thinice_style->color_cube.dark[state_type];
			color4 = &thinice_style->color_cube.light[state_type];
      break;
    case GTK_SHADOW_OUT:
			color1 = &thinice_style->color_cube.dark[state_type];
			color2 = &thinice_style->color_cube.light[state_type];
			color3 = &thinice_style->color_cube.dark[state_type];
			color4 = &thinice_style->color_cube.light[state_type];
      break;
    case GTK_SHADOW_ETCHED_OUT:
			color1 = &thinice_style->color_cube.dark[state_type];
			color2 = &thinice_style->color_cube.light[state_type];
			color3 = &thinice_style->color_cube.light[state_type];
			color4 = &thinice_style->color_cube.dark[state_type];
      break;
    default:
      return;
    }

  cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

  if (fill)
    ge_cairo_polygon(cr, &thinice_style->color_cube.bg[state_type], points, npoints);

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
          angle = atan2(points[i + 1].y - points[i].y,
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

          ge_cairo_line(cr, color1,
                        points[i].x - xadjust, points[i].y - yadjust,
                        points[i + 1].x - xadjust, points[i + 1].y - yadjust);
          ge_cairo_line(cr, color3,
                        points[i].x, points[i].y,
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

          ge_cairo_line(cr, color4,
                        points[i].x + xadjust, points[i].y + yadjust,
                        points[i + 1].x + xadjust, points[i + 1].y + yadjust);
          ge_cairo_line(cr, color2,
                        points[i].x, points[i].y,
                        points[i + 1].x, points[i + 1].y);
        }
    }

   cairo_destroy(cr);
}

static void
draw_arrow(GtkStyle * style,
           GdkWindow * window,
           GtkStateType state_type,
           GtkShadowType shadow_type,
           GdkRectangle * area,
           GtkWidget * widget,
           const gchar *detail,
           GtkArrowType arrow_type,
           gint fill, gint x, gint y, gint width, gint height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

	CairoColor *color1, *color2, *color3=NULL,*color4=NULL;
	gint half_width, half_height;
        gint ax, ay, aw, ah;

	CHECK_ARGS

	if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
		return;

	SANITIZE_SIZE

	switch (shadow_type)
	{
		case GTK_SHADOW_IN:
			color1 = &thinice_style->color_cube.bg[state_type];
			color2 = &thinice_style->color_cube.dark[state_type];
			color3 = &thinice_style->color_cube.light[state_type];
			color4 = &thinice_style->color_cube.black;
		break;

		case GTK_SHADOW_OUT:
			color1 = &thinice_style->color_cube.dark[state_type];
			color2 = &thinice_style->color_cube.light[state_type];
			color3 = &thinice_style->color_cube.black;
			color4 = &thinice_style->color_cube.bg[state_type];
		break;

		case GTK_SHADOW_ETCHED_IN:
			color2 = &thinice_style->color_cube.light[state_type];
			color1 = &thinice_style->color_cube.dark[state_type];
		break;

		case GTK_SHADOW_ETCHED_OUT:
			color1 = &thinice_style->color_cube.dark[state_type];
			color2 = &thinice_style->color_cube.light[state_type];
		break;

		case GTK_SHADOW_NONE:
			color1 = &thinice_style->color_cube.bg[state_type];
			color2 = &thinice_style->color_cube.bg[state_type];
		break;

		default:
			return;
	}

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

	if (CHECK_DETAIL (detail, "vscrollbar") || CHECK_DETAIL (detail, "hscrollbar"))
	{
		switch (THINICE_RC_STYLE (style->rc_style)->mark_type2)
		{
			case MARKS_NOTHING:
			break;

			case MARKS_INVSLASH:
				thinice_slash_one(cr, color2, color1, x, y, width, height);
			break;

			case MARKS_DOT:
				thinice_dot(cr, color2, color1, x + half_width, y + half_height);
			break;

			case MARKS_INVDOT:
				thinice_dot(cr, color1, color2, x + half_width, y + half_height);
			break;

			case MARKS_ARROW:
				if (state_type == GTK_STATE_INSENSITIVE)
					thinice_arrow (cr, &thinice_style->color_cube.white, arrow_type, TRUE, x+1, y+1, width, height);
				thinice_arrow (cr, &thinice_style->color_cube.fg[state_type], arrow_type, TRUE, x, y, width, height);
          		break;

			case MARKS_SLASH:
			default:
				thinice_slash_one(cr, color1, color2, x, y, width - 1, height - 1);
			break;
		}
	}
	else
	{
		if (state_type == GTK_STATE_INSENSITIVE)
			thinice_arrow (cr, &thinice_style->color_cube.white, arrow_type, TRUE, x+1, y+1, width, height);
		thinice_arrow (cr, &thinice_style->color_cube.fg[state_type], arrow_type, TRUE, x, y, width, height);
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
             const gchar *detail,
             gint x,
             gint y,
             gint width,
             gint height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);
  gint                half_width;
  gint                half_height;

  CHECK_ARGS

  SANITIZE_SIZE

  half_width = width / 2;
  half_height = height / 2;

  cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

  switch (shadow_type)
    {
    case GTK_SHADOW_IN:
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + 2, y + half_height,
                    x + half_width, y + height - 2);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + half_width, y + height - 2,
                    x + width - 2, y + half_height);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + 1, y + half_height,
                    x + half_width, y + height - 1);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + half_width, y + height - 1,
                    x + width - 1, y + half_height);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x, y + half_height,
                    x + half_width, y + height);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + half_width, y + height,
                    x + width, y + half_height);

      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + 2, y + half_height,
                    x + half_width, y + 2);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + half_width, y + 2,
                    x + width - 2, y + half_height);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + 1, y + half_height,
                    x + half_width, y + 1);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + half_width, y + 1,
                    x + width - 1, y + half_height);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x, y + half_height,
                    x + half_width, y);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + half_width, y,
                    x + width, y + half_height);
      break;
    case GTK_SHADOW_OUT:
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + 2, y + half_height,
                    x + half_width, y + height - 2);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + half_width, y + height - 2,
                    x + width - 2, y + half_height);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + 1, y + half_height,
                    x + half_width, y + height - 1);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + half_width, y + height - 1,
                    x + width - 1, y + half_height);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x, y + half_height,
                    x + half_width, y + height);
      ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                    x + half_width, y + height,
                    x + width, y + half_height);

      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + 2, y + half_height,
                    x + half_width, y + 2);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + half_width, y + 2,
                    x + width - 2, y + half_height);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + 1, y + half_height,
                    x + half_width, y + 1);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + half_width, y + 1,
                    x + width - 1, y + half_height);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x, y + half_height,
                    x + half_width, y);
      ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                    x + half_width, y,
                    x + width, y + half_height);
      break;
    default:
      break;
    }

  cairo_destroy(cr);
}

static void
draw_box(GtkStyle * style,
         GdkWindow * window,
         GtkStateType state_type,
         GtkShadowType shadow_type,
         GdkRectangle * area,
         GtkWidget * widget,
         const gchar *detail,
         gint x,
         gint y,
         gint width,
         gint height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

  CairoColor *light, *dark;

  CHECK_ARGS

  SANITIZE_SIZE

  if (CHECK_DETAIL(detail, "optionmenutab"))
	return;

  light = &thinice_style->color_cube.light[state_type];
  dark = &thinice_style->color_cube.dark[state_type];

  cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

 if (CHECK_DETAIL (detail, "trough"))
    {
           ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

           cairo_rectangle(cr, x, y, width, height);

           cairo_fill(cr);

      draw_shadow(style, window, state_type, shadow_type, area, widget,
                       detail, x, y, width, height);
    }
  else if (CHECK_DETAIL (detail, "slider"))
    {
    }
  else if (CHECK_DETAIL (detail, "buttondefault"))
    {
      /* I don't want no background on default buttons..
         Let's add that cute triangle (see below) instead... */
    }
  else if (CHECK_DETAIL (detail, "button"))
    {
      if ((!style->bg_pixmap[state_type]) || GDK_IS_PIXMAP (window))
        {
           ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

           cairo_rectangle(cr, x+1, y+1, width-2, height-2);

           cairo_fill(cr);

        }
      else
        {
          gtk_style_apply_default_background(style, window,
                                             widget && !GTK_WIDGET_NO_WINDOW(widget),
                                             state_type, area,
                                             x, y, width, height);
        }
      /* Paint a triangle here instead of in "buttondefault"
         which is drawn _behind_ the current button */
      if (GTK_WIDGET_HAS_DEFAULT (widget))
        {
          cairo_set_line_width (cr, 1);

          ge_cairo_set_color(cr, &thinice_style->color_cube.bg[GTK_STATE_SELECTED]);
          cairo_move_to(cr, x+2.5, y+2.5);
          cairo_line_to(cr, x+10.5, y+2.5);
          cairo_line_to(cr, x+2.5, y+10.5);
          cairo_line_to(cr, x+2.5, y+2.5);
	  cairo_fill(cr);

          ge_cairo_set_color(cr, dark);
	  cairo_move_to(cr, x + 2.5, y + 11);
	  cairo_line_to(cr, x + 2.5, y + 2.5);
  	  cairo_line_to(cr, x + 11, y + 2.5);
	  cairo_stroke(cr);

          cairo_set_line_width (cr, 0.5);
          ge_cairo_set_color(cr, light);
          cairo_move_to(cr, x+11, y+3);
          cairo_line_to(cr, x+3, y+11);
	  cairo_stroke(cr);
        }

      draw_shadow(style, window, state_type, shadow_type, area, widget,
                       detail, x, y, width, height);
    }
  else if (CHECK_DETAIL (detail, "bar"))
    {
      if ((height > 1) && (width > 1))
        {

           ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

           cairo_rectangle(cr, x+1, y+1, width-2, height-2);

           cairo_fill(cr);

        }
    }
  else if (CHECK_DETAIL (detail, "handlebox_bin"))
    {
      if ((!style->bg_pixmap[state_type]) || GDK_IS_PIXMAP(window))
        {
           ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

           cairo_rectangle(cr, x, y, width, height);

           cairo_fill(cr);

        }
      else
        {
          gtk_style_apply_default_background(style, window,
                                             widget && !GTK_WIDGET_NO_WINDOW(widget),
                                             state_type, area,
                                             x, y, width, height);
        }
    }
  else if (CHECK_DETAIL (detail, "menubar"))
    {
      if ((!style->bg_pixmap[state_type]) || GDK_IS_PIXMAP(window))
        {
           ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

           cairo_rectangle(cr, x, y, width, height);

           cairo_fill(cr);
        }
      else
        {
          gtk_style_apply_default_background(style, window,
                                             widget && !GTK_WIDGET_NO_WINDOW(widget),
                                             state_type, area,
                                             x, y, width, height);
        }
      draw_shadow(style, window, state_type, shadow_type, area, widget,
                       detail, x, y, width, height);
    }
  else
    {

      if ((!style->bg_pixmap[state_type]) || GDK_IS_PIXMAP(window))
        {
           ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

           cairo_rectangle(cr, x, y, width, height);

           cairo_fill(cr);
        }
      else
        {
          gtk_style_apply_default_background(style, window,
                                             widget && !GTK_WIDGET_NO_WINDOW(widget),
                                             state_type, area,
                                             x, y, width, height);
        }
      draw_shadow(style, window, state_type, shadow_type, area, widget,
                       detail, x, y, width, height);
    }

  if (CHECK_DETAIL(detail, "optionmenu") ||  (CHECK_DETAIL(detail, "button") && 
       (ge_is_combo_box(widget, FALSE)) && !(ge_is_combo_box_entry(widget))))
    {
      GtkRequisition indicator_size;
      GtkBorder indicator_spacing;
      gint vline_x;
 
      if (state_type != GTK_STATE_INSENSITIVE)
        state_type = GTK_STATE_NORMAL;
 
      ge_option_menu_get_props (widget, &indicator_size, &indicator_spacing);
 
      if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL))
	vline_x = x + indicator_size.width + indicator_spacing.left + indicator_spacing.right;
      else
	vline_x = x + width - (indicator_size.width + indicator_spacing.left + 
                               indicator_spacing.right) - style->xthickness;

      thinice_draw_separator(cr, FALSE, vline_x, y + style->ythickness + 1, style->xthickness, height - 2*style->ythickness - 2);
 
      if ((widget) && (gtk_widget_get_direction (GTK_WIDGET (widget)) == GTK_TEXT_DIR_RTL))
         x +=  indicator_spacing.right + style->xthickness;
      else
         x += width - indicator_size.width - indicator_spacing.right - style->xthickness;
 
      y += ((height - indicator_size.height) / 2) + 1;
 
      width = indicator_size.width;
      height = indicator_size.height;
 
      draw_arrow (style, window, state_type, shadow_type, area, NULL, "optionmenu", 
	                      GTK_ARROW_DOWN, TRUE,  x,  y,  width,  height);
   }

  cairo_destroy(cr);
}

static void
draw_check(GtkStyle * style,
           GdkWindow * window,
           GtkStateType state_type,
           GtkShadowType shadow_type,
           GdkRectangle * area,
           GtkWidget * widget,
           const gchar *detail,
           gint x,
           gint y,
           gint width,
           gint height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

	CairoColor              *color1 = NULL;

	CHECK_ARGS
	SANITIZE_SIZE

	if (shadow_type == GTK_SHADOW_IN)
	{
		color1 = &thinice_style->color_cube.bg[GTK_STATE_ACTIVE];
	}

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

	if (state_type == GTK_STATE_INSENSITIVE)
	{
	  ge_cairo_set_color(cr, &thinice_style->color_cube.dark[state_type]);	

   	  cairo_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);  

	  cairo_stroke(cr);
	}
	else
	{
		draw_box(style, window, state_type, shadow_type, area, widget,
                			detail, x, y, width, height);
		if (color1)
		{
		        ge_cairo_set_color(cr, color1);

		        cairo_rectangle(cr, x+1, y+1, width-2, height-2);

		        cairo_fill(cr);
		}
	}
	
	cairo_destroy(cr);
}


/* Thanks to Evan Lawrence */
static void
draw_option(GtkStyle * style,
            GdkWindow * window,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GdkRectangle * area,
            GtkWidget * widget,
            const gchar *detail,
            gint x,
            gint y,
            gint width,
            gint height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

	CairoColor              *color1 = NULL, *color2 = NULL, *color3 = NULL;

	CHECK_ARGS
	SANITIZE_SIZE

  if (shadow_type == GTK_SHADOW_IN ||
      shadow_type == GTK_SHADOW_ETCHED_IN)
    {
			color1 = &thinice_style->color_cube.dark[state_type];
			color2 = &thinice_style->color_cube.light[state_type];
			color3 = &thinice_style->color_cube.bg[GTK_STATE_ACTIVE];
    }
  else
    {
			color1 = &thinice_style->color_cube.light[state_type]; 
			color2 = &thinice_style->color_cube.dark[state_type];
			color3 = &thinice_style->color_cube.bg[state_type];
    }

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);
      cairo_set_line_width (cr, 0.5);

  gint centerX = x + floor(width/2);
  gint centerY = y + floor(height/2);
  gint radius = floor(width/2) - 0.5;

  switch (shadow_type)
    {
    case GTK_SHADOW_ETCHED_IN:
      ge_cairo_set_color(cr, color2);
      cairo_arc(cr, centerX + 1, centerY + 1, radius + 1, 0, 2 * M_PI);
      cairo_stroke(cr);

      ge_cairo_set_color(cr, color1); 
      cairo_arc(cr, centerX, centerY, radius + 1, 0, 2 * M_PI);
      cairo_stroke(cr);

      break;
    case GTK_SHADOW_ETCHED_OUT:
      ge_cairo_set_color(cr, color1); 
      cairo_arc(cr, centerX - 1, centerY - 1, radius + 1, 0, 2 * M_PI);
      cairo_stroke(cr);

      ge_cairo_set_color(cr, color2);
      cairo_arc(cr, centerX, centerY, radius + 1,  0, 2 * M_PI);
      cairo_stroke(cr);

      break;
    default:
      cairo_set_line_width (cr, 0.5);

      cairo_new_path (cr);

      cairo_move_to(cr, centerX + (radius + 2), centerY + (radius + 2));
      cairo_line_to(cr, centerX + (radius + 2)*sin(M_PI/4.0), centerY - (radius + 2)*cos(M_PI/4.0));
      cairo_line_to(cr, centerX - (radius + 2)*sin(M_PI/4.0), centerY + (radius + 2)*cos(M_PI/4.0));
      cairo_line_to(cr, centerX + (radius + 2), centerY + (radius + 2));

      cairo_close_path (cr);

      cairo_clip (cr);

      ge_cairo_set_color(cr, color2);
      cairo_arc(cr, centerX, centerY, radius + 1, 0,  2*M_PI);
      cairo_fill(cr);

      cairo_reset_clip(cr);

      cairo_new_path (cr);

      cairo_move_to(cr, centerX - (radius + 2), centerY - (radius + 2));
      cairo_line_to(cr, centerX + (radius + 2)*sin(M_PI/4.0), centerY - (radius + 2)*cos(M_PI/4.0));
      cairo_line_to(cr, centerX - (radius + 2)*sin(M_PI/4.0), centerY + (radius + 2)*cos(M_PI/4.0));
      cairo_line_to(cr, centerX - (radius + 2), centerY - (radius + 2));

      cairo_close_path (cr);

      cairo_clip (cr);

      ge_cairo_set_color(cr, color1); 
      cairo_arc(cr, centerX, centerY, radius + 1, 0,  2*M_PI);
      cairo_fill(cr);

      cairo_reset_clip(cr);

      ge_cairo_set_color(cr, color3);
      cairo_arc(cr, centerX, centerY, radius, 0, 2 * M_PI);
      cairo_fill(cr);

      break;
    }

	cairo_destroy(cr);
}

static void
draw_shadow_gap(GtkStyle * style,
                GdkWindow * window,
                GtkStateType state_type,
                GtkShadowType shadow_type,
                GdkRectangle * area,
                GtkWidget * widget,
                const gchar *detail,
                gint x,
                gint y,
                gint width,
                gint height,
                GtkPositionType gap_side,
                gint gap_x,
                gint gap_width)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

	CairoColor              *color1 = NULL, *color2 = NULL;

	CHECK_ARGS
	SANITIZE_SIZE

	shadow_type = thinice_shadow_type (style, detail, shadow_type);
	
	switch (shadow_type) {
	case GTK_SHADOW_NONE:
		return;
	case GTK_SHADOW_IN:
		color1 = &thinice_style->color_cube.dark[state_type];
		color2 = &thinice_style->color_cube.light[state_type];
		break;
	case GTK_SHADOW_OUT:
		color1 = &thinice_style->color_cube.light[state_type];
		color2 = &thinice_style->color_cube.dark[state_type];
		break;
	case GTK_SHADOW_ETCHED_IN:
	case GTK_SHADOW_ETCHED_OUT:
		color1 = &thinice_style->color_cube.dark[state_type];
		color2 = &thinice_style->color_cube.dark[state_type];
	}

	cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

	switch (gap_side) {
        case GTK_POS_TOP:
		if (gap_x > 0) {
			ge_cairo_line (cr, color1, 
				       x, y, 
				       x + gap_x, y);
		}
		if ((width - (gap_x + gap_width)) > 0) {
			ge_cairo_line (cr, color1, 
				       x + gap_x + gap_width - 1, y,
				       x + width - 1, y);
		}
		ge_cairo_line (cr, color1, 
			       x, y, 
			       x, y + height - 1);
		ge_cairo_line (cr, color2,
			       x + width - 1, y,
			       x + width - 1, y + height - 1);
		ge_cairo_line (cr, color2,
			       x, y + height - 1,
			       x + width - 1, y + height - 1);
		break;
        case GTK_POS_BOTTOM:
		ge_cairo_line (cr, color1,
			       x, y,
			       x + width - 1, y);
		ge_cairo_line (cr, color1, 
			       x, y, 
			       x, y + height - 1);
		ge_cairo_line (cr, color2,
			       x + width - 1, y,
			       x + width - 1, y + height - 1);

		if (gap_x > 0) {
			ge_cairo_line (cr, color2, 
				       x, y + height - 1, 
				       x + gap_x, y + height - 1);
		}
		if ((width - (gap_x + gap_width)) > 0) {
			ge_cairo_line (cr, color2, 
				       x + gap_x + gap_width - 1, y + height - 1,
				       x + width - 1, y + height - 1);
		}
		
		break;
        case GTK_POS_LEFT:
		ge_cairo_line (cr, color1,
			       x, y,
			       x + width - 1, y);
		if (gap_x > 0) {
			ge_cairo_line (cr, color1, 
				       x, y,
				       x, y + gap_x);
		}
		if ((height - (gap_x + gap_width)) > 0) {
			ge_cairo_line (cr, color1, 
				       x, y + gap_x + gap_width - 1,
				       x, y + height - 1);
		}

		ge_cairo_line (cr, color2,
			       x + width - 1, y,
			       x + width - 1, y + height - 1);
		ge_cairo_line (cr, color2,
			       x, y + height - 1,
			       x + width - 1, y + height - 1);
		break;
        case GTK_POS_RIGHT:
		ge_cairo_line (cr, color1,
			       x, y,
			       x + width - 1, y);
		ge_cairo_line (cr, color1, 
			       x, y, 
			       x, y + height - 1);


		if (gap_x > 0) {
			ge_cairo_line (cr, color2, 
				       x + width - 1, y,
				       x + width - 1, y + gap_x);
		}
		if ((height - (gap_x + gap_width)) > 0) {
			ge_cairo_line (cr, color2, 
				       x + width - 1, y + gap_x + gap_width - 1,
				       x + width - 1, y + height - 1);
		}

		ge_cairo_line (cr, color2,
			       x, y + height - 1,
			       x + width - 1, y + height - 1);

	}

	cairo_destroy(cr);
}


static void
draw_box_gap(GtkStyle * style,
             GdkWindow * window,
             GtkStateType state_type,
             GtkShadowType shadow_type,
             GdkRectangle * area,
             GtkWidget * widget,
             const gchar *detail,
             gint x,
             gint y,
             gint width,
             gint height,
             GtkPositionType gap_side,
             gint gap_x,
             gint gap_width)
{
    SANITIZE_SIZE

    gtk_style_apply_default_background(style, window,
            widget && !GTK_WIDGET_NO_WINDOW(widget),
            state_type, area,
            x, y, width, height);
    draw_shadow_gap (style, window, state_type, shadow_type,
            area, widget, detail, x, y, width, height,
            gap_side, gap_x, gap_width);
}


static void
draw_extension(DRAW_ARGS, GtkPositionType gap_side)
{
	switch (gap_side)
	{
		case GTK_POS_TOP: gap_side = GTK_POS_BOTTOM; break;
		case GTK_POS_BOTTOM: gap_side = GTK_POS_TOP; break;
		case GTK_POS_LEFT: gap_side = GTK_POS_RIGHT; break;
		case GTK_POS_RIGHT: gap_side = GTK_POS_LEFT; break;
	}

	thinice_tab(DRAW_VARS, gap_side);
}

static void
draw_slider(GtkStyle * style,
            GdkWindow * window,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GdkRectangle * area,
            GtkWidget * widget,
            const gchar *detail,
            gint x,
            gint y,
            gint width,
            gint height,
            GtkOrientation orientation)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

  GdkPoint pointsh[7];
  gint i, rect = FALSE, midlines = MARKS_SLASH;
  gint modx, mody;
  

  CHECK_ARGS

  SANITIZE_SIZE

#ifdef DEBUG
  printf("draw_slider(%s, %d, %d, %d)\n", detail, x, y, orientation);
#endif

#ifdef DEBUG
      printf("Slider... x,y=%d,%d width = %d, height = %d (%d)\n",x,y,width,height, state_type);
#endif
      midlines = THINICE_RC_STYLE (style->rc_style)->mark_type1;
#ifdef DEBUG
      printf("Midlines = %d\n", midlines);
#endif

      if (THINICE_RC_STYLE (style->rc_style)->scrollbar_type == SCROLL_RECT)
        {
          rect = TRUE;
        }

      /* too small, use rect & no midlines */
      if ((width <= SMALLEST_HANDLE) && (height <= SMALLEST_HANDLE))
        {
          midlines = MARKS_NOTHING;
          rect = TRUE;
        }

      if (rect)
        {
          pointsh[0].x = x;             pointsh[0].y = y;
          pointsh[1].x = x + width - 1; pointsh[1].y = y;
          pointsh[2].x = x + width - 1; pointsh[2].y = y + height - 1;
          pointsh[3].x = x;             pointsh[3].y = y + height - 1;
          pointsh[4].x = x;             pointsh[4].y = y;
        } 
      else
        {
          int chopoff;

          if (orientation == GTK_ORIENTATION_HORIZONTAL) {
            chopoff = max(0, min(6, width-SMALLEST_HANDLE));
          } else {
            chopoff = max(0, min(6, height-SMALLEST_HANDLE));
          }

          pointsh[0].x = x;                  pointsh[0].y = y+height-1;
          pointsh[1].x = x;                  pointsh[1].y = y+chopoff;
          pointsh[2].x = x+chopoff;          pointsh[2].y = y;
          pointsh[3].x = x+width-1;          pointsh[3].y = y;
          pointsh[4].x = x+width-1;          pointsh[4].y = y+height-1-chopoff;
          pointsh[5].x = x+width-1-chopoff;  pointsh[5].y = y+height-1;
          pointsh[6].x = x;                  pointsh[6].y = y+height-1;
        }

      cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

      if (rect)
        {
           ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

           cairo_rectangle(cr, x, y, width, height);

           cairo_fill(cr);

           draw_shadow(style, window, state_type, shadow_type, area,
                            widget, detail, x, y, width, height);
        }
      else
        {
          /* Fill the polygon */
          ge_cairo_polygon(cr, &thinice_style->color_cube.bg[state_type],
                           pointsh, 6);

          /* Draw the light border */
          for (i=0;i<3;i++)
            {
              ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                            pointsh[i].x,pointsh[i].y,
                            pointsh[i+1].x,pointsh[i+1].y);
            }
          /* Draw the dark border */
          for (i=3;i<6;i++)
            {
              ge_cairo_line(cr, &thinice_style->color_cube.dark[state_type],
                            pointsh[i].x,pointsh[i].y,
                            pointsh[i+1].x,pointsh[i+1].y);
            }

        }

      if (orientation == GTK_ORIENTATION_HORIZONTAL)
        { modx = 4; mody = 0; }
      else
        { modx = 0; mody = 4; }

      switch (midlines)
        {
        case MARKS_NOTHING:
          break;
        case MARKS_INVSLASH: /* Inverse //:es */
          thinice_slash_two(cr,
                            &thinice_style->color_cube.dark[state_type],
                            &thinice_style->color_cube.light[state_type],
                            x, y, width, height);
          break;
        case MARKS_DOT:
          thinice_dot(cr,
                      &thinice_style->color_cube.light[state_type],
                      &thinice_style->color_cube.dark[state_type],
                      x + width / 2 - modx,
                      y + height / 2 - mody);
          thinice_dot(cr,
                      &thinice_style->color_cube.light[state_type],
                      &thinice_style->color_cube.dark[state_type],
                      x + width / 2,
                      y + height / 2);
          thinice_dot(cr,
                      &thinice_style->color_cube.light[state_type],
                      &thinice_style->color_cube.dark[state_type],
                      x + width / 2 + modx,
                      y + height / 2 + mody);
          break;
        case MARKS_INVDOT: /* Inverted */
          thinice_dot(cr,
                      &thinice_style->color_cube.dark[state_type],
                      &thinice_style->color_cube.light[state_type],
                      x + width / 2 - modx,
                      y + height / 2 - mody);
          thinice_dot(cr,
                      &thinice_style->color_cube.dark[state_type],
                      &thinice_style->color_cube.light[state_type],
                      x + width / 2,
                      y + height / 2);
          thinice_dot(cr,
                      &thinice_style->color_cube.dark[state_type],
                      &thinice_style->color_cube.light[state_type],
                      x + width / 2 + modx,
                      y + height / 2 + mody);
          break;
	case MARKS_SLASH:
        default:
          thinice_slash_two(cr,
                            &thinice_style->color_cube.light[state_type],
                            &thinice_style->color_cube.dark[state_type],
                            x, y, width, height);
          break;
        }

	cairo_destroy(cr);

}

static void
draw_handle(GtkStyle * style,
            GdkWindow * window,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GdkRectangle * area,
            GtkWidget * widget,
            const gchar *detail,
            gint x,
            gint y,
            gint width,
            gint height,
            GtkOrientation orientation)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

  CairoColor              *light=NULL, *dark=NULL;
  GdkRectangle        dest;
//  ThemeStyleData     *data = NULL;//style->engine_data;
  gint                modx, mody;

  CHECK_ARGS

#ifdef DEBUG
  printf("draw_handle(state=%d, shadow=%d, detail=%s, [%d,%d]@[%d,%d]\n",
         state_type, shadow_type, detail?detail:"nul",width,height,x,y);
#endif
  SANITIZE_SIZE

  cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

  if (CHECK_DETAIL (detail, "paned")) 
    {
      int i, w;
      int start_i, end_i;

      dest.x = x;
      dest.y = y;
      dest.width = width;
      dest.height = height;

      light = &thinice_style->color_cube.light[state_type];
      dark = &thinice_style->color_cube.dark[state_type];

      if (orientation == GTK_ORIENTATION_HORIZONTAL)
        w = width;
      else
        w = height;
      switch (THINICE_RC_STYLE (style->rc_style)->paned_dots) {
      default:
      case PANED_DOTSFULL:
        start_i = 5;
        end_i = w - 5;
        break;
      case PANED_DOTSSOME:
        start_i = w/2 - 16;
        end_i = w/2 + 16;
        break;
      case PANED_DOTSNONE:
        start_i = w;
        end_i = 0;
        break;
      }

      if (orientation == GTK_ORIENTATION_HORIZONTAL)
        {
          for (i=x + start_i; i <= x + end_i; i+=8)
            {
              thinice_dot(cr, light, dark, i, y + height / 2);
            }
        }
      else
        {
          for (i=y + start_i; i <= y + end_i; i+=8)
            {
              thinice_dot(cr, light, dark, x + width / 2, i);
            }
        }

       cairo_destroy(cr);

      return;
    }

  draw_box(style, window, state_type, shadow_type, area, widget,
                detail, x, y, width, height);

  ge_cairo_line(cr, &thinice_style->color_cube.light[state_type],
                x + width, y, x + width, y + height - 2);

  /* Draw something in the box if so wanted */
  if (THINICE_RC_STYLE (style->rc_style)->mark_type1 != MARKS_NOTHING)
    {
      light = &thinice_style->color_cube.light[state_type];
      dark = &thinice_style->color_cube.dark[state_type];

      orientation = GTK_ORIENTATION_HORIZONTAL;
      if (height > width)
        orientation = GTK_ORIENTATION_VERTICAL;

      dest.x = x + style->xthickness;
      dest.y = y + style->ythickness;
      dest.width = width - (style->xthickness * 2);
      dest.height = height - (style->ythickness * 2);

      if (orientation == GTK_ORIENTATION_HORIZONTAL)
        { modx = 4; mody = 0; }
      else
        { modx = 0; mody = 4; }

      switch (THINICE_RC_STYLE (style->rc_style)->mark_type1)
        {
        case MARKS_INVSLASH: /* Inverse //:es */
          thinice_slash_two(cr, dark, light,
                            x, y, width, height);
          break;
        case MARKS_DOT: /* Dots */
          thinice_dot(cr, light, dark,
                      x + width / 2 - modx,
                      y + height / 2 - mody);
          thinice_dot(cr, light, dark,
                      x + width / 2,
                      y + height / 2);
          thinice_dot(cr, light, dark,
                      x + width / 2 + modx,
                      y + height / 2 + mody);
          break;
        case MARKS_INVDOT: /* Inverted dots */
          thinice_dot(cr, dark, light,
                      x + width / 2 - modx,
                      y + height / 2 - mody);
          thinice_dot(cr, dark, light,
                      x + width / 2,
                      y + height / 2);
          thinice_dot(cr, dark, light,
                      x + width / 2 + modx,
                      y + height / 2 + mody);
          break;
	case MARKS_SLASH:
        default:
          thinice_slash_two(cr, light, dark,
                            x, y, width, height);
          break;
        }
    }

    cairo_destroy(cr);
}

static void
thinice_tab(DRAW_ARGS, gint orientation)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

  CHECK_ARGS

  SANITIZE_SIZE

  CairoColor *light, *dark;

  light = &thinice_style->color_cube.light[state_type];
  dark = &thinice_style->color_cube.dark[state_type];

  cairo_t *cr = ge_gdk_drawable_to_cairo (window, area);

  if ((!style->bg_pixmap[state_type]) || GDK_IS_PIXMAP(window))
    {
      ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

      cairo_rectangle(cr, x, y, width - 1, height - 1);

      cairo_fill(cr);
    }
  else
    {
      gtk_style_apply_default_background(style, window,
                                         widget && !GTK_WIDGET_NO_WINDOW(widget),
                                         state_type, area, x, y, width, height);
    }

  switch(orientation)
    {
    case GTK_POS_TOP:
      ge_cairo_line(cr, light,
                    x, y + height, x, y);
      ge_cairo_line(cr, light,
                    x, y, x + width - 2, y);
      ge_cairo_line(cr, dark,
                    x + width - 2, y, x + width - 2, y + height - 1);
      break;
    case GTK_POS_BOTTOM:
      ge_cairo_line(cr, light,
                    x, y - 1, x, y + height - 1);
      ge_cairo_line(cr, dark,
                    x, y + height - 1, x + width - 2, y + height - 1);
      ge_cairo_line(cr, dark,
                    x + width - 2, y + height - 1, x + width - 2, y);
      break;
    case GTK_POS_LEFT:
      ge_cairo_line(cr, light,
                    x, y + height - 1, x, y);
      ge_cairo_line(cr, light,
                    x, y, x + width, y);
      ge_cairo_line(cr, dark,
                    x, y + height - 1, x + width - 1, y + height - 1);
      break;
    case GTK_POS_RIGHT:
      ge_cairo_line(cr, light,
                    x - 1, y, x + width - 1, y);
      ge_cairo_line(cr, dark,
                    x + width - 1, y, x + width - 1, y + height - 1);
      ge_cairo_line(cr, dark,
                    x, y + height - 1, x + width - 1, y + height - 1);
      break;
    }

    cairo_destroy(cr);
}

GType thinice_type_style = 0;

void
thinice_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (ThiniceStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) thinice_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (ThiniceStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) thinice_style_init,
  };
  
  thinice_type_style = g_type_module_register_type (module,
						    GTK_TYPE_STYLE,
						    "ThiniceStyle",
						    &object_info, 0);
}

static void
thinice_style_init (ThiniceStyle *style)
{
}

static void
thinice_style_realize (GtkStyle * style)
{
  ThiniceStyle *thinice_style = THINICE_STYLE (style);
  int i;
 
  parent_class->realize (style);
 
  ge_gtk_style_to_cairo_color_cube (style, &thinice_style->color_cube);
}

static void
thinice_style_class_init (ThiniceStyleClass *klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  style_class->realize = thinice_style_realize;

  style_class->draw_hline = draw_hline;
  style_class->draw_vline = draw_vline;
  style_class->draw_shadow = draw_shadow;
  style_class->draw_polygon = draw_polygon;
  style_class->draw_arrow = draw_arrow;
  style_class->draw_diamond = draw_diamond;
  style_class->draw_box = draw_box;
  style_class->draw_tab = draw_box;

  style_class->draw_check = draw_check;
  style_class->draw_option = draw_option;
  style_class->draw_shadow_gap = draw_shadow_gap;
  style_class->draw_box_gap = draw_box_gap;
  style_class->draw_extension = draw_extension;
  style_class->draw_slider = draw_slider;
  style_class->draw_handle = draw_handle;
}


/*
FIXME: file/font selector background OK
FIXME: radioknappar varierar storlek r�tt bra.. OK
FIXME: scrollbars OK
FIXME: inconsistent-radioknappar OK
FIXME: paned-pluttarna OK
FIXME: spinbuttons, vid max OK

FIXME: Bl� triangeln vid RTL, WONTFIX?
*/

