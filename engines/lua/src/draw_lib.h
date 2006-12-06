/** Provides a library of drawing functions for Lua 
  */
  
#include <cairo.h>

#include "lua.h"

#include "ge-support.h"

typedef struct {
	gboolean is_bezier;
	gdouble x;
	gdouble y;
	gdouble x1;
	gdouble y1;
	gdouble x2;
	gdouble y2;
} BezierPoint;

GE_INTERNAL gboolean luaopen_draw (lua_State *L);
