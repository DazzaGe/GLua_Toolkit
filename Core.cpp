/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Core - Manages interaction with lua_shared.dll and the iluashared interface

*/

#include "Core.h"




namespace
{
	/*
		Global Variables
	*/
	
	GKit::GLua::ILuaShared::LuaCore*	currentCore = nullptr;


	/*
		Obtaining Menustate
	*/

	void __stdcall menuInit(void* state)
	{
		// Do callback
		GKit::GLua::ILuaShared::GetTop_Info infoStruct;
		infoStruct.state = state;

		currentCore->DoEvents(GKit::GLua::ILuaShared::Event::GET_TOP, &infoStruct);
	}

	//	Mainthreads are passed into lua_shared.dll exports, this allows us to get the mainthread to the menustate
	__declspec(naked) void d_lua_gettop(void* L)
	{
		_asm
		{
			push[esp + 4]
			call    menuInit

			mov     ecx, [esp + 4]
			mov     eax, [ecx + 14h]
			sub     eax, [ecx + 10h]
			sar     eax, 3
			retn
		}
	}


	/*
		Obtaining Newstate (Clientstate)
	*/

	//Function signature taken from Lennys ROC
	void* __fastcall hooked_CreateLuaInterface(void* thisptr, int edx, unsigned char cstate, bool renew)
	{
		//Run the old function we have saved
		GKit::GLua::ILuaShared::CreateLuaInterfaceFn oldCreateInterface = (GKit::GLua::ILuaShared::CreateLuaInterfaceFn)currentCore->o_createLuaInterface;
		void* state = oldCreateInterface(thisptr, edx, cstate, renew);

		// Do callback
		GKit::GLua::ILuaShared::New_State_Info infoStruct;
		infoStruct.state = state;
		infoStruct.type = (GKit::GLua::ILuaShared::State)cstate;

		currentCore->DoEvents(GKit::GLua::ILuaShared::Event::NEW_STATE, &infoStruct);

		return state;
	}

	void __fastcall hooked_CloseLuaInterface(void* thisptr, int edx, void* state)
	{
		//Run the old function we have saved
		GKit::GLua::ILuaShared::CloseLuaInterfaceFn oldCloseInterface = (GKit::GLua::ILuaShared::CloseLuaInterfaceFn)currentCore->o_closeLuaInterface;
		oldCloseInterface(thisptr, edx, state);

		// Do callback
		GKit::GLua::ILuaShared::GetTop_Info infoStruct;
		infoStruct.state = state;

		currentCore->DoEvents(GKit::GLua::ILuaShared::Event::DELETE_STATE, &infoStruct);
	}
}



/*
	Public Functions
*/

GKit::GLua::ILuaShared::LuaCore::LuaCore(HMODULE lua_Shared, void* iLuaShared) : GKit::Event(NUMBER_OF_EVENTS, MAX_EVENT_HOOKS)
{
	// Variable Initialization
	currentCore = this;

	h_LuaShared = lua_Shared;
	o_createLuaInterface = nullptr;
	gettopDetour = nullptr;

	// VTable
	p_iLuaShared = new GKit::Util::VTable(iLuaShared);
}

GKit::GLua::ILuaShared::LuaCore::~LuaCore()
{
	delete gettopDetour;
	delete p_iLuaShared;

	currentCore = nullptr;
}


HMODULE GKit::GLua::ILuaShared::LuaCore::GetLuaShared()
{
	return h_LuaShared;
}


void GKit::GLua::ILuaShared::LuaCore::HijackVTable(GKit::GLua::ILuaShared::Event event)
{
	switch (event)
	{
		case GKit::GLua::ILuaShared::Event::NEW_STATE:
		{
			o_createLuaInterface = p_iLuaShared->AddHook(hooked_CreateLuaInterface, CREATELUAINTERFACE_OFFSET);
			break;
		}


		case GKit::GLua::ILuaShared::Event::DELETE_STATE:
		{
			o_closeLuaInterface = p_iLuaShared->AddHook(hooked_CloseLuaInterface, CLOSELUAINTERFACE_OFFSET);
			break;
		}
	}
}

void GKit::GLua::ILuaShared::LuaCore::HijackExports(GKit::GLua::ILuaShared::Event event)
{
	switch (event)
	{
		case GKit::GLua::ILuaShared::Event::GET_TOP:
		{
			FARPROC gettopAddress = GetProcAddress(h_LuaShared, "lua_gettop");
			gettopDetour = new GKit::Util::Detour(gettopAddress, d_lua_gettop, GETTOP_SIZE);
			break;
		}
	}
}


void GKit::GLua::ILuaShared::LuaCore::ReleaseVTable(GKit::GLua::ILuaShared::Event event)
{
	switch (event)
	{
		case GKit::GLua::ILuaShared::Event::NEW_STATE:
		{
			p_iLuaShared->RemoveHook(CREATELUAINTERFACE_OFFSET);
			break;
		}


		case GKit::GLua::ILuaShared::Event::DELETE_STATE:
		{
			p_iLuaShared->RemoveHook(CLOSELUAINTERFACE_OFFSET);
			break;
		}
	}
}

void GKit::GLua::ILuaShared::LuaCore::ReleaseExports(GKit::GLua::ILuaShared::Event event)
{
	switch (event)
	{
		case GKit::GLua::ILuaShared::Event::GET_TOP:
		{
			delete gettopDetour;
			gettopDetour = nullptr;
			break;
		}
	}
}