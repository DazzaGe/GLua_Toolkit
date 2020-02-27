/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	MenuURL - Allows custom menu background and loading screen background to be set

*/

#include "MenuURL.h"




namespace
{
	GKit::GLua::MenuState* _menu = nullptr;
	GKit::GLua::ClientState* _client = nullptr;
	GKit::Util::File* _files = nullptr;


	int SetLoadingURL(GKit::GLua::lua_State* state)
	{
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->PushNumber(1);
		LUA->GetTable(3);
		LUA->SetField(-2, "gkit_custom_loading_url");
		LUA->Pop();

		return 0;
	}

	int SetMainURL(GKit::GLua::lua_State* state)
	{
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "SetBackgroundURL");
		LUA->PushNumber(1);
		LUA->GetTable(3);
		LUA->Call(1, 0);
		LUA->Pop();

		return 0;
	}


	void DoCommands(GKit::GLua::lua_State* state)
	{
		GKit::Util::GLuaHelper::ConCommand(state, GKIT_MENU_LOADING_URL, SetLoadingURL);
		GKit::Util::GLuaHelper::ConCommand(state, GKIT_MENU_MAIN_URL, SetMainURL);
	}
}



void GKit::Tools::MenuURL::Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* files)
{
	_menu = menu;
	_client = client;
	_files = files;

	try
	{
		const char* initLua = GKit::Util::GetLuaResource(IDR_LUA1);
		menu->LoadBuffer(initLua, strlen(initLua), "");

		menu->ExecuteCLua((GKit::GLua::GState::CLuaFunction)DoCommands);
	}
	catch (GKit::Util::Exception& e)
	{
		GKit::Util::ExceptionMenu(&e);
	}
}