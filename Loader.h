/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Interface.h"
#include "Lua_Shared.h"








typedef const char * (*lua_Reader) (GKit::GLua::lua_State *L, void *ud, size_t *sz);

typedef struct StringReaderCtx {
	const char *str;
	size_t size;
} StringReaderCtx;



namespace GKit
{
	namespace GLua
	{
		namespace Loader
		{
			int lua_load(lua_State* L, lua_Reader reader, void* data, const char* chunkname, const char* mode);

			int luaL_loadbufferx(lua_State* L, const char* buf, size_t size, const char* name, const char* mode);
			int luaL_loadbuffer(lua_State* L, const char* buf, size_t size, const char* name);

			int luaL_loadstring(lua_State* L, const char* s, const char* name);
		}
	}
}

