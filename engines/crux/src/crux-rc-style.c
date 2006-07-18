#include <gtk/gtk.h>

#include "crux-rc-style.h"
#include "crux-style.h"

#include "debug.h"

#include <stdio.h>
#include <string.h>

GType crux_type_rc_style = 0;

static GtkStyle *crux_rc_style_create_style (GtkRcStyle *rc_style);

static void
crux_rc_style_class_init (CruxRcStyleClass *klass)
{
	GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);
	rc_style_class->create_style = crux_rc_style_create_style;
}

void
crux_rc_style_register_type (GTypeModule *module)
{
  static const GTypeInfo object_info =
  {
    sizeof (CruxRcStyleClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) crux_rc_style_class_init,
    NULL,           /* class_finalize */
    NULL,           /* class_data */
    sizeof (CruxRcStyle),
    0,              /* n_preallocs */
    (GInstanceInitFunc) NULL,
  };

  crux_type_rc_style = g_type_module_register_type (module,
                                                      GTK_TYPE_RC_STYLE,
                                                      "CruxRcStyle",
                                                      &object_info, 0);
}

static GtkStyle *
crux_rc_style_create_style (GtkRcStyle *rc_style)
{
  return GTK_STYLE (g_object_new (CRUX_TYPE_STYLE, NULL));
}
