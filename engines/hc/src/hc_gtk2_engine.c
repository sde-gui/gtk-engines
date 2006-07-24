/*
 * HighContrast GTK+ rendering engine for Gnome-Themes.
 *
 * Copyright 2003 Sun Microsystems Inc.
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
 */
 
#include "hc_gtk2_engine.h"
#include "hc_gtk2_support.h"
#include "hc_gtk2_drawing.h"

/**********************************/ 
/* Register & Initialize RC Style */ 
/**********************************/ 
GType hc_type_rc_style = 0;

/* Create an empty style suitable to this RC style */ 
static GtkStyle *
hc_rc_style_create_style (GtkRcStyle *rc_style)
{
  return GTK_STYLE (g_object_new (HC_TYPE_STYLE, NULL));;
}

static void hc_rc_style_class_init (HcRcStyleClass *klass)
{
  GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);

  rc_style_class->create_style = hc_rc_style_create_style;
}

void hc_rc_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (HcRcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) hc_rc_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (HcRcStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
  
  hc_type_rc_style = g_type_module_register_type (module,
							      GTK_TYPE_RC_STYLE,
							      "HcRcStyle",
							      &object_info, 0);
}

/***************************************/ 
/* Register & Initialize Drawing Style */ 
/***************************************/ 
GtkStyleClass *hc_parent_class = NULL;

static void
hc_style_realize (GtkStyle *style)
{
  HcStyle *hc_style = HC_STYLE (style);

  GdkGCValues gc_values;
  GdkGCValuesMask gc_values_mask;
  
  gint i;

  for (i = 0; i < 5; i++)
    {
      style->light[i].red = MIN (65535, style->bg[i].red + 0.5 * 65535);
      style->light[i].green = MIN (65535, style->bg[i].green + 0.5 * 65535);
      style->light[i].blue = MIN (65535, style->bg[i].blue + 0.5 * 65535);

      style->dark[i].red = MAX (0, style->bg[i].red - 0.5 * 65535);
      style->dark[i].green = MAX (0, style->bg[i].green - 0.5 * 65535);
      style->dark[i].blue = MAX (0, style->bg[i].blue - 0.5 * 65535);

      style->mid[i].red = (style->light[i].red + style->dark[i].red) / 2;
      style->mid[i].green = (style->light[i].green + style->dark[i].green) / 2;
      style->mid[i].blue = (style->light[i].blue + style->dark[i].blue) / 2;

      style->text_aa[i].red = (style->text[i].red + style->base[i].red) / 2;
      style->text_aa[i].green = (style->text[i].green + style->base[i].green) / 2;
      style->text_aa[i].blue = (style->text[i].blue + style->base[i].blue) / 2;
    }

  style->black.red = 0x0000;
  style->black.green = 0x0000;
  style->black.blue = 0x0000;
  gdk_colormap_alloc_color (style->colormap, &style->black, FALSE, TRUE);

  style->white.red = 0xffff;
  style->white.green = 0xffff;
  style->white.blue = 0xffff;
  gdk_colormap_alloc_color (style->colormap, &style->white, FALSE, TRUE);

  gc_values_mask = GDK_GC_FOREGROUND;
  
  gc_values.foreground = style->black;
  style->black_gc = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
  
  gc_values.foreground = style->white;
  style->white_gc = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
  
  for (i = 0; i < 5; i++)
    {
      if (!gdk_colormap_alloc_color (style->colormap, &style->fg[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->fg[i].red, style->fg[i].green, style->fg[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->bg[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->bg[i].red, style->bg[i].green, style->bg[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->light[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->light[i].red, style->light[i].green, style->light[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->dark[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->dark[i].red, style->dark[i].green, style->dark[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->mid[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->mid[i].red, style->mid[i].green, style->mid[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->text[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->text[i].red, style->text[i].green, style->text[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->base[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->base[i].red, style->base[i].green, style->base[i].blue);
      if (!gdk_colormap_alloc_color (style->colormap, &style->text_aa[i], FALSE, TRUE))
        g_warning ("unable to allocate color: ( %d %d %d )",
                   style->text_aa[i].red, style->text_aa[i].green, style->text_aa[i].blue);
      
      gc_values.foreground = style->fg[i];
      style->fg_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->bg[i];
      style->bg_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->light[i];
      style->light_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->dark[i];
      style->dark_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->mid[i];
      style->mid_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->text[i];
      style->text_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
      
      gc_values.foreground = style->base[i];
      style->base_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);

      gc_values.foreground = style->text_aa[i];
      style->text_aa_gc[i] = gtk_gc_get (style->depth, style->colormap, &gc_values, gc_values_mask);
    }

  ge_gtk_style_to_cairo_color_cube (style, &hc_style->color_cube);
}

static void
hc_style_class_init (HcStyleClass *klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

  hc_parent_class = g_type_class_peek_parent (klass);
 
  style_class->realize = hc_style_realize;

  style_class->draw_shadow = hc_draw_shadow;
  style_class->realize = hc_style_realize;
  style_class->draw_check = hc_draw_check;
  style_class->draw_option = hc_draw_option;
  style_class->draw_layout = hc_draw_layout;
  style_class->draw_extension = hc_draw_extension;
  style_class->draw_tab = hc_draw_tab;
  style_class->draw_shadow_gap = hc_draw_shadow_gap;
  style_class->draw_arrow = hc_draw_arrow;
  style_class->draw_flat_box = hc_draw_flat_box;
  style_class->draw_box = hc_draw_box;
  style_class->draw_box_gap = hc_draw_box_gap;
  style_class->draw_handle = hc_draw_handle;  
  style_class->draw_slider = hc_draw_slider;  
  style_class->draw_hline = hc_draw_hline; 
  style_class->draw_vline = hc_draw_vline; 
  style_class->draw_diamond = hc_draw_diamond;
  style_class->draw_polygon = hc_draw_polygon;
}

GType hc_type_style = 0;

void
hc_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (HcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) hc_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (HcStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
  
  hc_type_style = g_type_module_register_type (module,
							   GTK_TYPE_STYLE,
							   "HcStyle",
							   &object_info, 0);
}

/****************/ 
/* Engine Hooks */ 
/****************/ 
G_MODULE_EXPORT void
theme_init (GTypeModule * module)
{
  hc_rc_style_register_type (module);
  hc_style_register_type (module);
}
 
G_MODULE_EXPORT void
theme_exit (void)
{
}
 
G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  return GTK_RC_STYLE (g_object_new (HC_TYPE_RC_STYLE, NULL));
}
