/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Interface.h"
#include "GKit.h"




namespace GKit
{
	namespace Util
	{
		namespace GLuaHelper
		{
			int						AbsoluteIndex(GKit::GLua::lua_State* state, int index);
			GKit::GLua::TValue*		GetLuaValueAddress(GKit::GLua::lua_State* state, int index);

			void					ConCommand(GKit::GLua::lua_State* state, const char* name, GarrysMod::Lua::CFunc callback);
			void					ConCommand(GKit::GLua::lua_State* state, const char* name, GarrysMod::Lua::CFunc callback, GarrysMod::Lua::CFunc autoComplete, const char* help, int flags);

			bool					PushGKitInterfaceTable(GKit::GLua::lua_State* state, const char* interfaceName);

			void					DetourCLuaFunction(GKit::GLua::lua_State* state, int table);
		}
	}
}