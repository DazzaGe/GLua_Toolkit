/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Lua_Shared - Allows use of Lua_Shared.dll export functions

*/

#define USING_GMOD_CLUA_API_EXPORTS
#include "Lua_Shared.h"




luaL_checktypeFn	luaL_checktype = nullptr;
luaL_getmetafieldFn	luaL_getmetafield = nullptr;
luaL_loadbufferFn	luaL_loadbuffer = nullptr;

lua_callFn			lua_call = nullptr;
lua_dumpFn			lua_dump = nullptr;
lua_gettopFn		lua_gettop = nullptr;
lua_getupvalueFn	lua_getupvalue = nullptr;
lua_isstringFn		lua_isstring = nullptr;
lua_loadFn			lua_load = nullptr;
lua_loadxFn			lua_loadx = nullptr;
lua_nextFn			lua_next = nullptr;
lua_pcallFn			lua_pcall = nullptr;
lua_pushnilFn		lua_pushnil = nullptr;
lua_pushvalueFn		lua_pushvalue = nullptr;
lua_setfenvFn		lua_setfenv = nullptr;
lua_settopFn		lua_settop = nullptr;
lua_setupvalueFn	lua_setupvalue = nullptr;
lua_tolstringFn		lua_tolstring = nullptr;


void GKit::GLua::GetExports(HMODULE luaShared)
{
	luaL_checktype		= (luaL_checktypeFn)	GetProcAddress(luaShared, "luaL_checktype");
	luaL_getmetafield	= (luaL_getmetafieldFn)	GetProcAddress(luaShared, "luaL_getmetafield");
	luaL_loadbuffer		= (luaL_loadbufferFn)	GetProcAddress(luaShared, "luaL_loadbuffer");

	lua_call			= (lua_callFn)			GetProcAddress(luaShared, "lua_call");
	lua_dump			= (lua_dumpFn)			GetProcAddress(luaShared, "lua_dump");
	lua_gettop			= (lua_gettopFn)		GetProcAddress(luaShared, "lua_gettop");
	lua_getupvalue		= (lua_getupvalueFn)	GetProcAddress(luaShared, "lua_getupvalue");
	lua_isstring		= (lua_isstringFn)		GetProcAddress(luaShared, "lua_isstring");
	lua_load			= (lua_loadFn)			GetProcAddress(luaShared, "lua_load");
	lua_loadx			= (lua_loadxFn)			GetProcAddress(luaShared, "lua_loadx");
	lua_next			= (lua_nextFn)			GetProcAddress(luaShared, "lua_next");
	lua_pcall			= (lua_pcallFn)			GetProcAddress(luaShared, "lua_pcall");
	lua_pushnil			= (lua_pushnilFn)		GetProcAddress(luaShared, "lua_pushnil");
	lua_pushvalue		= (lua_pushvalueFn)		GetProcAddress(luaShared, "lua_pushvalue");
	lua_setfenv			= (lua_setfenvFn)		GetProcAddress(luaShared, "lua_setfenv");
	lua_settop			= (lua_settopFn)		GetProcAddress(luaShared, "lua_settop");
	lua_setupvalue		= (lua_setupvalueFn)	GetProcAddress(luaShared, "lua_setupvalue");
	lua_tolstring		= (lua_tolstringFn)		GetProcAddress(luaShared, "lua_tolstring");
}
