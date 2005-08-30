#ifndef SUPPORT_H
#define SUPPORT_H

#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#include "clearlooks_types.h"

/* GTK 2.2 compatibility */
#ifndef GTK_IS_COMBO_BOX_ENTRY
	#define GTK_IS_COMBO_BOX_ENTRY(x) 0
#endif
#ifndef GTK_IS_COMBO_BOX
	#define GTK_IS_COMBO_BOX(x) 0
#endif

#define RADIO_SIZE 13
#define CHECK_SIZE 13

GtkTextDirection get_direction     (GtkWidget *widget);

GdkPixbuf *generate_bit            (unsigned char   alpha[],
                                    GdkColor       *color,
                                    double          mult);

GdkPixbuf *colorize_bit            (unsigned char  *bit,
                                    unsigned char  *alpha,
                                    GdkColor       *new_color);

GdkPixmap *pixbuf_to_pixmap        (GtkStyle     *style,
                                    GdkPixbuf    *pixbuf,
                                    GdkScreen    *screen);

gboolean   sanitize_size (          GdkWindow    *window,
                                    gint         *width,
                                    gint         *height);

void       rgb_to_hls              (gdouble      *r,
                                    gdouble      *g,
                                    gdouble      *b);

void       hls_to_rgb              (gdouble      *h,
                                    gdouble      *l,
                                    gdouble      *s);

void       shade                   (const CairoColor *a,
                                    CairoColor       *b,
                                    float             k);
						  
GtkWidget *special_get_ancestor    (GtkWidget    *widget,
                                    GType         widget_type);
					   
void       blend                   (GdkColormap  *colormap,
                                    GdkColor     *a,
                                    GdkColor     *b,
                                    GdkColor     *c,
                                    int           alpha);
			
GtkWidget*        get_parent_window       (GtkWidget    *widget);
GdkColor*         get_parent_bgcolor      (GtkWidget    *widget);
gboolean          is_combo_box            (GtkWidget    *widget);
GtkWidget*        find_combo_box_widget   (GtkWidget    *widget);

void              clearlooks_gdk_color_to_rgb   (GdkColor     *c,
                                                 double       *r,
                                                 double       *g,
                                                 double       *b);

void              gtk_treeview_get_header_index (GtkTreeView  *tv,
                                                 GtkWidget    *header,
                                                 gint         *column_index,
                                                 gint         *columns,
                                                 gboolean     *resizable);

void              gtk_clist_get_header_index    (GtkCList     *clist,
                                                 GtkWidget    *button,
                                                 gint         *column_index,
                                                 gint         *columns);

void              clearlooks_get_parent_bg      (const GtkWidget *widget,
                                                 CairoColor      *color);

void              option_menu_get_props         (GtkWidget       *widget,
                                                 GtkRequisition  *indicator_size,
                                                 GtkBorder       *indicator_spacing);

ClearlooksStepper scrollbar_get_stepper         (GtkWidget       *widget,
                                                 GdkRectangle    *stepper);
ClearlooksStepper scrollbar_visible_steppers    (GtkWidget       *widget);
ClearlooksJunction scrollbar_get_junction       (GtkWidget    *widget);

#endif /* SUPPORT_H */
