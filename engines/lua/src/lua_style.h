#include <gtk/gtkstyle.h>
#include <cairo.h>

#include "lua.h"
#include "ge-support.h"

typedef struct _LuaStyle LuaStyle;
typedef struct _LuaStyleClass LuaStyleClass;

GE_INTERNAL extern GType type_lua_style;

#define TYPE_LUA_STYLE              type_lua_style
#define LUA_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), TYPE_LUA_STYLE, LuaStyle))
#define LUA_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LUA_STYLE, LuaStyleClass))
#define LUA_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), TYPE_LUA_STYLE))
#define LUA_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LUA_STYLE))
#define LUA_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LUA_STYLE, LuaStyleClass))

typedef struct
{
	CairoColor bg[5];
	CairoColor base[5];
	CairoColor text[5];
	
	CairoColor shade[9];
	CairoColor spot[3];
} LuaColors;

struct _LuaStyle
{
	GtkStyle parent_instance;
	
	LuaColors colors;
	gchar *theme;
	
	lua_State *L;
};

struct _LuaStyleClass
{
  GtkStyleClass parent_class;
};


GE_INTERNAL void lua_style_register_type (GTypeModule *module);
