#include "crux-style.h"
#include "crux-draw.h"

#include <gtk/gtk.h>
#include <stdio.h>

GType crux_type_style = 0;

static void crux_style_class_init (CruxStyleClass *klass);

void
crux_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (CruxStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) crux_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (CruxStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) NULL,
  };

  crux_type_style = g_type_module_register_type (module,
						 GTK_TYPE_STYLE,
						 "CruxStyle",
						 &object_info, 0);
}

static void
crux_style_class_init (CruxStyleClass *klass)
{
  GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);
  crux_draw_style_class_init (style_class);
}
    

