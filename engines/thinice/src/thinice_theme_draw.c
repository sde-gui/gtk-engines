/* ThinIce theme for gtk, based on raster's Motif theme and the Metal theme.
Authors: Tim Gerla <timg@rrv.net>
Tomas Ögren <stric@ing.umu.se>
 */ 
/* #define DEBUG 1 */

#include <math.h>
#include <string.h>
#include <gtk/gtk.h>

#include "thinice_style.h"
#include "thinice_rc_style.h"
#include "thinice_misc.h"

#define DRAW_ARGS    GtkStyle       *style, \
                     cairo_t        *cr, \
                     GtkStateType    state_type, \
                     GtkShadowType   shadow_type, \
                     GtkWidget      *widget, \
                     const gchar    *detail, \
                     gint            x, \
                     gint            y, \
                     gint            width, \
                     gint            height

#define DRAW_VARS    style, cr, state_type, shadow_type, widget, detail, x, y, width, height

G_DEFINE_DYNAMIC_TYPE (ThiniceStyle, thinice_style, GTK_TYPE_STYLE)

void
thinice_style_register_types (GTypeModule *module)
{
	thinice_style_register_type (module);
}

static void
thinice_style_draw_hline(GtkStyle * style,
           cairo_t * cr,
           GtkStateType state_type,
           GtkWidget * widget,
           const gchar *detail,
           gint x1,
           gint x2,
           gint y)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);
	CairoColor *dark;
        CairoColor *light;
	
	CHECK_ARGS

	dark = &thinice_style->color_cube.dark[state_type];
	light = &thinice_style->color_cube.light[state_type];
	thinice_draw_separator(cr, dark, light, TRUE, x1, y, x2-x1, 2);
}

static void
thinice_style_draw_vline(GtkStyle * style,
           cairo_t * cr,
           GtkStateType state_type,
           GtkWidget * widget,
           const gchar *detail,
           gint y1,
           gint y2,
           gint x)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);
	CairoColor *dark;
	CairoColor *light;

	CHECK_ARGS

	if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
		return;

	dark = &thinice_style->color_cube.dark[state_type];
	light = &thinice_style->color_cube.light[state_type];
	thinice_draw_separator(cr, dark, light, FALSE, x, y1, 2, y2-y1);
}

static void
thinice_style_draw_shadow(GtkStyle     *style,
	    cairo_t      *cr,
	    GtkStateType  state_type,
	    GtkShadowType shadow_type,
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

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      break;

    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
	  ge_cairo_set_color(cr, color1);
   	  ge_cairo_stroke_rectangle(cr, x + 1.5, y + 1.5, width - 3, height - 3);  

	  ge_cairo_set_color(cr, color2);	
   	  ge_cairo_stroke_rectangle(cr, x + 0.5, y + 0.5, width - 3, height - 3);  
      break;

    case GTK_SHADOW_IN:
    case GTK_SHADOW_OUT:
	ge_cairo_simple_border (cr, color2, color1, x, y, width, height, FALSE);
      break;
    }
}

static void
thinice_style_draw_arrow(GtkStyle * style,
           cairo_t * cr,
           GtkStateType state_type,
           GtkShadowType shadow_type,
           GtkWidget * widget,
           const gchar *detail,
           GtkArrowType arrow_type,
           gint fill, gint x, gint y, gint width, gint height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

	CairoColor *color1, *color2, *color3=NULL,*color4=NULL;
	gint half_width, half_height;

	CHECK_ARGS

	if (ge_is_combo_box(widget, FALSE) && (!ge_is_combo_box_entry(widget)))
		return;

	half_width = width / 2;
	half_height = height / 2;

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
}

static void
thinice_style_draw_diamond(GtkStyle * style,
             cairo_t * cr,
             GtkStateType state_type,
             GtkShadowType shadow_type,
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

  half_width = width / 2;
  half_height = height / 2;

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
}

static void
thinice_style_draw_box(GtkStyle * style,
         cairo_t *cr,
         GtkStateType state_type,
         GtkShadowType shadow_type,
         GtkWidget * widget,
         const gchar *detail,
         gint x,
         gint y,
         gint width,
         gint height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);
	gboolean draw_border = TRUE;

	/***********************************************/
	/* GTK Sanity Checks                           */
	/***********************************************/
	CHECK_ARGS


	/***********************************************/
	/* GTK Special Cases - Ignored Widgets         */
	/***********************************************/
	if ((CHECK_DETAIL(detail, "optionmenutab")) ||
		(CHECK_DETAIL (detail, "slider")) ||
		(CHECK_DETAIL (detail, "buttondefault")) ||
		(CHECK_DETAIL (detail, "bar") && 
			((height < 1) && (width < 1))))
	{
		return;
	}


	/***********************************************/
	/* GTK Special Cases - adjust Size/Offset      */
	/***********************************************/
	if ((CHECK_DETAIL (detail, "bar")))
	{
		x += 1;
		y += 1;
		width -= 2;
		height -= 2;

		draw_border=FALSE;
    }
	else if (CHECK_DETAIL (detail, "handlebox_bin"))
	{
		draw_border=FALSE;
	}
	else if (CHECK_DETAIL (detail, "entry-progress"))
	{
        	draw_border=FALSE;
	}


	/***********************************************/
	/* Fill Box                                    */
	/***********************************************/
        gtk_style_apply_default_background(style, cr,
                        gtk_widget_get_window (widget),
                        state_type,
                        x, y, width, height);
	
	/***********************************************/
	/* Draw Box Border                             */
	/***********************************************/
	if (draw_border)
	{
		thinice_style_draw_shadow(style, cr, state_type, shadow_type, widget,
						detail, x, y, width, height);
	}

	/***********************************************/
	/* Draw Widget Specific Sub-Parts              */
	/***********************************************/
	if (CHECK_DETAIL (detail, "button"))
	{
		/* Paint a triangle here instead of in "buttondefault"
			which is drawn _behind_ the current button */
		if (widget && GE_WIDGET_HAS_DEFAULT (widget))
		{
			ge_cairo_set_color(cr, &thinice_style->color_cube.bg[GTK_STATE_SELECTED]);
			cairo_move_to(cr, x+2.5, y+2.5);
			cairo_line_to(cr, x+10.5, y+2.5);
			cairo_line_to(cr, x+2.5, y+10.5);
			cairo_line_to(cr, x+2.5, y+2.5);
			cairo_fill(cr);

			ge_cairo_set_color(cr, &thinice_style->color_cube.dark[state_type]);
			cairo_move_to(cr, x + 2.5, y + 11);
			cairo_line_to(cr, x + 2.5, y + 2.5);
			cairo_line_to(cr, x + 11, y + 2.5);
			cairo_stroke(cr);

			cairo_set_line_width (cr, 0.5);
			ge_cairo_set_color(cr, &thinice_style->color_cube.light[state_type]);
			cairo_move_to(cr, x+11, y+3);
			cairo_line_to(cr, x+3, y+11);
			cairo_stroke(cr);
		}
	}

	/* Draw Option Menus and Combo Box "Tab" The Same Way Here */
	if (CHECK_DETAIL(detail, "optionmenu") ||  (CHECK_DETAIL(detail, "button") && 
		(ge_is_combo_box(widget, FALSE)) && !(ge_is_combo_box_entry(widget))))
	{
		GtkRequisition indicator_size;
		GtkBorder indicator_spacing;
		CairoColor *dark;
		CairoColor *light;
		gint vline_x;
 
		if (state_type != GTK_STATE_INSENSITIVE)
			state_type = GTK_STATE_NORMAL;
 
		ge_option_menu_get_props (widget, &indicator_size, &indicator_spacing);
 
		if ((!widget) || (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL))
			vline_x = x + indicator_size.width + indicator_spacing.left + indicator_spacing.right;
		else
			vline_x = x + width - (indicator_size.width + indicator_spacing.left + 
						indicator_spacing.right) - style->xthickness;

		dark = &thinice_style->color_cube.dark[state_type];
		light = &thinice_style->color_cube.light[state_type];
		thinice_draw_separator(cr, dark, light, FALSE,
                                       vline_x, y + style->ythickness + 1,
                                       style->xthickness, height - 2*style->ythickness - 2);

		if ((widget) && (gtk_widget_get_direction (GTK_WIDGET (widget)) == GTK_TEXT_DIR_RTL))
			x +=  indicator_spacing.right + style->xthickness;
		else
			x += width - indicator_size.width - indicator_spacing.right - style->xthickness;
 
		y += ((height - indicator_size.height) / 2) + 1;
 
		width = indicator_size.width;
		height = indicator_size.height;
 
		thinice_style_draw_arrow (style, cr, state_type, shadow_type, NULL, "optionmenu", 
						GTK_ARROW_DOWN, TRUE,  x,  y,  width,  height);
	}
}

static void
thinice_style_draw_check(GtkStyle * style,
           cairo_t * cr,
           GtkStateType state_type,
           GtkShadowType shadow_type,
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

	if (shadow_type == GTK_SHADOW_IN)
	{
		color1 = &thinice_style->color_cube.bg[GTK_STATE_ACTIVE];
	}

	if (state_type == GTK_STATE_INSENSITIVE)
	{
	  ge_cairo_set_color(cr, &thinice_style->color_cube.dark[state_type]);	

   	  ge_cairo_stroke_rectangle(cr, x + 0.5, y + 0.5, width - 1, height - 1);  
	}
	else
	{
		thinice_style_draw_box(style, cr, state_type, shadow_type, widget,
                			detail, x, y, width, height);
		if (color1)
		{
		        ge_cairo_set_color(cr, color1);

		        cairo_rectangle(cr, x+1, y+1, width-2, height-2);

		        cairo_fill(cr);
		}
	}
}


/* Thanks to Evan Lawrence */
static void
thinice_style_draw_option(GtkStyle * style,
            cairo_t * cr,
            GtkStateType state_type,
            GtkShadowType shadow_type,
            GtkWidget * widget,
            const gchar *detail,
            gint x,
            gint y,
            gint width,
            gint height)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

	CairoColor              *color1 = NULL, *color2 = NULL, *color3 = NULL;

  gint centerX;
  gint centerY;
  gint radius;

	CHECK_ARGS

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

      cairo_set_line_width (cr, 0.5);

  centerX = x + floor(width/2);
  centerY = y + floor(height/2);
  radius = floor(MIN(width, height)/2) - 0.5;

  switch (shadow_type)
    {
    case GTK_SHADOW_ETCHED_IN:
      ge_cairo_set_color(cr, color2);
      cairo_arc(cr, centerX + 1, centerY + 1, radius + 1, 0, 2 * G_PI);
      cairo_stroke(cr);

      ge_cairo_set_color(cr, color1); 
      cairo_arc(cr, centerX, centerY, radius + 1, 0, 2 * G_PI);
      cairo_stroke(cr);

      break;
    case GTK_SHADOW_ETCHED_OUT:
      ge_cairo_set_color(cr, color1); 
      cairo_arc(cr, centerX - 1, centerY - 1, radius + 1, 0, 2 * G_PI);
      cairo_stroke(cr);

      ge_cairo_set_color(cr, color2);
      cairo_arc(cr, centerX, centerY, radius + 1,  0, 2 * G_PI);
      cairo_stroke(cr);

      break;
    default:
      cairo_set_line_width (cr, 0.5);

      cairo_new_path (cr);

      cairo_move_to(cr, centerX + (radius + 2), centerY + (radius + 2));
      cairo_line_to(cr, centerX + (radius + 2)*sin(G_PI/4.0), centerY - (radius + 2)*cos(G_PI/4.0));
      cairo_line_to(cr, centerX - (radius + 2)*sin(G_PI/4.0), centerY + (radius + 2)*cos(G_PI/4.0));
      cairo_line_to(cr, centerX + (radius + 2), centerY + (radius + 2));

      cairo_close_path (cr);

      cairo_save(cr);
      cairo_clip (cr);

      ge_cairo_set_color(cr, color2);
      cairo_arc(cr, centerX, centerY, radius + 1, 0,  2*G_PI);
      cairo_fill(cr);

      cairo_restore(cr);

      cairo_new_path (cr);

      cairo_move_to(cr, centerX - (radius + 2), centerY - (radius + 2));
      cairo_line_to(cr, centerX + (radius + 2)*sin(G_PI/4.0), centerY - (radius + 2)*cos(G_PI/4.0));
      cairo_line_to(cr, centerX - (radius + 2)*sin(G_PI/4.0), centerY + (radius + 2)*cos(G_PI/4.0));
      cairo_line_to(cr, centerX - (radius + 2), centerY - (radius + 2));

      cairo_close_path (cr);

      cairo_save(cr);

      cairo_clip (cr);

      ge_cairo_set_color(cr, color1); 
      cairo_arc(cr, centerX, centerY, radius + 1, 0,  2*G_PI);
      cairo_fill(cr);

      cairo_restore(cr);

      ge_cairo_set_color(cr, color3);
      cairo_arc(cr, centerX, centerY, radius, 0, 2 * G_PI);
      cairo_fill(cr);

      break;
    }
}

static void
thinice_style_draw_shadow_gap(GtkStyle * style,
                cairo_t * cr,
                GtkStateType state_type,
                GtkShadowType shadow_type,
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
	gint start;

	CHECK_ARGS

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

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_rectangle (cr, x, y, width, height);
	
	switch (gap_side) {
        case GTK_POS_TOP:
        	start = MAX (1, gap_x + 1);
        	cairo_rectangle (cr, x + start, y, MIN(width-1, gap_x+gap_width - 1) - start, 1);
		break;
        case GTK_POS_BOTTOM:
        	start = MAX (1, gap_x + 1);
        	cairo_rectangle (cr, x + start, y + height - 1, MIN(width-1, gap_x+gap_width - 1) - start, 1);
		break;
        case GTK_POS_LEFT:
        	start = MAX (1, gap_x + 1);
        	cairo_rectangle (cr, x, y + start, 1, MIN(width-1, gap_x+gap_width - 1) - start);
		break;
        case GTK_POS_RIGHT:
        	start = MAX (1, gap_x + 1);
        	cairo_rectangle (cr, x + width - 1, y + start, 1, MIN(width-1, gap_x+gap_width - 1) - start);
        	break;
	}
	cairo_clip (cr);
	cairo_new_path (cr);
	ge_cairo_simple_border (cr, color1, color2, x, y, width, height, FALSE);
}


static void
thinice_style_draw_box_gap(GtkStyle * style,
             cairo_t * cr,
             GtkStateType state_type,
             GtkShadowType shadow_type,
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
    CHECK_ARGS

    gtk_style_apply_default_background(style, cr,
            gtk_widget_get_window (widget),
            state_type,
            x, y, width, height);
    thinice_style_draw_shadow_gap (style, cr, state_type, shadow_type,
            widget, detail, x, y, width, height,
            gap_side, gap_x, gap_width);
}


static void
thinice_style_draw_extension(DRAW_ARGS, GtkPositionType gap_side)
{
	ThiniceStyle *thinice_style = THINICE_STYLE (style);

	CairoColor *background, *light, *dark;

	CHECK_ARGS

	background = &thinice_style->color_cube.bg[state_type];
	light = &thinice_style->color_cube.light[state_type];
	dark = &thinice_style->color_cube.dark[state_type];

        gtk_style_apply_default_background(style, cr,
                        gtk_widget_get_window (widget),
                        state_type, x, y, width, height);

	cairo_rectangle (cr, x, y, width, height);
	cairo_clip (cr);

	switch(gap_side)
	{
		case GTK_POS_TOP:
			y--;
			height++;
		break;

		case GTK_POS_LEFT:
			x--;
			width++;
		break;

		case GTK_POS_BOTTOM:
			height++;
		break;

		case GTK_POS_RIGHT:
			width++;
		break;
	}

	ge_cairo_simple_border (cr, light, dark, x, y, width, height, FALSE);
}

static void
thinice_style_draw_slider(GtkStyle * style,
            cairo_t * cr,
            GtkStateType state_type,
            GtkShadowType shadow_type,
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
            chopoff = CLAMP(width-SMALLEST_HANDLE, 0, 6);
          } else {
            chopoff = CLAMP(height-SMALLEST_HANDLE, 0, 6);
          }

          pointsh[0].x = x;                  pointsh[0].y = y+height-1;
          pointsh[1].x = x;                  pointsh[1].y = y+chopoff;
          pointsh[2].x = x+chopoff;          pointsh[2].y = y;
          pointsh[3].x = x+width-1;          pointsh[3].y = y;
          pointsh[4].x = x+width-1;          pointsh[4].y = y+height-1-chopoff;
          pointsh[5].x = x+width-1-chopoff;  pointsh[5].y = y+height-1;
          pointsh[6].x = x;                  pointsh[6].y = y+height-1;
        }

      if (rect)
        {
           ge_cairo_set_color(cr, &thinice_style->color_cube.bg[state_type]);

           cairo_rectangle(cr, x, y, width, height);

           cairo_fill(cr);

           thinice_style_draw_shadow(style, cr, state_type, shadow_type,
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
}

static void
thinice_style_draw_handle(GtkStyle * style,
            cairo_t * cr,
            GtkStateType state_type,
            GtkShadowType shadow_type,
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
  gint                modx, mody;

  CHECK_ARGS

#ifdef DEBUG
  printf("draw_handle(state=%d, shadow=%d, detail=%s, [%d,%d]@[%d,%d]\n",
         state_type, shadow_type, detail?detail:"nul",width,height,x,y);
#endif

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

      return;
    }

  thinice_style_draw_box(style, cr, state_type, shadow_type, widget,
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
}

static void
thinice_style_init (ThiniceStyle *style)
{
}

static void
thinice_style_realize (GtkStyle * style)
{
  ThiniceStyle *thinice_style = THINICE_STYLE (style);
 
  GTK_STYLE_CLASS (thinice_style_parent_class)->realize (style);
 
  ge_gtk_style_to_cairo_color_cube (style, &thinice_style->color_cube);
}

static void
thinice_style_class_init (ThiniceStyleClass *klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

  style_class->realize = thinice_style_realize;

  style_class->draw_hline = thinice_style_draw_hline;
  style_class->draw_vline = thinice_style_draw_vline;
  style_class->draw_shadow = thinice_style_draw_shadow;
  style_class->draw_arrow = thinice_style_draw_arrow;
  style_class->draw_diamond = thinice_style_draw_diamond;
  style_class->draw_box = thinice_style_draw_box;
  style_class->draw_tab = thinice_style_draw_box;

  style_class->draw_check = thinice_style_draw_check;
  style_class->draw_option = thinice_style_draw_option;
  style_class->draw_shadow_gap = thinice_style_draw_shadow_gap;
  style_class->draw_box_gap = thinice_style_draw_box_gap;
  style_class->draw_extension = thinice_style_draw_extension;
  style_class->draw_slider = thinice_style_draw_slider;
  style_class->draw_handle = thinice_style_draw_handle;
}

static void
thinice_style_class_finalize (ThiniceStyleClass *klass)
{
}

/*
FIXME: file/font selector background OK
FIXME: radioknappar varierar storlek rätt bra.. OK
FIXME: scrollbars OK
FIXME: inconsistent-radioknappar OK
FIXME: paned-pluttarna OK
FIXME: spinbuttons, vid max OK

FIXME: Blå triangeln vid RTL, WONTFIX?
*/

