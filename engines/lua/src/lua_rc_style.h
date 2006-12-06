#include <gtk/gtkrc.h>

#include "ge-support.h"

typedef struct _LuaRcStyle LuaRcStyle;
typedef struct _LuaRcStyleClass LuaRcStyleClass;

GE_INTERNAL extern GType type_lua_rc_style;

#define TYPE_LUA_RC_STYLE              type_lua_rc_style
#define LUA_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), TYPE_LUA_RC_STYLE, LuaRcStyle))
#define LUA_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LUA_RC_STYLE, LuaRcStyleClass))
#define IS_LUA_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), TYPE_LUA_RC_STYLE))
#define IS_LUA_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LUA_RC_STYLE))
#define LUA_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LUA_RC_STYLE, LuaRcStyleClass))

struct _LuaRcStyle
{
	GtkRcStyle parent_instance;

	gchar *theme;
};

struct _LuaRcStyleClass
{
	GtkRcStyleClass parent_class;
};

GE_INTERNAL void lua_rc_style_register_type (GTypeModule *module);
