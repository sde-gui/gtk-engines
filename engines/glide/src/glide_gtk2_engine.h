/* Glide theme engine
 * Copyright (C) 2006 Andrew Johnson
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
 * Written by Andrew Johnson <acjgenius@earthlink.net>
 */ 

#include <gmodule.h>
#include <gtk/gtk.h>
 
#include "ge-support.h"

/*#warning NO MORE MACROS! - Replace with pattern precedence in configuration parsing*/
#define DEFAULT_BACKGROUND_PATTERN(glide_style, state, alternate) ((glide_style->bg_image[state])?glide_style->bg_image[state]:alternate)

#define DEFAULT_OVERLAY_PATTERN(glide_style, detail, vertical) &glide_style->overlay[CHECK_DETAIL(detail, "menuitem")][vertical]

/*****************************/
/* RC Style Declaration      */
/*****************************/
GE_INTERNAL extern GType glide_type_rc_style;
 
#define GLIDE_TYPE_RC_STYLE              glide_type_rc_style
#define GLIDE_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GLIDE_TYPE_RC_STYLE, GlideRcStyle))
#define GLIDE_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GLIDE_TYPE_RC_STYLE, GlideRcStyleClass))
#define GLIDE_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GLIDE_TYPE_RC_STYLE))
#define GLIDE_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GLIDE_TYPE_RC_STYLE))
#define GLIDE_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GLIDE_TYPE_RC_STYLE, GlideRcStyleClass))
 
typedef struct
{
  GtkRcStyle parent_instance;
} GlideRcStyle;
 
typedef struct
{
  GtkRcStyleClass parent_class;
} GlideRcStyleClass;
 
/*****************************/
/* Drawing Style Declaration */
/*****************************/
GE_INTERNAL extern GType glide_type_style;
GE_INTERNAL extern GtkStyleClass *glide_parent_class;
 
#define GLIDE_TYPE_STYLE              glide_type_style
#define GLIDE_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GLIDE_TYPE_STYLE, GlideStyle))
#define GLIDE_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GLIDE_TYPE_STYLE, GlideStyleClass))
#define GLIDE_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GLIDE_TYPE_STYLE))
#define GLIDE_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GLIDE_TYPE_STYLE))
#define GLIDE_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GLIDE_TYPE_STYLE, GlideStyleClass))
 
typedef struct
{
  GtkStyle parent_instance;
  CairoColorCube color_cube;

  CairoPattern *bg_solid[5];
  CairoPattern *bg_image[5];
  CairoPattern *bg_gradient[2][5];
  CairoPattern *active_tab_gradient[4][5];

  CairoPattern overlay[2][2];

} GlideStyle;
 
typedef struct
{
  GtkStyleClass parent_class;
} GlideStyleClass;
