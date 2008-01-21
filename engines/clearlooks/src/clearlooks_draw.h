#ifndef CLEARLOOKS_DRAW_H
#define CLEARLOOKS_DRAW_H

#include "clearlooks_types.h"
#include "clearlooks_style.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <cairo.h>

GE_INTERNAL void clearlooks_register_style_classic (ClearlooksStyleFunctions *functions);
GE_INTERNAL void clearlooks_register_style_glossy (ClearlooksStyleFunctions *functions);
GE_INTERNAL void clearlooks_register_style_gummy (ClearlooksStyleFunctions *functions);
GE_INTERNAL void clearlooks_register_style_inverted (ClearlooksStyleFunctions *functions);

/* Fallback focus function */
GE_INTERNAL void clearlooks_draw_focus (cairo_t *cr,
                                        const ClearlooksColors *colors,
                                        const WidgetParameters *widget,
                                        const FocusParameters  *focus,
                                        int x, int y, int width, int height);

#endif /* CLEARLOOKS_DRAW_H */
