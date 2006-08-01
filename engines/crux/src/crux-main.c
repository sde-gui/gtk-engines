#include "crux.h"

#include <gtk/gtk.h>
#include <gmodule.h>
#include <stdio.h>

#include "crux-rc-style.h"
#include "crux-style.h"
#include "crux-draw.h"

#include "debug.h"


G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
  crux_rc_style_register_type (module);
  crux_style_register_type (module);
}

G_MODULE_EXPORT void
theme_exit(void)
{
}

G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  return GTK_RC_STYLE (g_object_new (CRUX_TYPE_RC_STYLE, NULL));
}
