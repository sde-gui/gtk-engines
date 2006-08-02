#ifndef SUPPORT_H
#define SUPPORT_H

#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#include "clearlooks_types.h"

#define RADIO_SIZE 13
#define CHECK_SIZE 13

void              clearlooks_treeview_get_header_index (GtkTreeView  *tv,
                                                 GtkWidget    *header,
                                                 gint         *column_index,
                                                 gint         *columns,
                                                 gboolean     *resizable) G_GNUC_INTERNAL;

void              clearlooks_clist_get_header_index    (GtkCList     *clist,
                                                 GtkWidget    *button,
                                                 gint         *column_index,
                                                 gint         *columns) G_GNUC_INTERNAL;

#warning clearlooks_get_parent_bg is a bad hack - find out why its needed, and figure out a better way.
void              clearlooks_get_parent_bg      (const GtkWidget *widget,
                                                 CairoColor      *color) G_GNUC_INTERNAL;

ClearlooksStepper clearlooks_scrollbar_get_stepper         (GtkWidget       *widget,
                                                 GdkRectangle    *stepper) G_GNUC_INTERNAL;
ClearlooksStepper clearlooks_scrollbar_visible_steppers    (GtkWidget       *widget) G_GNUC_INTERNAL;
ClearlooksJunction clearlooks_scrollbar_get_junction       (GtkWidget    *widget) G_GNUC_INTERNAL;

#endif /* SUPPORT_H */
