/* Redmond Engine
 * Copyright (C) 2001 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Written by Owen Taylor <otaylor@redhat.com>
 * modified by Andrew Johnson <ajgenius@ajgenius.us>
 */
 
#include "redmond_gtk2_engine.h"
#include "redmond_gtk2_drawing.h"
#include "redmond_gtk2_misc.h"
 
/**********************************/ 
/* Register & Initialize RC Style */ 
/**********************************/ 
GType redmond_type_rc_style = 0;
 
/* Create an empty style suitable to this RC style */ 
static GtkStyle *
redmond_rc_style_create_style (GtkRcStyle * rc_style)
{
  return GTK_STYLE (g_object_new (REDMOND_TYPE_STYLE, NULL));;
}
 
static void
redmond_rc_style_class_init (RedmondRcStyleClass * klass)
{
  GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);
 
  rc_style_class->create_style = redmond_rc_style_create_style;
}
 
static void
redmond_rc_style_register_type (GTypeModule * module)
{
  static const GTypeInfo object_info = {
    sizeof (RedmondRcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) redmond_rc_style_class_init,
    NULL,			/* class_finalize */
    NULL,			/* class_data */
    sizeof (RedmondRcStyle),
    0,				/* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
 
  redmond_type_rc_style = g_type_module_register_type (module,
						       GTK_TYPE_RC_STYLE,
						       "RedmondRcStyle",
						       &object_info, 0);
}
 
/***************************************/ 
/* Register & Initialize Drawing Style */ 
/***************************************/ 
GType redmond_type_style = 0;
static GtkStyleClass *parent_class;

static void redmond_simple_color_pattern(CairoColor *base, CairoPattern *pattern)
{	
	pattern->scale = REDMOND_DIRECTION_NONE;
	pattern->translate = REDMOND_DIRECTION_NONE;

	pattern->handle = cairo_pattern_create_rgba(base->r, base->g, base->b, base->a);
}

static void redmond_simple_pixmap_pattern(GdkPixmap *pixmap, CairoPattern *pattern)
{	
	cairo_t *cr;
	cairo_surface_t * surface;
	GdkPixbuf * pixbuf;
	gint width, height;

	pattern->scale = REDMOND_DIRECTION_NONE;
	pattern->translate = REDMOND_DIRECTION_BOTH;

	gdk_drawable_get_size (GDK_DRAWABLE (pixmap), &width, &height);

	pixbuf = gdk_pixbuf_get_from_drawable(NULL, GDK_DRAWABLE (pixmap), 
				gdk_drawable_get_colormap(GDK_DRAWABLE (pixmap)), 
				0, 0, 0, 0, width, height);

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

	cr = cairo_create(surface);

	gdk_cairo_set_source_pixbuf (cr, pixbuf, 0, 0);
	cairo_rectangle (cr, 0, 0, width, height);
	cairo_fill (cr);
	cairo_destroy(cr);
	g_object_unref (pixbuf);

	pattern->handle = cairo_pattern_create_for_surface (surface);
	cairo_surface_destroy(surface);

	cairo_pattern_set_extend (pattern->handle, CAIRO_EXTEND_REPEAT);
}

static void
redmond_simple_hatch_mask_pattern(CairoPattern *pattern)
{
	cairo_t * cr;
	cairo_surface_t *surface;

	pattern->scale = REDMOND_DIRECTION_NONE;
	pattern->translate = REDMOND_DIRECTION_NONE;

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 2, 2);

	cr = cairo_create(surface);

	cairo_set_source_rgba(cr, 0, 0, 0, 0);
	cairo_rectangle (cr, 0, 0, 2, 2);
	cairo_fill (cr);

	cairo_set_source_rgba(cr, 1, 1, 1, 1);
	cairo_rectangle (cr, 1, 0, 1, 1);
	cairo_fill (cr);

	cairo_rectangle (cr, 0, 1, 1, 1);
	cairo_fill (cr);

	cairo_destroy(cr);

	pattern->handle = cairo_pattern_create_for_surface (surface);
	cairo_surface_destroy(surface);

	cairo_pattern_set_extend (pattern->handle, CAIRO_EXTEND_REPEAT);
}

static void
redmond_style_realize (GtkStyle * style)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  int i;
 
  parent_class->realize (style);
 
  ge_gtk_style_to_cairo_color_cube (style, &redmond_style->color_cube);

  redmond_simple_hatch_mask_pattern(&redmond_style->hatch_mask);

  for (i = 0; i < 5; i++)
    { 
      ge_shade_color(&(redmond_style->color_cube.bg[i]), 0.3, &(redmond_style->black_border[i]));

      redmond_simple_color_pattern(&(redmond_style->color_cube.bg[i]), &redmond_style->bg_color[i]);

      redmond_style->bg_image[i].handle = NULL;

      if ((style->bg_pixmap[i]) && (style->bg_pixmap[i] != (GdkPixmap*) GDK_PARENT_RELATIVE))
      {
        redmond_simple_pixmap_pattern(style->bg_pixmap[i], &redmond_style->bg_image[i]);
      }
    }
}
 
static void
redmond_style_unrealize (GtkStyle * style)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  int i;

  cairo_pattern_destroy(redmond_style->hatch_mask.handle);

	for (i = 0; i < 5; i++)
	{
		cairo_pattern_destroy(redmond_style->bg_color[i].handle);

		if (redmond_style->bg_image[i].handle)
		{
			cairo_pattern_destroy(redmond_style->bg_image[i].handle);
		}
        }

  parent_class->unrealize (style);
}
 
static void
redmond_style_class_init (RedmondStyleClass * klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);
 
  parent_class = g_type_class_peek_parent (klass);
 
  style_class->realize = redmond_style_realize;
  style_class->unrealize = redmond_style_unrealize;
 
  style_class->draw_hline = redmond_draw_hline;
  style_class->draw_vline = redmond_draw_vline;
  style_class->draw_arrow = redmond_draw_arrow;
  style_class->draw_box = redmond_draw_box;
  style_class->draw_check = redmond_draw_check;
  style_class->draw_option = redmond_draw_option;
  style_class->draw_tab = redmond_draw_tab;
  style_class->draw_slider = redmond_draw_slider;
  style_class->draw_shadow = redmond_draw_shadow;
  style_class->draw_extension = redmond_draw_extension;
  style_class->draw_handle = redmond_draw_handle;
}
  
static void
redmond_style_register_type (GTypeModule * module)
{
  static const GTypeInfo object_info = {
    sizeof (RedmondStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) redmond_style_class_init,
    NULL,			/* class_finalize */
    NULL,			/* class_data */
    sizeof (RedmondStyle),
    0,				/* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
 
  redmond_type_style = g_type_module_register_type (module,
						    GTK_TYPE_STYLE,
						    "RedmondStyle",
						    &object_info, 0);
}
 
/****************/ 
/* Engine Hooks */ 
/****************/ 
G_MODULE_EXPORT void
theme_init (GTypeModule * module)
{
  redmond_rc_style_register_type (module);
  redmond_style_register_type (module);
}
 
G_MODULE_EXPORT void
theme_exit (void)
{
}
 
G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  void *ptr;
  ptr = GTK_RC_STYLE (g_object_new (REDMOND_TYPE_RC_STYLE, NULL));  
  return (GtkRcStyle *)ptr;
}
