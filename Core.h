/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Detour.h"
#include "Hooking.h"
#include "ThreadSwitch.h"
#include "Event.h"




#define MAX_EVENT_HOOKS				16
#define NUMBER_OF_EVENTS			5

#define CREATELUAINTERFACE_OFFSET	4
#define CLOSELUAINTERFACE_OFFSET	5
#define RUNSTRINGEX_OFFSET			111
#define PCALL_OFFSET				11

#define GETTOP_SIZE					14



namespace GKit
{
	namespace GLua
	{
		namespace ILuaShared
		{
			typedef void*	(__fastcall  *CreateLuaInterfaceFn)	(void* thisptr, int edx, unsigned char cstate, bool renew);	//Taken from lennys ROC
			typedef void	(__fastcall  *CloseLuaInterfaceFn)	(void* thisptr, int edx, void* state);


			enum State
			{
				CLIENT_STATE,
				SERVER_STATE,
				MENU_STATE
			};

			enum Event
			{
				NEW_STATE,
				DELETE_STATE,
				GET_TOP,
			};


			// callback info structs
			struct New_State_Info
			{
				State	type;
				void*	state;
			};

			struct GetTop_Info
			{
				void*	state;
			};


			// I need to find someway to not have to hardcode the core and originals into the hooked functions
			class LuaCore : public GKit::Event
			{
				public:
							LuaCore(HMODULE lua_Shared, void* iLuaShared);
							~LuaCore();

					HMODULE	GetLuaShared();

					void	HijackVTable(GKit::GLua::ILuaShared::Event event);									// Setup hooking for lua interface events
					void	HijackExports(GKit::GLua::ILuaShared::Event event);									// Setup hooking for lua shared export events

					void	ReleaseVTable(GKit::GLua::ILuaShared::Event event);
					void	ReleaseExports(GKit::GLua::ILuaShared::Event event);


				public:
					// DON'T USE, FOR INTERNAL USE
					void*				o_createLuaInterface;
					void*				o_closeLuaInterface;
					GKit::Util::Detour*	gettopDetour;

				private:
					HMODULE				h_LuaShared;
					GKit::Util::VTable*	p_iLuaShared;
			};
		}
	}
}