/******************************************************************************/
/* smooth_gtk2_rc.h - RC Declarations for GTK2 Engine                         */
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
/* Portions Based on the EnGradient Engine                                    */
/*   Andrew Cattau                                                            */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#ifndef SMOOTH_GTK2_RC_H
#define SMOOTH_GTK2_RC_H

#include <engine/shared/gtk/smooth_gtk_rc.h>

struct _smooth_focus_style{
  gboolean use_foreground[5];/* SmoothWidgetState */
  SmoothColor foreground[5];/* SmoothWidgetState */

  gchar * pattern[5];/* GtkStateType */

  gboolean use_line_width[5];/* GtkStateType */
  gint line_width[5];/* GtkStateType */
};

typedef struct
{
  gboolean buffered_fill;
  
  gboolean		  real_sliders;
  gboolean		  resize_grip;
  
  SmoothColorCube         colors;
  SmoothFillPart	  fill;
  SmoothLinePart	  line;
  smooth_edge_style	  edge;

  smooth_focus_style	  focus;

  smooth_bg_style	  background;
  smooth_fg_style	  foreground;

  smooth_trough_style     trough;
  SmoothStepperStyle      stepper;

  smooth_part_style       progress;

  smooth_grip_style	  grip;
  smooth_check_style	  check;
  smooth_check_style	  option;
  SmoothArrowPart	  arrow;
  smooth_button_style	  button;
  smooth_tab_style	  tabs;
} SmoothRcStyleData;


typedef struct
{
  GtkRcStyle parent_instance;
  
  SmoothRcStyleData *engine_data;
  
  /* for quit dispose hack */
  guint quit_handler_id;
} SmoothRcStyle;

void smooth_rc_style_init (SmoothRcStyle *style) G_GNUC_INTERNAL;

void
smooth_gtkrc_style_merge (SmoothRcStyleData *dest_data,
                          SmoothRcStyleData *src_data) G_GNUC_INTERNAL;

guint
smooth_gtkrc_parse(GScanner   *Scanner,
			GtkSettings  *Settings,
			SmoothRcStyle *Style,
			guint Token) G_GNUC_INTERNAL;

#define is_enum(str, value)  (g_ascii_strncasecmp(str, value, strlen(value)) == 0)

guint 
theme_parse_pixmap(GScanner *scanner, 
                   GTokenType wanted_token, 
                   gchar *default_value,
                   GString **retval) G_GNUC_INTERNAL;

#endif /*SMOOTH_GTK2_RC*/
