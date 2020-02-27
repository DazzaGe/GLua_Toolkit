/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Main - Central file manages the DLL

*/

#include "Main.h"




HINSTANCE GKit::_this;



void Main()
{
	// Get Lua_Shared.dll interface
	HMODULE luaShared = GetModuleHandleA("lua_shared.dll");
	void* iLuaSharedClass = GKit::Util::Source::CreateInterface("lua_shared.dll", "LUASHARED003");
	
	// Get Lua_Shared.dll exports
	GKit::GLua::GetExports(luaShared);

	// Setup FileIO
	GKit::Util::File files = GKit::Util::File(GKIT_DIRECTORY);

	// Setup Core
	GKit::GLua::ILuaShared::LuaCore core = GKit::GLua::ILuaShared::LuaCore(luaShared, iLuaSharedClass);

	// Setup States
	GKit::GLua::MenuState menu = GKit::GLua::MenuState(&core);
	GKit::GLua::ClientState client = GKit::GLua::ClientState(&core);

	GKit::Util::SetMenu(&menu);

	// Open Tools
	GKit::Tools::ClientInit::Init(&menu, &client, &files);
	GKit::Tools::CCalls::Init(&menu, &client, &files);
	GKit::Tools::RunstringEx::Init(&menu, &client, &files);
	GKit::Tools::LuaDetour::Init(&menu, &client, &files);
	//GKit::Tools::StringTables::Init(&menu, &client, &files);
	//GKit::Tools::Engine::Init(&menu, &client, &files);
	GKit::Tools::MenuURL::Init(&menu, &client, &files);

	// Need to load last
	GKit::Tools::ScriptLoader::Init(&menu, &client, &files);
	GKit::Tools::Base::Init(&menu, &client, &files);

	while (true)
	{
		Sleep(1000);
	}
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	GKit::_this = hinstDLL;

	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Main, NULL, 0, NULL);
	}
}