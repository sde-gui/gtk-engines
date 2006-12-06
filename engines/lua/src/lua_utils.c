#include "lua_utils.h"

void *
lua_utils_fetch_pointer (lua_State *L, const gchar *name)
{
	void *res;
	
	g_return_if_fail (L);
	
	lua_getfield (L, LUA_REGISTRYINDEX, name);
	if (!lua_isuserdata (L, -1))
	{
		lua_pop (L, 1);
		return NULL;
	}
	res = lua_touserdata (L, -1);
	lua_pop (L, 1);
	return res;
}

void
lua_utils_push_pointer (lua_State *L, const gchar *name, void *pointer)
{
	g_return_if_fail (L);

	lua_pushlightuserdata (L, pointer);
	lua_setfield (L, LUA_REGISTRYINDEX, name);
}