#include <gmodule.h>
#include <gtk/gtk.h>

#include "clearlooks_style.h"
#include "clearlooks_rc_style.h"

G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
  clearlooks_rc_style_register_type (module);
  clearlooks_style_register_type (module);
}

G_MODULE_EXPORT void
theme_exit (void)
{
}

G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  return GTK_RC_STYLE (g_object_new (CLEARLOOKS_TYPE_RC_STYLE, NULL));  
}
