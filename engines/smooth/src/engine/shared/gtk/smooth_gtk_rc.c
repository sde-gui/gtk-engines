/******************************************************************************/
/* smooth_gtk_rc.c - Shared RC functions for all GTK+ versions                */
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
#include ENGINE_HEADER
#include ENGINE_RC_HEADER

#define CHECK_DETAIL(detail, value) ((detail) && (!strcmp(value, detail)))

ThemeSymbols smooth_theme_symbols[] =
{
  { "real_sliders",        TOKEN_REAL_SLIDERS },
  { "resize_grip",         TOKEN_RESIZE_GRIP },

  { "style",               TOKEN_STYLE },

  { "file",                TOKEN_FILE },

  { "fill",                TOKEN_FILL },
  { "vdirection",          TOKEN_VDIRECTION },
  { "hdirection",          TOKEN_HDIRECTION },
  { "color1",              TOKEN_COLOR1 },
  { "color2",              TOKEN_COLOR2 },
  { "shade1",              TOKEN_SHADE1_VALUE },
  { "shade2",              TOKEN_SHADE2_VALUE },
  { "quadratic",           TOKEN_QUADRATIC_GRADIENT },
  { "dither",              TOKEN_DITHER_DEPTH },
  
  { "edge",                TOKEN_EDGE },

  { "line",                TOKEN_LINE },
  { "thickness",           TOKEN_THICKNESS },

  { "foreground",          TOKEN_FOREGROUND },

  { "arrow",     	   TOKEN_ARROW },
  { "solid",               TOKEN_SOLID },
  { "etched",              TOKEN_ETCHED },
  { "tail",                TOKEN_TAIL },

  { "focus",               TOKEN_FOCUS },
  { "pattern",             TOKEN_PATTERN },
  { "width",               TOKEN_WIDTH },

  { "button",              TOKEN_BUTTON },
  { "default",             TOKEN_BUTTON_DEFAULT },
  { "embeddable",          TOKEN_BUTTON_EMBEDDABLE },

  { "show_triangle",       TOKEN_DEFAULT_TRIANGLE },

  { "tabs",                TOKEN_TABS },
  { "active_tab",          TOKEN_ACTIVE_TAB },
  { "highlight",           TOKEN_HIGHLIGHT },

  { "progress",            TOKEN_PROGRESS },

  { "trough",              TOKEN_TROUGH },
  { "show_value",          TOKEN_TROUGH_SHOW_VALUE },

  { "stepper",          TOKEN_STEPPER },

  { "grip",                TOKEN_GRIP },
  { "count",               TOKEN_COUNT },
  { "spacing",             TOKEN_SPACING },
  { "toolbar_overlap",     TOKEN_TOOLBAR_OVERLAP },

  { "motif",               TOKEN_MOTIF },
  { "check",               TOKEN_CHECK },

  { "option",              TOKEN_OPTION },

  { "xpadding",            TOKEN_XPADDING },
  { "ypadding",            TOKEN_YPADDING },

  /* backward compatibility for <= 0.5.4 */
  { "tab_style",           TOKEN_DEPRECATED_TABSTYLE },

  /* backward compatibility for <= 0.5.0 */
  { "arrow_style",     	   TOKEN_DEPRECATED_ARROWSTYLE },
  { "solid_arrow",         TOKEN_DEPRECATED_SOLIDARROW },
  { "etched_arrow",        TOKEN_DEPRECATED_ETCHEDARROW },

  /* backward compatibility for <= 0.4.0 */
  { "use_gradient",        TOKEN_DEPRECATED_GRADIENT },
  { NULL,                  0 }
};

/* fill parts */
gint
smooth_fill_style(GtkStyle * style, void *part)
{
  if ((part) && (THEME_PART(part)->use_fill))
    return THEME_PART(part)->fill.style;
  else
    return THEME_DATA(style)->fill.style;
}

gint
smooth_fill_gradient_direction(GtkStyle * style, void *part, gboolean horizontal)
{
	if ((part) && (THEME_PART(part)->use_fill))
		return THEME_PART(part)->fill.gradient_direction[horizontal];
	else
		return THEME_DATA(style)->fill.gradient_direction[horizontal];
}

gboolean
smooth_fill_quadratic_gradient(GtkStyle * style, void *part)
{
	if ((part) && (THEME_PART(part)->use_fill))
		return THEME_PART(part)->fill.quadratic_gradient;
	else
		return THEME_DATA(style)->fill.quadratic_gradient;
}

gdouble
smooth_fill_shade1_value(GtkStyle * style, void *part)
{
	if ((part) && (THEME_PART(part)->use_fill))
		return THEME_PART(part)->fill.shade1;
	else
		return THEME_DATA(style)->fill.shade1;
}

gdouble
smooth_fill_shade2_value(GtkStyle * style, void *part)
{
	if ((part) && (THEME_PART(part)->use_fill))
		return THEME_PART(part)->fill.shade2;
	else
		return THEME_DATA(style)->fill.shade2;
}

SmoothColor
smooth_fill_color1(GtkStyle * style, void *part, GtkStateType state)
{
	SmoothColor result;
	SmoothWidgetState widget_state = SmoothGtkWidgetState(state);
	
	if ((part) && (THEME_PART(part)->use_fill))
	{
		if (THEME_PART(part)->fill.use_color1[widget_state])
			result = THEME_PART(part)->fill.color1[widget_state];
		else 
		{
			if (THEME_DATA(style)->fill.use_color1[widget_state])
				result = THEME_DATA(style)->fill.color1[widget_state];
			else 
				SmoothColorAssignGdkColor(&result, style->bg[state], 1.0);
		}
	}
	else
	{
		if (THEME_DATA(style)->fill.use_color1[widget_state])
			result = THEME_DATA(style)->fill.color1[widget_state];
		else 
			SmoothColorAssignGdkColor(&result, style->bg[state], 1.0);
	}
	
	return result;
}

SmoothColor
smooth_fill_color2(GtkStyle * style, void *part, GtkStateType state)
{
	SmoothColor result;
	SmoothWidgetState widget_state = SmoothGtkWidgetState(state);

	if ((part) && (THEME_PART(part)->use_fill))
	{
		if (THEME_PART(part)->fill.use_color2[widget_state])
			result = THEME_PART(part)->fill.color2[widget_state];
		else 
		{
			if (THEME_DATA(style)->fill.use_color2[widget_state])
				result = THEME_DATA(style)->fill.color2[widget_state];
			else 
				SmoothColorAssignGdkColor(&result, style->bg[state], 1.0);
		}
	}
	else
	{
		if (THEME_DATA(style)->fill.use_color2[widget_state])
			result = THEME_DATA(style)->fill.color2[widget_state];
		else 
			SmoothColorAssignGdkColor(&result, style->bg[state], 1.0);
	}
	
	return result;
}

GString *
smooth_fill_file_name(GtkStyle * style, void *part, GtkStateType state)
{
	GString *result;
	
	if ((part) && (THEME_PART(part)->use_fill))
	{
		if (THEME_PART(part)->fill.file_name[state])
			result = THEME_PART(part)->fill.file_name[state];
		else
		{
			if (THEME_DATA(style)->fill.file_name[state])
				result = THEME_DATA(style)->fill.file_name[state];
			else
				result = NULL;
		}
	}
	else
	{
		if (THEME_DATA(style)->fill.file_name[state]) 
			result = THEME_DATA(style)->fill.file_name[state];
		else
			result = NULL;
	}
	
	return result;
}

/* tab part styles*/
smooth_part_style 
*smooth_tab_part(GtkStyle * style, gboolean for_active_tab)
{
  smooth_tab_style tabs = THEME_DATA(style)->tabs;
  
  if ((for_active_tab) && (tabs.use_active_tab))
    return &THEME_DATA(style)->tabs.active_tab;
  else
    return THEME_PART(&THEME_DATA(style)->tabs);
}

gint
smooth_tab_get_style(GtkStyle * style, gboolean for_active_tab)
{
  smooth_tab_style tabs = THEME_DATA(style)->tabs;
  smooth_part_style active_tab = THEME_DATA(style)->tabs.active_tab;
  
  if ((for_active_tab) && (tabs.use_active_tab))
    return active_tab.style;
  else
    return THEME_PART(&tabs)->style;
}

SmoothFillPart *
smooth_tab_fill(GtkStyle * style, gboolean for_active_tab)
{
  smooth_tab_style tabs = THEME_DATA(style)->tabs;
  smooth_part_style active_tab = THEME_DATA(style)->tabs.active_tab;
  
  if ((for_active_tab) && (tabs.use_active_tab) && (active_tab.use_fill))
    return &THEME_DATA(style)->tabs.active_tab.fill;
  else
    return &THEME_PART(&THEME_DATA(style)->tabs)->fill;
}

SmoothBevelStyle
smooth_tab_edge_line_style(GtkStyle * style, gboolean for_active_tab)
{
  smooth_tab_style tabs = THEME_DATA(style)->tabs;
  smooth_part_style active_tab = THEME_DATA(style)->tabs.active_tab;
  
  if ((for_active_tab) && (tabs.use_active_tab) && (active_tab.edge.use_line || active_tab.use_line))
    return EDGE_LINE_STYLE(style, &active_tab);
  else
    return EDGE_LINE_STYLE(style, THEME_PART(&tabs));
}

gint
smooth_tab_edge_line_thickness(GtkStyle * style, gboolean for_active_tab)
{
  smooth_tab_style tabs = THEME_DATA(style)->tabs;
  smooth_part_style active_tab = THEME_DATA(style)->tabs.active_tab;
  
  if ((for_active_tab) && (tabs.use_active_tab) && (active_tab.edge.use_line || active_tab.use_line))
    return EDGE_LINE_THICKNESS(style, &active_tab);
  else
    return EDGE_LINE_THICKNESS(style, THEME_PART(&tabs));
}

/* button part styles*/
smooth_part_style 
*smooth_button_part(GtkStyle * style, gboolean for_button_default)
{
  smooth_button_style button = THEME_DATA(style)->button;
  
  if ((for_button_default) && (button.use_button_default))
    return &THEME_DATA(style)->button.button_default;
  else
    return THEME_PART(&THEME_DATA(style)->button);
}

gint
smooth_button_get_style(GtkStyle * style, gboolean for_button_default)
{
  smooth_button_style button = THEME_DATA(style)->button;
  smooth_part_style button_default = THEME_DATA(style)->button.button_default;
  
  if ((for_button_default) && (button.use_button_default))
    return button_default.style;
  else
    return THEME_PART(&button)->style;
}

gboolean
smooth_button_default_triangle(GtkStyle * style)
{
  smooth_button_style button = THEME_DATA(style)->button;
  smooth_part_style button_default = THEME_DATA(style)->button.button_default;
  
  if ((button.use_button_default))
    return ((button_default.style==SMOOTH_BUTTON_DEFAULT_STYLE_TRIANGLE) || (button.default_triangle));
  else
    return (button.default_triangle);
}

SmoothFillPart *
smooth_button_fill(GtkStyle * style, gboolean for_button_default)
{
  smooth_button_style button = THEME_DATA(style)->button;
  smooth_part_style button_default = THEME_DATA(style)->button.button_default;
  
  if ((for_button_default) && (button.use_button_default) && (button_default.use_fill))
    return &THEME_DATA(style)->button.button_default.fill;
  else
    return &THEME_PART(&THEME_DATA(style)->button)->fill;
}

SmoothBevelStyle
smooth_button_edge_line_style(GtkStyle * style, gboolean for_button_default)
{
  smooth_button_style button = THEME_DATA(style)->button;
  smooth_part_style button_default = THEME_DATA(style)->button.button_default;
  
  if ((for_button_default) && (button.use_button_default) && (button_default.edge.use_line || button_default.use_line))
    return EDGE_LINE_STYLE(style, &button_default);
  else
    return EDGE_LINE_STYLE(style, THEME_PART(&button));
}

gint
smooth_button_edge_line_thickness(GtkStyle * style, gboolean for_button_default)
{
  smooth_button_style button = THEME_DATA(style)->button;
  smooth_part_style button_default = THEME_DATA(style)->button.button_default;
  
  if ((for_button_default) && (button.use_button_default) && (button_default.edge.use_line || button_default.use_line))
    return EDGE_LINE_THICKNESS(style, &button_default);
  else
    return EDGE_LINE_THICKNESS(style, THEME_PART(&button));
}

/* Custom Enums */
gboolean 
SmoothTranslateFillStyleName (gchar * str, SmoothFillStyle *retval)
{
  if (is_enum(str, "flat") || is_enum(str, "solid"))
    *retval = SMOOTH_FILL_STYLE_SOLID;
  else if (is_enum(str, "gradient"))
    *retval = SMOOTH_FILL_STYLE_GRADIENT;
  else if (is_enum(str, "shade_gradient") || is_enum(str, "shaded") || is_enum(str, "shade"))
    *retval = SMOOTH_FILL_STYLE_SHADE_GRADIENT;
  else if (is_enum(str, "tile") || is_enum(str, "pixbuf") || is_enum(str, "pixmap") || is_enum(str, "xpm"))
    *retval = SMOOTH_FILL_STYLE_TILE;
  else
    return FALSE; 
  return TRUE;
}

gboolean 
SmoothTranslateGradientDirectionName (gchar * str, gint *retval)
{
  if (is_enum(str, "horizontal"))
    *retval = SMOOTH_GRADIENT_HORIZONTAL;
  else if (is_enum(str, "vertical"))
    *retval = SMOOTH_GRADIENT_VERTICAL;
  else if (is_enum(str, "fdiagonal") || is_enum(str, "forward_diagonal") || is_enum(str, "forwarddiagonal") || is_enum(str, "ndiagonal") || is_enum(str, "northern_diagonal") || is_enum(str, "northerndiagonal"))
    *retval = SMOOTH_GRADIENT_NORTHERN_DIAGONAL;
  else if (is_enum(str, "bdiagonal") || is_enum(str, "backward_diagonal") || is_enum(str, "backwarddiagonal") || is_enum(str, "sdiagonal") || is_enum(str, "southern_diagonal") || is_enum(str, "southerndiagonal"))
    *retval = SMOOTH_GRADIENT_SOUTHERN_DIAGONAL;
  else
    return FALSE; 
  return TRUE;
}

gboolean 
SmoothTranslateLineStyleName (gchar * str, SmoothBevelStyle *retval)
{
	if (is_enum(str, "smooth") ||is_enum(str, "smoothed"))
		*retval = SMOOTH_BEVEL_STYLE_SMOOTHED;
	else if (is_enum(str, "smoothbevel"))
		*retval = SMOOTH_BEVEL_STYLE_SMOOTHEDBEVEL;	
	else if (is_enum(str, "bevel") || is_enum(str, "beveled"))
		*retval = SMOOTH_BEVEL_STYLE_BEVELED;

	else if (is_enum(str, "thin"))
		*retval = SMOOTH_BEVEL_STYLE_THIN;
	else if (is_enum(str, "soft"))
		*retval = SMOOTH_BEVEL_STYLE_SOFT;

	else if (is_enum(str, "standard") || is_enum(str, "normal"))
		*retval = SMOOTH_BEVEL_STYLE_STANDARD;
	else if (is_enum(str, "redmond"))
		*retval = SMOOTH_BEVEL_STYLE_REDMOND;
	
	else if (is_enum(str, "cold"))
		*retval = SMOOTH_BEVEL_STYLE_COLD;

	else if (is_enum(str, "iced"))
		*retval = SMOOTH_BEVEL_STYLE_ICED;
	
	else if (is_enum(str, "flat"))
		*retval = SMOOTH_BEVEL_STYLE_FLAT;

	else if (is_enum(str, "none"))
		*retval = SMOOTH_BEVEL_STYLE_NONE;

	else if (is_enum(str, "default"))
		*retval = SMOOTH_BEVEL_STYLE_DEFAULT;

	else if (is_enum(str, "wonderland"))
		*retval = SMOOTH_BEVEL_STYLE_DEPRECATED_WONDERLAND;
	else
		return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateButtonDefaultStyleName (gchar * str, gint *retval)
{
  if (is_enum(str, "gtk") || is_enum(str, "gtk1") || is_enum(str, "normal") || is_enum(str, "standard"))
    *retval = SMOOTH_BUTTON_DEFAULT_STYLE_GTK;
  else if (is_enum(str, "win32") || is_enum(str, "redmond"))
    *retval = SMOOTH_BUTTON_DEFAULT_STYLE_WIN32;
  else if (is_enum(str, "triangle"))
    *retval = SMOOTH_BUTTON_DEFAULT_STYLE_TRIANGLE;
  else if (is_enum(str, "none"))
    *retval = SMOOTH_BUTTON_DEFAULT_STYLE_NONE;
  else if (is_enum(str, "default"))
    *retval = SMOOTH_BUTTON_DEFAULT_STYLE_DEFAULT;
  else
    return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateTabStyleName (gchar * str, gint *retval)
{
  if (is_enum(str, "square") || is_enum(str, "default") || is_enum(str, "normal") || is_enum(str, "standard"))
    *retval = SMOOTH_TAB_NORMAL;
  else if (is_enum(str, "round"))
    *retval = SMOOTH_TAB_ROUND;
  else if (is_enum(str, "triangle"))
    *retval = SMOOTH_TAB_TRIANGLE;
  else if (is_enum(str, "xpm"))
    *retval = SMOOTH_TAB_PIXBUF;
  else
    return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateGripStyleName (gchar * str, gint *retval)
{
  if (is_enum(str, "none"))
    *retval = NO_GRIP;

  else if (is_enum(str, "bars_in"))
    *retval = BARS_IN_GRIP;
  else if (is_enum(str, "bars") || is_enum(str, "bars_out"))
    *retval = BARS_OUT_GRIP;

  else if (is_enum(str, "lines_in"))
    *retval = LINES_IN_GRIP;
  else if (is_enum(str, "lines") || is_enum(str, "lines_out"))
    *retval = LINES_OUT_GRIP;

  else if (is_enum(str, "fixedlines_out"))
    *retval = FIXEDLINES_OUT_GRIP;
  else if (is_enum(str, "midlines") || is_enum(str, "fixedlines") || is_enum(str, "fixedlines_in"))
    *retval = FIXEDLINES_IN_GRIP;

  else if (is_enum(str, "slashes"))
    *retval = SLASHES_GRIP;

  else if (is_enum(str, "mac_buds_out"))
    *retval = MAC_BUDS_OUT_GRIP;
  else if (is_enum(str, "mac_buds") || is_enum(str, "mac_buds_in"))
    *retval = MAC_BUDS_IN_GRIP;

  else if (is_enum(str, "buds_out") || is_enum(str, "ns_buds_out"))
    *retval = NS_BUDS_OUT_GRIP;
  else if (is_enum(str, "buds") || is_enum(str, "buds_in") || is_enum(str, "ns_buds") || is_enum(str, "ns_buds_in"))
    *retval = NS_BUDS_IN_GRIP;

  else if (is_enum(str, "smalldots_out"))
    *retval = SMALLDOTS_OUT_GRIP;
  else if (is_enum(str, "smalldots") || is_enum(str, "smalldots_in"))
    *retval = SMALLDOTS_IN_GRIP;

  else if (is_enum(str, "dots_out"))
    *retval = DOTS_OUT_GRIP;
  else if (is_enum(str, "dots") || is_enum(str, "dots_in"))
    *retval = DOTS_IN_GRIP;

  else if (is_enum(str, "autodots_out"))
    *retval = AUTODOTS_OUT_GRIP;
  else if (is_enum(str, "autodots") || is_enum(str, "autodots_in"))
    *retval = AUTODOTS_IN_GRIP;

  else if (is_enum(str, "xpm"))
    *retval = XPM_GRIP;
  else
    return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateCheckStyleName (gchar * str, gint *retval)
{
  if (is_enum(str, "clean"))
    *retval = SMOOTH_CHECKMARK_STYLE_CLEAN;
  else if (is_enum(str, "fast"))
    *retval = SMOOTH_CHECKMARK_STYLE_FAST;
    
  else if (is_enum(str, "criss_cross") || is_enum(str, "criss-cross") || is_enum(str, "crisscross") || is_enum(str, "x") || is_enum(str, "cross"))
    *retval = SMOOTH_CHECKMARK_STYLE_CROSS;
  else if (is_enum(str, "plus"))
    *retval = SMOOTH_CHECKMARK_STYLE_PLUS;

  else if (is_enum(str, "block") || is_enum(str, "square") || is_enum(str, "box"))
    *retval = SMOOTH_CHECKMARK_STYLE_BLOCK;
  else if (is_enum(str, "circle") || is_enum(str, "round"))
    *retval = SMOOTH_CHECKMARK_STYLE_CIRCLE;
  else if (is_enum(str, "diamond"))
    *retval = SMOOTH_CHECKMARK_STYLE_DIAMOND;

  else if (is_enum(str, "sloppy"))
    *retval = SMOOTH_CHECKMARK_STYLE_SLOPPY;

  else if (is_enum(str, "xpm") || is_enum(str, "pixmap") || is_enum(str, "pixbuf") || is_enum(str, "png"))
    *retval = SMOOTH_CHECKMARK_STYLE_PIXBUF;

  else if (is_enum(str, "none"))
    *retval = SMOOTH_CHECKMARK_STYLE_NONE;
    
  else
    return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateEdgeStyleName (gchar * str, gint *retval)
{
  if (is_enum(str, "none"))
    *retval = SMOOTH_EDGE_NONE;
    
  else if (is_enum(str, "line") || is_enum(str, "normal") || is_enum(str, "default") || is_enum(str, "standard"))
    *retval = SMOOTH_EDGE_LINE;
    
  else if (is_enum(str, "gradient"))
    *retval = SMOOTH_EDGE_GRADIENT;
    
  else if (is_enum(str, "xpm") || is_enum(str, "pixmap") || is_enum(str, "pixbuf"))
    *retval = SMOOTH_EDGE_PIXBUF;
    
  else
    return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateArrowStyleName (gchar * str, SmoothArrowStyle *retval)
{
  if (is_enum(str, "clean"))
    *retval = SMOOTH_ARROW_STYLE_CLEAN;
  else if (is_enum(str, "dirty"))
    *retval = SMOOTH_ARROW_STYLE_DIRTY;
  else if (is_enum(str, "slick"))
    *retval = SMOOTH_ARROW_STYLE_SLICK;
  else if (is_enum(str, "xpm"))
    *retval = SMOOTH_ARROW_STYLE_XPM;

  /* Deprecated theme styles - superceded by Slick & Dirty */
  else if (is_enum(str, "wonderland"))
    *retval = SMOOTH_ARROW_STYLE_DEPRECATED_WONDERLAND;
  else if (is_enum(str, "icegradient"))
    *retval = SMOOTH_ARROW_STYLE_DEPRECATED_ICEGRADIENT;

  /* Deprecated theme styles - superceded by Clean w/Tail */
  else if (is_enum(str, "cleanice"))
    *retval = SMOOTH_ARROW_STYLE_DEPRECATED_CLEANICE;
  else if (is_enum(str, "xfce"))
    *retval = SMOOTH_ARROW_STYLE_DEPRECATED_XFCE;
  else if (is_enum(str, "thinice"))
    *retval = SMOOTH_ARROW_STYLE_DEPRECATED_THINICE;
  
  /* use whichever is built in default */
  else if (is_enum(str, "default"))
    *retval = SMOOTH_ARROW_STYLE_DEFAULT;
  else
    return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateArrowTypeName (gchar * str, SmoothArrowStyle *retval)
{
  if (is_enum(str, "up"))
    *retval = SMOOTH_ARROW_TYPE_UP;
  else if (is_enum(str, "down"))
    *retval = SMOOTH_ARROW_TYPE_DOWN;
  else if (is_enum(str, "left"))
    *retval = SMOOTH_ARROW_TYPE_LEFT;
  else if (is_enum(str, "right"))
    *retval = SMOOTH_ARROW_TYPE_RIGHT;
  else
    return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateStateName (gchar * str, int *retval)
{
  if (is_enum(str, "normal"))
    *retval = 0;
  else if (is_enum(str, "active"))
    *retval = 1;
  else if (is_enum(str, "prelight"))
    *retval = 2;
  else if (is_enum(str, "selected"))
    *retval = 3;
  else if (is_enum(str, "insensitive"))
    *retval = 4;
  else
    return FALSE; 

  return TRUE;
}

gboolean 
SmoothTranslateBooleanName (gchar * str, gint *retval)
{
  if (is_enum(str, "TRUE") || is_enum(str, "T") || is_enum(str, "YES") || is_enum(str, "Y"))
    *retval = TRUE;
  else if (is_enum(str, "FALSE") || is_enum(str, "F") || is_enum(str, "NO") || is_enum(str, "N"))
    *retval = FALSE;
  else
    return FALSE; 

  return TRUE;
}

/* General Parsing Functions */
guint 
smooth_rc_parse_int (GScanner *scanner, 
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

guint 
smooth_rc_parse_float (GScanner *scanner, 
                 GTokenType wanted_token, 
		 gdouble return_default, 
		 gdouble *retval, 
		 gdouble lower_limit, 
		 gdouble upper_limit)
{
  guint token;

  token = g_scanner_cur_token (scanner);
  if (token != wanted_token)
  {
	  token = g_scanner_get_next_token (scanner);
	  if (token != wanted_token)
	    {
	      return wanted_token;
	    }
  }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_EQUAL_SIGN)
    {
      return G_TOKEN_EQUAL_SIGN;
    }

  token = g_scanner_get_next_token (scanner);

  if (token != G_TOKEN_FLOAT)
    *retval = return_default;
  else
    *retval = scanner->value.v_float;
  
  if (*retval < lower_limit) *retval = lower_limit; 
  if ((*retval > upper_limit) && (upper_limit > lower_limit)) *retval = upper_limit; 
  
  return G_TOKEN_NONE;
}

guint 
smooth_rc_parse_custom_enum (GScanner *scanner, 
                         GTokenType wanted_token, 
			 SmoothSmoothTranslateEnumFunc translate_enum,
			 guint return_default,
			 guint *retval)
{
  guint token;
   
  token = g_scanner_cur_token (scanner);
  if (token != wanted_token)
  {
	  token = g_scanner_get_next_token (scanner);
	  if (token != wanted_token)
	    {
	      return wanted_token;
	    }
  }
  
  if (wanted_token != G_TOKEN_IDENTIFIER)
  {
	  token = g_scanner_get_next_token (scanner);
	  if (token != G_TOKEN_EQUAL_SIGN)
	    {
	      return G_TOKEN_EQUAL_SIGN;
	    }
  token = g_scanner_get_next_token (scanner);
  }

  if (token != G_TOKEN_IDENTIFIER)
    *retval = return_default;
  else
    if (!translate_enum(scanner->value.v_identifier, retval)) *retval = return_default;
  
  return G_TOKEN_NONE;
}

guint 
smooth_rc_parse_boolean(GScanner *scanner,  
                    GTokenType wanted_token, 
		    gboolean return_default, 
		    guint *retval)
{
  guint token;
  guint result=0;

  token = smooth_rc_parse_custom_enum (scanner, wanted_token, (SmoothSmoothTranslateEnumFunc)SmoothTranslateBooleanName, return_default, &result);	
  
  *retval = result;
  
  return token;
}

guint 
smooth_rc_parse_line(GScanner *scanner, 
                 GTokenType wanted_token, 
                 SmoothLinePart *retval)
{
  guint token;

  token = g_scanner_cur_token (scanner);
  if (token != wanted_token)
  {
	  token = g_scanner_get_next_token (scanner);
	  if (token != wanted_token)
	    {
	      return wanted_token;
	    }
  }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_STYLE:
      token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateLineStyleName, SMOOTH_BEVEL_STYLE_DEFAULT,  &retval->style);
      break;
    case TOKEN_THICKNESS:
      token = smooth_rc_parse_int (scanner, TOKEN_THICKNESS, 2, &retval->thickness, 1, 10);
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_focus(GScanner *scanner, 
                  GTokenType wanted_token, 
                  smooth_focus_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_PATTERN:
      {
        GtkStateType state;
        
        token = g_scanner_get_next_token (scanner);
  
        token = gtk_rc_parse_state (scanner, &state);
        if (token != G_TOKEN_NONE)
          return token;

        token = g_scanner_get_next_token (scanner);
        if (token != G_TOKEN_EQUAL_SIGN)
          return G_TOKEN_EQUAL_SIGN;

        token = g_scanner_get_next_token (scanner);
        if (token == G_TOKEN_STRING)
          if (scanner->value.v_string)
            retval->pattern[state]= g_strdup(scanner->value.v_string);   
      }

      break;
    case TOKEN_WIDTH:
      {
        GtkStateType state;
	gboolean negate = FALSE;
        
        token = g_scanner_get_next_token (scanner);
  
        token = gtk_rc_parse_state (scanner, &state);
        if (token != G_TOKEN_NONE)
          return token;

        token = g_scanner_get_next_token (scanner);
        if (token != G_TOKEN_EQUAL_SIGN)
          return G_TOKEN_EQUAL_SIGN;

        token = g_scanner_get_next_token (scanner);
        if (g_scanner_peek_next_token (scanner) == '-')
        {
          g_scanner_get_next_token (scanner); /* eat sign */
          negate = TRUE;
        }

        token = g_scanner_get_next_token (scanner);

        if (token != G_TOKEN_INT)
          retval->line_width[state] = 1;
        else
          retval->line_width[state] = (guint) scanner->value.v_int;

        if (negate) retval->line_width[state] = -(retval->line_width[state]); 

        if (retval->line_width[state] < -5) retval->line_width[state] = -5; 
        if ((retval->line_width[state] > 5)) retval->line_width[state] = 5; 

        retval->use_line_width[state] = TRUE;
      }
      break;
    case TOKEN_FOREGROUND:
      {
        GtkStateType state;
	GdkColor color;
	
        token = g_scanner_get_next_token (scanner);
        if (token != TOKEN_FOREGROUND)
          return TOKEN_FOREGROUND;
	  
        token = gtk_rc_parse_state (scanner, &state);
        if (token != G_TOKEN_NONE)
          return token;
  
        token = g_scanner_get_next_token (scanner);
        if (token != G_TOKEN_EQUAL_SIGN)
          return G_TOKEN_EQUAL_SIGN;

        retval->use_foreground[SmoothGtkWidgetState(state)] = TRUE;
        token = gtk_rc_parse_color (scanner, &color);        
	SmoothColorAssignGdkColor(&retval->foreground[SmoothGtkWidgetState(state)], color, 1.0);
      }
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}


static guint 
smooth_rc_parse_arrow(GScanner *scanner, 
                  SmoothArrow *retval)
{
  guint token;

  token = g_scanner_cur_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
      case TOKEN_STYLE:
	token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateArrowStyleName, SMOOTH_ARROW_STYLE_DEFAULT, &retval->Style);
	retval->HasStyle = SmoothTrue;

	switch (retval->Style)
	{
		case SMOOTH_ARROW_STYLE_DEPRECATED_CLEANICE :
			retval->Style = SMOOTH_ARROW_STYLE_CLEAN;
			retval->Tail = 0;
			retval->HasTail = SmoothTrue;
		break;

		case SMOOTH_ARROW_STYLE_DEPRECATED_XFCE :
			retval->Style = SMOOTH_ARROW_STYLE_CLEAN;
			retval->Tail = 1;
			retval->HasTail = SmoothTrue;
		break;

		case SMOOTH_ARROW_STYLE_DEPRECATED_THINICE :
			retval->Style = SMOOTH_ARROW_STYLE_CLEAN;
			retval->Tail = 2;
			retval->HasTail = SmoothTrue;
		break;

		default :
		break;
	}
	break;
	
      case TOKEN_SOLID:
	token = smooth_rc_parse_boolean (scanner, TOKEN_SOLID, DEFAULT_SOLIDARROW, &retval->Solid);
	retval->HasSolid = SmoothTrue;
	break;	  
	
      case TOKEN_ETCHED:
	token = smooth_rc_parse_boolean (scanner, TOKEN_ETCHED, DEFAULT_ETCHEDARROW, &retval->Etched);
	retval->HasEtched = SmoothTrue;
	break;	  
      case TOKEN_TAIL:
        token = smooth_rc_parse_int (scanner, TOKEN_TAIL, 0, &retval->Tail, 0, 3);
	retval->HasTail = SmoothTrue;
        break;
      case TOKEN_XPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &retval->XPadding, -25, 25);
	retval->HasXPadding = SmoothTrue;
        break;
      case TOKEN_YPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &retval->YPadding, -25, 25);
	retval->HasYPadding = SmoothTrue;
        break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_arrow_part(GScanner *scanner, 
                       GTokenType wanted_token, 
                       SmoothArrowPart *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
      case TOKEN_STYLE:
	if (!retval->DefaultStyle)
		retval->DefaultStyle = g_new0(SmoothArrow, 1);

	token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateArrowStyleName, SMOOTH_ARROW_STYLE_DEFAULT, &retval->DefaultStyle->Style);
	retval->DefaultStyle->HasStyle = SmoothTrue;

	switch (retval->DefaultStyle->Style)
	{
		case SMOOTH_ARROW_STYLE_DEPRECATED_CLEANICE :
			retval->DefaultStyle->Style = SMOOTH_ARROW_STYLE_CLEAN;
			retval->DefaultStyle->Tail = 0;
			retval->DefaultStyle->HasTail = SmoothTrue;
		break;

		case SMOOTH_ARROW_STYLE_DEPRECATED_XFCE :
			retval->DefaultStyle->Style = SMOOTH_ARROW_STYLE_CLEAN;
			retval->DefaultStyle->Tail = 1;
			retval->DefaultStyle->HasTail = SmoothTrue;
		break;

		case SMOOTH_ARROW_STYLE_DEPRECATED_THINICE :
			retval->DefaultStyle->Style = SMOOTH_ARROW_STYLE_CLEAN;
			retval->DefaultStyle->Tail = 2;
			retval->DefaultStyle->HasTail = SmoothTrue;
		break;

		default :
		break;
	}
	break;
	
      case TOKEN_SOLID:
	if (!retval->DefaultStyle)
		retval->DefaultStyle = g_new0(SmoothArrow, 1);

	token = smooth_rc_parse_boolean (scanner, TOKEN_SOLID, DEFAULT_SOLIDARROW, &retval->DefaultStyle->Solid);
	retval->DefaultStyle->HasSolid = SmoothTrue;
	break;	  
      case TOKEN_ETCHED:
	if (!retval->DefaultStyle)
		retval->DefaultStyle = g_new0(SmoothArrow, 1);

	token = smooth_rc_parse_boolean (scanner, TOKEN_ETCHED, DEFAULT_ETCHEDARROW, &retval->DefaultStyle->Etched);
	retval->DefaultStyle->HasEtched = SmoothTrue;
	break;	  
      case TOKEN_TAIL:
	if (!retval->DefaultStyle)
		retval->DefaultStyle = g_new0(SmoothArrow, 1);

        token = smooth_rc_parse_int (scanner, TOKEN_TAIL, 0, &retval->DefaultStyle->Tail, 0, 3);
	retval->DefaultStyle->HasTail = SmoothTrue;
        break;
      case TOKEN_XPADDING:
	if (!retval->DefaultStyle)
		retval->DefaultStyle = g_new0(SmoothArrow, 1);

        token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &retval->DefaultStyle->XPadding, -25, 25);
	retval->DefaultStyle->HasXPadding = SmoothTrue;
        break;
      case TOKEN_YPADDING:
	if (!retval->DefaultStyle)
		retval->DefaultStyle = g_new0(SmoothArrow, 1);

        token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &retval->DefaultStyle->YPadding, -25, 25);
	retval->DefaultStyle->HasYPadding = SmoothTrue;
        break;

	case G_TOKEN_IDENTIFIER:
	{
		int type = SMOOTH_ARROW_TYPE_NONE;

		token = smooth_rc_parse_custom_enum(scanner, G_TOKEN_IDENTIFIER, (SmoothSmoothTranslateEnumFunc)SmoothTranslateArrowTypeName, SMOOTH_ARROW_TYPE_NONE, &type);
		
		if (type == SMOOTH_ARROW_TYPE_NONE) 
		{
		       	int state;

			token = smooth_rc_parse_custom_enum(scanner, G_TOKEN_IDENTIFIER, (SmoothSmoothTranslateEnumFunc)SmoothTranslateStateName, -1, &state);
			
			if (state == -1)
				return token;
			 
			token = g_scanner_peek_next_token (scanner);
	
			if (token != G_TOKEN_LEFT_CURLY)
				return G_TOKEN_LEFT_CURLY;

			if (!retval->DefaultStateStyles)
				retval->DefaultStateStyles = g_new0(SmoothArrow, 5);

			token = smooth_rc_parse_arrow (scanner, &retval->DefaultStateStyles[state]);
		}
		else
		{		
			token = g_scanner_get_next_token(scanner);
		
			if ((token == G_TOKEN_LEFT_BRACE))
			{
		        	GtkStateType state;
		  
				token = g_scanner_get_next_token(scanner);
				
				token = smooth_rc_parse_custom_enum(scanner, G_TOKEN_IDENTIFIER, (SmoothSmoothTranslateEnumFunc)SmoothTranslateStateName, -1, &state);
			
				token = g_scanner_get_next_token (scanner);
	
				if (token != G_TOKEN_RIGHT_BRACE)
					return token;
	  
				token = g_scanner_peek_next_token (scanner);
	
				if (token != G_TOKEN_LEFT_CURLY)
					return G_TOKEN_LEFT_CURLY;
	
				if (!retval->Styles[state])
					retval->Styles[state] = g_new0(SmoothArrow, SMOOTH_ARROW_TYPE_COUNT);

				token = smooth_rc_parse_arrow (scanner, &retval->Styles[state][type]);
			}
			else
			{
				token = g_scanner_peek_next_token (scanner);
				while (token != G_TOKEN_RIGHT_CURLY) {
					switch (token) {
						case TOKEN_STYLE:
							if (!retval->DefaultTypeStyles)
								retval->DefaultTypeStyles = g_new0(SmoothArrow, SMOOTH_ARROW_TYPE_COUNT);

							token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateArrowStyleName, SMOOTH_ARROW_STYLE_DEFAULT, 	&retval->DefaultTypeStyles[type].Style);
							retval->DefaultTypeStyles[type].HasStyle = SmoothTrue;
	
							switch (retval->DefaultTypeStyles[type].Style)
							{
								case SMOOTH_ARROW_STYLE_DEPRECATED_CLEANICE :
									retval->DefaultTypeStyles[type].Style = SMOOTH_ARROW_STYLE_CLEAN;
									retval->DefaultTypeStyles[type].Tail = 0;
									retval->DefaultTypeStyles[type].HasTail = SmoothTrue;
								break;
	
								case SMOOTH_ARROW_STYLE_DEPRECATED_XFCE :
									retval->DefaultTypeStyles[type].Style = SMOOTH_ARROW_STYLE_CLEAN;
									retval->DefaultTypeStyles[type].Tail = 1;
									retval->DefaultTypeStyles[type].HasTail = SmoothTrue;
								break;
	
								case SMOOTH_ARROW_STYLE_DEPRECATED_THINICE :
									retval->DefaultTypeStyles[type].Style = SMOOTH_ARROW_STYLE_CLEAN;
									retval->DefaultTypeStyles[type].Tail = 2;
									retval->DefaultTypeStyles[type].HasTail = SmoothTrue;
								break;
		
								default :
								break;
							}
						break;
		
						case TOKEN_SOLID:
							if (!retval->DefaultTypeStyles)
								retval->DefaultTypeStyles = g_new0(SmoothArrow, SMOOTH_ARROW_TYPE_COUNT);

							token = smooth_rc_parse_boolean (scanner, TOKEN_SOLID, DEFAULT_SOLIDARROW, &retval->DefaultTypeStyles[type].Solid);
							retval->DefaultTypeStyles[type].HasSolid = SmoothTrue;
						break;	  
	
						case TOKEN_ETCHED:
							if (!retval->DefaultTypeStyles)
								retval->DefaultTypeStyles = g_new0(SmoothArrow, SMOOTH_ARROW_TYPE_COUNT);

							token = smooth_rc_parse_boolean (scanner, TOKEN_ETCHED, DEFAULT_ETCHEDARROW, &retval->DefaultTypeStyles[type].Etched);
							retval->DefaultTypeStyles[type].HasEtched = SmoothTrue;
						break;	  
	
	      					case TOKEN_TAIL:
							if (!retval->DefaultTypeStyles)
								retval->DefaultTypeStyles = g_new0(SmoothArrow, SMOOTH_ARROW_TYPE_COUNT);

							token = smooth_rc_parse_int (scanner, TOKEN_TAIL, 0, &retval->DefaultTypeStyles[type].Tail, 0, 3);
							retval->DefaultTypeStyles[type].HasTail = SmoothTrue;
						break;
	
						case G_TOKEN_IDENTIFIER:
						{
					        	GtkStateType state;
		  
							token = smooth_rc_parse_custom_enum(scanner, G_TOKEN_IDENTIFIER, (SmoothSmoothTranslateEnumFunc)SmoothTranslateStateName, -1, &state);

							if (state == -1)
								return token;
			 
							token = g_scanner_peek_next_token (scanner);

							if (token != G_TOKEN_LEFT_CURLY)
								return G_TOKEN_LEFT_CURLY;

							if (!retval->Styles[state])
								retval->Styles[state] = g_new0(SmoothArrow, SMOOTH_ARROW_TYPE_COUNT);
	
							token = smooth_rc_parse_arrow (scanner, &retval->Styles[state][type]);
						}
						break;
	
						default:
							g_scanner_get_next_token (scanner);
							token = G_TOKEN_RIGHT_CURLY;
						break;
					}
					
					token = g_scanner_get_next_token (scanner);
				}
			}
		}
	}
	break;

    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_fill(GScanner *scanner, 
                 GTokenType wanted_token, 
                 SmoothFillPart *retval)
{
  guint token;

  token = g_scanner_cur_token (scanner);
  if (token != wanted_token)
  {
	  token = g_scanner_get_next_token (scanner);
	  if (token != wanted_token)
	    {
	      return wanted_token;
	    }
  }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_STYLE:
      token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateFillStyleName, SMOOTH_FILL_STYLE_DEFAULT,  &THEME_PART(retval)->style);
      break;
    case TOKEN_DITHER_DEPTH:
    {
        token = g_scanner_get_next_token (scanner);
        if (token != TOKEN_DITHER_DEPTH)
          return TOKEN_DITHER_DEPTH;
  
	token = g_scanner_peek_next_token (scanner);
	
	if (token == G_TOKEN_LEFT_BRACE) {
	  GtkStateType state;
  
          token = gtk_rc_parse_state (scanner, &state);
          if (token != G_TOKEN_NONE)
            return token;
  
          token = smooth_rc_parse_int (scanner, G_TOKEN_EQUAL_SIGN, 8, &retval->dither_depth[state], 0, 24);
          retval->use_dither_depth[state] = TRUE;
        }
        else
        {
          token = smooth_rc_parse_int (scanner, TOKEN_DITHER_DEPTH, 8, &retval->default_dither_depth, 0, 24);
          retval->default_dither_depth_set = TRUE;
        }
    }
    break;
    case TOKEN_HDIRECTION:
      token = smooth_rc_parse_custom_enum(scanner, TOKEN_HDIRECTION, (SmoothSmoothTranslateEnumFunc)SmoothTranslateGradientDirectionName, DEFAULT_HGRADIENTDIRECTION,  &retval->gradient_direction[TRUE]);
      break;
    case TOKEN_VDIRECTION:
      token = smooth_rc_parse_custom_enum(scanner, TOKEN_VDIRECTION, (SmoothSmoothTranslateEnumFunc)SmoothTranslateGradientDirectionName, DEFAULT_VGRADIENTDIRECTION,  &retval->gradient_direction[FALSE]);
      break;
    case TOKEN_SHADE1_VALUE:
      token = smooth_rc_parse_float (scanner, TOKEN_SHADE1_VALUE, 1.3, &retval->shade1, 0.0, 2.5);
      break;
    case TOKEN_SHADE2_VALUE:
      token = smooth_rc_parse_float (scanner, TOKEN_SHADE2_VALUE, 0.7, &retval->shade2, 0.0, 2.5);
      break;
    case TOKEN_COLOR1:
      {
        GtkStateType state;
	GdkColor color;
	
        token = g_scanner_get_next_token (scanner);
        if (token != TOKEN_COLOR1)
          return TOKEN_COLOR1;
  
        token = gtk_rc_parse_state (scanner, &state);
        if (token != G_TOKEN_NONE)
          return token;
  
        token = g_scanner_get_next_token (scanner);
        if (token != G_TOKEN_EQUAL_SIGN)
          return G_TOKEN_EQUAL_SIGN;

        color.pixel = -1;
	token = gtk_rc_parse_color (scanner, &color);        
	SmoothColorAssignGdkColor(&retval->color1[SmoothGtkWidgetState(state)], color, 1.0);

        retval->use_color1[SmoothGtkWidgetState(state)] = TRUE;
      }
      break;
    case TOKEN_COLOR2:
      {
	GdkColor color;
        GtkStateType state;
  
        token = g_scanner_get_next_token (scanner);
        if (token != TOKEN_COLOR2)
          return TOKEN_COLOR2;
	  
        token = gtk_rc_parse_state (scanner, &state);
        if (token != G_TOKEN_NONE)
          return token;
  
        token = g_scanner_get_next_token (scanner);
        if (token != G_TOKEN_EQUAL_SIGN)
          return G_TOKEN_EQUAL_SIGN;

        color.pixel = -1;
	token = gtk_rc_parse_color (scanner, &color);        
	SmoothColorAssignGdkColor(&retval->color2[SmoothGtkWidgetState(state)], color, 1.0);
	
        retval->use_color2[SmoothGtkWidgetState(state)] = TRUE;
      }
      break;
    case TOKEN_FILE:
      {
        GtkStateType state;
          
        token = g_scanner_get_next_token (scanner);
        if (token != TOKEN_FILE)
          return TOKEN_FILE;
  
        token = gtk_rc_parse_state (scanner, &state);
        if (token != G_TOKEN_NONE)
          return token;
  
	token = smooth_rc_parse_pixmap(scanner, G_TOKEN_EQUAL_SIGN, NULL, &retval->file_name[state]);
      }	
      break;
    case TOKEN_QUADRATIC_GRADIENT:
      token = smooth_rc_parse_boolean (scanner, TOKEN_QUADRATIC_GRADIENT, FALSE, &retval->quadratic_gradient);
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_edge(GScanner *scanner, 
                 GTokenType wanted_token, 
                 smooth_edge_style *retval)
{
  smooth_edge_style junk_edge;
  SmoothFillPart junk_fill;
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_STYLE:
      token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateEdgeStyleName, DEFAULT_EDGESTYLE, &retval->style);
      break;
    case TOKEN_LINE:
      token = smooth_rc_parse_line (scanner, TOKEN_LINE, &retval->line);
      retval->use_line = TRUE;
      break;
    case TOKEN_EDGE:    
      token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &junk_edge);
      break;
    case TOKEN_FILL :
      token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &junk_fill);
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_button_default(GScanner *scanner, 
                           GTokenType wanted_token, 
                           smooth_part_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
      case TOKEN_STYLE:
	token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateButtonDefaultStyleName, SMOOTH_BUTTON_DEFAULT_STYLE_DEFAULT, &THEME_PART(retval)->style);
	break;
	
      case TOKEN_LINE:
        token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
        THEME_PART(retval)->use_line = TRUE;
        break;

      case TOKEN_FILL :
        token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
        THEME_PART(retval)->use_fill = TRUE;
        break;
  
      case TOKEN_EDGE:
        token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
        break;

      case TOKEN_XPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
        break;

      case TOKEN_YPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
        break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_button(GScanner *scanner, 
                   GTokenType wanted_token, 
                   smooth_button_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
      case TOKEN_BUTTON_DEFAULT:
	token = smooth_rc_parse_button_default (scanner, TOKEN_BUTTON_DEFAULT, &retval->button_default);
	retval->use_button_default = TRUE;
	break;	  

      case TOKEN_BUTTON_EMBEDDABLE:
	token = smooth_rc_parse_boolean (scanner, TOKEN_BUTTON_EMBEDDABLE, FALSE,  &retval->embeddable);
	break;	  

      case TOKEN_DEFAULT_TRIANGLE:
	token = smooth_rc_parse_boolean (scanner, TOKEN_DEFAULT_TRIANGLE, DEFAULT_BUTTONDEFAULTTRIANGLE,  &retval->default_triangle);
	break;	  

      case TOKEN_LINE:
        token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
        THEME_PART(retval)->use_line = TRUE;
        break;

      case TOKEN_FILL :
        token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
        THEME_PART(retval)->use_fill = TRUE;
        break;

      case TOKEN_EDGE:
        token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
        break;

      case TOKEN_XPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
        break;

      case TOKEN_YPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
        break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_active_tab(GScanner *scanner, 
                       GTokenType wanted_token, 
                       smooth_part_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
      case TOKEN_STYLE:
	token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateTabStyleName, DEFAULT_TABSTYLE, &THEME_PART(retval)->style);
	break;
	
      case TOKEN_LINE:
        token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
        THEME_PART(retval)->use_line = TRUE;
        break;

      case TOKEN_FILL :
        token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
        THEME_PART(retval)->use_fill = TRUE;
        break;
  
      case TOKEN_EDGE:
        token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
        break;

      case TOKEN_XPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
        break;

      case TOKEN_YPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
        break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_tab(GScanner *scanner, 
                GTokenType wanted_token, 
                smooth_tab_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
      case TOKEN_STYLE:
	token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateTabStyleName, DEFAULT_TABSTYLE, &THEME_PART(retval)->style);
	break;
	
      case TOKEN_ACTIVE_TAB:
	token = smooth_rc_parse_active_tab (scanner, TOKEN_ACTIVE_TAB, &retval->active_tab);
	retval->use_active_tab = TRUE;
	break;	  

      case TOKEN_HIGHLIGHT:
	token = smooth_rc_parse_boolean (scanner, TOKEN_HIGHLIGHT, FALSE, &retval->highlight);
	break;	  

      case TOKEN_LINE:
        token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
        THEME_PART(retval)->use_line = TRUE;
        break;

      case TOKEN_FILL :
        token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
        THEME_PART(retval)->use_fill = TRUE;
        break;

      case TOKEN_EDGE:
        token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
        break;

      case TOKEN_XPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
        break;

      case TOKEN_YPADDING:
        token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
        break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_option(GScanner *scanner, 
                   GTokenType wanted_token, 
                   smooth_check_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_STYLE:
      token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateCheckStyleName, SMOOTH_CHECKMARK_STYLE_DEFAULT_OPTION, &THEME_PART(retval)->style);
      break;
    case TOKEN_FILL :
      token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
      THEME_PART(retval)->use_fill = TRUE;
      break;
    case TOKEN_EDGE:
      token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
      break;
    case TOKEN_MOTIF:
      token = smooth_rc_parse_boolean (scanner, TOKEN_MOTIF, TRUE, &retval->motif);
      break;
    case TOKEN_LINE:
      token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
      THEME_PART(retval)->use_line = TRUE;
      break;
    case TOKEN_XPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
      break;
    case TOKEN_YPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_grip(GScanner *scanner, 
                 GTokenType wanted_token, 
                 smooth_grip_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_STYLE:
      token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateGripStyleName, DEFAULT_GRIPSTYLE,  &THEME_PART(retval)->style);
      break;
    case TOKEN_LINE:
      token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
      THEME_PART(retval)->use_line = TRUE;
      break;
    case TOKEN_EDGE:
      token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
      break;
    case TOKEN_FILL :
      token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
      THEME_PART(retval)->use_fill = TRUE;
      break;
    case TOKEN_XPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
      break;
    case TOKEN_YPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
      break;
    case TOKEN_COUNT:
      token = smooth_rc_parse_int (scanner, TOKEN_COUNT, DEFAULT_GRIPCOUNT, &retval->count, 1, -1);
      break;
    case TOKEN_SPACING:
      token = smooth_rc_parse_int (scanner, TOKEN_SPACING, DEFAULT_GRIPSPACING, &retval->spacing, 0, -1);
      break;
    case TOKEN_TOOLBAR_OVERLAP:
      token = smooth_rc_parse_boolean (scanner, TOKEN_TOOLBAR_OVERLAP, DEFAULT_GRIPOVERLAP, &retval->overlap);
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_stepper(GScanner *scanner, 
                 GTokenType wanted_token, 
                 SmoothStepperStyle *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_LINE:
      token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
      THEME_PART(retval)->use_line = TRUE;
      break;
    case TOKEN_EDGE:
      token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
      break;
    case TOKEN_FILL :
      token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
      THEME_PART(retval)->use_fill = TRUE;
      break;
    case TOKEN_XPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
      break;
    case TOKEN_YPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
      break;
    case TOKEN_ARROW:
      token = smooth_rc_parse_arrow_part (scanner, TOKEN_ARROW, &retval->Arrow);
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_check(GScanner *scanner, 
                  GTokenType wanted_token,
                  smooth_check_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_STYLE:
      token = smooth_rc_parse_custom_enum(scanner, TOKEN_STYLE, (SmoothSmoothTranslateEnumFunc)SmoothTranslateCheckStyleName, SMOOTH_CHECKMARK_STYLE_DEFAULT,  &THEME_PART(retval)->style);
      break;
    case TOKEN_FILL :
      token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
      THEME_PART(retval)->use_fill = TRUE;
      break;
    case TOKEN_MOTIF:
      token = smooth_rc_parse_boolean (scanner, TOKEN_MOTIF, TRUE, &retval->motif);
      break;
    case TOKEN_EDGE:
      token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
      break;
    case TOKEN_XPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
      break;
    case TOKEN_YPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
      break;
    case TOKEN_LINE:
      token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
      THEME_PART(retval)->use_line = TRUE;
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_generic_part(GScanner *scanner, 
                         GTokenType wanted_token, 
                         smooth_part_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_FILL :
      token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
      THEME_PART(retval)->use_fill = TRUE;
      break;
    case TOKEN_LINE:
      token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
      THEME_PART(retval)->use_line = TRUE;
      break;
    case TOKEN_EDGE:
      token = smooth_rc_parse_edge (scanner, TOKEN_EDGE, &THEME_PART(retval)->edge);
      break;
    case TOKEN_XPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
      break;
    case TOKEN_YPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

guint 
smooth_rc_parse_trough_part(GScanner *scanner, 
                        GTokenType wanted_token, 
                        smooth_trough_style *retval)
{
  guint token;

  token = g_scanner_get_next_token (scanner);
  if (token != wanted_token)
    {
      return wanted_token;
    }

  token = g_scanner_get_next_token (scanner);
  if (token != G_TOKEN_LEFT_CURLY)
    return G_TOKEN_LEFT_CURLY;

  token = g_scanner_peek_next_token (scanner);
  while (token != G_TOKEN_RIGHT_CURLY) {
    switch (token) {
    case TOKEN_TROUGH_SHOW_VALUE:
      token = smooth_rc_parse_boolean (scanner, TOKEN_TROUGH_SHOW_VALUE, DEFAULT_TROUGH_SHOW_VALUE, &retval->show_value);
      break;
    case TOKEN_FILL :
      token = smooth_rc_parse_fill (scanner, TOKEN_FILL, &THEME_PART(retval)->fill);
      THEME_PART(retval)->use_fill = TRUE;
      break;
    case TOKEN_LINE:
      token = smooth_rc_parse_line (scanner, TOKEN_LINE, &THEME_PART(retval)->line);
      THEME_PART(retval)->use_line = TRUE;
      break;
    case TOKEN_XPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_XPADDING, 0, &THEME_PART(retval)->xpadding, -25, 25);
      break;
    case TOKEN_YPADDING:
      token = smooth_rc_parse_int (scanner, TOKEN_YPADDING, 0, &THEME_PART(retval)->ypadding, -25, 25);
      break;
    default:
      g_scanner_get_next_token (scanner);
      token = G_TOKEN_RIGHT_CURLY;
      break;
    }

    token = g_scanner_peek_next_token (scanner);
  }

  g_scanner_get_next_token (scanner);
  token = G_TOKEN_NONE;

  return token;
}

void 
smooth_part_init(smooth_part_style *part, 
          gint partstyle)
{
  gint i;
  
  part->style      	= partstyle;

  part->use_line  	= FALSE;
  part->line.style      = SMOOTH_BEVEL_STYLE_DEFAULT;
  part->line.thickness  = DEFAULT_LINETHICKNESS;

  part->use_fill  	= FALSE;
  part->fill.style      = SMOOTH_FILL_STYLE_DEFAULT;
  part->fill.gradient_direction[FALSE] = DEFAULT_VGRADIENTDIRECTION;
  part->fill.gradient_direction[TRUE] = DEFAULT_HGRADIENTDIRECTION;
  part->fill.quadratic_gradient = FALSE;
  part->fill.default_dither_depth = 8;
  part->fill.default_dither_depth_set = FALSE;

  part->fill.shade1 = 1.3;
  part->fill.shade2 = 0.7;

  for (i=0; i < 5; i++) {
    part->fill.use_color1[i] = FALSE;
    part->fill.use_color2[i] = FALSE;
    part->fill.use_dither_depth[i] = FALSE;

    part->fill.file_name[i] = NULL;
  }

  memset(&part->edge, 0, sizeof(smooth_edge_style));

  part->edge.use_line        = FALSE;
  part->edge.line.style      = SMOOTH_BEVEL_STYLE_DEFAULT;
  part->edge.line.thickness  = DEFAULT_LINETHICKNESS;
  
  part->xpadding      	= 0;
  part->ypadding  	= 0;
}

void 
smooth_part_merge (smooth_part_style *dest_part, 
            smooth_part_style *src_part)
{
  gint i;
  
  dest_part->style           = src_part->style;

  dest_part->use_line  	     = src_part->use_line;
  dest_part->line.style      = src_part->line.style;
  dest_part->line.thickness  = src_part->line.thickness;

  dest_part->use_fill  	     = src_part->use_fill;
  dest_part->fill.style = src_part->fill.style;
  dest_part->fill.quadratic_gradient = src_part->fill.quadratic_gradient;
  dest_part->fill.gradient_direction[FALSE] = src_part->fill.gradient_direction[FALSE];
  dest_part->fill.gradient_direction[TRUE] = src_part->fill.gradient_direction[TRUE];
  dest_part->fill.shade1 = src_part->fill.shade1;
  dest_part->fill.shade2 = src_part->fill.shade2;
  dest_part->fill.default_dither_depth = src_part->fill.default_dither_depth;
  dest_part->fill.default_dither_depth_set = src_part->fill.default_dither_depth_set;
  
  for (i=0; i < 5; i++) {
    dest_part->fill.use_color1[i] = src_part->fill.use_color1[i];
    if (src_part->fill.use_color1[i])
      dest_part->fill.color1[i] = src_part->fill.color1[i];

    dest_part->fill.use_color2[i] = src_part->fill.use_color2[i];
    if (src_part->fill.use_color2[i])
      dest_part->fill.color2[i] = src_part->fill.color2[i];

    dest_part->fill.use_dither_depth[i] = src_part->fill.use_dither_depth[i];
    if (src_part->fill.use_dither_depth[i])
	    dest_part->fill.dither_depth[i] = src_part->fill.dither_depth[i];

	if (src_part->fill.file_name[i]) 
	{
		if (!dest_part->fill.file_name[i])
			dest_part->fill.file_name[i] =  g_string_sized_new(src_part->fill.file_name[i]->len);
		
		g_string_assign(dest_part->fill.file_name[i], src_part->fill.file_name[i]->str);
	}
  }
  
  dest_part->edge.use_line  	  = src_part->edge.use_line;
  dest_part->edge.line.style      = src_part->edge.line.style;
  dest_part->edge.line.thickness  = src_part->edge.line.thickness;

  dest_part->xpadding  	     = src_part->xpadding;
  dest_part->ypadding  	     = src_part->ypadding;
}

void
smooth_arrow_merge (SmoothArrowPart *dest_arrow,
             SmoothArrowPart *src_arrow)
{
  SmoothArrow dummy;

  /* what is this for? */
  smooth_style_get_arrow(src_arrow, 0, 0, &dummy);

  SmoothCopyArrowPart(dest_arrow, src_arrow);

  /* Only used to inherit in one rc style. So this should work fine, I think. */
  dest_arrow->Inherited = NULL;
}

/* Smooth RC Parts Config -> Smooth Drawing Struct */
void
smooth_style_get_border(GtkStyle * style,
		            GtkStateType state_type,
	  		    GtkShadowType shadow_type,
			    const gchar * detail,
			    void * part,
			    gdouble CornerRoundness,
			    SmoothBorder *Border)
{
	switch (shadow_type)
	{
		case GTK_SHADOW_NONE:
			Border->Type = SMOOTH_BORDER_TYPE_NONE;
		break;	

		case GTK_SHADOW_ETCHED_IN:
			Border->Type = SMOOTH_BORDER_TYPE_ETCHED;
		break;	

		case GTK_SHADOW_ETCHED_OUT:
			Border->Type = SMOOTH_BORDER_TYPE_ENGRAVED;
		break;	

		case GTK_SHADOW_IN:
			Border->Type = SMOOTH_BORDER_TYPE_IN;
		break;	

		case GTK_SHADOW_OUT:
		default:
			Border->Type = SMOOTH_BORDER_TYPE_OUT;
		break;	

	} 

	Border->Style = EDGE_LINE_STYLE(style, part);
        if (Border->Style == SMOOTH_BEVEL_STYLE_DEPRECATED_WONDERLAND)
        {
		if (Border->Type == SMOOTH_BORDER_TYPE_IN)
		{
			if ((CHECK_DETAIL(detail, "button") || CHECK_DETAIL(detail, "togglebutton") || CHECK_DETAIL(detail, "optionmenu") || 
				CHECK_DETAIL(detail, "slider") || CHECK_DETAIL(detail, "vscrollbar") || CHECK_DETAIL(detail, "hscrollbar")))
			{ 
				Border->Style = SMOOTH_BEVEL_STYLE_COLD;
			} 
			else 
			{
				Border->Style = SMOOTH_BEVEL_STYLE_ICED;
			}
		}
		else
		{
			Border->Style = SMOOTH_BEVEL_STYLE_COLD;
		}
	}	
		
	Border->CornerRadius = CornerRoundness;
	Border->Thickness = EDGE_LINE_THICKNESS(style, part);
}

void
smooth_style_get_arrow(SmoothArrowPart *Part,
				GtkStateType State,
				GtkArrowType ArrowType,
				SmoothArrow *Arrow)
{
	/* possible ways for setting values in order of 
	   priority -
		1.) DefaultStyle is used if nothing else is set,
			arrow {
				style = value
				solid = value
				etched = value
				tail = value
				...
			}
	

		2.) DefaultStateStyles is the defaults per state
			arrow {
				...
				state {
					style = value
					solid = value
					etched = value
					tail = value
				}
				...
			}
		3.) DefaultTypeStyles is the default per arrow type
			arrow {
				...
				type {
					style = value
					solid = value
					etched = value
					tail = value
				}
				...
			}
		
		4.) Styles is the final override
			arrow {
				...
				type[STATE] {
					style = value
					solid = value
					etched = value
					tail = value
				}
				...
			}

		5.) Same as 4
			arrow {
				...
				type {
					STATE {
						style = value
						solid = value
						etched = value
						tail = value
					}
				}
				...
			}
	*/
	SmoothArrowType type;

	switch (ArrowType)
	{
		case GTK_ARROW_UP : 
			type = SMOOTH_ARROW_TYPE_UP;
		break;
		
		case GTK_ARROW_LEFT : 
			type = SMOOTH_ARROW_TYPE_LEFT;
		break;
		
		case GTK_ARROW_DOWN : 
			type = SMOOTH_ARROW_TYPE_DOWN;
		break;
		
		case GTK_ARROW_RIGHT : 
			type = SMOOTH_ARROW_TYPE_RIGHT;
		break;
		
		default :
			type = SMOOTH_ARROW_TYPE_NONE;
		break;
	}

	if (Part->CompositeArrowsSet[State][type])
	{
		memcpy(Arrow, &Part->CompositeArrows[State][type], sizeof(SmoothArrow));
	}
	else
	{	
		SmoothArrow Inherited;
	
		if (Part->Inherited)
		{
			smooth_style_get_arrow(Part->Inherited, State, ArrowType, &Inherited);
			
			memcpy(Arrow, &Inherited, sizeof(SmoothArrow));
		}
		else
		{
			memcpy(Arrow, Part->DefaultStyle, sizeof(SmoothArrow));
		}
		
		Arrow->Type = type;

		if ((Part->Styles[State]) && (Part->Styles[State][type].HasStyle))
			Arrow->Style = Part->Styles[State][type].Style;
		else
			if ((Part->DefaultTypeStyles) && (Part->DefaultTypeStyles[type].HasStyle))
				Arrow->Style = Part->DefaultTypeStyles[type].Style;
		else
			if ((Part->DefaultStateStyles) && (Part->DefaultStateStyles[State].HasStyle))
				Arrow->Style = Part->DefaultStateStyles[State].Style;
		else
			if ((Part->DefaultStyle) && (Part->DefaultStyle->HasStyle))
				Arrow->Style = Part->DefaultStyle->Style;
		
		if ((Part->Styles[State]) && (Part->Styles[State][type].HasEtched))
			Arrow->Etched = Part->Styles[State][type].Etched;
		else
			if ((Part->DefaultTypeStyles) && (Part->DefaultTypeStyles[type].HasEtched))
				Arrow->Etched = Part->DefaultTypeStyles[type].Etched;
		else
			if ((Part->DefaultStateStyles) && (Part->DefaultStateStyles[State].HasEtched))
				Arrow->Etched = Part->DefaultStateStyles[State].Etched;
		else
			if ((Part->DefaultStyle) && (Part->DefaultStyle->HasEtched))
				Arrow->Etched = Part->DefaultStyle->Etched;
	
		if ((Part->Styles[State]) && (Part->Styles[State][type].HasSolid))
			Arrow->Solid = Part->Styles[State][type].Solid;
		else
			if ((Part->DefaultTypeStyles) && (Part->DefaultTypeStyles[type].HasSolid))
				Arrow->Solid = Part->DefaultTypeStyles[type].Solid;
		else
			if ((Part->DefaultStateStyles) && (Part->DefaultStateStyles[State].HasSolid))
				Arrow->Solid = Part->DefaultStateStyles[State].Solid;
		else
			if ((Part->DefaultStyle) && (Part->DefaultStyle->HasSolid))
				Arrow->Solid = Part->DefaultStyle->Solid;
	
		if ((Part->Styles[State]) && (Part->Styles[State][type].HasTail))
			Arrow->Tail = Part->Styles[State][type].Tail;
		else
			if ((Part->DefaultTypeStyles) && (Part->DefaultTypeStyles[type].HasTail))
				Arrow->Tail = Part->DefaultTypeStyles[type].Tail;
		else
			if ((Part->DefaultStateStyles) && (Part->DefaultStateStyles[State].HasTail))
				Arrow->Tail = Part->DefaultStateStyles[State].Tail;
		else
			if ((Part->DefaultStyle) && (Part->DefaultStyle->HasTail))
				Arrow->Tail = Part->DefaultStyle->Tail;

		if ((Part->Styles[State]) && (Part->Styles[State][type].HasXPadding))
			Arrow->XPadding = Part->Styles[State][type].XPadding;
		else
			if ((Part->DefaultTypeStyles) && (Part->DefaultTypeStyles[type].HasXPadding))
				Arrow->XPadding = Part->DefaultTypeStyles[type].XPadding;
		else
			if ((Part->DefaultStateStyles) && (Part->DefaultStateStyles[State].HasXPadding))
				Arrow->XPadding = Part->DefaultStateStyles[State].XPadding;
		else
			if ((Part->DefaultStyle) && (Part->DefaultStyle->HasXPadding))
				Arrow->XPadding = Part->DefaultStyle->XPadding;

		if ((Part->Styles[State]) && (Part->Styles[State][type].HasYPadding))
			Arrow->YPadding = Part->Styles[State][type].YPadding;
		else
			if ((Part->DefaultTypeStyles) && (Part->DefaultTypeStyles[type].HasYPadding))
				Arrow->YPadding = Part->DefaultTypeStyles[type].YPadding;
		else
			if ((Part->DefaultStateStyles) && (Part->DefaultStateStyles[State].HasYPadding))
				Arrow->YPadding = Part->DefaultStateStyles[State].YPadding;
		else
			if ((Part->DefaultStyle) && (Part->DefaultStyle->HasYPadding))
				Arrow->YPadding = Part->DefaultStyle->YPadding;

		Part->CompositeArrowsSet[State][type] = SmoothTrue;
		memcpy(&Part->CompositeArrows[State][type], Arrow, sizeof(SmoothArrow));

		/* Free something along the lines of sizeof(SmoothArrow)*(6 + SMOOTH_ARROW_TYPE_COUNT*6)) bytes we no longer need */
		if (!Part->StylesFreed)
		{
			SmoothArrow dummy;

			for (State = 0; State < 5; State++)
			{
				for (type = 0; type < SMOOTH_ARROW_TYPE_COUNT; type++)
				{
					if (!Part->CompositeArrowsSet[State][type])
						smooth_style_get_arrow(Part, State, type, &dummy);
				}
			}

			SmoothFreeArrowStyles(Part);
		}
	}
}	

/* Ensure Composite/Inherited Parts here to cleanup extra memory */
void
smooth_gtkrc_ensure_arrows(SmoothArrowPart *Part)
{
	SmoothArrow dummy;

	smooth_style_get_arrow(Part, 0, 0, &dummy);
}
