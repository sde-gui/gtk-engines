#if ß
#include <gtk/gtk.h>

/* While the UNKOWN cases should never happen, they shall be
 * handled in a sane way by the theme. ie. don't fuck up in that case ;-) */
typedef enum {
  GE_HEADER_UNKOWN = 0,
  GE_HEADER_FIRST,
  GE_HEADER_MIDDLE,
  GE_HEADER_LAST,
} GEHeaderPosition;

typedef enum {
  GE_STEPPER_UNKOWN = 0,
  GE_STEPPER_A,
  GE_STEPPER_B,
  GE_STEPPER_C,
  GE_STEPPER_D,
} GEStepper;

/* get the text direction of the widget, if widget is NULL the
 * default direction will be returned. */
GtkTextDirection ge_get_direction	(GtkWidget *widget) G_GNUC_INTERNAL;

/* Will try to get the header index that is drawn*/
GEHeaderPosition ge_get_header_index	(GtkWidget	*widget,
                                         GtkWidget	*button,
                                         gboolean	 resizable) G_GNUC_INTERNAL;

GEStepper ge_get_stepper                (GtkWidget      *widget,
                                         GdkRectangle   *position) G_GNUC_INTERNAL;


#endif