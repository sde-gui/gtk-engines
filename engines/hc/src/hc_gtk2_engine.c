/* High Contrast - a cairo based GTK+ engine
 * Copyright (C) 2003 Sun Microsystems Inc.
 * Copyright (C) 2006 Andrew Johnson <acjgenius@earthlink.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Project contact: <gnome-themes-list@gnome.org>
 *
 */
 
 
#include "hc_gtk2_engine.h"
#include "hc_gtk2_support.h"
#include "hc_gtk2_drawing.h"


/**********************************/ 
/* Parse RC Style                 */ 
/**********************************/ 
enum {
	TOKEN_EDGE_THICKNESS = G_TOKEN_LAST + 1,
	TOKEN_CELL_INDICATOR_SIZE
};

static struct {
	const gchar *name;
	guint token;
} hc_rc_symbols[] = {
	{ "edge_thickness",		TOKEN_EDGE_THICKNESS },
	{ "cell_indicator_size", TOKEN_CELL_INDICATOR_SIZE }
};

static guint 
hc_rc_parse_int (GScanner *scanner, 
                 GTokenType wanted_token, 
		 guint return_default,
		 gint *retval, 
		 gint lower_limit, 
		 gint upper_limit)
{
  guint token;
  gboolean negate=FALSE;
  
  token = g_scanner_cur_token (scanner);
  if (token != wanted_token)
  {
	  token = g_scanner_get_next_token (scanner);
	  if (token != wanted_token)
	    {
	      return wanted_token;
	    }
  }

  if (token != G_TOKEN_EQUAL_SIGN)
  {
    token = g_scanner_get_next_token (scanner);
    if (token != G_TOKEN_EQUAL_SIGN)
      {
        return G_TOKEN_EQUAL_SIGN;
      }
  }
  
  if (g_scanner_peek_next_token (scanner) == '-')
    {
      g_scanner_get_next_token (scanner); /* eat sign */
      negate = TRUE;
    }

  token = g_scanner_get_next_token (scanner);

  if (token != G_TOKEN_INT)
    *retval = return_default;
  else
    *retval = (guint) scanner->value.v_int;

  if (negate) *retval = -(*retval); 

  if (*retval < lower_limit) *retval = lower_limit; 
  if ((*retval > upper_limit) && (upper_limit > lower_limit)) *retval = upper_limit; 
  
  return G_TOKEN_NONE;
}

static guint
hc_rc_style_parse (GtkRcStyle  *rc_style,
			   GtkSettings *settings,
			   GScanner    *scanner)
{
	HcRcStyle *hc_rc_style = HC_RC_STYLE (rc_style);

	static GQuark scope_id = 0;
	guint old_scope;
	guint token;
	guint i;

	/* Set up a new scope in this scanner. */

	if (!scope_id)
		scope_id = g_quark_from_string ("hc_theme_engine");

	/* If we bail out due to errors, we *don't* reset the scope, so the
	 * error messaging code can make sense of our tokens.
	 */
	old_scope = g_scanner_set_scope (scanner, scope_id);

	if (!g_scanner_lookup_symbol (scanner, hc_rc_symbols[0].name)) {
		for (i = 0; i < G_N_ELEMENTS (hc_rc_symbols); i++)
			g_scanner_scope_add_symbol (scanner, scope_id, hc_rc_symbols[i].name,
						    GINT_TO_POINTER (hc_rc_symbols[i].token));
	}

	token = g_scanner_peek_next_token (scanner);
	while (token != G_TOKEN_RIGHT_CURLY) {
		switch (token) {
		case TOKEN_EDGE_THICKNESS:
			token = hc_rc_parse_int (scanner, TOKEN_EDGE_THICKNESS, 2, &hc_rc_style->edge_thickness, 1, 25);
			hc_rc_style->flags |= HC_RC_FLAG_EDGE_THICKNESS;
			break;
		case TOKEN_CELL_INDICATOR_SIZE:
			token = hc_rc_parse_int (scanner, TOKEN_CELL_INDICATOR_SIZE, 12, &hc_rc_style->cell_indicator_size, 1, 100);
			hc_rc_style->flags |= HC_RC_FLAG_CELL_INDICATOR_SIZE;
			break;		
		default:
			g_scanner_get_next_token (scanner);
			token = G_TOKEN_RIGHT_CURLY;
			break;
		}

		if (token != G_TOKEN_NONE)
			return token;

		token = g_scanner_peek_next_token (scanner);
	}

	g_scanner_get_next_token (scanner);

	g_scanner_set_scope (scanner, old_scope);

	return G_TOKEN_NONE;
}

/**********************************/ 
/* Register & Initialize RC Style */ 
/**********************************/ 


G_DEFINE_DYNAMIC_TYPE (HcRcStyle, hc_rc_style, GTK_TYPE_RC_STYLE)

/* Create an empty style suitable to this RC style */ 
static GtkStyle *
hc_rc_style_create_style (GtkRcStyle *rc_style)
{
  return GTK_STYLE (g_object_new (HC_TYPE_STYLE, NULL));;
}

static void
hc_rc_style_merge (GtkRcStyle *dest,
			   GtkRcStyle *src)
{
	HcRcFlags flags;
	HcRcStyle *dest_w, *src_w;

	GTK_RC_STYLE_CLASS (hc_rc_style_parent_class)->merge (dest, src);

	if (!HC_IS_RC_STYLE (src))
		return;

	src_w = HC_RC_STYLE (src);
	dest_w = HC_RC_STYLE (dest);

	flags = (~dest_w->flags) & src_w->flags;

	if (flags & HC_RC_FLAG_EDGE_THICKNESS)
		dest_w->edge_thickness = src_w->edge_thickness;
	if (flags & HC_RC_FLAG_CELL_INDICATOR_SIZE)
		dest_w->cell_indicator_size = src_w->cell_indicator_size;

	dest_w->flags = dest_w->flags | src_w->flags;
}

static void hc_rc_style_class_init (HcRcStyleClass *klass)
{
  GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);

  rc_style_class->create_style = hc_rc_style_create_style;
  rc_style_class->parse = hc_rc_style_parse;
  rc_style_class->merge = hc_rc_style_merge;
}

static void
hc_rc_style_init (HcRcStyle *hc_rc_style)
{
	hc_rc_style->flags = 0;
	hc_rc_style->edge_thickness = -1;
	hc_rc_style->cell_indicator_size = -1;
}

static void
hc_rc_style_class_finalize (HcRcStyleClass *klass)
{
}

/***************************************/ 
/* Register & Initialize Drawing Style */ 
/***************************************/ 

G_DEFINE_DYNAMIC_TYPE (HcStyle, hc_style, GTK_TYPE_STYLE)

static void
hc_style_realize (GtkStyle *style)
{
  HcStyle *hc_style = HC_STYLE (style);
  gint i;

  GTK_STYLE_CLASS (hc_style_parent_class)->realize (style);

  ge_gtk_style_to_cairo_color_cube (style, &hc_style->color_cube);
}

static void
hc_style_copy (GtkStyle * style, GtkStyle * src)
{
	HcStyle *hc_style = HC_STYLE (style);
	HcStyle *hc_src = HC_STYLE (src);

	hc_style->edge_thickness = hc_src->edge_thickness;
	hc_style->cell_indicator_size = hc_src->cell_indicator_size;

	GTK_STYLE_CLASS (hc_style_parent_class)->copy (style, src);
}

static void
hc_style_init_from_rc (GtkStyle * style, GtkRcStyle * rc_style)
{
	HcStyle *hc_style = HC_STYLE (style);

	GTK_STYLE_CLASS (hc_style_parent_class)->init_from_rc (style, rc_style);

	if (HC_RC_STYLE (rc_style)->edge_thickness > 0)
	{
		hc_style->edge_thickness = HC_RC_STYLE (rc_style)->edge_thickness;
	}
	
	if (HC_RC_STYLE (rc_style)->cell_indicator_size > 0)
	{
		hc_style->cell_indicator_size = HC_RC_STYLE (rc_style)->cell_indicator_size;
	}
}

static void
hc_style_class_init (HcStyleClass *klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

  style_class->realize = hc_style_realize;
  style_class->copy = hc_style_copy;
  style_class->init_from_rc = hc_style_init_from_rc;

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
  style_class->draw_expander = hc_draw_expander;
  style_class->draw_diamond = hc_draw_diamond;
}

static void
hc_style_init (HcStyle * style)
{
	style->edge_thickness = floor(MIN(GTK_STYLE(style)->xthickness,GTK_STYLE(style)->ythickness));
	style->cell_indicator_size = 12;
}

static void
hc_style_class_finalize (HcStyleClass *klass)
{
}

/****************/ 
/* Engine Hooks */ 
/****************/ 
GE_EXPORT void
theme_init (GTypeModule * module)
{
  hc_rc_style_register_type (module);
  hc_style_register_type (module);
}
 
GE_EXPORT void
theme_exit (void)
{
}
 
GE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  return GTK_RC_STYLE (g_object_new (HC_TYPE_RC_STYLE, NULL));
}
