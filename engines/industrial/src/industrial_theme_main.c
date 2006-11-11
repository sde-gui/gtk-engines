#include <gmodule.h>
#include <gtk/gtk.h>

#include "industrial_style.h"
#include "industrial_rc_style.h"

GE_EXPORT void
theme_init (GTypeModule * module)
{
	industrial_rc_style_register_type (module);
	industrial_style_register_type (module);
}

GE_EXPORT void
theme_exit (void)
{
}

GE_EXPORT GtkRcStyle*
theme_create_rc_style (void)
{
	return GTK_RC_STYLE (g_object_new (INDUSTRIAL_TYPE_RC_STYLE, NULL));
}
