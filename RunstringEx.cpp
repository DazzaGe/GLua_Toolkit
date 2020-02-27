/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	RunstringEx - Provides interface for lua to to intercept runstringex calls ran on client

*/

#include "RunstringEx.h"




namespace
{
	GKit::GLua::MenuState* _menu = nullptr;
	GKit::GLua::ClientState* _client = nullptr;
	GKit::Util::File* _files = nullptr;



	void RunstringEx_Callback(void* infostruct, void* var)
	{
		GKit::GLua::ILuaInterface::RunStringEx_Info* info = (GKit::GLua::ILuaInterface::RunStringEx_Info*)infostruct;
		GKit::GLua::lua_State* state = (info->state)->mainThread;

		// Push The RunstringEx Handler
		if (!GKit::Util::GLuaHelper::PushGKitInterfaceTable(state, _client->GetRandomString()))
			return;

		LUA->GetField(-1, "RunstringExHandler");
		if (!LUA->IsType(-1, GarrysMod::Lua::Type::FUNCTION))
		{
			// handler doesn't exist, don't do anything else
			LUA->Pop(2);
			return;
		}

		// Push Arguments
		LUA->PushString(info->filename);
		LUA->PushString(info->path);
		LUA->PushString(info->toRun);

		// Call Handler
		if (LUA->PCall(3, 1, 0) != 0)
		{
			// pop error and gkitinterface
			LUA->Pop(2);
			return;
		}

		if (LUA->GetBool(-1))
		{
			info->stopExecution = true;
			info->_return = 0x0;
		}

		LUA->Pop(2);
	}
}



void GKit::Tools::RunstringEx::Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* files)
{
	_menu = menu;
	_client = client;
	_files = files;

	client->HookEvent(GKit::GLua::Event::RUNSTRINGEX, RunstringEx_Callback, nullptr);
}