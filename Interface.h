/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Windows.h"
#include "Types.h"
#include "LuaBase.h"
#include "UserData.h"




#define LUA state->luabase



namespace GKit
{
	namespace GLua
	{
		struct Proto
		{

		};

		struct LClosure
		{
			BYTE		unknown1[6];
			BYTE		builtin_num;
		};

		struct CClosure
		{
			BYTE		unknown1[6];
			BYTE		builtin_num;
		};

		typedef union
		{
			LClosure	l;
			CClosure	c;
		} GCObject;

		typedef union
		{
			GCObject*	gc;
			void*		p;
			double		n;
			int			b;
		} Value;

		struct TValue
		{
			Value	value;	//gc
			int		tt;
		};

		struct global_State
		{
			void**		vTable;
			lua_State*	mainThread;
			// Other idk
		};

		struct lua_State
		{
			BYTE						unkown1[16];
			GKit::GLua::TValue*			base;			// stack base real pointer
			BYTE						unknown2[49];
			GarrysMod::Lua::ILuaBase*	luabase;		// globalstate
		};
	}
}
