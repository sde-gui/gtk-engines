/* eazel-theme.h -- definitions

   Copyright (C) 2000 Eazel, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   $Id$

   Authors: John Harper <jsh@eazel.com>  */

#ifndef EAZEL_THEME_H
#define EAZEL_THEME_H

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#define RANGE_WIDTH 15
#define MIN_THUMB_HEIGHT 24
#define SCALE_HEIGHT 16
#define CHECK_SIZE 10
#define CHECK_SPACING 3


/* parser / per-engine data */

enum {
    TOKEN_THICKNESS = GTK_RC_TOKEN_LAST + 1,
    TOKEN_FOCUS_THICKNESS,
    TOKEN_DEFAULT_THICKNESS,
    TOKEN_FOCUS_COLOR,
    TOKEN_INSENSITIVE_COLORS,
    TOKEN_NONE,
    TOKEN_VERTICAL,
    TOKEN_HORIZONTAL,
    TOKEN_GRADIENT,
    TOKEN_SHADOW,
    TOKEN_INVERSE_SHADOW,
    TOKEN_OUTLINE_SHADOW,
    TOKEN_YES,
    TOKEN_NO,
    TOKEN_STOCK,
    TOKEN_BORDER,
    TOKEN_RECOLOR,
    TOKEN_PALETTE,
    TOKEN_IMAGE,
    TOKEN_FG,
    TOKEN_BG,
    TOKEN_BASE,
    TOKEN_TEXT,
};

#define THEME_SYMBOLS						\
  { "thickness", TOKEN_THICKNESS }, 				\
  { "focus_thickness", TOKEN_FOCUS_THICKNESS },			\
  { "default_thickness", TOKEN_DEFAULT_THICKNESS },		\
  { "focus_color", TOKEN_FOCUS_COLOR },				\
  { "insensitive_colors", TOKEN_INSENSITIVE_COLORS },		\
  { "none", TOKEN_NONE },					\
  { "vertical", TOKEN_VERTICAL },				\
  { "horizontal", TOKEN_HORIZONTAL },				\
  { "gradient", TOKEN_GRADIENT },				\
  { "shadow", TOKEN_SHADOW },					\
  { "inverse_shadow", TOKEN_INVERSE_SHADOW },			\
  { "outline_shadow", TOKEN_OUTLINE_SHADOW },			\
  { "yes", TOKEN_YES },						\
  { "no", TOKEN_NO },						\
  { "border", TOKEN_BORDER },					\
  { "recolor", TOKEN_RECOLOR },					\
  { "palette", TOKEN_PALETTE },					\
  { "image", TOKEN_IMAGE },					\
  { "fg", TOKEN_FG },						\
  { "bg", TOKEN_BG },						\
  { "base", TOKEN_BASE },					\
  { "text", TOKEN_TEXT },					\
  { 0, G_TOKEN_NONE }

typedef enum {
    GRADIENT_NONE = 0,
    GRADIENT_VERTICAL,
    GRADIENT_HORIZONTAL,
} eazel_engine_gradient_direction;

typedef struct eazel_engine_gradient_component_struct eazel_engine_gradient_component;
struct eazel_engine_gradient_component_struct {
    GdkColor color;
    float weight;
};

typedef struct {
    guint refcount;
    eazel_engine_gradient_direction direction;
    GdkColor from;
    GSList *components;
} eazel_engine_gradient;

typedef struct pixmap_cache_node_struct eazel_engine_pixmap_cache_node;

typedef struct {
    char *filename;
    int border[4];
    eazel_engine_gradient *recolor;
    GdkPixbuf *pixbuf;
    eazel_engine_pixmap_cache_node *pixmap_first, *pixmap_last;
} eazel_engine_image;

#define EAZEL_ENGINE_PALETTE_SIZE 4

/* Information about a single RC style */
typedef struct {
    guint refcount;

    guint thickness;
    guint focus_thickness;
    guint default_thickness;

    guint no_shadow : 1;
    guint inverse_shadow : 1;
    guint outline_shadow : 1;

    GdkColor focus_color;
    GdkColor insensitive_colors[2];	/* inner, outer */

    eazel_engine_gradient *palette[EAZEL_ENGINE_PALETTE_SIZE];

    /* for each GTK widget state */
    eazel_engine_gradient *gradients[5];
} eazel_theme_data;

#define DEFAULT_THEME_DATA {			\
  0, 2, 2, 3, FALSE, FALSE, TRUE,		\
  { 0, 0x4a4a, 0x7d7d, 0x8484 },		\
  { { 0, 0x6363, 0x6565, 0x6363 },		\
    { 0, 0xcfcf, 0xd0d0, 0xcfcf } },		\
 }

#define STYLE_THEME_DATA(s) ((eazel_theme_data *) ((s)->engine_data))


/* prototypes */

/* from eazel-theme-main.c */
extern void theme_init (GtkThemeEngine *engine);
extern void theme_exit (void);

/* from eazel-theme-draw.c */
extern GtkStyleClass eazel_class_0, eazel_class_1, eazel_class_2, eazel_class_3;

/* from eazel-theme-gradient.c */
extern eazel_engine_gradient *eazel_engine_gradient_new (eazel_engine_gradient_direction direction,
							 GdkColor *from,
							 GSList *components);
extern eazel_engine_gradient *eazel_engine_gradient_ref (eazel_engine_gradient *g);
extern void eazel_engine_gradient_unref (eazel_engine_gradient *g);
extern eazel_engine_gradient *eazel_engine_make_two_point_gradient (eazel_engine_gradient_direction direction,
								    gulong from_rgb,
								    gulong to_rgb);

extern void eazel_engine_fill_gradient_rgb_buffer (const eazel_engine_gradient *gradient,
						   int rgb_total, guchar *rgb_buf,
						   int rgb_first, int rgb_last);

extern void eazel_engine_draw_gradient (GdkDrawable *drawable,
					GdkGC *gc,
					const GdkRectangle *full_rect,
					const GdkRectangle *clip_rect,
					const eazel_engine_gradient *gradient);

extern void eazel_engine_set_bg_gradient (GdkWindow *window,
					  eazel_engine_gradient *gradient);

/* from eazel-theme-shadow.c */
extern void eazel_engine_draw_shadow (GtkStyle *style,
				      GdkWindow *window,
				      GtkStateType state_type,
				      GtkShadowType shadow_type,
				      GdkRectangle *area,
				      GtkWidget *widget,
				      gchar *detail,
				      gint x, gint y, gint width, gint height);

#endif /* EAZEL_THEME_H */
