/******************************************************************************/
/* smooth_gtk2_engine.c - Style Functions and Engine Hooks for GTK2 Engine    */
/******************************************************************************/
/* Smooth Theme Engine                                                        */
/* Copyright (C) 2002-2004 Andrew Johnson                                     */
/*                                                                            */
/* This library is free software; you can redistribute it and/or              */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 2.1 of the License, or (at your option) any later version.         */
/*                                                                            */
/* This library is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with this library; if not, write to the Free Software        */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  */
/*                                                                            */
/* Author(s): Andrew Johnson <ajgenius@ajgenius.us>                           */
/******************************************************************************/
/* Portions Based on GTK+                                                     */
/*   Peter Mattis <petm@xcf.berkeley.edu>                                     */
/*   Spencer Kimball <spencer@xcf.berkeley.edu>                               */
/*   Josh MacDonald <jmacd@xcf.berkeley.edu>                                  */
/*                                                                            */
/* Portions Based on the EnGradient Engine                                    */
/*   Andrew Cattau                                                            */
/******************************************************************************/
#include "smooth_gtk2_engine.h"
#include "smooth_gtk2_drawing.h"
#include "smooth_gtk2_misc.h"
#include "smooth_gtk2_rc.h"

static gboolean smooth_finalized = FALSE;

/**********************************/
/* Register & Initialize RC Style */
/**********************************/
static GtkRcStyleClass *parent_rc_class;
GType smooth_type_rc_style = 0;
gint smooth_rc_references = 0;

static void
smooth_rc_style_merge (GtkRcStyle * dest,
                       GtkRcStyle * src)
{
	if (SMOOTH_IS_RC_STYLE (src)) 
	{
		SmoothRcStyleData *src_data = SMOOTH_RC_DATA (src);
		SmoothRcStyleData *dest_data = SMOOTH_RC_DATA (dest);

		smooth_gtkrc_style_merge (dest_data, src_data);
	}

	parent_rc_class->merge (dest, src);  
}

static guint 
smooth_rc_style_parse (GtkRcStyle *rc_style,
                       GtkSettings  *settings,
		       GScanner   *scanner)
{
  static GQuark scope_id = 0;
  SmoothRcStyle *smooth_style = SMOOTH_RC_STYLE(rc_style);
  
  guint old_scope;
  guint token;
  guint i;

  /* Set up a new scope in this scanner */
  if (!scope_id)
    {
      scope_id = g_quark_from_string  ("smooth_theme_engine");
    }

  /* If we bail out due to errors, we don't reset the scope, so the
   * error messaging code can make sense of our tokens. */
  old_scope = g_scanner_set_scope (scanner, scope_id);

  /* Check if we already added our symbols to this scope */
  if (!g_scanner_lookup_symbol (scanner, theme_symbols[0].name))
    {
      for (i = 0; i < n_theme_symbols; i++)
	{
	  g_scanner_scope_add_symbol (scanner, scope_id, theme_symbols[i].name, 
				      GINT_TO_POINTER (theme_symbols[i].token));
	}
    }

  /* Read to go, now parse the top level */
  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY)
    {
      token = smooth_gtkrc_parse(scanner, settings, smooth_style, token);

      if (token != G_TOKEN_NONE)
	{
          g_free (smooth_style);
	  return token;
	}

      token = g_scanner_peek_next_token (scanner);
    }

  smooth_gtkrc_ensure_arrows(&SMOOTH_RC_DATA(smooth_style)->arrow);

  g_scanner_get_next_token(scanner);
      
  g_scanner_set_scope (scanner, old_scope);

  return G_TOKEN_NONE;
}

static void part_finalize (smooth_part_style *part)
{
  gint i;
  for (i=0; i < 5; i++) {
    if (part->fill.file_name[i])
            g_string_free(part->fill.file_name[i], TRUE);
  }
}

static void smooth_rc_style_real_dispose (GObject *rc_style)
{
  gint i;

  if (SMOOTH_IS_RC_STYLE (rc_style)) 
    {     
      SmoothRcStyleData *data = SMOOTH_RC_DATA(rc_style);

      if (data) {  
        for (i=0; i < 5; i++) {
          if (data->fill.file_name[i])
            g_string_free(data->fill.file_name[i], TRUE);

          if (data->focus.pattern[i])
            g_free(data->focus.pattern[i]);
        }
        part_finalize(THEME_PART(&data->grip));
        part_finalize(THEME_PART(&data->check));
        part_finalize(THEME_PART(&data->option));
        part_finalize(THEME_PART(&data->trough));
        part_finalize(&data->progress);
        part_finalize(THEME_PART(&data->button));
        part_finalize(&data->button.button_default);
        part_finalize(THEME_PART(&data->tabs));
        part_finalize(&data->tabs.active_tab);
        
	SmoothFreeArrowStyles(&data->arrow);

        GDKFinalizeColorCube(&data->colors);

        g_free(data);
        
        SMOOTH_RC_STYLE(rc_style)->engine_data = NULL;
        
        smooth_rc_references -= 1;
      }
  }
}

static void smooth_rc_style_dispose (GObject *rc_style)
{
  smooth_rc_style_real_dispose(rc_style);
  G_OBJECT_CLASS(parent_rc_class)->dispose(G_OBJECT(rc_style));
}

gboolean smooth_rc_data_unref(GObject *rc_style)
{  
  if (SMOOTH_IS_RC_STYLE (rc_style)) 
    {     
	gboolean do_unref = FALSE;

    	if (SMOOTH_RC_STYLE(rc_style)->quit_handler_id)
    	{
	      	gtk_quit_remove(SMOOTH_RC_STYLE(rc_style)->quit_handler_id);
	      	SMOOTH_RC_STYLE(rc_style)->quit_handler_id = 0;
		do_unref = TRUE;
        }
        
        if (SMOOTH_RC_STYLE(rc_style)->engine_data)
        {
		smooth_rc_style_real_dispose(rc_style);
	}

	if (do_unref)
		g_object_unref(rc_style);
    }
  
  if ((smooth_rc_references <= 0) && (!smooth_finalized))
  {
    SmoothDrawingInterfaceFinalize();
    smooth_finalized = TRUE;
  }

  return FALSE;
}

/* Create an empty style suitable to this RC style
 */
static GtkStyle *
smooth_rc_style_create_style (GtkRcStyle *rc_style)
{
	GtkStyle * style = GTK_STYLE(g_object_new (SMOOTH_TYPE_STYLE, NULL));
	
	return style;
}

static void smooth_rc_style_class_init (SmoothRcStyleClass *klass)
{
  GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);

  parent_rc_class = g_type_class_peek_parent (klass);

  G_OBJECT_CLASS(klass)->dispose = smooth_rc_style_dispose;

  rc_style_class->parse        = smooth_rc_style_parse;
  rc_style_class->merge        = smooth_rc_style_merge;
  rc_style_class->create_style = smooth_rc_style_create_style;
}

static void smooth_rc_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (SmoothRcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) smooth_rc_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (SmoothRcStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) smooth_rc_style_init,
  };
  
  smooth_type_rc_style = g_type_module_register_type (module,
							      GTK_TYPE_RC_STYLE,
							      "SmoothRcStyle",
							      &object_info, 0);
}

/***************************************/
/* Register & Initialize Drawing Style */
/***************************************/
GType smooth_type_style = 0;
GtkStyleClass *smooth_theme_parent_class;

static void
smooth_style_class_init (SmoothStyleClass *klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

  smooth_theme_parent_class = g_type_class_peek_parent (klass);

  style_class->draw_hline = smooth_draw_hline;
  style_class->draw_vline = smooth_draw_vline;
  style_class->draw_shadow = smooth_draw_shadow;

  style_class->draw_polygon = smooth_draw_polygon;
  style_class->draw_diamond = smooth_draw_diamond;

  style_class->draw_box = smooth_draw_box;
  style_class->draw_flat_box = smooth_draw_flat_box;
  style_class->draw_check = smooth_draw_check;
  style_class->draw_option = smooth_draw_option;
  style_class->draw_tab = smooth_draw_tab;
  style_class->draw_shadow_gap = smooth_draw_shadow_gap;
  style_class->draw_box_gap = smooth_draw_box_gap;
  style_class->draw_extension = smooth_draw_extension;
  style_class->draw_slider = smooth_draw_slider;
  style_class->draw_handle = smooth_draw_handle;

  style_class->draw_focus = smooth_draw_focus;

  style_class->draw_arrow = smooth_draw_arrow;
  style_class->draw_resize_grip = smooth_draw_resize_grip;
}

static void
smooth_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (SmoothStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) smooth_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (SmoothStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) NULL,
  };
  
  smooth_type_style = g_type_module_register_type (module,
							   GTK_TYPE_STYLE,
							   "SmoothStyle",
							   &object_info, 0);
}

/****************/
/* Engine Hooks */
/****************/

static void
smooth_theme_init (GTypeModule *module)
{
  smooth_rc_style_register_type (module);
  smooth_style_register_type (module);
  SmoothDrawingInterfaceInitialize();
}

G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
	smooth_theme_init (module);
}

static void
smooth_theme_exit (void)
{
  if (!smooth_finalized)
  {
    SmoothDrawingInterfaceFinalize();
    smooth_finalized = TRUE;
  }
}

G_MODULE_EXPORT void
theme_exit (void)
{
	smooth_theme_exit();
}

static GtkRcStyle *
smooth_theme_create_rc_style (void)
{
  return (GtkRcStyle *)g_object_new (SMOOTH_TYPE_RC_STYLE, NULL);
}

G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  return smooth_theme_create_rc_style();
}
