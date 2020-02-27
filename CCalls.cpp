/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	CCalls - Provides interface to hook and concommand calls before they are called

*/

#define USING_GMOD_CLUA_API_EXPORTS
#include "CCalls.h"




namespace
{
	GKit::GLua::MenuState* _menu = nullptr;
	GKit::GLua::ClientState* _client = nullptr;
	GKit::Util::File* _files = nullptr;


	bool PushCCallHandler(GKit::GLua::lua_State* state)
	{
		// Push The RunstringEx Handler
		if (!GKit::Util::GLuaHelper::PushGKitInterfaceTable(state, _client->GetRandomString()))
			return false;

		LUA->GetField(-1, "EngineCallHandler");
		if (!LUA->IsType(-1, GarrysMod::Lua::Type::FUNCTION))
		{
			LUA->Pop(2);
			return false;
		}

		LUA->Remove(GKit::Util::GLuaHelper::AbsoluteIndex(state, -2));
		return true;
	}


	bool PushModuleFunc(GKit::GLua::lua_State* state, int environment, const char* module, const char* func)
	{
		environment = GKit::Util::GLuaHelper::AbsoluteIndex(state, environment);;

		LUA->GetField(environment, module);
		if (LUA->IsType(-1, GarrysMod::Lua::Type::TABLE))
		{
			LUA->GetField(-1, func);

			if (LUA->IsType(-1, GarrysMod::Lua::Type::FUNCTION))
			{
				LUA->Remove(GKit::Util::GLuaHelper::AbsoluteIndex(state, -2));
				return true;
			}

			LUA->Pop();
		}

		LUA->Pop();
		return false;
	}

	bool HijackCall(GKit::GLua::lua_State* state, GKit::Tools::CCalls::Call_Type type, int toCall, int firstArg, int numArgs, int results)
	{
		toCall = GKit::Util::GLuaHelper::AbsoluteIndex(state, toCall);
		firstArg = GKit::Util::GLuaHelper::AbsoluteIndex(state, firstArg);

		// Push copy of orignal function
		LUA->Push(toCall);

		// Push type
		LUA->PushNumber(type);

		// Push copies of original args
		for (int i = firstArg; i < firstArg + numArgs; i++)
		{
			LUA->Push(i);
		}

		// Pcall new function
		int res = lua_pcall(state, numArgs + 1, results, 0);
		if (res != 0)
		{
			LUA->Pop();
		}

		return res;
	}


	void PCall_Callback(void* infostruct, void* var)
	{
		GKit::GLua::ILuaInterface::PCall_Info* info = (GKit::GLua::ILuaInterface::PCall_Info*)infostruct;
		GKit::GLua::lua_State* state = (info->state)->mainThread;

		GKit::Tools::CCalls::Call_Type hijackType = GKit::Tools::CCalls::Call_Type::NONE;
		int callee = GKit::Util::GLuaHelper::AbsoluteIndex(state, -(info->args) - 1);
		bool block = false;

		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);

		//
		//	Determine if function call needs to be hijacked
		//

		// hook.Run
		if (PushModuleFunc(state, -1, "hook", "Run"))
		{
			if (LUA->RawEqual(callee, -1) == 1)
			{
				hijackType = GKit::Tools::CCalls::Call_Type::HOOK_RUN;
			}

			LUA->Pop();
		}

		// hook.Call
		if (hijackType == GKit::Tools::CCalls::Call_Type::NONE && PushModuleFunc(state, -1, "hook", "Call"))
		{
			int equal = LUA->RawEqual(callee, GKit::Util::GLuaHelper::AbsoluteIndex(state, -1));
			if (equal == 1)
			{
				hijackType = GKit::Tools::CCalls::Call_Type::HOOK_CALL;
			}

			LUA->Pop();
		}

		// concommand.Run
		if (hijackType == GKit::Tools::CCalls::Call_Type::NONE && PushModuleFunc(state, -1, "concommand", "Run"))
		{
			if (LUA->RawEqual(callee, -1) == 1)
			{
				hijackType = GKit::Tools::CCalls::Call_Type::CONCOMMAND_RUN;
			}

			LUA->Pop();
		}

		// pop global
		LUA->Pop();

		//
		//	Hijack Function Call
		//

		bool ccallexists = PushCCallHandler(state);

		if (ccallexists)
		{
			switch (hijackType)
			{
				case GKit::Tools::CCalls::Call_Type::HOOK_RUN:
					HijackCall(state, GKit::Tools::CCalls::Call_Type::HOOK_RUN, -1, callee + 1, info->args, 0);
					break;

				case GKit::Tools::CCalls::Call_Type::HOOK_CALL:
					HijackCall(state, GKit::Tools::CCalls::Call_Type::HOOK_CALL, -1, callee + 1, info->args, 0);
					break;

				case GKit::Tools::CCalls::Call_Type::CONCOMMAND_RUN:
					int res = HijackCall(state, GKit::Tools::CCalls::Call_Type::CONCOMMAND_RUN, -1, callee + 1, info->args, 1);

					// Determine from return if original needs to be blocked (so concommand defined by polymorph arent ran in the normal lua world allowing servers to blacklist commands)
					if (res == 0)
					{
						block = LUA->GetBool(-1);
						LUA->Pop();
					}
					break;
			}

			LUA->Pop();
		}

		// PCall original function with original args
		if (block)
		{
			// Clean stack
			LUA->Pop(info->args + 1);

			//LUA->PushBool(false);			(for some reason even though gmod says that concommand.Run returns bool, the concommand engine call doesn't check it, its probbaly just for when lua calls concommand.Run)
			info->stopExecution = true;
			info->_return = 0;
		}
	}
}



void GKit::Tools::CCalls::Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* files)
{
	_menu = menu;
	_client = client;
	_files = files;

	client->HookEvent(GKit::GLua::Event::PCALL, PCall_Callback, nullptr);
}