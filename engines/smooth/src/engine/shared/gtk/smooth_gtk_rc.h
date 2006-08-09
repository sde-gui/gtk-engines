/******************************************************************************/
/* smooth_gtk_rc.h - Shared RC definition for all GTK+ versions               */
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
#ifndef SMOOTH_GTK_RC_H
#define SMOOTH_GTK_RC_H

#include "ge-support.h"

#define SMOOTH_EDGE_NONE		1
#define SMOOTH_EDGE_LINE		2
#define SMOOTH_EDGE_GRADIENT		3
#define SMOOTH_EDGE_PIXBUF		4

typedef enum
{
	SMOOTH_BUTTON_DEFAULT_STYLE_NONE = 1,
	SMOOTH_BUTTON_DEFAULT_STYLE_GTK,
	SMOOTH_BUTTON_DEFAULT_STYLE_WIN32,
	SMOOTH_BUTTON_DEFAULT_STYLE_TRIANGLE,
	
	SMOOTH_BUTTON_DEFAULT_STYLE_DEFAULT = SMOOTH_BUTTON_DEFAULT_STYLE_TRIANGLE
} SmoothButtonDefaultStyle;

#define SMOOTH_TAB_NORMAL		1
#define SMOOTH_TAB_ROUND		2
#define SMOOTH_TAB_TRIANGLE		3
#define SMOOTH_TAB_PIXBUF		4

#define DEFAULT_GRADIENT		TRUE
#define DEFAULT_HGRADIENTDIRECTION	SMOOTH_GRADIENT_HORIZONTAL
#define DEFAULT_VGRADIENTDIRECTION	SMOOTH_GRADIENT_VERTICAL
#define DEFAULT_EDGESTYLE		SMOOTH_EDGE_LINE
#define DEFAULT_LINETHICKNESS		2
#define DEFAULT_BUTTONDEFAULTTRIANGLE	TRUE
#define DEFAULT_TABSTYLE		SMOOTH_TAB_NORMAL
#define DEFAULT_GRIPSTYLE		FIXEDLINES_IN_GRIP
#define DEFAULT_SOLIDARROW		FALSE
#define DEFAULT_ETCHEDARROW		FALSE
#define DEFAULT_TAILARROW		0
#define DEFAULT_GRIPCOUNT		3
#define DEFAULT_GRIPSPACING		2
#define DEFAULT_REAL_SLIDERS		FALSE
#define DEFAULT_RESIZE_GRIP		TRUE
#define DEFAULT_TROUGH_SHOW_VALUE	FALSE
#define DEFAULT_GRIPOVERLAP		FALSE

typedef gboolean (*SmoothSmoothTranslateEnumFunc) (gchar * str, guint *retval);

#define THEME_PART(part)                 ((smooth_part_style *) (part))

#define REAL_SLIDERS(style)(THEME_DATA(style)->real_sliders)
#define RESIZE_GRIP(style)(THEME_DATA(style)->resize_grip)
#define DO_CHECK_LINE_THICKNESS(line,thickness)(((line==SMOOTH_BEVEL_STYLE_BEVELED) || (line==SMOOTH_BEVEL_STYLE_FLAT) || (line==SMOOTH_BEVEL_STYLE_NONE))?thickness:2)

#define PART_STYLE(part)(THEME_PART(part)->style)
#define PART_XPADDING(part)(THEME_PART(part)->ypadding)
#define PART_YPADDING(part)(THEME_PART(part)->xpadding)

#define LINE_STYLE(style, part)(((part) && THEME_PART(part)->use_line)?THEME_PART(part)->line.style:THEME_DATA(style)->line.style)
#define LINE_THICKNESS(style, part) (((part) && THEME_PART(part)->use_line)?DO_CHECK_LINE_THICKNESS(THEME_PART(part)->line.style, THEME_PART(part)->line.thickness):DO_CHECK_LINE_THICKNESS(THEME_DATA(style)->line.style, THEME_DATA(style)->line.thickness))

#define EDGE_PART(style)(&THEME_DATA(style)->edge)
#define BACKGROUND_PART(style)(&THEME_DATA(style)->background)

#define EDGE_LINE_STYLE(style, part)((((part) && THEME_PART(part)->edge.use_line))?THEME_PART(part)->edge.line.style:(EDGE_PART(style)->use_line?EDGE_PART(style)->line.style:LINE_STYLE(style, part)))
#define EDGE_LINE_THICKNESS(style, part) ((((part) && THEME_PART(part)->edge.use_line))?DO_CHECK_LINE_THICKNESS(THEME_PART(part)->edge.line.style, THEME_PART(part)->edge.line.thickness):(EDGE_PART(style)->use_line?DO_CHECK_LINE_THICKNESS(EDGE_PART(style)->line.style, EDGE_PART(style)->line.thickness):LINE_THICKNESS(style, part)))

#define GRIP_PART(style)(&THEME_DATA(style)->grip)
#define GRIP_COUNT(style)(GRIP_PART(style)->count)
#define GRIP_SPACING(style)(GRIP_PART(style)->spacing)
#define GRIP_OVERLAP_TOOLBAR(style)(GRIP_PART(style)->overlap)

#define CHECK_PART(style)(&THEME_DATA(style)->check)
#define CHECK_MOTIF(style)(CHECK_PART(style)->motif)

#define OPTION_PART(style)(&THEME_DATA(style)->option)
#define OPTION_MOTIF(style)(THEME_DATA(style)->option.motif)

#define TROUGH_PART(style)(&THEME_DATA(style)->trough)
#define TROUGH_SHOW_VALUE(style) (THEME_DATA(style)->trough.show_value)
#define PROGRESS_PART(style)(&THEME_DATA(style)->progress)

#define STEPPER_PART(style)(&THEME_DATA(style)->stepper)

#define ARROW_PART(style)(THEME_DATA(style)->arrow)

#define COLOR_CUBE(style)(THEME_DATA(style)->colors)

#define FOCUS_USE_FOREGROUND(style, state)(THEME_DATA(style)->focus.use_foreground[state])
#define FOCUS_FOREGROUND(style, state)(THEME_DATA(style)->focus.foreground[state])

#define FOCUS_USE_PATTERN(style, state)(THEME_DATA(style)->focus.pattern[state]!=NULL)
#define FOCUS_PATTERN(style, state)(THEME_DATA(style)->focus.pattern[state])

#define FOCUS_USE_LINE_WIDTH(style, state)(THEME_DATA(style)->focus.use_line_width[state])
#define FOCUS_LINE_WIDTH(style, state)(THEME_DATA(style)->focus.line_width[state])

#define BUFFERED_FILL(style)(THEME_DATA(style)->buffered_fill)
#define ENTRY_BUTTON_EMBED(style) (THEME_DATA(style)->button.embeddable)

typedef struct {
	SmoothBevelStyle style;
	gint thickness;
} SmoothLinePart;

typedef struct {
  SmoothFillStyle style;/* none/flat/gradient/shaded */

  gint gradient_direction[2];/* horizontal/vertical/fdiagonal/bdiagonal */
  gint quadratic_gradient;

  gdouble shade1;/* 0.0/2.5 */
  gdouble shade2;/* 0.0/2.5 */

  gboolean use_color1[5];/* SmoothWidgetState */
  gboolean use_color2[5];/* SmoothWidgetState */

  SmoothColor color1[5];/* SmoothWidgetState */
  SmoothColor color2[5];/* SmoothWidgetState */

  /* state override dither depth */
  gboolean default_dither_depth_set;
  int default_dither_depth;

  gboolean use_dither_depth[5];/* GtkStateType */
  int dither_depth[5];/* GtkStateType */

    GString *file_name[5];/* GtkStateType */
} SmoothFillPart;

typedef struct {
  gint style;

  SmoothLinePart line;
  SmoothFillPart fill;
  gboolean use_line;
  
} smooth_edge_style;

typedef struct {
  gint style;

  smooth_edge_style edge;

  SmoothLinePart line;
  SmoothFillPart fill;

  gboolean use_line;
  gboolean use_fill;

  GString *file_name;
  GdkPixmap * part;

  gint xpadding;
  gint ypadding;
} smooth_part_style;

typedef struct {
  smooth_part_style part;
} smooth_bg_style;

typedef struct {
  smooth_part_style part;
} smooth_fg_style;

typedef struct {
  smooth_part_style part;

  gint count;
  gint spacing;
  gboolean overlap;
} smooth_grip_style;

typedef struct {
  smooth_part_style part;
  gboolean motif;  
} smooth_check_style;

typedef struct {
  smooth_part_style part;
  gboolean default_triangle;

  gboolean use_button_default;

  smooth_part_style button_default;
  gboolean embeddable;
} smooth_button_style;

typedef struct {
  smooth_part_style part;

  gboolean highlight;
  gboolean use_active_tab;

  smooth_part_style active_tab;
} smooth_tab_style;

typedef struct {
  smooth_part_style part;
  gboolean show_value;
} smooth_trough_style;

typedef struct {
	smooth_part_style Part;

	SmoothArrowPart Arrow;
} SmoothStepperStyle;

typedef struct 
{
	gchar       *name;
	guint        token;
} ThemeSymbols;

typedef struct _smooth_focus_style smooth_focus_style;

enum
  {
    TOKEN_FIRST = G_TOKEN_LAST + 1,
    
    TOKEN_REAL_SLIDERS,
    TOKEN_RESIZE_GRIP,

    TOKEN_STYLE,

    TOKEN_FILE,

    TOKEN_FILL,
    TOKEN_VDIRECTION,
    TOKEN_HDIRECTION,
    TOKEN_COLOR1,
    TOKEN_COLOR2,
    TOKEN_SHADE1_VALUE,
    TOKEN_SHADE2_VALUE,
    TOKEN_QUADRATIC_GRADIENT,
    TOKEN_DITHER_DEPTH,
    
    TOKEN_EDGE,

    TOKEN_LINE,
    TOKEN_THICKNESS,

    TOKEN_FOREGROUND,

    TOKEN_ARROW,
    TOKEN_SOLID,
    TOKEN_ETCHED,
    TOKEN_TAIL,

    TOKEN_FOCUS,
    TOKEN_PATTERN,
    TOKEN_WIDTH,

    TOKEN_BUTTON,
    TOKEN_BUTTON_EMBEDDABLE,
    TOKEN_BUTTON_DEFAULT,
    TOKEN_DEFAULT_TRIANGLE,

    TOKEN_TABS,
    TOKEN_ACTIVE_TAB,
    TOKEN_HIGHLIGHT,

    TOKEN_PROGRESS,
    
    TOKEN_TROUGH,
    TOKEN_TROUGH_SHOW_VALUE,
    
    TOKEN_STEPPER,

    TOKEN_GRIP,
    TOKEN_COUNT,
    TOKEN_SPACING,
    TOKEN_TOOLBAR_OVERLAP,
     
    TOKEN_MOTIF,

    TOKEN_CHECK,
    TOKEN_OPTION,

    TOKEN_XPADDING,
    TOKEN_YPADDING,

    /* backward compatibility for <= 0.5.4 */
    TOKEN_DEPRECATED_TABSTYLE,

    /* backward compatibility for <= 0.5.0 */
    TOKEN_DEPRECATED_ARROWSTYLE,
    TOKEN_DEPRECATED_SOLIDARROW,
    TOKEN_DEPRECATED_ETCHEDARROW,
    
    /* backward compatibility for <= 0.4.0 */
    TOKEN_DEPRECATED_GRADIENT,
    
    TOKEN_LAST = TOKEN_DEPRECATED_GRADIENT
  };

GE_INTERNAL extern ThemeSymbols smooth_theme_symbols[];

/* fill part */
GE_INTERNAL gint smooth_fill_style(GtkStyle * style, void *part);
GE_INTERNAL gint smooth_fill_gradient_direction(GtkStyle * style, void *part, gboolean horizontal);
GE_INTERNAL gboolean smooth_fill_quadratic_gradient(GtkStyle * style, void *part);
GE_INTERNAL gdouble smooth_fill_shade1_value(GtkStyle * style, void *part);
GE_INTERNAL gdouble smooth_fill_shade2_value(GtkStyle * style, void *part);

GE_INTERNAL SmoothColor smooth_fill_color1(GtkStyle * style, void *part, GtkStateType state);
GE_INTERNAL SmoothColor smooth_fill_color2(GtkStyle * style, void *part, GtkStateType state);

GE_INTERNAL GString *smooth_fill_file_name(GtkStyle * style, void *part, GtkStateType state);

/* tab part styles*/
GE_INTERNAL smooth_part_style *smooth_tab_part(GtkStyle * style, gboolean for_active_tab);

GE_INTERNAL gint smooth_tab_get_style(GtkStyle * style, gboolean for_active_tab);

GE_INTERNAL SmoothFillPart *smooth_tab_fill(GtkStyle * style, gboolean for_active_tab);

GE_INTERNAL SmoothBevelStyle smooth_tab_edge_line_style(GtkStyle * style, gboolean for_active_tab);
GE_INTERNAL gint smooth_tab_edge_line_thickness(GtkStyle * style, gboolean for_active_tab);

/* button part styles*/
GE_INTERNAL smooth_part_style *smooth_button_part(GtkStyle * style, gboolean for_default_button);

GE_INTERNAL gint smooth_button_get_style(GtkStyle * style, gboolean for_default_button);

GE_INTERNAL gboolean smooth_button_default_triangle(GtkStyle * style);

GE_INTERNAL SmoothFillPart *smooth_button_fill(GtkStyle * style, gboolean for_default_button);

GE_INTERNAL SmoothBevelStyle smooth_button_edge_line_style(GtkStyle * style, gboolean for_default_button);
GE_INTERNAL gint smooth_button_edge_line_thickness(GtkStyle * style, gboolean for_default_button);

/* Custom Enum Translators */
GE_INTERNAL gboolean SmoothTranslateFillStyleName (gchar * str, SmoothFillStyle *retval);
GE_INTERNAL gboolean SmoothTranslateGradientDirectionName (gchar * str, gint *retval);
GE_INTERNAL gboolean SmoothTranslateLineStyleName (gchar * str, SmoothBevelStyle *retval);
GE_INTERNAL gboolean SmoothTranslateButtonDefaultStyleName (gchar * str, gint *retval);
GE_INTERNAL gboolean SmoothTranslateTabStyleName (gchar * str, gint *retval);
GE_INTERNAL gboolean SmoothTranslateGripStyleName (gchar * str, gint *retval);
GE_INTERNAL gboolean SmoothTranslateCheckStyleName (gchar * str, gint *retval);
GE_INTERNAL gboolean SmoothTranslateEdgeStyleName (gchar * str, gint *retval);
GE_INTERNAL gboolean SmoothTranslateArrowStyleName (gchar * str, SmoothArrowStyle *retval);
GE_INTERNAL gboolean SmoothTranslateArrowTypeName (gchar * str, SmoothArrowStyle *retval);
GE_INTERNAL gboolean SmoothTranslateStateName (gchar * str, int *retval);
GE_INTERNAL gboolean SmoothTranslateBooleanName (gchar * str, gint *retval);

/* General Parsing Functions */
GE_INTERNAL guint 
smooth_rc_parse_int (GScanner *scanner, 
                 GTokenType wanted_token, 
		 guint return_default,
		 gint *retval, 
		 gint lower_limit, 
		 gint upper_limit);
		 
GE_INTERNAL guint  
smooth_rc_parse_float (GScanner *scanner, 
                 GTokenType wanted_token, 
		 gdouble return_default, 
		 gdouble *retval, 
		 gdouble lower_limit, 
		 gdouble upper_limit);

GE_INTERNAL guint 
smooth_rc_parse_custom_enum (GScanner *scanner, 
                         GTokenType wanted_token, 
			 SmoothSmoothTranslateEnumFunc translate_enum,
			 guint return_default,
			 guint *retval);

GE_INTERNAL guint 
smooth_rc_parse_boolean(GScanner *scanner,  
                    GTokenType wanted_token, 
		    gboolean return_default, 
		    guint *retval);

GE_INTERNAL guint 
smooth_rc_parse_line (GScanner *scanner, 
                  GTokenType wanted_token, 
                  SmoothLinePart *retval);

GE_INTERNAL guint 
smooth_rc_parse_focus(GScanner *scanner, 
                  GTokenType wanted_token, 
                  smooth_focus_style *retval);
                  
GE_INTERNAL guint 
smooth_rc_parse_arrow_part(GScanner *scanner, 
                       GTokenType wanted_token, 
                       SmoothArrowPart *retval);

GE_INTERNAL guint 
smooth_rc_parse_fill(GScanner *scanner, 
                 GTokenType wanted_token, 
                 SmoothFillPart *retval);

GE_INTERNAL guint 
smooth_rc_parse_edge(GScanner *scanner, 
                 GTokenType wanted_token, 
                 smooth_edge_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_button_default(GScanner *scanner, 
                           GTokenType wanted_token, 
                           smooth_part_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_button(GScanner *scanner, 
                   GTokenType wanted_token, 
                   smooth_button_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_active_tab(GScanner *scanner, 
                       GTokenType wanted_token, 
                       smooth_part_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_tab(GScanner *scanner, 
                GTokenType wanted_token, 
                smooth_tab_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_option(GScanner *scanner, 
                   GTokenType wanted_token, 
                   smooth_check_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_grip(GScanner *scanner, 
                 GTokenType wanted_token, 
                 smooth_grip_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_stepper(GScanner *scanner, 
                 GTokenType wanted_token, 
                 SmoothStepperStyle *retval);

GE_INTERNAL guint 
smooth_rc_parse_check(GScanner *scanner, 
                  GTokenType wanted_token,
                  smooth_check_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_generic_part(GScanner *scanner, 
                         GTokenType wanted_token, 
                         smooth_part_style *retval);

GE_INTERNAL guint 
smooth_rc_parse_trough_part(GScanner *scanner, 
                        GTokenType wanted_token, 
                        smooth_trough_style *retval);

GE_INTERNAL void 
part_init(smooth_part_style *part, 
          gint partstyle);
GE_INTERNAL void 
part_merge (smooth_part_style *dest_part, 
            smooth_part_style *src_part);

GE_INTERNAL void
smooth_arrow_merge (SmoothArrowPart *dest_arrow,
             SmoothArrowPart *src_arrow);

GE_INTERNAL void
smooth_style_get_border(GtkStyle * style,
                            GtkStateType state_type,
	  		    GtkShadowType shadow_type,
			    const gchar * detail,
			    void * part,
			    gdouble CornerRoundness,
			    SmoothBorder *Border);

GE_INTERNAL void
smooth_style_get_arrow(SmoothArrowPart *Part,
				GtkStateType State,
				GtkArrowType ArrowType,
				SmoothArrow *Arrow);

GE_INTERNAL void
smooth_gtkrc_ensure_arrows(SmoothArrowPart *Part);

#endif /*SMOOTH_GTK_RC*/
