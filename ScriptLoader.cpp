/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	ScriptLoader - Supplies lualoader commands to menu

*/

#include "ScriptLoader.h"




namespace
{
	GKit::GLua::MenuState* _menu = nullptr;
	GKit::GLua::ClientState* _client = nullptr;
	GKit::Util::File* _files = nullptr;


	void BeforeAutorun(void* infoStruct, void* arg)
	{
		GKit::Tools::ScriptLoader::LBA_Info* params = (GKit::Tools::ScriptLoader::LBA_Info*)arg;
		GKit::GLua::ILuaInterface::RunStringEx_Info* info = (GKit::GLua::ILuaInterface::RunStringEx_Info*)infoStruct;

		GKit::GLua::lua_State* state = ((GKit::GLua::global_State*)info->state)->mainThread;

		try
		{
			GKit::GLua::GState::LoadBuffer(state, params->toRun, strlen(params->toRun), params->dbgStr);
		}
		catch (GKit::Util::Exception& e)
		{
			GKit::Util::ExceptionMenu(&e);
		}
		// should add feature to remove from autorun
		//delete toRun;
	}


	int LoadScriptCallback(GKit::GLua::lua_State* state)
	{
		// Check Valud
		LUA->PushNumber(1);
		LUA->GetTable(3);
		LUA->PushNumber(2);
		LUA->GetTable(3);
		if (!LUA->IsType(-1, GarrysMod::Lua::Type::STRING) || !LUA->IsType(-2, GarrysMod::Lua::Type::STRING))
		{
			LUA->Pop(2);
			return 0;
		}
		LUA->Pop(2);

		// Get Arguments
		LUA->PushNumber(1);
		LUA->GetTable(3);
		const char*	scriptName = LUA->GetString(-1);

		LUA->PushNumber(2);
		LUA->GetTable(3);
		const char*	dbgStr = LUA->GetString(-1);

		LUA->PushNumber(3);
		LUA->GetTable(3);
		int lba = LUA->GetNumber(-1);

		LUA->Pop(3);

		// Get Script
		char* script = nullptr;
		void* openFile = nullptr;

		try
		{
			openFile = _files->OpenFile((char*)scriptName, GKit::Util::Open_Mode::NORMAL);
		}
		catch (GKit::Util::Exception& e)
		{
			GKit::Util::ExceptionMenu(&e);
			return 0;
		}

		_files->ReadFile(openFile, &script);

		// Run
		if (lba == 1)
		{
			GKit::Tools::ScriptLoader::LBA_Info* params = new GKit::Tools::ScriptLoader::LBA_Info;

			char* toRun = new char[strlen(script) + 1];
			strcpy(toRun, script);

			char* heap_dbgStr = new char[strlen(dbgStr) + 1];
			strcpy(heap_dbgStr, dbgStr);

			params->toRun = toRun;
			params->dbgStr = heap_dbgStr;

			_client->HookEvent(GKit::GLua::Event::STARTUP, BeforeAutorun, params);
		}
		else
		{
			try
			{
				_client->LoadBuffer(script, strlen(script), dbgStr);
			}
			catch (GKit::Util::Exception& e)
			{
				GKit::Util::ExceptionMenu(&e);
			}
		}

		_files->CloseFile(openFile);
		return 0;
	}

	int LoadStringCallback(GKit::GLua::lua_State* state)
	{
		// Get String
		const char* toRun = LUA->GetString(4);

		// Run
		try
		{
			_client->LoadBuffer((char*)toRun, strlen(toRun), "");
		}
		catch (GKit::Util::Exception& e)
		{
			GKit::Util::ExceptionMenu(&e);
		}

		return 0;
	}


	void DoCommands(GKit::GLua::lua_State* state)
	{
		GKit::Util::GLuaHelper::ConCommand(state, GKIT_LUA_LOADSCRIPT, LoadScriptCallback);
		GKit::Util::GLuaHelper::ConCommand(state, GKIT_LUA_LOADSTRING, LoadStringCallback);
	}
}



void GKit::Tools::ScriptLoader::Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* files)
{
	_menu = menu;
	_client = client;
	_files = files;

	try
	{
		menu->ExecuteCLua((GKit::GLua::GState::CLuaFunction)DoCommands);
	}
	catch (GKit::Util::Exception& e)
	{
		GKit::Util::ExceptionMenu(&e);
	}
}