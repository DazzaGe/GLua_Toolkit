/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Windows.h"
#include "Interface.h"
#include "Lua_Shared.h"
#include "Loader.h"
#include "Error.h"




namespace GKit
{
	namespace GLua
	{
		namespace GState
		{
			typedef void(*CLuaFunction)(void* state);


			void	LoadBuffer(GLua::lua_State* state, const char* buffer, size_t bufferSize, const char* dbgStr);
			void	LoadBuffer(GLua::lua_State* state, const char* buffer, size_t bufferSize, const char* dbgStr, const char* mode);

			void	ExecuteCLua(GLua::lua_State* state, CLuaFunction func);
		}
	}
}