#include <gtk/gtkstyle.h>
#include <cairo.h>

#include "lua.h"
#include "ge-support.h"

typedef struct _LuaStyle LuaStyle;
typedef struct _LuaStyleClass LuaStyleClass;

#define TYPE_LUA_STYLE              (lua_style_get_type ())
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


GE_INTERNAL void  lua_style_register_types (GTypeModule *module);
GE_INTERNAL GType lua_style_get_type (void);

