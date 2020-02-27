/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Base - Supplies base menu printout

*/

#include "Base.h"




namespace
{
	GKit::Util::File* files = nullptr;


	void ExecuteCommand(GKit::GLua::lua_State* state, char* command)
	{
		if (strlen(command) < 2)
			return;

		// comments
		if (command[0] == '/' && command[1] == '/')
			return;

		// get first real character
		size_t len = strlen(command);
		char* realCommand = nullptr;
		for (unsigned int i = 0; i < len; i++)
		{
			if (isalnum(command[i]))
			{
				realCommand = command + i;
				break;
			}
		}

		if (realCommand == nullptr)
			return;

		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);

		// Notify Menu
		LUA->GetField(-1, "print");
		LUA->PushString("RAN COMMAND:");
		LUA->PushString(realCommand);
		LUA->Call(2, 0);

		// Push Function
		LUA->GetField(-1, "RunConsoleCommand");

		// Push Command Name
		char* save = nullptr;
		char* token = strtok_s(realCommand, " \t", &save);
		unsigned int args = 0;

		// Push Args
		while (token != nullptr)
		{
			if (strlen(token) >= 1 && isalnum(token[0]))
			LUA->PushString(token);
			token = strtok_s(nullptr, " \t", &save);
			args++;
		}

		// Call Function (Can't PCall as PCall changes token or something, just deal with the lua crash errors if you screwup startup.txt)
		LUA->Call(args, 0);

		LUA->Pop();
	}


	const char* display = "\nGLua Advanced Toolkit 1.3.0\n\n"
		"=============================================================\n\n"
		"  Modules:			                                          \n\n"
		"- Lua Loader                                                 \n"
		"- RunStringEx hooker                                         \n"
		"- Hook Hooker                                                \n"
		"- ConCommand Hooker                                          \n"
		"- (SEMI IMPLEMENTED) Advanced Lua Detour                     \n"
		"- (NOT IMPLEMENTED) Stringtables API                         \n"
		"- (NOT IMPLEMENTED) Source Engine API                        \n\n"
		"RAT SUCCESSFULLY INSTALLED\n\n";

	void Welcome(GKit::GLua::lua_State* state)
	{
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "print");
		LUA->PushString(display);
		LUA->Call(1, 0);
		LUA->Pop();
	}


	void Startup(GKit::GLua::lua_State* state)
	{
		// Read Startup
		void* startupFile = files->OpenFile((char*)GKIT_STARTUP_FILE, GKit::Util::Open_Mode::NORMAL);
		char* startup = nullptr;
		files->ReadFile(startupFile, &startup);

		// Read Lines and Execeute Command
		char* save = nullptr;
		char* token = strtok_s(startup, "\r\n", &save);	// i should create custom token function with strstr to replace both \r and \n

		while (token != nullptr)
		{
			ExecuteCommand(state, token);
			token = strtok_s(nullptr, "\r\n", &save);
		}

		// Cleanup
		files->CloseFile(startupFile);
	}
}



void GKit::Tools::Base::Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* _files)
{
	files = _files;

	try
	{
		menu->ExecuteCLua((GKit::GLua::GState::CLuaFunction)Welcome);
		menu->ExecuteCLua((GKit::GLua::GState::CLuaFunction)Startup);
	}
	catch (GKit::Util::Exception& e)
	{
		GKit::Util::ExceptionMenu(&e);
	}
}
