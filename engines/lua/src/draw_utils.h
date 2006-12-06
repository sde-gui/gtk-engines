/** Cairo based drawing functions.
  */

#include <cairo.h>
#include "ge-support.h"

GE_INTERNAL void draw_rectangle (cairo_t *cr, gdouble x, gdouble y, gdouble width, gdouble height,
                                 gdouble radius, gboolean tl, gboolean tr, gboolean bl, gboolean br,
                                 gboolean filled);
