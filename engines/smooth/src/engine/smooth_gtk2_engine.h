/******************************************************************************/
/* smooth_gtk2_engine.h - Declarations for GTK2 Engine                        */
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
/******************************************************************************/
#ifndef SMOOTH_GTK_ENGINE_H
#define SMOOTH_GTK_ENGINE_H

#include <gmodule.h>
#include <gtk/gtk.h>
#include <gtk/gtkstyle.h>

#include <math.h>

#include <utils/smooth_parts.h>
#include <interfaces/gdk2/gdk2_drawing_interface.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

GE_INTERNAL extern GType smooth_type_style;
GE_INTERNAL extern GtkStyleClass *smooth_theme_parent_class;
GE_INTERNAL extern GType smooth_type_rc_style;

#define SMOOTH_TYPE_STYLE              smooth_type_style
#define SMOOTH_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), SMOOTH_TYPE_STYLE, SmoothStyle))
#define SMOOTH_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SMOOTH_TYPE_STYLE, SmoothStyleClass))
#define SMOOTH_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), SMOOTH_TYPE_STYLE))
#define SMOOTH_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SMOOTH_TYPE_STYLE))
#define SMOOTH_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SMOOTH_TYPE_STYLE, SmoothStyleClass))

#define SMOOTH_TYPE_RC_STYLE              smooth_type_rc_style
#define SMOOTH_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), SMOOTH_TYPE_RC_STYLE, SmoothRcStyle))
#define SMOOTH_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SMOOTH_TYPE_RC_STYLE, SmoothRcStyleClass))
#define SMOOTH_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), SMOOTH_TYPE_RC_STYLE))
#define SMOOTH_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SMOOTH_TYPE_RC_STYLE))
#define SMOOTH_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SMOOTH_TYPE_RC_STYLE, SmoothRcStyleClass))
#define SMOOTH_RC_DATA(rc_style)          (SMOOTH_RC_STYLE (rc_style)->engine_data)

#define THEME_DATA(style)                 (SMOOTH_RC_STYLE (style->rc_style)->engine_data)

typedef struct
{
  GtkStyle parent_instance;
} SmoothStyle;

typedef struct
{
  GtkStyleClass parent_class;
} SmoothStyleClass;

typedef struct 
{
  GtkRcStyleClass parent_class;
} SmoothRcStyleClass;

GE_INTERNAL extern gint smooth_rc_references;

GE_INTERNAL gboolean smooth_rc_data_unref(GObject *rc_style);

#endif /* SMOOTH_GTK_ENGINE_H */
