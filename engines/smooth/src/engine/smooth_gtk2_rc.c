/******************************************************************************/
/* smooth_gtk2_rc.c - RC Function for GTK2 Engine                             */
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
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#include <stdio.h>
#include "smooth_gtk2_engine.h"
#include "smooth_gtk2_drawing.h"
#include "smooth_gtk2_misc.h"
#include "smooth_gtk2_rc.h"

static void smooth_rc_style_init_data (SmoothRcStyleData *style)
{
  gint i;

  style->real_sliders    = DEFAULT_REAL_SLIDERS;
  style->resize_grip     = DEFAULT_RESIZE_GRIP;

  style->line.style      = SMOOTH_BEVEL_STYLE_DEFAULT;
  style->line.thickness  = DEFAULT_LINETHICKNESS;

  memset(&style->edge, 0, sizeof(smooth_edge_style));

  style->edge.style           = DEFAULT_EDGESTYLE;
  style->edge.use_line        = FALSE;
  style->edge.line.style      = SMOOTH_BEVEL_STYLE_DEFAULT;
  style->edge.line.thickness  = DEFAULT_LINETHICKNESS;

  style->fill.style      = SMOOTH_FILL_STYLE_DEFAULT;
  style->fill.default_dither_depth_set = FALSE;
  style->fill.default_dither_depth = 8;
  style->fill.gradient_direction[FALSE] = DEFAULT_VGRADIENTDIRECTION;
  style->fill.gradient_direction[TRUE] = DEFAULT_HGRADIENTDIRECTION;
  style->fill.quadratic_gradient = FALSE;
  style->fill.shade1 = 1.3;
  style->fill.shade2 = 0.7;
  
  style->buffered_fill = TRUE;

  for (i=0; i < 5; i++) {
    style->fill.use_color1[i] = FALSE;
    style->fill.use_color2[i] = FALSE;
    style->fill.file_name[i] = NULL;
    style->fill.use_dither_depth[i] = FALSE;

    style->focus.use_foreground[i] = FALSE;
    style->focus.pattern[i] = NULL;
    style->focus.use_line_width[i] = FALSE;
    style->focus.line_width[i] = 1;
  }

  memset(&style->arrow, 0, sizeof(SmoothArrowPart));
  style->arrow.DefaultStyle = g_new0(SmoothArrow, 1);
  style->arrow.DefaultStyle->Style     = SMOOTH_ARROW_STYLE_DEFAULT;
  style->arrow.DefaultStyle->Solid     = DEFAULT_SOLIDARROW;
  style->arrow.DefaultStyle->Etched    = DEFAULT_ETCHEDARROW;
  style->arrow.DefaultStyle->Tail      = DEFAULT_TAILARROW;

  smooth_part_init(THEME_PART(&style->grip), DEFAULT_GRIPSTYLE);

  style->grip.count      = DEFAULT_GRIPCOUNT;
  style->grip.spacing    = DEFAULT_GRIPSPACING;
  style->grip.overlap    = DEFAULT_GRIPOVERLAP;


  smooth_part_init(THEME_PART(&style->check), SMOOTH_CHECKMARK_STYLE_DEFAULT);
  style->check.motif = TRUE;

  smooth_part_init(THEME_PART(&style->option), SMOOTH_CHECKMARK_STYLE_DEFAULT_OPTION);
  style->option.motif = TRUE;

  smooth_part_init(&style->progress, 0);

  smooth_part_init(THEME_PART(&style->trough), 0);
  style->trough.show_value = DEFAULT_TROUGH_SHOW_VALUE;

  smooth_part_init(THEME_PART(&style->stepper), 0);
  memset(&(style->stepper.Arrow), 0, sizeof(SmoothArrowPart));
  style->stepper.Arrow.Inherited = &style->arrow;
  
  smooth_part_init(THEME_PART(&style->button), 0);

  smooth_part_init(&style->button.button_default, SMOOTH_BUTTON_DEFAULT_STYLE_DEFAULT);
  style->button.default_triangle = SMOOTH_BUTTON_DEFAULT_STYLE_DEFAULT;
  style->button.embeddable = FALSE;
  style->button.use_button_default = FALSE;
  
  smooth_part_init(THEME_PART(&style->tabs), DEFAULT_TABSTYLE);

  smooth_part_init(&style->tabs.active_tab, DEFAULT_TABSTYLE);
  style->tabs.use_active_tab	= FALSE;
}

void smooth_rc_style_init (SmoothRcStyle *style)
{	
	style->engine_data = g_new0(SmoothRcStyleData, 1);
	smooth_rc_style_init_data (style->engine_data);
	
	#if 0
	style->quit_handler_id = gtk_quit_add(gtk_main_level(), (GtkFunction)smooth_rc_data_unref,(gpointer)style);
	g_object_ref(style);
	#endif
	
	smooth_rc_references += 1;
}

static GtkSettings  *smooth_rc_settings = NULL;

guint 
smooth_rc_parse_pixmap(GScanner *scanner, 
                   GTokenType wanted_token, 
                   gchar *default_value,
                   GString **retval)
{
	guint token;
	gchar *tmpstr=NULL;

	token = g_scanner_cur_token (scanner);
	if (token != wanted_token)
	{	
		token = g_scanner_get_next_token (scanner);
		if (token != wanted_token)
		{
			return wanted_token;
		}
	}
	
	if (wanted_token != G_TOKEN_EQUAL_SIGN)
	{
		token = g_scanner_get_next_token (scanner);
		if (token != G_TOKEN_EQUAL_SIGN)
			return G_TOKEN_EQUAL_SIGN;
	}
		
	token = g_scanner_get_next_token (scanner);
	if (token == G_TOKEN_STRING)
		tmpstr = gtk_rc_find_pixmap_in_path(smooth_rc_settings, scanner, scanner->value.v_string);	

	if (tmpstr == NULL)
		tmpstr = default_value;

	if (tmpstr == NULL)
		*retval =  NULL;
	else  
		*retval =  g_string_new(tmpstr);

	return token;
}

guint
smooth_gtkrc_parse(GScanner   *Scanner,
			GtkSettings  *Settings,
			SmoothRcStyle *Style,
			guint Token)
{
	smooth_rc_settings = Settings;
	
	switch (Token)
	{
		case TOKEN_EDGE:
			Token = smooth_rc_parse_edge (Scanner, TOKEN_EDGE, &SMOOTH_RC_DATA(Style)->edge);
		break;

		case TOKEN_FILL :
			Token = smooth_rc_parse_fill (Scanner, TOKEN_FILL, &SMOOTH_RC_DATA(Style)->fill);
		break;

		case TOKEN_TABS:
			Token = smooth_rc_parse_tab (Scanner, TOKEN_TABS, &SMOOTH_RC_DATA(Style)->tabs);
		break;

		case TOKEN_BUTTON:
			Token = smooth_rc_parse_button (Scanner, TOKEN_BUTTON, &SMOOTH_RC_DATA(Style)->button);
		break;

		case TOKEN_REAL_SLIDERS:
			Token = smooth_rc_parse_boolean (Scanner, TOKEN_REAL_SLIDERS, DEFAULT_REAL_SLIDERS, &SMOOTH_RC_DATA(Style)->real_sliders);
		break;

		case TOKEN_RESIZE_GRIP:
			Token = smooth_rc_parse_boolean (Scanner, TOKEN_RESIZE_GRIP, DEFAULT_RESIZE_GRIP, &SMOOTH_RC_DATA(Style)->resize_grip);
		break;

		case TOKEN_LINE:
			Token = smooth_rc_parse_line (Scanner, TOKEN_LINE, &SMOOTH_RC_DATA(Style)->line);
		break;

		case TOKEN_GRIP:
			Token = smooth_rc_parse_grip (Scanner, TOKEN_GRIP, &SMOOTH_RC_DATA(Style)->grip);
		break;

		case TOKEN_STEPPER:
			Token = smooth_rc_parse_stepper (Scanner, TOKEN_STEPPER, &SMOOTH_RC_DATA(Style)->stepper);
		break;

		case TOKEN_ARROW:
			Token = smooth_rc_parse_arrow_part (Scanner, TOKEN_ARROW, &SMOOTH_RC_DATA(Style)->arrow);
		break;	  

		case TOKEN_FOCUS:
			Token = smooth_rc_parse_focus (Scanner, TOKEN_FOCUS, &SMOOTH_RC_DATA(Style)->focus);
		break;

		case TOKEN_PROGRESS:
			Token = smooth_rc_parse_generic_part (Scanner, TOKEN_PROGRESS, &SMOOTH_RC_DATA(Style)->progress);
		break;

		case TOKEN_TROUGH:
			Token = smooth_rc_parse_trough_part (Scanner, TOKEN_TROUGH, &SMOOTH_RC_DATA(Style)->trough);
		break;

		case TOKEN_CHECK:
			Token = smooth_rc_parse_check (Scanner, TOKEN_CHECK, &SMOOTH_RC_DATA(Style)->check);
		break;

		case TOKEN_OPTION:
			Token = smooth_rc_parse_option (Scanner, TOKEN_OPTION, &SMOOTH_RC_DATA(Style)->option);
		break;

		/* backward compatibility for <=0.5.4 */
		case TOKEN_DEPRECATED_TABSTYLE:
			Token = smooth_rc_parse_custom_enum(Scanner, TOKEN_DEPRECATED_TABSTYLE, SmoothTranslateTabStyleName, DEFAULT_TABSTYLE, 
							&THEME_PART(&SMOOTH_RC_DATA(Style)->tabs)->style);
		break;

		/* backward compatibility for <=0.5.0 */
		case TOKEN_DEPRECATED_ARROWSTYLE:
		{
			if (!SMOOTH_RC_DATA(Style)->arrow.DefaultStyle)
				SMOOTH_RC_DATA(Style)->arrow.DefaultStyle = g_new0(SmoothArrow, 1);

			Token = smooth_rc_parse_custom_enum(Scanner, TOKEN_DEPRECATED_ARROWSTYLE, 
							(SmoothSmoothTranslateEnumFunc)SmoothTranslateArrowStyleName, 
							SMOOTH_ARROW_STYLE_DEFAULT, 
							&SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Style);
			SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->HasStyle = SmoothTrue;

			switch (SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Style)
			{
				case SMOOTH_ARROW_STYLE_DEPRECATED_CLEANICE :
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Style = SMOOTH_ARROW_STYLE_CLEAN;
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Tail = 0;
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->HasTail = SmoothTrue;
				break;
	
				case SMOOTH_ARROW_STYLE_DEPRECATED_XFCE :
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Style = SMOOTH_ARROW_STYLE_CLEAN;
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Tail = 1;
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->HasTail = SmoothTrue;
				break;

				case SMOOTH_ARROW_STYLE_DEPRECATED_THINICE :
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Style = SMOOTH_ARROW_STYLE_CLEAN;
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Tail = 2;
					SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->HasTail = SmoothTrue;
				break;

				default :
				break;
			}
		}
		break;

		case TOKEN_DEPRECATED_SOLIDARROW:
		{
			if (!SMOOTH_RC_DATA(Style)->arrow.DefaultStyle)
				SMOOTH_RC_DATA(Style)->arrow.DefaultStyle = g_new0(SmoothArrow, 1);

			Token = smooth_rc_parse_boolean (Scanner, TOKEN_DEPRECATED_SOLIDARROW, 
							DEFAULT_SOLIDARROW, 
							&SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Solid);
			SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->HasSolid = SmoothTrue;
		}
		break;

		case TOKEN_DEPRECATED_ETCHEDARROW:
		{
			if (!SMOOTH_RC_DATA(Style)->arrow.DefaultStyle)
				SMOOTH_RC_DATA(Style)->arrow.DefaultStyle = g_new0(SmoothArrow, 1);

			Token = smooth_rc_parse_boolean (Scanner, TOKEN_DEPRECATED_ETCHEDARROW, 
							DEFAULT_ETCHEDARROW, &SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->Etched);
			SMOOTH_RC_DATA(Style)->arrow.DefaultStyle->HasEtched = SmoothTrue;
		}
		break;

		/* backward compatibility for <=0.4.0 */
		case TOKEN_DEPRECATED_GRADIENT:
		{
			gboolean use_gradient=FALSE;
	
			Token = smooth_rc_parse_boolean (Scanner, TOKEN_DEPRECATED_GRADIENT, TRUE, &use_gradient);
	    
			if (use_gradient) 
			{
				SMOOTH_RC_DATA(Style)->fill.style = SMOOTH_FILL_STYLE_SHADE_GRADIENT;
				SMOOTH_RC_DATA(Style)->fill.quadratic_gradient = TRUE;
				SMOOTH_RC_DATA(Style)->fill.gradient_direction[FALSE] = SMOOTH_GRADIENT_HORIZONTAL;			
				SMOOTH_RC_DATA(Style)->fill.gradient_direction[TRUE] = SMOOTH_GRADIENT_VERTICAL;			
				SMOOTH_RC_DATA(Style)->fill.shade1 = 1.3;
				SMOOTH_RC_DATA(Style)->fill.shade2 = 0.7;
			}
		}  
		break;

		default:
			g_scanner_get_next_token (Scanner);
			Token = G_TOKEN_RIGHT_CURLY;
		break;
	}

	return Token;
}

void
smooth_gtkrc_style_merge (SmoothRcStyleData *dest_data,
                          SmoothRcStyleData *src_data)
{
      gint i;

      dest_data->real_sliders = src_data->real_sliders;
      dest_data->resize_grip = src_data->resize_grip;
      dest_data->line.style = src_data->line.style;
      dest_data->line.thickness = src_data->line.thickness;

      dest_data->edge.use_line        = src_data->edge.use_line;
      dest_data->edge.line.style      = src_data->edge.line.style;
      dest_data->edge.line.thickness  = src_data->edge.line.thickness;

      dest_data->fill.style = src_data->fill.style;
      dest_data->fill.quadratic_gradient = src_data->fill.quadratic_gradient;
      dest_data->fill.gradient_direction[FALSE] = src_data->fill.gradient_direction[FALSE];
      dest_data->fill.gradient_direction[TRUE] = src_data->fill.gradient_direction[TRUE];
      dest_data->fill.shade1 = src_data->fill.shade1;
      dest_data->fill.shade2 = src_data->fill.shade2;
      dest_data->fill.default_dither_depth_set = src_data->fill.default_dither_depth_set;
      dest_data->fill.default_dither_depth = src_data->fill.default_dither_depth;

      for (i=0; i < 5; i++) {
        dest_data->fill.use_color1[i] = src_data->fill.use_color1[i];
        if (src_data->fill.use_color1[i])
          dest_data->fill.color1[i] = src_data->fill.color1[i];

        dest_data->fill.use_color2[i] = src_data->fill.use_color2[i];
        if (src_data->fill.use_color2[i])
          dest_data->fill.color2[i] = src_data->fill.color2[i];

        dest_data->fill.use_dither_depth[i] = src_data->fill.use_dither_depth[i];
        if (src_data->fill.use_dither_depth[i])
	        dest_data->fill.dither_depth[i] = src_data->fill.dither_depth[i];

        if (src_data->fill.file_name[i]) 
	{
		if (!dest_data->fill.file_name[i])
			dest_data->fill.file_name[i] =  g_string_sized_new(src_data->fill.file_name[i]->len);
		
		g_string_assign(dest_data->fill.file_name[i], src_data->fill.file_name[i]->str);
        }
        
        if (src_data->focus.pattern[i]) {
          if (dest_data->focus.pattern[i])
            g_free(dest_data->focus.pattern[i]);

          dest_data->focus.pattern[i] = g_strdup(src_data->focus.pattern[i]);
        }

        dest_data->focus.use_foreground[i] = src_data->focus.use_foreground[i];
        if (src_data->focus.use_foreground[i])
          dest_data->focus.foreground[i] = src_data->focus.foreground[i];

        dest_data->focus.use_line_width[i] = src_data->focus.use_line_width[i];
        if (src_data->focus.use_line_width[i])
          dest_data->focus.line_width[i] = src_data->focus.line_width[i];

      }

      smooth_arrow_merge (&dest_data->arrow, &src_data->arrow);

      smooth_part_merge(THEME_PART(&dest_data->grip),THEME_PART(&src_data->grip));
      dest_data->grip.count = src_data->grip.count;
      dest_data->grip.spacing = src_data->grip.spacing;
      dest_data->grip.overlap = src_data->grip.overlap;

      smooth_part_merge(THEME_PART(&dest_data->check),THEME_PART(&src_data->check));
      dest_data->check.motif = src_data->check.motif;
      
      smooth_part_merge(THEME_PART(&dest_data->option),THEME_PART(&src_data->option));
      dest_data->option.motif = src_data->option.motif;

      smooth_part_merge(THEME_PART(&dest_data->trough),THEME_PART(&src_data->trough));
      dest_data->trough.show_value = src_data->trough.show_value;
      
      smooth_part_merge(THEME_PART(&dest_data->stepper),THEME_PART(&src_data->stepper));
      smooth_arrow_merge (&dest_data->stepper.Arrow, &src_data->stepper.Arrow);
      dest_data->stepper.Arrow.Inherited = &dest_data->arrow;
      
      smooth_part_merge(&dest_data->progress,&src_data->progress);
 
      smooth_part_merge(THEME_PART(&dest_data->button), THEME_PART(&src_data->button));
      dest_data->button.default_triangle = src_data->button.default_triangle;
      dest_data->button.embeddable = src_data->button.embeddable;
      dest_data->button.use_button_default = src_data->button.use_button_default;

      smooth_part_merge(&dest_data->button.button_default,&src_data->button.button_default);

      smooth_part_merge(THEME_PART(&dest_data->tabs), THEME_PART(&src_data->tabs));
      
      dest_data->tabs.highlight = src_data->tabs.highlight;
      dest_data->tabs.use_active_tab = src_data->tabs.use_active_tab;
      smooth_part_merge(&dest_data->tabs.active_tab,&src_data->tabs.active_tab);

      dest_data->buffered_fill = src_data->buffered_fill;
}
