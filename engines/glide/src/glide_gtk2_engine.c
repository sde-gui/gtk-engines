/* Glide theme engine
 * Copyright (C) 2006 Andrew Johnson
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
 * Written by Andrew Johnson <acjgenius@earthlink.net>
 */ 
 
#include "glide_gtk2_engine.h"
#include "glide_gtk2_support.h"
#include "glide_gtk2_drawing.h"
enum
  {
    TOKEN_PROFILE = G_TOKEN_LAST + 1,
  };

static struct
  {
    gchar       *name;
    guint        token;
  }
theme_symbols[] =
{
  { "profile",      TOKEN_PROFILE }
};
 
/**********************************/ 
/* Register & Initialize RC Style */ 
/**********************************/ 
GType glide_type_rc_style = 0;
 
/* Create an empty style suitable to this RC style */ 
static GtkStyle *
glide_rc_style_create_style (GtkRcStyle * rc_style)
{
  return GTK_STYLE (g_object_new (GLIDE_TYPE_STYLE, NULL));;
}
 
static void
glide_rc_style_class_init (GlideRcStyleClass * klass)
{
  GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);
 
  rc_style_class->create_style = glide_rc_style_create_style;
}
 
void
glide_rc_style_register_type (GTypeModule * module)
{
  static const GTypeInfo object_info = {
    sizeof (GlideRcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) glide_rc_style_class_init,
    NULL,			/* class_finalize */
    NULL,			/* class_data */
    sizeof (GlideRcStyle),
    0,				/* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
 
  glide_type_rc_style = g_type_module_register_type (module,
						       GTK_TYPE_RC_STYLE,
						       "GlideRcStyle",
						       &object_info, 0);
}
 
/***************************************/ 
/* Register & Initialize Drawing Style */ 
/***************************************/ 
GType glide_type_style = 0;
GtkStyleClass *glide_parent_class;

static glide_simple_color_pattern(CairoColor *base, CairoPattern *pattern)
{	
	#if  ((CAIRO_VERSION_MAJOR < 1) || ((CAIRO_VERSION_MAJOR == 1) && (CAIRO_VERSION_MINOR < 2)))
		pattern->type = CAIRO_PATTERN_TYPE_SOLID;
	#endif

	pattern->scale = GLIDE_DIRECTION_NONE;
	pattern->translate = GLIDE_DIRECTION_NONE;

	pattern->handle = cairo_pattern_create_rgba(base->r, base->g, base->b, base->a);
}

static glide_simple_pixmap_pattern(GdkPixmap *pixmap, CairoPattern *pattern)
{	
	#if  ((CAIRO_VERSION_MAJOR < 1) || ((CAIRO_VERSION_MAJOR == 1) && (CAIRO_VERSION_MINOR < 2)))
		pattern->type = CAIRO_PATTERN_TYPE_SURFACE;
	#endif

	pattern->scale = GLIDE_DIRECTION_NONE;
	pattern->translate = GLIDE_DIRECTION_BOTH;

	cairo_t *cr;
	cairo_surface_t * surface;
	GdkPixbuf * pixbuf;
	gint width, height;

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
glide_simple_linear_shade_gradient_pattern(CairoColor *base, gdouble shade1, gdouble shade2, gboolean vertical, CairoPattern *pattern)
{
	#if  ((CAIRO_VERSION_MAJOR < 1) || ((CAIRO_VERSION_MAJOR == 1) && (CAIRO_VERSION_MINOR < 2)))
		pattern->type = CAIRO_PATTERN_TYPE_LINEAR;
	#endif

	if (vertical)
	{
		pattern->scale = GLIDE_DIRECTION_VERTICAL;

		pattern->handle = cairo_pattern_create_linear(0, 0, 1, 0);
	}
	else
	{
		pattern->scale = GLIDE_DIRECTION_HORIZONTAL;

		pattern->handle = cairo_pattern_create_linear(0, 0, 0, 1);
	}

	pattern->translate = GLIDE_DIRECTION_BOTH;

	ge_cairo_pattern_add_shade_color_stop(pattern->handle, 0, base, shade1);
	ge_cairo_pattern_add_shade_color_stop(pattern->handle, 1, base, shade2);
}
 
static void
glide_style_realize (GtkStyle * style)
{
	GlideStyle *glide_style = GLIDE_STYLE (style);
	int i;
 
	glide_parent_class->realize (style);
 
	ge_gtk_style_to_cairo_color_cube (style, &glide_style->color_cube);

	for (i = 0; i < 5; i++)
	{
		CairoColor base = glide_style->color_cube.bg[i];
		
		/* Use Smooth's alternate shades for Light/Dark/Mid */
		ge_shade_color(&base, 0.666667, &glide_style->color_cube.dark[i]);
		ge_shade_color(&base, 1.2, &glide_style->color_cube.light[i]);
		ge_blend_color(&glide_style->color_cube.dark[i], &glide_style->color_cube.light[i], &glide_style->color_cube.mid[i]);

		glide_simple_color_pattern(&base, &glide_style->bg_solid[i]);

		glide_style->bg_pixmap[i].handle = NULL;
		if (style->bg_pixmap[i])
		{
			glide_simple_pixmap_pattern(style->bg_pixmap[i], &glide_style->bg_pixmap[i]);
		}

		glide_simple_linear_shade_gradient_pattern(&base, 1.05, 0.95, FALSE, 
				&glide_style->bg_gradient[0][i]);
		glide_simple_linear_shade_gradient_pattern(&base, 1.05, 0.95, TRUE, 
				&glide_style->bg_gradient[1][i]);

		glide_simple_linear_shade_gradient_pattern(&base, 1.0, 0.95, TRUE, 
				&glide_style->active_tab_gradient[GTK_POS_LEFT][i]);
		glide_simple_linear_shade_gradient_pattern(&base, 1.05, 1.0, TRUE, 
				&glide_style->active_tab_gradient[GTK_POS_RIGHT][i]);
		glide_simple_linear_shade_gradient_pattern(&base, 1.0, 0.95, FALSE, 
				&glide_style->active_tab_gradient[GTK_POS_TOP][i]);
		glide_simple_linear_shade_gradient_pattern(&base, 1.05, 1.0, FALSE, 
				&glide_style->active_tab_gradient[GTK_POS_BOTTOM][i]);
	}
}

static void
glide_style_unrealize (GtkStyle * style)
{
	GlideStyle *glide_style = GLIDE_STYLE (style);
	int i;
 
	for (i = 0; i < 5; i++)
	{
		cairo_pattern_destroy(glide_style->bg_solid[i].handle);

		if (glide_style->bg_pixmap[i].handle)
		{
			cairo_pattern_destroy(glide_style->bg_pixmap[i].handle);
		}

		cairo_pattern_destroy(glide_style->bg_gradient[FALSE][i].handle);
		cairo_pattern_destroy(glide_style->bg_gradient[TRUE][i].handle);

		cairo_pattern_destroy(glide_style->active_tab_gradient[GTK_POS_LEFT][i].handle);
		cairo_pattern_destroy(glide_style->active_tab_gradient[GTK_POS_RIGHT][i].handle);
		cairo_pattern_destroy(glide_style->active_tab_gradient[GTK_POS_TOP][i].handle);
		cairo_pattern_destroy(glide_style->active_tab_gradient[GTK_POS_BOTTOM][i].handle);
	}
 
	glide_parent_class->unrealize (style);
}
 
static void
glide_style_class_init (GlideStyleClass * klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);
 
  glide_parent_class = g_type_class_peek_parent (klass);
 
  style_class->realize = glide_style_realize;
  style_class->unrealize = glide_style_unrealize;
 
  style_class->draw_hline = glide_draw_hline;
  style_class->draw_vline = glide_draw_vline;
  style_class->draw_arrow = glide_draw_arrow;
  style_class->draw_box = glide_draw_box;
  style_class->draw_check = glide_draw_check;
  style_class->draw_option = glide_draw_option;
  style_class->draw_tab = glide_draw_tab;
  style_class->draw_slider = glide_draw_slider;
  style_class->draw_shadow = glide_draw_shadow;
  style_class->draw_shadow_gap = glide_draw_shadow_gap;
  style_class->draw_box_gap = glide_draw_box_gap;
  style_class->draw_extension = glide_draw_extension;
  style_class->draw_handle = glide_draw_handle;
  style_class->draw_focus = glide_draw_focus;
  style_class->draw_layout = glide_draw_layout;
}
  
static void
glide_style_register_type (GTypeModule * module)
{
  static const GTypeInfo object_info = {
    sizeof (GlideStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) glide_style_class_init,
    NULL,			/* class_finalize */
    NULL,			/* class_data */
    sizeof (GlideStyle),
    0,				/* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
 
  glide_type_style = g_type_module_register_type (module,
						    GTK_TYPE_STYLE,
						    "GlideStyle",
						    &object_info, 0);
}
 
/****************/ 
/* Engine Hooks */ 
/****************/ 
G_MODULE_EXPORT void
theme_init (GTypeModule * module)
{
  glide_rc_style_register_type (module);
  glide_style_register_type (module);
}
 
G_MODULE_EXPORT void
theme_exit (void)
{
}
 
G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  void *ptr;
  ptr = GTK_RC_STYLE (g_object_new (GLIDE_TYPE_RC_STYLE, NULL));  
  return (GtkRcStyle *)ptr;
}
