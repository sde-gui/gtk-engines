#include <gmodule.h>
#include "thinice_rc_style.h"
#include "thinice_style.h"

G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
  thinice_rc_style_register_type (module);
  thinice_style_register_type (module);
}

G_MODULE_EXPORT void
theme_exit (void)
{
}

G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  void *ptr;
  ptr = GTK_RC_STYLE (g_object_new (THINICE_TYPE_RC_STYLE, NULL));  
  return (GtkRcStyle *)ptr;
}
