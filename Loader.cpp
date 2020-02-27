/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Loader - Manages the loading of lua files into a lua state

*/

#define USING_GMOD_CLUA_API_EXPORTS
#include "Loader.h"




static const char *reader_string(GKit::GLua::lua_State *L, void *ud, size_t *size)
{
	StringReaderCtx *ctx = (StringReaderCtx *)ud;
	(void)(L);
	if (ctx->size == 0) return NULL;
	*size = ctx->size;
	ctx->size = 0;
	return ctx->str;
}


int GKit::GLua::Loader::lua_load(lua_State* L, lua_Reader reader, void* data, const char* chunkname, const char* mode)
{
	return ::lua_loadx(L, reader, data, chunkname, mode);
}


int GKit::GLua::Loader::luaL_loadbufferx(lua_State* L, const char* buf, size_t size, const char* name, const char* mode)
{
	StringReaderCtx ctx;
	ctx.str = buf;
	ctx.size = size;

	return lua_load(L, reader_string, &ctx, name, mode);
}

int GKit::GLua::Loader::luaL_loadbuffer(lua_State* L, const char* buf, size_t size, const char* name)
{
	return luaL_loadbufferx(L, buf, size, name, NULL);
}


int GKit::GLua::Loader::luaL_loadstring(lua_State* L, const char* s, const char* name)
{
	return luaL_loadbuffer(L, s, strlen(s), name);
}