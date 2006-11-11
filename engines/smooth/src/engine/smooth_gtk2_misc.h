/******************************************************************************/
/* smooth_gtk2_misc.h - Misc Functions for GTK2 Engine                        */
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
/* Portions Based on the XFCE Engine                                          */
/*   Olivier Fourdan <fourdan@xfce.org>                                       */
/*                                                                            */
/* Portions Based on the Xenophilia Engine                                    */
/*   Johan Hanson <misagon@bahnhof.se>                                        */
/******************************************************************************/
#ifndef SMOOTH_GTK2_MISC_H
#define SMOOTH_GTK2_MISC_H

#if GTK_CHECK_VERSION(2, 6, 0)
#	define GTK2_6		1
#	define GTK2_4		1
#	define GTK2_2		1
#	define GTK2_0		1
#	define GTK2		1
#elif GTK_CHECK_VERSION(2, 4, 0)
#	define GTK2_4		1
#	define GTK2_2		1
#	define GTK2_0		1
#	define GTK2		1
#elif GTK_CHECK_VERSION(2, 2, 0)
#	define GTK2_2		1
#	define GTK2_0		1
#	define GTK2		1
#elif GTK_CHECK_VERSION(2, 0, 0)
#		define GTK2_0		1
#		define GTK2		1
#else
#	error "Unsuported version of GTK2"
#endif

/* Misc Functions/Macros */
#define xthickness(style) (((GtkStyle *)style)->xthickness)
#define ythickness(style) (((GtkStyle *)style)->ythickness)

#define FUZZY_COMPARE(i, j, fudge) ((i>j)?(i - j <= fudge):(j - i <= fudge))

GE_INTERNAL GdkBitmap * smooth_arc_clip_mask(gint width, gint height);
#define clip_mask_unref g_object_unref

GE_INTERNAL gboolean sanitize_parameters(GtkStyle * style, GdkWindow * window, gint * width, gint * height);

GE_INTERNAL GtkWidget* get_combo_box_widget_parent (GtkWidget * widget);

/* Notebook Tab Info */

/*
GE_INTERNAL GtkWidget * get_notebook_page(GtkWidget * widget);
GE_INTERNAL GtkWidget * get_tab_label(GtkWidget *page, GtkWidget * widget);

GE_INTERNAL gboolean widget_is_tab_label(GtkWidget *page, GtkWidget * widget);
GE_INTERNAL gboolean tab_label_is_current_page(GtkWidget *page, GtkWidget * widget);
*/
/* Arrow Sizing Workarounds */
GE_INTERNAL void smooth_reverse_engineer_stepper_box(GtkWidget *range, GtkArrowType arrow_type, gint *x, gint *y, gint *width, gint *height);
GE_INTERNAL void smooth_reverse_engineer_spin_button(GtkWidget *widget, GtkArrowType arrow_type, gint *x, gint *y, gint *width, gint *height);

GE_INTERNAL void smooth_reverse_engineer_arrow_box(GtkWidget *widget, const gchar * detail, GtkArrowType arrow_type, gint *x, gint *y, gint *width, gint *height);

GE_INTERNAL void smooth_gtk_button_get_props (GtkWidget *widget, GtkBorder *default_border, GtkBorder *default_outside_border, gboolean  *interior_focus);
GE_INTERNAL void smooth_gtk_widget_get_focus_props (GtkWidget *widget, gint *focus_line_width, gint *focus_padding, gboolean  *interior_focus);
GE_INTERNAL gboolean smooth_cell_renderer_toggle_get_inconsistent (GtkWidget *widget);

#endif /* SMOOTH_GTK2_MISC_H */
