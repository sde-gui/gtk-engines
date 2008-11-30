#include <gmodule.h>
#include <gtk/gtk.h>

#include "lua_style.h"
#include "lua_rc_style.h"

GE_EXPORT void
theme_init (GTypeModule *module)
{
	lua_rc_style_register_types (module);
	lua_style_register_types (module);
}

GE_EXPORT void
theme_exit (void)
{
}

GE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
	return GTK_RC_STYLE (g_object_new (TYPE_LUA_RC_STYLE, NULL));  
}
