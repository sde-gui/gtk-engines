/* Metal theme engine
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
 */

#include "metal_gtk2_engine.h"
#include "metal_gtk2_drawing.h"
#include "metal_gtk2_misc.h"
 
/**********************************/ 
/* Register & Initialize RC Style */ 
/**********************************/ 
GType metal_type_rc_style = 0;
 
/* Create an empty style suitable to this RC style */ 
static GtkStyle *
metal_rc_style_create_style (GtkRcStyle * rc_style)
{
  return GTK_STYLE (g_object_new (METAL_TYPE_STYLE, NULL));;
}
 
static void
metal_rc_style_class_init (MetalRcStyleClass * klass)
{
  GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);
 
  rc_style_class->create_style = metal_rc_style_create_style;
}
 
void
metal_rc_style_register_type (GTypeModule * module)
{
  static const GTypeInfo object_info = {
    sizeof (MetalRcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) metal_rc_style_class_init,
    NULL,			/* class_finalize */
    NULL,			/* class_data */
    sizeof (MetalRcStyle),
    0,				/* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
 
  metal_type_rc_style = g_type_module_register_type (module,
						       GTK_TYPE_RC_STYLE,
						       "MetalRcStyle",
						       &object_info, 0);
}
 
/***************************************/ 
/* Register & Initialize Drawing Style */ 
/***************************************/ 
GType metal_type_style = 0;
GtkStyleClass *parent_class;
 
static void
shade (GdkColor * oldcolor, GdkColor * newcolor, float mult)
{
  newcolor->red = oldcolor->red * mult;
  newcolor->green = oldcolor->green * mult;
  newcolor->blue = oldcolor->blue * mult;
}

static void
metal_style_init_from_rc (GtkStyle * style,
			  GtkRcStyle * rc_style)
{
  MetalStyle *metal_style = METAL_STYLE (style);

  parent_class->init_from_rc (style, rc_style);

  /* Light Gray */
  shade (&style->white, &metal_style->light_gray, 0.8);
  shade (&style->white, &metal_style->mid_gray, 0.6);
  shade (&style->white, &metal_style->dark_gray, 0.4);
}

static GdkGC *
realize_color (GtkStyle * style,
	       GdkColor * color)
{
  GdkGCValues gc_values;

  gdk_colormap_alloc_color (style->colormap, color,
			    FALSE, TRUE);

  gc_values.foreground = *color;

  return gtk_gc_get (style->depth, style->colormap,
		     &gc_values, GDK_GC_FOREGROUND);
}

static void
metal_style_realize (GtkStyle * style)
{
  MetalStyle *metal_style = METAL_STYLE (style);

  parent_class->realize (style);

  metal_style->light_gray_gc = realize_color (style, &metal_style->light_gray);
  metal_style->mid_gray_gc = realize_color (style, &metal_style->mid_gray);
  metal_style->dark_gray_gc = realize_color (style, &metal_style->dark_gray);
}

static void
metal_style_unrealize (GtkStyle * style)
{
  MetalStyle *metal_style = METAL_STYLE (style);

  /* We don't free the colors, because we don't know if
   * gtk_gc_release() actually freed the GC. FIXME - need
   * a way of ref'ing colors explicitely so GtkGC can
   * handle things properly.
   */
  gtk_gc_release (metal_style->light_gray_gc);
  gtk_gc_release (metal_style->mid_gray_gc);
  gtk_gc_release (metal_style->dark_gray_gc);

  parent_class->unrealize (style);
}
 
static void
metal_style_class_init (MetalStyleClass * klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);
 
  parent_class = g_type_class_peek_parent (klass);
 
  style_class->realize = metal_style_realize;
  style_class->unrealize = metal_style_unrealize;
  style_class->init_from_rc = metal_style_init_from_rc;

  style_class->draw_hline = metal_draw_hline;
  style_class->draw_vline = metal_draw_vline;
  style_class->draw_shadow = metal_draw_shadow;
  style_class->draw_polygon = metal_draw_polygon;
  style_class->draw_arrow = metal_draw_arrow;
  style_class->draw_diamond = metal_draw_diamond;
  style_class->draw_string = metal_draw_string;
  style_class->draw_box = metal_draw_box;
  style_class->draw_check = metal_draw_check;
  style_class->draw_option = metal_draw_option;
  style_class->draw_tab = metal_draw_tab;
  style_class->draw_shadow_gap = metal_draw_shadow_gap;
  style_class->draw_box_gap = metal_draw_box_gap;
  style_class->draw_extension = metal_draw_extension;
  style_class->draw_focus = metal_draw_focus;
  style_class->draw_slider = metal_draw_slider;
  style_class->draw_handle = metal_draw_handle;
}
  
void
metal_style_register_type (GTypeModule * module)
{
  static const GTypeInfo object_info = {
    sizeof (MetalStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) metal_style_class_init,
    NULL,			/* class_finalize */
    NULL,			/* class_data */
    sizeof (MetalStyle),
    0,				/* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
 
  metal_type_style = g_type_module_register_type (module,
						    GTK_TYPE_STYLE,
						    "MetalStyle",
						    &object_info, 0);
}
 
/****************/ 
/* Engine Hooks */ 
/****************/ 
G_MODULE_EXPORT void
theme_init (GTypeModule * module)
{
  metal_rc_style_register_type (module);
  metal_style_register_type (module);
}
 
G_MODULE_EXPORT void
theme_exit (void)
{
}
 
G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  void *ptr;
  ptr = GTK_RC_STYLE (g_object_new (METAL_TYPE_RC_STYLE, NULL));  
  return (GtkRcStyle *)ptr;
}
