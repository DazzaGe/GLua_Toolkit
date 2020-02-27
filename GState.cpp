/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	GState - Functions for interacting with a lua state

*/

#define USING_GMOD_CLUA_API_EXPORTS
#include "GState.h"




namespace
{
	bool PushLuaFunction(GKit::GLua::lua_State* state, const char* buffer, unsigned int bufferSize, const char* dbgStr, const char* mode)
	{
		int result = GKit::GLua::Loader::luaL_loadbufferx(state, buffer, bufferSize, dbgStr, mode);
		if (result != 0)
		{
			const char* error = state->luabase->GetString(-1);

			// Pop error message off
			lua_settop(state, lua_gettop(state) - 1);

			throw GKit::Util::Exception(GKIT_ERROR_SCRIPT_LOAD, error, result);
		}

		return true;
	}
}



void GKit::GLua::GState::LoadBuffer(GLua::lua_State* state, const char* buffer, size_t bufferSize, const char* dbgStr)
{
	LoadBuffer(state, buffer, bufferSize, dbgStr, "bt");
}

void GKit::GLua::GState::LoadBuffer(GLua::lua_State* state, const char* buffer, size_t bufferSize, const char* dbgStr, const char* mode)
{
	PushLuaFunction(state, buffer, bufferSize, dbgStr, mode);

	int result = lua_pcall(state, 0, 0, 0);
	if (result != 0)
	{
		const char* error = state->luabase->GetString(-1);

		// Pop error message off (pcall)
		lua_settop(state, lua_gettop(state) - 1);

		throw GKit::Util::Exception(GKIT_ERROR_SCRIPT_EXECUTION, error, result);
	}

}


void GKit::GLua::GState::ExecuteCLua(GLua::lua_State* state, CLuaFunction func)
{
	func(state);
}
