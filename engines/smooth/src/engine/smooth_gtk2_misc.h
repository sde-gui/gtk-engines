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

#include <engine/shared/gtk/smooth_gtk_misc.h>

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

#ifndef max
#define max(x,y) ((x)>=(y)?(x):(y))
#endif
#ifndef min
#define min(x,y) ((x)<=(y)?(x):(y))
#endif

#ifndef M_PI
  #define M_PI    3.14159265358979323846
#endif

#ifndef M_PI_4
  #define M_PI_4  0.78539816339744830962
#endif

/* Widget Type Lookups/Macros */
#define IS_WIDGET(object) ((object)  && object_is_a (object, "GtkWidget"))
#define IS_CONTAINER(object) ((object)  && object_is_a (object, "GtkContainer"))
#define IS_BIN(object) ((object)  && object_is_a (object, "GtkBin"))
 
#define IS_HANDLE_BOX(object) ((object) && object_is_a (object, "GtkHandleBox"))
#define IS_HANDLE_BOX_ITEM(object) ((object) && IS_HANDLE_BOX(object->parent))
#define IS_BONOBO_DOCK_ITEM(object) ((object) && object_is_a (object, "BonoboDockItem"))
#define IS_BONOBO_DOCK_ITEM_GRIP(object) ((object) && object_is_a (object, "BonoboDockItemGrip"))
#define IS_BONOBO_TOOLBAR(object) ((object) && object_is_a (object, "BonoboUIToolbar"))
#define IS_EGG_TOOLBAR(object) ((object) && object_is_a (object, "Toolbar"))
#define IS_TOOLBAR(object) ((object) && object_is_a (object, "GtkToolbar"))
#define IS_PANEL_WIDGET(object) ((object) && object_is_a (object, "PanelWidget"))
 
#define IS_COMBO_BOX_ENTRY(object) ((widget) && object_is_a (widget, "GtkComboBoxEntry"))
#define IS_COMBO_BOX(object) ((widget) && object_is_a (widget, "GtkComboBox"))
#define IS_COMBO(object) ((widget) && object_is_a (widget, "GtkCombo"))
#define IS_OPTION_MENU(object) ((widget) && object_is_a (widget, "GtkOptionMenu"))
 
#define IS_TOGGLE_BUTTON(object) ((object) && object_is_a (object, "GtkToggleButton"))
#define IS_SPIN_BUTTON(object) ((object) && object_is_a (object, "GtkSpinButton"))
 
#define IS_STATUS_BAR(object) ((object) && object_is_a (object, "GtkStatusBar"))
#define IS_PROGRESS_BAR(object) ((object) && object_is_a (object, "GtkProgressBar"))
 
#define IS_MENU_SHELL(object) ((object) && object_is_a (object, "GtkMenuShell"))
#define IS_MENU(object) ((object) && object_is_a (object, "GtkMenu"))
#define IS_MENU_BAR(object) ((object) && object_is_a (object, "GtkMenuBar"))
#define IS_MENU_ITEM(object) ((object) && object_is_a (object, "GtkMenuItem"))
 
#define IS_SCROLLBAR(object) ((object) && object_is_a (object, "GtkScrollbar"))
#define IS_VSCROLLBAR(object) ((object) && object_is_a (object, "GtkVScrollbar"))
#define IS_HSCROLLBAR(object) ((object) && object_is_a (object, "GtkHScrollbar"))
 
#define IS_SCALE(object) ((object) && object_is_a (object, "GtkScale"))
#define IS_VSCALE(object) ((object) && object_is_a (object, "GtkVScale"))
#define IS_HSCALE(object) ((object) && object_is_a (object, "GtkHScale"))
  
#define IS_PANED(object) ((object) && object_is_a (object, "GtkPaned"))
#define IS_VPANED(object) ((object) && object_is_a (object, "GtkVPaned"))
#define IS_HPANED(object) ((object) && object_is_a (object, "GtkHPaned"))
 
#define IS_BOX(object) ((object) && object_is_a (object, "GtkBox"))
#define IS_VBOX(object) ((object) && object_is_a (object, "GtkVBox"))
#define IS_HBOX(object) ((object) && object_is_a (object, "GtkHBox"))
 
#define TOGGLE_BUTTON(object) (IS_TOGGLE_BUTTON(object)?(GtkToggleButton *)object:NULL)

gboolean object_is_a (gpointer object, gchar * type_name);

gboolean is_toolbar_item (GtkWidget * widget);
gboolean is_panel_widget_item (GtkWidget * widget);
gboolean is_combo_box_entry (GtkWidget * widget);
gboolean is_combo_box (GtkWidget * widget, gboolean as_list);
gboolean is_combo (GtkWidget * widget);
gboolean is_in_combo_box (GtkWidget * widget);

/* Misc Functions/Macros */
#define xthickness(style) (((GtkStyle *)style)->xthickness)
#define ythickness(style) (((GtkStyle *)style)->ythickness)

#define CHECK_DETAIL(detail, value) ((detail) && (!strcmp(value, detail)))
#define FUZZY_COMPARE(i, j, fudge) ((i>j)?(i - j <= fudge):(j - i <= fudge))

GdkBitmap * arc_clip_mask(gint width, gint height);
#define clip_mask_unref g_object_unref

gboolean sanitize_parameters(GtkStyle * style, GdkWindow * window, gint * width, gint * height);

/* Notebook Tab Info */
GtkWidget * get_notebook_page(GtkWidget * widget);
GtkWidget * get_tab_label(GtkWidget *page, GtkWidget * widget);
GtkWidget* get_combo_box_widget_parent (GtkWidget * widget);

gboolean widget_is_tab_label(GtkWidget *page, GtkWidget * widget);
gboolean tab_label_is_current_page(GtkWidget *page, GtkWidget * widget);

/* Arrow Sizing Workarounds */
void reverse_engineer_stepper_box(GtkWidget *range, GtkArrowType arrow_type, gint *x, gint *y, gint *width, gint *height);
void reverse_engineer_spin_button(GtkWidget *widget, GtkArrowType arrow_type, gint *x, gint *y, gint *width, gint *height);

void reverse_engineer_arrow_box(GtkWidget *widget, const gchar * detail, GtkArrowType arrow_type, gint *x, gint *y, gint *width, gint *height);

void option_menu_get_props (GtkWidget *widget, GtkRequisition *indicator_size, GtkBorder *indicator_spacing);
void gtk_button_get_props (GtkWidget *widget, GtkBorder *default_border, GtkBorder *default_outside_border, gboolean  *interior_focus);
void gtk_widget_get_focus_props (GtkWidget *widget, gint *focus_line_width, gint *focus_padding, gboolean  *interior_focus);
gboolean gtk_cell_renderer_toggle_get_inconsistent (GtkWidget *widget);

#endif /* SMOOTH_GTK2_MISC_H */
