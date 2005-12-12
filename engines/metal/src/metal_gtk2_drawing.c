#include "metal_gtk2_engine.h"
#include "metal_gtk2_drawing.h"
#include "metal_gtk2_misc.h"

/**************************************************************************
* GTK Metal Theme
*
* Version 0.9, Oct 2, 1998
*
* Copyright 1998: Randy Gordon, Integrand Systems
*                 http://www.integrand.com
*                 mailto://randy@integrand.com
*
* Heavily modified by Owen Taylor <otaylor@redhat.com>
*
* License: GPL (Gnu Public License)
*
*
**************************************************************************/

void
metal_draw_hline (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x1,
	    gint x2,
	    gint y)
{
  gint thickness_light;
  gint thickness_dark;
  gint i;
  GdkGC *lightgc, *darkgc;

  g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));

  thickness_light = style->ythickness / 2;
  thickness_dark = style->ythickness - thickness_light;

  lightgc = style->light_gc[state_type];
  darkgc = style->dark_gc[state_type];

  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
    }

  for (i = 0; i < thickness_dark; i++)
    {
      gdk_draw_line (window, lightgc, x2 - i - 1, y + i, x2, y + i);
      gdk_draw_line (window, darkgc, x1, y + i, x2 - i - 1, y + i);
    }

  y += thickness_dark;
  for (i = 0; i < thickness_light; i++)
    {
      gdk_draw_line (window, darkgc, x1, y + i, x1 + thickness_light - i -
		     1, y + i);
      gdk_draw_line (window, lightgc, x1 + thickness_light - i - 1, y + i,
		     x2, y + i);
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
    }
}
/**************************************************************************/
void
metal_draw_vline (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint y1,
	    gint y2,
	    gint x)
{
  gint thickness_light;
  gint thickness_dark;
  gint i;
  GdkGC *lightgc, *darkgc;

  g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));

  thickness_light = style->xthickness / 2;
  thickness_dark = style->xthickness - thickness_light;

  lightgc = style->light_gc[state_type];
  darkgc = style->dark_gc[state_type];

  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
    }

  for (i = 0; i < thickness_dark; i++)
    {
      gdk_draw_line (window, lightgc, x + i, y2 - i - 1, x + i, y2);
      gdk_draw_line (window, darkgc, x + i, y1, x + i, y2 - i - 1);
    }

  x += thickness_dark;

  for (i = 0; i < thickness_light; i++)
    {
      gdk_draw_line (window, darkgc, x + i, y1, x + i, y1 + thickness_light
		     - i);
      gdk_draw_line (window, lightgc, x + i, y1 + thickness_light - i, x +
		     i, y2);
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
    }
}
/**************************************************************************/
void
metal_draw_shadow (GtkStyle     *style,
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
  GdkGC *gc1 = NULL;		/* Initialize to quiet GCC */
  GdkGC *gc2 = NULL;
  GdkGC *gc3 = NULL;
  GdkGC *gc4 = NULL;
  
  gint thickness_light;
  gint thickness_dark;
  gint i;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

/* return; */
#if DEBUG
  printf ("metal_draw_shadow: %p %p %s %i %i %i %i\n", widget, window, detail, x,
	  y, width, height);
#endif

  if (shadow_type == GTK_SHADOW_NONE)
    return;

  /* Override shadow-type for Metal button */
  if (CHECK_DETAIL(detail, "button") || CHECK_DETAIL(detail, "buttondefault"))
    shadow_type = GTK_SHADOW_ETCHED_IN;
  if (CHECK_DETAIL(detail, "optionmenu"))
    shadow_type = GTK_SHADOW_ETCHED_IN;
  if (CHECK_DETAIL(detail, "handlebox_bin"))
    shadow_type = GTK_SHADOW_ETCHED_IN;

  /* Short-circuit some metal styles for now */
  if (CHECK_DETAIL(detail, "frame"))
    {
      gc1 = style->dark_gc[state_type];
      if (area)
	gdk_gc_set_clip_rectangle (gc1, area);
      gdk_draw_rectangle (window, gc1, FALSE, x, y, width - 1, height - 1);
      if (area)
	gdk_gc_set_clip_rectangle (gc1, NULL);
      return;			/* tbd */
    }
  if (CHECK_DETAIL(detail, "optionmenutab"))
    {
      gc1 = style->black_gc;
      if (area)
	gdk_gc_set_clip_rectangle (gc1, area);
      gdk_draw_line (window, gc1, x, y, x + 10, y);
      gdk_draw_line (window, gc1, x + 1, y + 1, x + 9, y + 1);
      gdk_draw_line (window, gc1, x + 2, y + 2, x + 8, y + 2);
      gdk_draw_line (window, gc1, x + 3, y + 3, x + 7, y + 3);
      gdk_draw_line (window, gc1, x + 4, y + 4, x + 6, y + 4);
      gdk_draw_line (window, gc1, x + 5, y + 5, x + 5, y + 4);
      if (area)
	gdk_gc_set_clip_rectangle (gc1, NULL);
      return;			/* tbd */
    }

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      /* Handled above */
    case GTK_SHADOW_IN:
    case GTK_SHADOW_ETCHED_IN:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->black_gc;
      gc4 = style->bg_gc[state_type];
      break;
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->black_gc;
      gc4 = style->bg_gc[state_type];
      break;
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, area);
      gdk_gc_set_clip_rectangle (gc2, area);
      gdk_gc_set_clip_rectangle (gc3, area);
      gdk_gc_set_clip_rectangle (gc4, area);
    }

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      break;
    case GTK_SHADOW_IN:
      gdk_draw_line (window, gc1,
		     x, y + height - 1, x + width - 1, y + height - 1);
      gdk_draw_line (window, gc1,
		     x + width - 1, y, x + width - 1, y + height - 1);

      gdk_draw_line (window, gc4,
		     x + 1, y + height - 2, x + width - 2, y + height - 2);
      gdk_draw_line (window, gc4,
		     x + width - 2, y + 1, x + width - 2, y + height - 2);

      gdk_draw_line (window, gc3,
		     x + 1, y + 1, x + width - 2, y + 1);
      gdk_draw_line (window, gc3,
		     x + 1, y + 1, x + 1, y + height - 2);

      gdk_draw_line (window, gc2,
		     x, y, x + width - 1, y);
      gdk_draw_line (window, gc2,
		     x, y, x, y + height - 1);
      break;

    case GTK_SHADOW_OUT:
      gdk_draw_line (window, gc1,
		     x + 1, y + height - 2, x + width - 2, y + height - 2);
      gdk_draw_line (window, gc1,
		     x + width - 2, y + 1, x + width - 2, y + height - 2);

      gdk_draw_line (window, gc2,
		     x, y, x + width - 1, y);
      gdk_draw_line (window, gc2,
		     x, y, x, y + height - 1);

      gdk_draw_line (window, gc4,
		     x + 1, y + 1, x + width - 2, y + 1);
      gdk_draw_line (window, gc4,
		     x + 1, y + 1, x + 1, y + height - 2);

      gdk_draw_line (window, gc3,
		     x, y + height - 1, x + width - 1, y + height - 1);
      gdk_draw_line (window, gc3,
		     x + width - 1, y, x + width - 1, y + height - 1);
      break;
    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
      thickness_light = 1;
      thickness_dark = 1;

      for (i = 0; i < thickness_dark; i++)
	{
	  gdk_draw_line (window, gc1,
			 x + i,
			 y + height - i - 1,
			 x + width - i - 1,
			 y + height - i - 1);
	  gdk_draw_line (window, gc1,
			 x + width - i - 1,
			 y + i,
			 x + width - i - 1,
			 y + height - i - 1);

	  gdk_draw_line (window, gc2,
			 x + i,
			 y + i,
			 x + width - i - 2,
			 y + i);
	  gdk_draw_line (window, gc2,
			 x + i,
			 y + i,
			 x + i,
			 y + height - i - 2);
	}

      for (i = 0; i < thickness_light; i++)
	{
	  gdk_draw_line (window, gc1,
			 x + thickness_dark + i,
			 y + thickness_dark + i,
			 x + width - thickness_dark - i - 1,
			 y + thickness_dark + i);
	  gdk_draw_line (window, gc1,
			 x + thickness_dark + i,
			 y + thickness_dark + i,
			 x + thickness_dark + i,
			 y + height - thickness_dark - i - 1);

	  gdk_draw_line (window, gc2,
			 x + thickness_dark + i,
			 y + height - thickness_light - i - 1,
			 x + width - thickness_light - 1,
			 y + height - thickness_light - i - 1);
	  gdk_draw_line (window, gc2,
			 x + width - thickness_light - i - 1,
			 y + thickness_dark + i,
			 x + width - thickness_light - i - 1,
			 y + height - thickness_light - 1);
	}
      break;
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, NULL);
      gdk_gc_set_clip_rectangle (gc2, NULL);
      gdk_gc_set_clip_rectangle (gc3, NULL);
      gdk_gc_set_clip_rectangle (gc4, NULL);
    }
}
/**************************************************************************/
void
metal_draw_polygon (GtkStyle * style,
	      GdkWindow * window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle * area,
	      GtkWidget * widget,
	      const gchar * detail,
	      GdkPoint * points,
	      gint npoints,
	      gint fill)
{
  static const gdouble pi_3_over_4 = G_PI_4 * 3;

  GdkGC *gc1;
  GdkGC *gc2;
  GdkGC *gc3;
  GdkGC *gc4;
  gdouble angle;
  gint xadjust;
  gint yadjust;
  gint i;

  g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));
  g_return_if_fail (points != NULL);

#if DEBUG
  printf ("metal_draw_polygon: %p %p %s\n", widget, window, detail);
#endif

  switch (shadow_type)
    {
    case GTK_SHADOW_IN:
      gc1 = style->bg_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->black_gc;
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
      gc3 = style->black_gc;
      gc4 = style->bg_gc[state_type];
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
    gdk_draw_polygon (window, style->bg_gc[state_type], TRUE, points, npoints);

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

      if ((angle > -pi_3_over_4) && (angle < M_PI_4))
	{
	  if (angle > -M_PI_4)
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
			 points[i + 1].x - xadjust, points[i + 1].y - yadjust);
	  gdk_draw_line (window, gc3,
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

	  gdk_draw_line (window, gc4,
			 points[i].x + xadjust, points[i].y + yadjust,
			 points[i + 1].x + xadjust, points[i + 1].y + yadjust);
	  gdk_draw_line (window, gc2,
			 points[i].x, points[i].y,
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

void
scrollbar_stepper (GtkStyle     *style,
		   GdkWindow    *window,
		   GtkStateType  state_type,
		   GdkRectangle *area,
		   GtkWidget    *widget,
		   const gchar  *detail,
		   GtkArrowType  arrow_type,
		   gint          x,
		   gint          y,
		   gint          width,
		   gint          height)
{
  GdkRectangle clip;
  
  MetalStyle *metal_style = METAL_STYLE (style);
  
  clip.x = x;
  clip.y = y;
  clip.width = width;
  clip.height = height;

  if (area)
    gdk_rectangle_intersect (&clip, area, &clip);
  
  /* We draw the last couple of pixels of the sliders on
   * the trough, since the slider should go over them at
   * the ends.
   */
  switch (arrow_type)
    {
    case GTK_ARROW_RIGHT:
      x -= 2;
      /* fall through */
    case GTK_ARROW_LEFT:
      width += 2;
      break;
    case GTK_ARROW_DOWN:
      y -= 2;
      /* fall through */
    case GTK_ARROW_UP:
      height += 2;
      break;
    }

  gdk_gc_set_clip_rectangle (metal_style->dark_gray_gc, &clip);
  gdk_gc_set_clip_rectangle (metal_style->light_gray_gc, &clip);
  gdk_gc_set_clip_rectangle (style->white_gc, &clip);

  gdk_draw_rectangle (window, style->white_gc, FALSE,
		      x + 1, y + 1, width - 2, height - 2);
  gdk_draw_rectangle (window, metal_style->dark_gray_gc, FALSE,
		      x, y, width - 2, height - 2);

  if (arrow_type != GTK_ARROW_RIGHT)
    gdk_draw_point (window, metal_style->light_gray_gc,
		    x + 1, y + height - 2);
  
  if (arrow_type != GTK_ARROW_DOWN)
    gdk_draw_point (window, metal_style->light_gray_gc,
		    x + width - 2, y + 1);
		 
  gdk_gc_set_clip_rectangle (metal_style->dark_gray_gc, NULL);
  gdk_gc_set_clip_rectangle (metal_style->light_gray_gc, NULL);
  gdk_gc_set_clip_rectangle (style->white_gc, NULL);
}

/**************************************************************************/
void
metal_draw_arrow (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    GtkArrowType arrow_type,
	    gint fill,
	    gint x,
	    gint y,
	    gint width,
	    gint height)
{
  GdkGC *gc;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
  
  gc = style->black_gc;

  if (CHECK_DETAIL(detail, "menuitem"))
    gc = style->fg_gc[state_type];

  if (CHECK_DETAIL(detail, "hscrollbar") || CHECK_DETAIL(detail, "vscrollbar"))
    {
      /* We need to restore the full area of the entire box,
       * not just the restricted area of the stepper.
       */
      reverse_engineer_stepper_box (widget, arrow_type,
				    &x, &y, &width, &height);
      
      scrollbar_stepper (style, window, state_type, area,
			 widget, detail, arrow_type,
			 x, y, width, height);

      x += 4;
      y += 4;
      width -= 9;
      height -= 9;
    }
  else if (CHECK_DETAIL(detail, "spinbutton"))
    {
      x += 2;
      width -= 4;
    }

  if (area)
    gdk_gc_set_clip_rectangle (gc, area);

  do_metal_draw_arrow (window, widget, gc, arrow_type, x, y, width, height);

  if (area)
    gdk_gc_set_clip_rectangle (gc, NULL);
}
/**************************************************************************/
void
metal_draw_diamond (GtkStyle * style,
	      GdkWindow * window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle * area,
	      GtkWidget * widget,
	      const gchar * detail,
	      gint x,
	      gint y,
	      gint width,
	      gint height)
{
  gint half_width;
  gint half_height;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

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
      gdk_draw_line (window, style->bg_gc[state_type],
		     x + 2, y + half_height,
		     x + half_width, y + height - 2);
      gdk_draw_line (window, style->bg_gc[state_type],
		     x + half_width, y + height - 2,
		     x + width - 2, y + half_height);
      gdk_draw_line (window, style->light_gc[state_type],
		     x + 1, y + half_height,
		     x + half_width, y + height - 1);
      gdk_draw_line (window, style->light_gc[state_type],
		     x + half_width, y + height - 1,
		     x + width - 1, y + half_height);
      gdk_draw_line (window, style->light_gc[state_type],
		     x, y + half_height,
		     x + half_width, y + height);
      gdk_draw_line (window, style->light_gc[state_type],
		     x + half_width, y + height,
		     x + width, y + half_height);

      gdk_draw_line (window, style->black_gc,
		     x + 2, y + half_height,
		     x + half_width, y + 2);
      gdk_draw_line (window, style->black_gc,
		     x + half_width, y + 2,
		     x + width - 2, y + half_height);
      gdk_draw_line (window, style->dark_gc[state_type],
		     x + 1, y + half_height,
		     x + half_width, y + 1);
      gdk_draw_line (window, style->dark_gc[state_type],
		     x + half_width, y + 1,
		     x + width - 1, y + half_height);
      gdk_draw_line (window, style->dark_gc[state_type],
		     x, y + half_height,
		     x + half_width, y);
      gdk_draw_line (window, style->dark_gc[state_type],
		     x + half_width, y,
		     x + width, y + half_height);
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
      gdk_draw_line (window, style->black_gc,
		     x, y + half_height,
		     x + half_width, y + height);
      gdk_draw_line (window, style->black_gc,
		     x + half_width, y + height,
		     x + width, y + half_height);

      gdk_draw_line (window, style->bg_gc[state_type],
		     x + 2, y + half_height,
		     x + half_width, y + 2);
      gdk_draw_line (window, style->bg_gc[state_type],
		     x + half_width, y + 2,
		     x + width - 2, y + half_height);
      gdk_draw_line (window, style->light_gc[state_type],
		     x + 1, y + half_height,
		     x + half_width, y + 1);
      gdk_draw_line (window, style->light_gc[state_type],
		     x + half_width, y + 1,
		     x + width - 1, y + half_height);
      gdk_draw_line (window, style->light_gc[state_type],
		     x, y + half_height,
		     x + half_width, y);
      gdk_draw_line (window, style->light_gc[state_type],
		     x + half_width, y,
		     x + width, y + half_height);
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
/**************************************************************************/
void
metal_draw_string (GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail,
	     gint x,
	     gint y,
	     const gchar * string)
{
  GdkGC *fggc, *whitegc, *midgc;
  MetalStyle *metal_style = METAL_STYLE (style);

  g_return_if_fail(sanitize_parameters(style, window, NULL, NULL));

#if DEBUG
  printf ("metal_draw_string: %p %p %s %i %i\n", widget, window, detail, x, y);
#endif

  if (CHECK_DETAIL(detail, "label"))
    {
      fggc = style->black_gc;
      whitegc = style->white_gc;
      midgc = metal_style->mid_gray_gc;
    }
  else
    {
      fggc = style->fg_gc[state_type];
      whitegc = style->white_gc;
      midgc = metal_style->mid_gray_gc;
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (fggc, area);
      gdk_gc_set_clip_rectangle (whitegc, area);
      gdk_gc_set_clip_rectangle (midgc, area);
    }

  
  if (state_type == GTK_STATE_INSENSITIVE)
    {
      gdk_draw_string (window, gtk_style_get_font (style), whitegc, x + 1, y + 1, string);
      gdk_draw_string (window, gtk_style_get_font (style), midgc, x, y, string);
    }
  else
    {
      gdk_draw_string (window, gtk_style_get_font (style), fggc, x, y, string);
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (fggc, NULL);
      gdk_gc_set_clip_rectangle (whitegc, NULL);
      gdk_gc_set_clip_rectangle (midgc, NULL);
    }
}

/**************************************************************************/
void
metal_draw_box (GtkStyle      *style,
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
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

#if DEBUG
  printf ("metal_draw_box: %p %p %s %i %i %i %i\n", widget, window, detail, x, y,
	  width, height);
#endif

  /* ===================================================================== */

  if (widget && CHECK_DETAIL(detail, "trough"))
    {

      if (GTK_IS_PROGRESS_BAR (widget))
	{
	  if (area)
	    gdk_gc_set_clip_rectangle (style->light_gc[GTK_STATE_NORMAL], area);
	  gdk_draw_rectangle (window, style->light_gc[GTK_STATE_NORMAL],
			      TRUE, x, y, width, height);
	  if (area)
	    gdk_gc_set_clip_rectangle (style->light_gc[GTK_STATE_NORMAL], NULL);
	  metal_draw_shadow (style, window, state_type, shadow_type, area,
			    widget, detail,
			    x, y, width, height);
	}
      else if (GTK_IS_SCROLLBAR (widget))
	{
	  do_metal_draw_scrollbar_trough (style, window, state_type, shadow_type,
				  area, widget, detail, x, y, width, height);
	}
      else if (GTK_IS_SCALE (widget))
	{
	 do_metal_draw_scale_trough (style, window, state_type, shadow_type,
			      area, widget, detail, x, y, width, height);
	}
      else
	{
#if 0
	  GdkPixmap *pm;
	  gint xthik;
	  gint ythik;

	  xthik = style->xthickness;
	  ythik = style->ythickness;

	  pm = gdk_pixmap_new (window, 2, 2, -1);

	  gdk_draw_point (pm, style->bg_gc[GTK_STATE_NORMAL], 0, 0);
	  gdk_draw_point (pm, style->bg_gc[GTK_STATE_NORMAL], 1, 1);
	  gdk_draw_point (pm, style->light_gc[GTK_STATE_NORMAL], 1, 0);
	  gdk_draw_point (pm, style->light_gc[GTK_STATE_NORMAL], 0, 1);
	  gdk_window_set_back_pixmap (window, pm, FALSE);
	  gdk_window_clear (window);

	  gdk_pixmap_unref (pm);
#endif /* 0 */
	}
    }
  else if (CHECK_DETAIL(detail, "menu"))
    {
      do_metal_draw_menu (style, window, state_type, shadow_type,
		  area, widget, detail, x, y, width, height);
    }
  else if (CHECK_DETAIL(detail, "menuitem"))
    {
      do_metal_draw_menu_item (style, window, state_type, shadow_type,
		       area, widget, detail, x, y, width, height);
    }
  else if (CHECK_DETAIL(detail, "bar"))
    {
      if (area)
	gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_SELECTED], area);
      gdk_draw_rectangle (window, style->bg_gc[GTK_STATE_SELECTED],
			  TRUE, x + 1, y + 1, width - 2, height - 2);
      if (area)
	gdk_gc_set_clip_rectangle (style->bg_gc[GTK_STATE_SELECTED], NULL);
    }
  else if (CHECK_DETAIL(detail, "menubar"))
    {
      if (area)
	gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);
      gdk_draw_rectangle (window, style->bg_gc[state_type], TRUE,
			  x, y, width, height);
      if (area)
	gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL);
    }
  else if (CHECK_DETAIL(detail, "notebook"))
    {
      do_metal_draw_notebook (style, window, state_type, shadow_type,
		      area, widget, detail, x, y, width, height);
    }
/*  else if (CHECK_DETAIL(detail, "tab"))
    {
      do_metal_draw_notebook_tab (style, window, state_type, shadow_type,
		 area, widget, detail, x, y, width, height);
    }
*/
  else if (CHECK_DETAIL(detail, "button") || CHECK_DETAIL(detail, "togglebutton"))
    {
      do_metal_draw_button (style, window, state_type, shadow_type,
		    area, widget, detail, x, y, width, height);
    }
  else if (CHECK_DETAIL(detail, "buttondefault"))
    {
    }
  else if (CHECK_DETAIL(detail, "hscrollbar") || CHECK_DETAIL(detail, "vscrollbar"))
    {
      /* We do all the drawing in metal_draw_arrow () */
    }
  else
    {
      if ((!style->bg_pixmap[state_type]) || GDK_IS_PIXMAP (window))
	{
	  if (area)
	    gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);
	  gdk_draw_rectangle (window, style->bg_gc[state_type], TRUE,
			      x, y, width, height);
	  if (area)
	    gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL);
	}
      else
	{
	  gtk_style_apply_default_pixmap (style, window, state_type, area,
					  x, y, width, height);
	}
      metal_draw_shadow (style, window, state_type, shadow_type, area,
			widget, detail,
			x, y, width, height);
    }
}
/**************************************************************************/
void
metal_scrollbar_slider (GtkStyle * style,
			GdkWindow * window,
			GtkStateType state_type,
			GtkShadowType shadow_type,
			GdkRectangle * area,
			GtkWidget * widget,
			const gchar * detail,
			gint x,
			gint y,
			gint width,
			gint height)
{
  MetalStyle *metal_style = METAL_STYLE (style);
  GdkPixmap *pm;
  GdkGC *fillgc;
  GdkGCValues values;
  GdkGC *lightgc, *midgc, *darkgc, *whitegc;
  int w, h;

  gint stepper_size = 15;

  if (widget && GTK_IS_RANGE (widget))
    gtk_widget_style_get (widget, "stepper_size", &stepper_size, NULL);

  stepper_size += 2;

  lightgc = style->bg_gc[GTK_STATE_PRELIGHT];
  midgc = style->bg_gc[GTK_STATE_SELECTED];
  darkgc = style->fg_gc[GTK_STATE_PRELIGHT];
  whitegc = style->white_gc;

  /* Draw textured surface */
  pm = gdk_pixmap_new (window, 4, 4, -1);

  gdk_draw_rectangle (pm, midgc, TRUE, 0, 0, 4, 4);

  gdk_draw_point (pm, darkgc, 0, 0);
  gdk_draw_point (pm, lightgc, 1, 1);
  gdk_draw_point (pm, darkgc, 2, 2);
  gdk_draw_point (pm, lightgc, 3, 3);

  values.fill = GDK_TILED;
  values.ts_x_origin = (x + 5) % 4;
  values.ts_y_origin = (y + 3) % 4;
  fillgc = gdk_gc_new_with_values (window, &values,
				   GDK_GC_FILL | GDK_GC_TS_X_ORIGIN | GDK_GC_TS_Y_ORIGIN);
  gdk_gc_set_tile (fillgc, pm);

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (midgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
      gdk_gc_set_clip_rectangle (whitegc, area);
      gdk_gc_set_clip_rectangle (fillgc, area);
    }

  /* Draw backgound */
  gdk_draw_rectangle (window, midgc, TRUE, x, y, width, height);

  /* Draw border */
  gdk_draw_rectangle (window, lightgc, FALSE, x + 1, y + 1,
		      width - 2, height - 2);
  gdk_draw_rectangle (window, darkgc, FALSE, x + 0, y + 0,
		      width - 2, height - 2);

  if (GTK_CHECK_TYPE (widget, gtk_hscrollbar_get_type ()))
    {
      gdk_draw_line (window, whitegc, 
		     x + 0,         y + height - 1, 
		     x + width - 1, y + height - 1);
      gdk_draw_line (window, metal_style->dark_gray_gc,
		     x,         y + height - 2,
		     x + width, y + height - 2);
      gdk_draw_point (window, metal_style->dark_gray_gc, x + width - 1, y);

      /* At the right end of the scrollbar, don't draw the shadow beneath
       * the scrollbar, instead draw the highlight of the button
       */
      if (widget &&
	  (x + width + stepper_size - 2 == widget->allocation.x + widget->allocation.width))
	gdk_draw_line (window, whitegc,
		       x + width - 1, y + 1, 
		       x + width - 1, y + height - 3);
      else
	gdk_draw_line (window, metal_style->mid_gray_gc,
		       x + width - 1, y + 1, 
		       x + width - 1, y + height - 3);
    }
  else
    {
      gdk_draw_line (window, whitegc, 
		     x + width - 1, y + 0, 
		     x + width - 1, y + height - 1);
      gdk_draw_line (window, metal_style->dark_gray_gc,
		     x + width - 2, y,
		     x + width - 2, y + height);
      gdk_draw_point (window, metal_style->dark_gray_gc, x, y + height - 1);

      /* At the lower end of the scrollbar, don't draw the shadow beneath
       * the scrollbar, instead draw the highlight of the button
       */
      if (widget &&
	  (y + height + stepper_size - 2 == widget->allocation.y + widget->allocation.height))
	gdk_draw_line (window, whitegc,
		       x + 1,         y + height - 1,
		       x + width - 3, y + height - 1);
      else
	gdk_draw_line (window, metal_style->mid_gray_gc,
		       x + 1,         y + height - 1,
		       x + width - 3, y + height - 1);
    }

  if (GTK_CHECK_TYPE (widget, gtk_hscrollbar_get_type ()))
    {
      w = width & 1 ? width - 11 : width - 10;
      h = height & 1 ? height - 7 : height - 8;
      gdk_draw_rectangle (window, fillgc, TRUE, x + 5, y + 3, w, h);
    }
  else
    {
      w = width & 1 ? width - 7 : width - 8;
      h = height & 1 ? height - 11 : height - 10;
      gdk_draw_rectangle (window, fillgc, TRUE, x + 3, y + 5, w, h);
    }
  gdk_gc_unref (fillgc);
  gdk_pixmap_unref (pm);

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (midgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
      gdk_gc_set_clip_rectangle (whitegc, NULL);
    }
}
/**************************************************************************/
void
metal_scale_slider (GtkStyle * style,
		    GdkWindow * window,
		    GtkStateType state_type,
		    GtkShadowType shadow_type,
		    GdkRectangle * area,
		    GtkWidget * widget,
		    const gchar * detail,
		    gint x,
		    gint y,
		    gint width,
		    gint height,
		    GtkOrientation orientation)
{
  GdkPixmap *pm;
  GdkGC *fillgc;
  GdkGCValues values;
  GdkGC *lightgc, *midgc, *darkgc, *whitegc, *blackgc;
  int w, h;

  /* Get colors */
  lightgc = style->bg_gc[GTK_STATE_PRELIGHT];
  midgc = style->bg_gc[GTK_STATE_SELECTED];
  darkgc = style->fg_gc[GTK_STATE_PRELIGHT];
  whitegc = style->white_gc;
  blackgc = style->black_gc;

#if 1
  /* Create pixmap for drawing textured surface; we need to do this before
   * setting the clip.
   */
  pm = gdk_pixmap_new (window, 4, 4, -1);
  
  gdk_draw_rectangle (pm, midgc, TRUE, 0, 0, 4, 4);
  gdk_draw_point (pm, darkgc, 0, 0);
  gdk_draw_point (pm, lightgc, 1, 1);
  gdk_draw_point (pm, darkgc, 2, 2);
  gdk_draw_point (pm, lightgc, 3, 3);

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (midgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
      gdk_gc_set_clip_rectangle (whitegc, area);
#if 0      
      gdk_gc_set_clip_rectangle (blackgc, area);
      gdk_gc_set_clip_rectangle (metal_style->light_gray_gc, area);
#endif
    }

  /* Draw backgound */
  gdk_draw_rectangle (window, midgc, TRUE, x, y, width, height);

  /* Draw border */
  gdk_draw_rectangle (window, lightgc, FALSE, x + 1, y + 1, width - 2, height - 2);
  gdk_draw_rectangle (window, darkgc, FALSE, x + 0, y + 0,  width - 2, height - 2);
  if (GTK_CHECK_TYPE (widget, gtk_hscale_get_type ()))
    {
      gdk_draw_line (window, whitegc, x + 0, y + height - 1, x + width - 1,
		     y + height - 1);
      gdk_draw_line (window, midgc, x + width - 1, y + 1, x + width - 1, y +
		     height - 2);
    }
  else
    {
      gdk_draw_line (window, whitegc, x + width - 1, y + 0, x + width - 1, y
		     + height - 1);
      gdk_draw_line (window, midgc, x + 0, y + height - 1, x + width - 2, y
		     + height - 1);
    }
  /* Draw textured surface */
  values.fill = GDK_TILED;
  values.ts_x_origin = x + 5;
  values.ts_y_origin = y + 3;
  fillgc = gdk_gc_new_with_values (window, &values,
				   GDK_GC_FILL | GDK_GC_TS_X_ORIGIN | GDK_GC_TS_Y_ORIGIN);
  if (area)
    gdk_gc_set_clip_rectangle (fillgc, area);
  gdk_gc_set_tile (fillgc, pm);
  if (GTK_CHECK_TYPE (widget, gtk_hscale_get_type ()))
    {
      w = width & 1 ? width - 11 : width - 10;
      h = height & 1 ? height - 7 : height - 8;
      gdk_draw_rectangle (window, fillgc, TRUE, x + 5, y + 3, w, h);
    }
  else
    {
      w = width & 1 ? width - 7 : width - 8;
      h = height & 1 ? height - 11 : height - 10;
      gdk_draw_rectangle (window, fillgc, TRUE, x + 3, y + 5, w, h);
    }
  gdk_gc_unref (fillgc);
  gdk_pixmap_unref (pm);

  /* Draw middle line */
  if (GTK_CHECK_TYPE (widget, gtk_hscale_get_type ()))
    {
      if (state_type == GTK_STATE_PRELIGHT)
	{
	  gdk_draw_line (window, darkgc, x + width / 2, y + 2, x + width /
			 2, y + height - 4);
	  gdk_draw_line (window, whitegc, x + width / 2 + 1, y + 2, x +
			 width / 2 + 1, y + height - 4);
	}
      else
	{
	  gdk_draw_line (window, darkgc, x + width / 2, y + 2, x + width /
			 2, y + height - 4);
	  gdk_draw_line (window, lightgc, x + width / 2 + 1, y + 2, x +
			 width / 2 + 1, y + height - 4);
	}
    }
  else
    {
      if (state_type == GTK_STATE_PRELIGHT)
	{
	  gdk_draw_line (window, darkgc, x + 2, y + height / 2, x + width -
			 4, y + height / 2);
	  gdk_draw_line (window, whitegc, x + 2, y + height / 2 + 1, x +
			 width - 4, y + height / 2 + 1);
	}
      else
	{
	  gdk_draw_line (window, darkgc, x + 2, y + height / 2, x + width -
			 4, y + height / 2);
	  gdk_draw_line (window, lightgc, x + 2, y + height / 2 + 1, x +
			 width - 4, y + height / 2 + 1);
	}
    }

#else
  /* The following code draws the sliders more faithfully to the 
     JL&F spec, but I think it looks bad. */

  /* Requires GtkScale::slider-length of 15 */

  GdkPoint points[5];

  /* Draw backgound */
  gdk_draw_rectangle (window, lightgc, TRUE, x, y, width - 1, height - 1);

  /* Draw textured surface */
  pm = gdk_pixmap_new (window, 4, 4, -1);

  gdk_draw_rectangle (pm, midgc, TRUE, 0, 0, 4, 4);
  if (state_type == GTK_STATE_PRELIGHT)
    {
      gdk_draw_point (pm, darkgc, 0, 0);
      gdk_draw_point (pm, whitegc, 1, 1);
      gdk_draw_point (pm, darkgc, 2, 2);
      gdk_draw_point (pm, whitegc, 3, 3);
    }
  else
    {
      gdk_draw_point (pm, darkgc, 0, 0);
      gdk_draw_point (pm, lightgc, 1, 1);
      gdk_draw_point (pm, darkgc, 2, 2);
      gdk_draw_point (pm, lightgc, 3, 3);
    }

  values.fill = GDK_TILED;
  values.ts_x_origin = x + 5;
  values.ts_y_origin = y + 3;
  fillgc = gdk_gc_new_with_values (window, &values,
				   GDK_GC_FILL | GDK_GC_TS_X_ORIGIN | GDK_GC_TS_Y_ORIGIN);
  if (area)
    gdk_gc_set_clip_rectangle (fillgc, area);
  gdk_gc_set_tile (fillgc, pm);
  w = width - 4;
  h = height - 4;
  gdk_draw_rectangle (window, fillgc, TRUE, x + 2, y + 2, w, h);
  gdk_gc_unref (fillgc);
  gdk_pixmap_unref (pm);

  if (GTK_CHECK_TYPE (widget, gtk_hscale_get_type ()))
    {
      /* Draw border */
      points[0].x = x;
      points[0].y = y;
      points[1].x = x + 14;
      points[1].y = y;
      points[2].x = x + 14;
      points[2].y = y + 7;
      points[3].x = x + 7;
      points[3].y = y + 14;
      points[4].x = x;
      points[4].y = y + 7;
      gdk_draw_polygon (window, blackgc, FALSE, points, 5);
      points[0].x = x + 1;
      points[0].y = y + 1;
      points[1].x = x + 13;
      points[1].y = y + 1;
      points[2].x = x + 13;
      points[2].y = y + 7;
      points[3].x = x + 7;
      points[3].y = y + 13;
      points[4].x = x + 1;
      points[4].y = y + 7;
      gdk_draw_polygon (window, lightgc, FALSE, points, 5);

      /* Fix bottom corners */
      points[0].x = x;
      points[0].y = y + 14;
      points[1].x = x;
      points[1].y = y + 8;
      points[2].x = x + 6;
      points[2].y = y + 14;
      gdk_draw_polygon (window, metal_style->light_gray_gc, FALSE, points, 3);
      gdk_draw_polygon (window, metal_style->light_gray_gc, TRUE, points, 3);
      points[0].x = x + 14;
      points[0].y = y + 14;
      points[1].x = x + 14;
      points[1].y = y + 8;
      points[2].x = x + 8;
      points[2].y = y + 14;
      gdk_draw_polygon (window, metal_style->light_gray_gc, FALSE, points, 3);
      gdk_draw_polygon (window, metal_style->light_gray_gc, TRUE, points, 3);
      gdk_draw_rectangle (window, metal_style->light_gray_gc, TRUE, x, y +
			  15, width, height - 15);
    }
  else
    {
      /* Draw border */
      points[0].x = x;
      points[0].y = y + 7;
      points[1].x = x + 7;
      points[1].y = y;
      points[2].x = x + 14;
      points[2].y = y;
      points[3].x = x + 14;
      points[3].y = y + 14;
      points[4].x = x + 7;
      points[4].y = y + 14;
      gdk_draw_polygon (window, blackgc, FALSE, points, 5);

      points[0].x = x + 1;
      points[0].y = y + 7;
      points[1].x = x + 7;
      points[1].y = y + 1;
      points[2].x = x + 13;
      points[2].y = y + 1;
      points[3].x = x + 13;
      points[3].y = y + 13;
      points[4].x = x + 7;
      points[4].y = y + 13;
      gdk_draw_polygon (window, lightgc, FALSE, points, 5);

      /* Fix corners */
      points[0].x = x;
      points[0].y = y;
      points[1].x = x + 6;
      points[1].y = y;
      points[2].x = x;
      points[2].y = y + 6;
      gdk_draw_polygon (window, metal_style->light_gray_gc, FALSE, points, 3);
      gdk_draw_polygon (window, metal_style->light_gray_gc, TRUE, points, 3);
      points[0].x = x;
      points[0].y = y + 8;
      points[1].x = x;
      points[1].y = y + 14;
      points[2].x = x + 6;
      points[2].y = y + 14;
      gdk_draw_polygon (window, metal_style->light_gray_gc, FALSE, points, 3);
      gdk_draw_polygon (window, metal_style->light_gray_gc, TRUE, points, 3);
/*      gdk_draw_rectangle(window, metal_style->light_gray_gc, TRUE,  x, y+15, width, height-15); */
    }
#endif

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (midgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
      gdk_gc_set_clip_rectangle (whitegc, NULL);
#if 0      
      gdk_gc_set_clip_rectangle (blackgc, NULL);
      gdk_gc_set_clip_rectangle (metal_style->light_gray_gc, NULL);
#endif      
    }
}
/**************************************************************************/
void
metal_draw_check (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height)
{
  GdkGC *gc1, *gc2, *gc3, *gc4;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

  /* Fixed size only */

#if DEBUG
  printf ("metal_draw_check: %p %p %s %i %i %i %i\n", widget, window, detail, x,
	  y, width, height);
#endif

  gc1 = style->black_gc;
  gc2 = style->bg_gc[GTK_STATE_NORMAL];
  gc3 = style->dark_gc[state_type];
  gc4 = style->light_gc[state_type];

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc1, area);
      gdk_gc_set_clip_rectangle (gc2, area);
      gdk_gc_set_clip_rectangle (gc3, area);
      gdk_gc_set_clip_rectangle (gc4, area);
    }

  /* Draw box */
  if (GTK_CHECK_TYPE (widget, gtk_menu_item_get_type ()))
    {
      gdk_draw_rectangle (window, gc3, FALSE, x - 2, y - 2, 8, 8);
      gdk_draw_rectangle (window, gc4, FALSE, x - 1, y - 1, 8, 8);

      if (shadow_type == GTK_SHADOW_IN)
	{
	  gdk_draw_line (window, gc1, x + 1, y + 0, x + 1, y + 4);
	  gdk_draw_line (window, gc1, x + 2, y + 0, x + 2, y + 4);
	  gdk_draw_line (window, gc1, x + 3, y + 3, x + 7, y - 1);
	  gdk_draw_line (window, gc1, x + 3, y + 2, x + 7, y - 2);
	}
    }
  else
    {
      gdk_draw_rectangle (window, gc2, TRUE, x, y, width, height);

      gdk_draw_rectangle (window, gc3, FALSE, x - 2, y - 2, 11, 11);
      gdk_draw_rectangle (window, gc4, FALSE, x - 1, y - 1, 11, 11);

      if (shadow_type == GTK_SHADOW_IN)
	{
	  gdk_draw_line (window, gc1, x + 1, y + 3, x + 1, y + 7);
	  gdk_draw_line (window, gc1, x + 2, y + 3, x + 2, y + 7);
	  gdk_draw_line (window, gc1, x + 3, y + 6, x + 7, y + 2);
	  gdk_draw_line (window, gc1, x + 3, y + 5, x + 7, y + 1);
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
/**************************************************************************/
void
metal_draw_option (GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail,
	     gint x,
	     gint y,
	     gint width,
	     gint height)
{
  GdkGC *gc0;
  GdkGC *gc1;
  GdkGC *gc2;
  GdkGC *gc3;
  GdkGC *gc4;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

  x -= 1;
  y -= 1;
  width += 2;
  height += 2;

  gc0 = style->white_gc;
  gc1 = style->light_gc[GTK_STATE_NORMAL];
  gc2 = style->bg_gc[GTK_STATE_NORMAL];
  gc3 = style->dark_gc[GTK_STATE_NORMAL];
  gc4 = style->black_gc;

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc0, area);
      gdk_gc_set_clip_rectangle (gc1, area);
      gdk_gc_set_clip_rectangle (gc2, area);
      gdk_gc_set_clip_rectangle (gc3, area);
      gdk_gc_set_clip_rectangle (gc4, area);
    }

  /* Draw radio button, metal-stle
     There is probably a better way to do this
     with pixmaps. Fix later. */

  if (GTK_CHECK_TYPE (widget, gtk_menu_item_get_type ()))
    {
      /* dark */
      gdk_draw_line (window, gc3, x + 2, y, x + 6, y);
      gdk_draw_line (window, gc3, x + 1, y + 1, x + 1, y + 1);
      gdk_draw_line (window, gc3, x + 7, y + 1, x + 7, y + 1);
      gdk_draw_line (window, gc3, x + 2, y + 8, x + 2, y + 8);
      gdk_draw_line (window, gc3, x + 7, y + 7, x + 7, y + 7);
      gdk_draw_line (window, gc3, x + 2, y + 8, x + 6, y + 8);
      gdk_draw_line (window, gc3, x, y + 2, x, y + 6);
      gdk_draw_line (window, gc3, x + 8, y + 2, x + 8, y + 6);

      /* white */
      gdk_draw_line (window, gc0, x + 3, y + 1, x + 6, y + 1);
      gdk_draw_line (window, gc0, x + 8, y + 1, x + 8, y + 1);
      gdk_draw_line (window, gc0, x + 2, y + 2, x + 2, y + 2);
      gdk_draw_line (window, gc0, x + 1, y + 3, x + 1, y + 6);
      gdk_draw_line (window, gc0, x + 9, y + 2, x + 9, y + 7);
      gdk_draw_line (window, gc0, x + 1, y + 8, x + 1, y + 8);
      gdk_draw_line (window, gc0, x + 8, y + 8, x + 8, y + 8);
      gdk_draw_line (window, gc0, x + 2, y + 9, x + 7, y + 9);

      if (shadow_type == GTK_SHADOW_IN)
	{
	  gdk_draw_rectangle (window, gc4, TRUE, x + 2, y + 3, 5, 3);
	  gdk_draw_rectangle (window, gc4, TRUE, x + 3, y + 2, 3, 5);
	}
    }
  else
    {
      /* background */
      gdk_draw_rectangle (window, gc2, TRUE, x, y, width, height);

      /* dark */
      gdk_draw_line (window, gc3, x + 4, y, x + 7, y);
      gdk_draw_line (window, gc3, x + 2, y + 1, x + 3, y + 1);
      gdk_draw_line (window, gc3, x + 8, y + 1, x + 9, y + 1);
      gdk_draw_line (window, gc3, x + 2, y + 10, x + 3, y + 10);
      gdk_draw_line (window, gc3, x + 8, y + 10, x + 9, y + 10);
      gdk_draw_line (window, gc3, x + 4, y + 11, x + 7, y + 11);


      gdk_draw_line (window, gc3, x, y + 4, x, y + 7);
      gdk_draw_line (window, gc3, x + 1, y + 2, x + 1, y + 3);
      gdk_draw_line (window, gc3, x + 1, y + 8, x + 1, y + 9);
      gdk_draw_line (window, gc3, x + 10, y + 2, x + 10, y + 3);
      gdk_draw_line (window, gc3, x + 10, y + 8, x + 10, y + 9);
      gdk_draw_line (window, gc3, x + 11, y + 4, x + 11, y + 7);

      /* white */
      gdk_draw_line (window, gc0, x + 4, y + 1, x + 7, y + 1);
      gdk_draw_line (window, gc0, x + 2, y + 2, x + 3, y + 2);
      gdk_draw_line (window, gc0, x + 8, y + 2, x + 9, y + 2);
      gdk_draw_line (window, gc0, x + 2, y + 11, x + 3, y + 11);
      gdk_draw_line (window, gc0, x + 8, y + 11, x + 9, y + 11);
      gdk_draw_line (window, gc0, x + 4, y + 12, x + 7, y + 12);


      gdk_draw_line (window, gc0, x + 1, y + 4, x + 1, y + 7);
      gdk_draw_line (window, gc0, x + 2, y + 2, x + 2, y + 3);
      gdk_draw_line (window, gc0, x + 2, y + 8, x + 2, y + 9);
      gdk_draw_line (window, gc0, x + 11, y + 2, x + 11, y + 3);
      gdk_draw_line (window, gc0, x + 11, y + 8, x + 11, y + 9);
      gdk_draw_line (window, gc0, x + 12, y + 4, x + 12, y + 7);
      gdk_draw_point (window, gc0, x + 10, y + 1);
      gdk_draw_point (window, gc0, x + 10, y + 10);

      if (shadow_type == GTK_SHADOW_IN)
	{
	  gdk_draw_rectangle (window, gc4, TRUE, x + 3, y + 4, 6, 4);
	  gdk_draw_rectangle (window, gc4, TRUE, x + 4, y + 3, 4, 6);
	}
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle (gc0, NULL);
      gdk_gc_set_clip_rectangle (gc1, NULL);
      gdk_gc_set_clip_rectangle (gc2, NULL);
      gdk_gc_set_clip_rectangle (gc3, NULL);
      gdk_gc_set_clip_rectangle (gc4, NULL);
    }
}
/**************************************************************************/
void
metal_draw_tab (GtkStyle * style,
	  GdkWindow * window,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GdkRectangle * area,
	  GtkWidget * widget,
	  const gchar * detail,
	  gint x,
	  gint y,
	  gint width,
	  gint height)
{
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

#if DEBUG
  printf ("metal_draw_tab: %p %s %i %i\n", detail, detail, width, height);
#endif

}

/**************************************************************************/
void
metal_draw_shadow_gap (GtkStyle * style,
		 GdkWindow * window,
		 GtkStateType state_type,
		 GtkShadowType shadow_type,
		 GdkRectangle * area,
		 GtkWidget * widget,
		 const gchar * detail,
		 gint x,
		 gint y,
		 gint width,
		 gint height,
		 GtkPositionType gap_side,
		 gint gap_x,
		 gint gap_width)
{
  GdkRectangle rect;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

#if DEBUG
  printf ("metal_draw_shadow_gap: %p %p %s %i %i %i %i\n", widget, window, detail,
	  x, y, width, height);
#endif

  gap_width -= 1;

  metal_draw_shadow (style, window, state_type, shadow_type, area, widget, detail,
		    x, y, width, height);

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
/**************************************************************************/
void
metal_draw_box_gap (GtkStyle       *style,
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
  GdkRectangle rect;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

#if DEBUG
  printf ("metal_draw_box_gap: %p %p %s %i %i %i %i\n", widget, window, detail, x,
	  y, width, height);
#endif

  metal_draw_box (style, window, state_type, shadow_type, area, widget, detail,
		 x, y, width, height);

  /* The default overlap is two pixels, but we only want a one pixel overlap
   */
  gap_width -= 1;

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
/**************************************************************************/
void
metal_draw_extension (GtkStyle * style,
		GdkWindow * window,
		GtkStateType state_type,
		GtkShadowType shadow_type,
		GdkRectangle * area,
		GtkWidget * widget,
		const gchar * detail,
		gint x,
		gint y,
		gint width,
		gint height,
		GtkPositionType gap_side)
{
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

#if DEBUG
  printf ("metal_draw_extension: %p %p %s %i %i %i %i\n", widget, window, detail,
	  x, y, width, height);
#endif
	switch (gap_side)
	{
		case GTK_POS_TOP: gap_side = GTK_POS_BOTTOM; break;
		case GTK_POS_BOTTOM: gap_side = GTK_POS_TOP; break;
		case GTK_POS_LEFT: gap_side = GTK_POS_RIGHT; break;
		case GTK_POS_RIGHT: gap_side = GTK_POS_LEFT; break;
	}
  do_metal_draw_notebook_tab (style, window, state_type, shadow_type, area, widget, detail, x, y, width, height, gap_side);
}
/**************************************************************************/
void
metal_draw_notebook_focus (GtkWidget *widget,
		     GdkWindow *window,
		     GdkGC     *gc,
		     gint       x,
		     gint       y,
		     gint       width,
		     gint       height)
{
  GtkPositionType tab_position = GTK_POS_TOP;
  GdkPoint points[6];
  gint tab_hborder = 2;
  gint tab_vborder = 2;

  if (widget && GTK_IS_NOTEBOOK (widget))
    {
      GtkNotebook *notebook = GTK_NOTEBOOK (widget);
      
      tab_hborder = notebook->tab_hborder;
      tab_vborder = notebook->tab_vborder;
      tab_position = gtk_notebook_get_tab_pos (notebook);
    }

  adjust_notebook_tab_size (tab_position, &width, &height);

  x -= tab_hborder;
  y -= tab_vborder;
  width += 2 * tab_hborder;
  height += 2 * tab_vborder;

  switch (tab_position)
    {
    default:
    case GTK_POS_TOP:
      points[0].x = x + 4;          points[0].y = y;
      points[1].x = x + width - 1;  points[1].y = y;
      points[2].x = x + width - 1;  points[2].y = y + height;
      points[3].x = x;              points[3].y = y + height;
      points[4].x = x;              points[4].y = y + 4;
      break;
    case GTK_POS_LEFT:
      points[0].x = x + 4;          points[0].y = y - 1;
      points[1].x = x + width - 1;  points[1].y = y - 1;
      points[2].x = x + width - 1;  points[2].y = y + height;
      points[3].x = x;              points[3].y = y + height;
      points[4].x = x;              points[4].y = y + 3;
      break;
    case GTK_POS_RIGHT:
      points[0].x = x;              points[0].y = y - 1;
      points[1].x = x + width - 5;  points[1].y = y - 1;
      points[2].x = x + width - 1;  points[2].y = y + 3;
      points[3].x = x + width - 1;  points[3].y = y + height;
      points[4].x = x;              points[4].y = y + height;
      break;
    case GTK_POS_BOTTOM:
      points[0].x = x;              points[0].y = y;
      points[1].x = x + width - 1;  points[1].y = y;
      points[2].x = x + width - 1;  points[2].y = y + height - 1;
      points[3].x = x + 4;          points[3].y = y + height - 1;
      points[4].x = x;              points[4].y = y + height - 5;
      break;
    }

  points[5] = points[0];
  
  gdk_draw_polygon (window, gc, FALSE, points, 6);
}

void
metal_draw_focus (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height)
{
  GdkGC *focusgc;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));
  
#if DEBUG
  printf ("metal_draw_focus: %p %p %s %i %i %i %i\n", widget, window, detail, x,
	  y, width, height);
#endif

  if (detail && strcmp (detail, "add-mode") == 0)
    {
      parent_class->draw_focus (style, window, state_type, area, widget, detail, x, y, width, height);
      return;
    }

  focusgc = style->bg_gc[GTK_STATE_SELECTED];

  if (area)
    gdk_gc_set_clip_rectangle (focusgc, area);

  if (CHECK_DETAIL(detail, "tab"))
    metal_draw_notebook_focus (widget, window, focusgc, x, y, width, height);
  else
    gdk_draw_rectangle (window, focusgc, FALSE, x, y, width - 1, height - 1);

  if (area)
    gdk_gc_set_clip_rectangle (focusgc, NULL);
}
/**************************************************************************/
void
metal_draw_slider (GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     const gchar * detail,
	     gint x,
	     gint y,
	     gint width,
	     gint height,
	     GtkOrientation orientation)
{
  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

  if (CHECK_DETAIL(detail, "slider"))
    metal_scrollbar_slider (style, window, state_type, shadow_type,
			    area, widget, detail, x, y, width, height);
  else
    metal_scale_slider (style, window, state_type, shadow_type,
			area, widget, detail, x, y, width, height, orientation);
}
/**************************************************************************/
void
metal_draw_paned_handle (GtkStyle      *style,
		   GdkWindow     *window,
		   GtkStateType   state_type,
		   GtkShadowType  shadow_type,
		   GdkRectangle  *area,
		   GtkWidget     *widget,
		   gint           x,
		   gint           y,
		   gint           width,
		   gint           height,
		   GtkOrientation orientation)
{
  MetalStyle *metal_style = METAL_STYLE (style);
  GdkPixmap *pm;
  GdkGC *fillgc;
  GdkGCValues values;
  GdkGC *lightgc, *darkgc, *whitegc;

  /* Get colors */
  if (state_type == GTK_STATE_PRELIGHT)
    {
      lightgc = style->bg_gc[GTK_STATE_PRELIGHT];
      darkgc = style->fg_gc[GTK_STATE_PRELIGHT];
      whitegc = style->white_gc;
    }
  else
    {
      lightgc = metal_style->light_gray_gc;
      darkgc = metal_style->dark_gray_gc;
      whitegc = style->white_gc;
    }

  /* Draw textured surface */
  pm = gdk_pixmap_new (window, 4, 4, -1);

  gdk_draw_rectangle (pm, lightgc, TRUE, 0, 0, 4, 4);
  
  gdk_draw_point (pm, whitegc, 0, 0);
  gdk_draw_point (pm, darkgc, 1, 1);
  gdk_draw_point (pm, whitegc, 2, 2);
  gdk_draw_point (pm, darkgc, 3, 3);

  values.fill = GDK_TILED;
  values.ts_x_origin = x + 2;
  values.ts_y_origin = y + 2;
  fillgc = gdk_gc_new_with_values (window, &values,
				   GDK_GC_FILL | GDK_GC_TS_X_ORIGIN | GDK_GC_TS_Y_ORIGIN);

  if (area)
    gdk_gc_set_clip_rectangle (fillgc, area);
  gdk_gc_set_tile (fillgc, pm);

  gdk_draw_rectangle (window, fillgc, TRUE, x + 2, y + 2, width - 4, height - 4);

  gdk_gc_unref (fillgc);
  gdk_pixmap_unref (pm);
}
/**************************************************************************/
void
metal_draw_handle (GtkStyle      *style,
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
  MetalStyle *metal_style = METAL_STYLE (style);
  GdkPixmap *pm;
  GdkGC *fillgc;
  GdkGCValues values;
  GdkGC *lightgc, *midgc, *darkgc, *whitegc, *blackgc;

  g_return_if_fail(sanitize_parameters(style, window, &width, &height));

  if (CHECK_DETAIL(detail, "paned"))
    {
      metal_draw_paned_handle (style, window, state_type, shadow_type,
			 area, widget, x, y, width, height,
			 orientation);
      return;
    }
    
  /* Get colors */
  if (state_type == GTK_STATE_PRELIGHT)
    {
      lightgc = style->bg_gc[GTK_STATE_PRELIGHT];
      midgc = style->bg_gc[GTK_STATE_SELECTED];
      darkgc = style->fg_gc[GTK_STATE_PRELIGHT];
      whitegc = style->white_gc;
      blackgc = style->black_gc;
    }
  else
    {
      lightgc = metal_style->light_gray_gc;
      midgc = metal_style->mid_gray_gc;
      darkgc = metal_style->mid_gray_gc;
      whitegc = style->white_gc;
      blackgc = style->black_gc;
    }

  /* Draw textured surface */
  pm = gdk_pixmap_new (window, 8, 3, -1);

  gdk_draw_rectangle (pm, lightgc, TRUE, 0, 0, 8, 3);
  gdk_draw_point (pm, whitegc, 3, 0);
  gdk_draw_point (pm, whitegc, 0, 1);
  gdk_draw_point (pm, blackgc, 4, 1);
  gdk_draw_point (pm, blackgc, 1, 2);

  values.fill = GDK_TILED;
  values.ts_x_origin = x + 2;	/*5; */
  values.ts_y_origin = y + 2;	/*3; */
  fillgc = gdk_gc_new_with_values (window, &values,
				   GDK_GC_FILL | GDK_GC_TS_X_ORIGIN | GDK_GC_TS_Y_ORIGIN);

  /* Set Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, area);
      gdk_gc_set_clip_rectangle (midgc, area);
      gdk_gc_set_clip_rectangle (darkgc, area);
      gdk_gc_set_clip_rectangle (whitegc, area);
      gdk_gc_set_clip_rectangle (blackgc, area);
    }

  /* Draw backgound */
  gdk_draw_rectangle (window, lightgc, TRUE, x, y, width, height);

  /* Draw border */
  gdk_draw_rectangle (window, whitegc, FALSE, x + 1, y + 1, width - 2,
		      height - 2);
  gdk_draw_rectangle (window, darkgc, FALSE, x + 0, y + 0, width - 2, height
		      - 2);

  if (area)
    gdk_gc_set_clip_rectangle (fillgc, area);
  gdk_gc_set_tile (fillgc, pm);
  gdk_draw_rectangle (window, fillgc, TRUE, x + 2, y + 2, width - 4, height
		      - 4);

  gdk_gc_unref (fillgc);
  gdk_pixmap_unref (pm);

  /* Reset Clip Region */
  if (area)
    {
      gdk_gc_set_clip_rectangle (lightgc, NULL);
      gdk_gc_set_clip_rectangle (midgc, NULL);
      gdk_gc_set_clip_rectangle (darkgc, NULL);
      gdk_gc_set_clip_rectangle (whitegc, NULL);
      gdk_gc_set_clip_rectangle (blackgc, NULL);
    }
}
