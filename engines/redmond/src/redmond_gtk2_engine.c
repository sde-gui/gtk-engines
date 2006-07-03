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
 
void
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
 
static void
redmond_style_realize (GtkStyle * style)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  int i;
 
  parent_class->realize (style);
 
  ge_gtk_style_to_cairo_color_cube (style, &redmond_style->color_cube);

  for (i = 0; i < 5; i++)
    { 
      GdkGCValues gc_values;
 
      ge_shade_color(&(redmond_style->color_cube.bg[i]), 0.3, &(redmond_style->black_border[i]));

      ge_cairo_color_to_gtk (&redmond_style->black_border[i], &gc_values.foreground);
      gdk_colormap_alloc_color (style->colormap, &gc_values.foreground, FALSE, TRUE);
 
      redmond_style->black_border_gc[i] =
	gtk_gc_get (style->depth, style->colormap, &gc_values,
		    GDK_GC_FOREGROUND);
    }
}
 
static void
redmond_style_unrealize (GtkStyle * style)
{
  RedmondStyle *redmond_style = REDMOND_STYLE (style);
  int i;
 
  for (i = 0; i < 5; i++)
    gtk_gc_release (redmond_style->black_border_gc[i]);
 
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
