#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "ge-support.h"

GE_INTERNAL void* lua_utils_fetch_pointer (lua_State *L, const gchar *name);
GE_INTERNAL void lua_utils_push_pointer (lua_State *L, const gchar *name, void *pointer);
