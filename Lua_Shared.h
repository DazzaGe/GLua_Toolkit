/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Interface.h"




namespace GKit
{
	namespace GLua
	{
		void GetExports(HMODULE luaShared);
	}
}



#ifdef USING_GMOD_CLUA_API_EXPORTS



#define LUA_GLOBALSINDEX        (-10002)



typedef const char*		(*lua_Reader)			(GKit::GLua::lua_State* L, void* ud, size_t* sz);
typedef int				(*lua_Writer)			(GKit::GLua::lua_State* L, const void* p, size_t sz, void* ud);


typedef void			(*luaL_checktypeFn)		(GKit::GLua::lua_State* L, int arg, int t);
typedef int				(*luaL_getmetafieldFn)	(GKit::GLua::lua_State* L, int obj, const char* e);
typedef int				(*luaL_loadbufferFn)	(GKit::GLua::lua_State* L, const char* buffer, int bufferSize, const char* debugName);

typedef void			(*lua_callFn)			(GKit::GLua::lua_State* L, int nargs, int nresults);
typedef	int				(*lua_dumpFn)			(GKit::GLua::lua_State* L, lua_Writer writer, void *data);
typedef int				(*lua_gettopFn)			(GKit::GLua::lua_State* L);
typedef const char*		(*lua_getupvalueFn)		(GKit::GLua::lua_State* L, int funcindex, int n);
typedef int				(*lua_isstringFn)		(GKit::GLua::lua_State* L, int index);
typedef int				(*lua_loadFn)			(GKit::GLua::lua_State* L, lua_Reader reader, void* data, const char* chunkname);
typedef int				(*lua_loadxFn)			(GKit::GLua::lua_State* L, lua_Reader reader, void* data, const char* chunkname, const char* mode);
typedef	int				(*lua_nextFn)			(GKit::GLua::lua_State* L, int index);
typedef int				(*lua_pcallFn)			(GKit::GLua::lua_State* L, int nargs, int nresults, int msgh);
typedef void			(*lua_pushnilFn)		(GKit::GLua::lua_State* L);
typedef void			(*lua_pushvalueFn)		(GKit::GLua::lua_State* L, int index);
typedef int				(*lua_setfenvFn)		(GKit::GLua::lua_State* L, int index);
typedef void			(*lua_settopFn)			(GKit::GLua::lua_State* L, int index);
typedef const char*		(*lua_setupvalueFn)		(GKit::GLua::lua_State* L, int funcindex, int n);
typedef	const char*		(*lua_tolstringFn)		(GKit::GLua::lua_State* L, int index, size_t *len);



extern	luaL_checktypeFn		luaL_checktype;
extern	luaL_getmetafieldFn		luaL_getmetafield;
extern	luaL_loadbufferFn		luaL_loadbuffer;

extern	lua_callFn				lua_call;
extern	lua_dumpFn				lua_dump;
extern	lua_gettopFn			lua_gettop;
extern	lua_getupvalueFn		lua_getupvalue;
extern	lua_isstringFn			lua_isstring;
extern	lua_loadFn				lua_load;
extern	lua_loadxFn				lua_loadx;
extern	lua_nextFn				lua_next;
extern	lua_pcallFn				lua_pcall;
extern	lua_pushnilFn			lua_pushnil;
extern	lua_pushvalueFn			lua_pushvalue;
extern	lua_setfenvFn			lua_setfenv;
extern	lua_settopFn			lua_settop;
extern	lua_setupvalueFn		lua_setupvalue;
extern	lua_tolstringFn			lua_tolstring;



#define lua_upvalueindex(i)     (LUA_GLOBALSINDEX-(i))



#endif