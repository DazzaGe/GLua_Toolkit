/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	GLuaHelperFunctions - Helper functions for GLua

*/

#include "GLuaHelperFunctions.h"




int	GKit::Util::GLuaHelper::AbsoluteIndex(GKit::GLua::lua_State* state, int index)
{
	return index < 0 ? LUA->Top() + index + 1 : index;
}

GKit::GLua::TValue* GKit::Util::GLuaHelper::GetLuaValueAddress(GKit::GLua::lua_State* state, int index)
{
	index = AbsoluteIndex(state, index);

	return state->base + (index - 1);
}


void GKit::Util::GLuaHelper::ConCommand(GKit::GLua::lua_State* state, const char* name, GarrysMod::Lua::CFunc callback)
{
	ConCommand(state, name, callback, nullptr, nullptr, 0);
}

void GKit::Util::GLuaHelper::ConCommand(GKit::GLua::lua_State* state, const char* name, GarrysMod::Lua::CFunc callback, GarrysMod::Lua::CFunc autoComplete, const char* help, int flags)
{
	// Push concommand.Add
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->GetField(-1, "concommand");
	LUA->GetField(-1, "Add");

	// Push args
	LUA->PushString(name);
	LUA->PushCFunction(callback);

	if (autoComplete == nullptr)
		LUA->PushNil();
	else
		LUA->PushCFunction(autoComplete);

	if (help == nullptr)
		LUA->PushNil();
	else
		LUA->PushString(help);

	LUA->PushNumber(flags);

	// Call concommand.Add
	LUA->Call(5, 0);

	// Pop concommand and global off
	LUA->Pop(2);
}


bool GKit::Util::GLuaHelper::PushGKitInterfaceTable(GKit::GLua::lua_State* state, const char* interfaceName)
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG);

	// Check if interface table exists
	LUA->PushString(interfaceName);
	LUA->RawGet(-2);
	if (!LUA->IsType(-1, GarrysMod::Lua::Type::TABLE))
	{
		LUA->Pop(2);
		return false;
	}

	// Remove Reg
	LUA->Remove(AbsoluteIndex(state, -2));
	return true;
}


void GKit::Util::GLuaHelper::DetourCLuaFunction(GKit::GLua::lua_State* state, int table)
{
	table = AbsoluteIndex(state, table);
	int key = LUA->Top() - 1;
	int detour = LUA->Top();

	// Get builtinnum
	LUA->Push(key);
	LUA->RawGet(table);

	if (!LUA->IsType(-1, GarrysMod::Lua::Type::FUNCTION))
	{
		// Pop Args and Non Func
		LUA->Pop(3);
		return;
	}

	BYTE builtinnum = GetLuaValueAddress(state, -1)->value.gc->c.builtin_num;
	LUA->Pop();	// Pop Function to detour

	// Change New builtinnum
	GetLuaValueAddress(state, detour)->value.gc->c.builtin_num = builtinnum;

	// Detour Function
	LUA->RawSet(table);
}