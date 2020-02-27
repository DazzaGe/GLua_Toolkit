/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	ClientInit - Initializes essential gkit facilities in the clientstate

*/

#define USING_GMOD_CLUA_API_EXPORTS
#include "ClientInit.h"




namespace
{
	GKit::GLua::MenuState* _menu = nullptr;
	GKit::GLua::ClientState* _client = nullptr;
	GKit::Util::File* _files = nullptr;



	int Next_Detour(GKit::GLua::lua_State* state)
	{
		LUA->CheckType(1, GarrysMod::Lua::Type::TABLE);
		lua_settop(state, 2);
		if (LUA->Next(1))
		{
			if (LUA->IsType(-2, GarrysMod::Lua::Type::STRING) && strcmp(LUA->GetString(-2), _client->GetRandomString()) == 0)
			{
				// If tablen is our interface skip over it
				LUA->Pop();
				return Next_Detour(state);
			}
			else
			{
				return 2;
			}
		}
		else
		{
			LUA->PushNil();
			return 1;
		}
	}

	int Pairs_Detour(GKit::GLua::lua_State* state)
	{
		LUA->CheckType(1, GarrysMod::Lua::Type::TABLE);
		if (luaL_getmetafield(state, 1, "__pairs") == 0) 
		{
			LUA->PushCFunction(Next_Detour);
			LUA->Push(1);
			LUA->PushNil();
		}
		else 
		{
			LUA->Push(1);
			LUA->Call(1, 3);
		}

		return 3;
	}


	void Startup_Callback(void* infostruct, void* var)
	{
		GKit::GLua::ILuaInterface::RunStringEx_Info* info = (GKit::GLua::ILuaInterface::RunStringEx_Info*)infostruct;
		GKit::GLua::lua_State* state = info->state->mainThread;

		// Create GKit interface table
		const char* interfacName = _client->GetRandomString();

		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG);
		LUA->CreateTable();
		LUA->SetField(-2, interfacName);

		// Set GKit interface table name in global (for gkit scripts to know, they can remove after)
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->PushString(interfacName);
		LUA->SetField(-2, "GKIT_INTERFACE");

		// Detour next and pairs (so they can't be used to find gkit interface)
		LUA->PushString("next");
		LUA->PushCFunction(Next_Detour);
		GKit::Util::GLuaHelper::DetourCLuaFunction(state, -3);

		LUA->PushString("pairs");
		LUA->PushCFunction(Pairs_Detour);
		GKit::Util::GLuaHelper::DetourCLuaFunction(state, -3);

		LUA->Pop(2);
	}
}



void GKit::Tools::ClientInit::Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* files)
{
	_menu = menu;
	_client = client;
	_files = files;

	client->HookEvent(GKit::GLua::Event::STARTUP, Startup_Callback, nullptr);
}