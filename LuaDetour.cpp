/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	LuaDetour - Provides advanced function detouring to lua (change debug info of function)

*/

#include "LuaDetour.h"




namespace
{
	GKit::GLua::MenuState* _menu = nullptr;
	GKit::GLua::ClientState* _client = nullptr;
	GKit::Util::File* _files = nullptr;



	int SetBuiltin(GKit::GLua::lua_State* state)
	{
		int newbuiltin = LUA->GetNumber(2);

		GKit::Util::GLuaHelper::GetLuaValueAddress(state, 1)->value.gc->c.builtin_num = newbuiltin;

		return 0;
	}

	int SetInfo(GKit::GLua::lua_State* state)
	{
		// NEED TO FIND LOCATION OF VALUE
		return 0;
	}

	int Detour(GKit::GLua::lua_State* state)
	{
		// NEED TO FIND LOCATION OF VALUE
		return 0;
	}


	void CreateDetourFunctions(GKit::GLua::lua_State* state)
	{
		// Get Detour Table
		if (!GKit::Util::GLuaHelper::PushGKitInterfaceTable(state, _client->GetRandomString()))
			return;

		LUA->CreateTable();

		// Push Functions
		LUA->PushCFunction(SetBuiltin);
		LUA->SetField(-2, "setbuiltin");

		LUA->PushCFunction(SetInfo);
		LUA->SetField(-2, "setinfo");

		LUA->PushCFunction(Detour);
		LUA->SetField(-2, "detour");

		// Cleanup
		LUA->SetField(-2, "detour");
		LUA->Pop();
	}


	void AutorunHook(void* infoStruct, void* arg)
	{
		GKit::GLua::ILuaInterface::RunStringEx_Info* info = (GKit::GLua::ILuaInterface::RunStringEx_Info*)infoStruct;
		GKit::GLua::lua_State* state = ((GKit::GLua::global_State*)info->state)->mainThread;

		try
		{
			GKit::GLua::GState::ExecuteCLua(state, (GKit::GLua::GState::CLuaFunction)CreateDetourFunctions);
		}
		catch (GKit::Util::Exception& e)
		{
			GKit::Util::ExceptionMenu(&e);
		}
	}
}



void GKit::Tools::LuaDetour::Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* files)
{
	_menu = menu;
	_client = client;
	_files = files;

	client->HookEvent(GKit::GLua::Event::STARTUP, AutorunHook, nullptr);
}