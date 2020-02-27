/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Windows.h"
#include "Hooking.h"
#include "Interface.h"
#include "Event.h"
#include "RuntimeFunction.h"




#define MAX_EVENT_HOOKS							16
#define NUMBER_OF_EVENTS						2

#define RUNSTRINGEX_OFFSET						111
#define PCALL_OFFSET							11

#define GETTOP_SIZE								14

#define HOOKED_PCALL_BASE_SIZE					0x6D // the function was 2 bytes bigger than it said in IDA? -- coz the return is 3 bytes not 1 byte
#define HOOKED_PCALL_INTERFACE_OFFSET			0x39
#define HOOKED_PCALL_CLASS_CALL_OFFSET			0x3E
#define HOOKED_PCALL_ORIG_CALL_OFFSET			0x5E

#define HOOKED_RUNSTRINGEX_BASE_SIZE			0x99
#define HOOKED_RUNSTRINGEX_INTERFACE_OFFSET		0x51
#define HOOKED_RUNSTRINGEX_CLASS_CALL_OFFSET	0x56
#define HOOKED_RUNSTRINGEX_ORIG_CALL_OFFSET		0x8A



namespace GKit
{
	namespace GLua
	{
		namespace ILuaInterface
		{
			typedef void*	(__thiscall *RunStringExFn)			(void*, char const*, char const*, char const*, bool, bool, bool, bool);	// Taken from lennys gm_roc
			typedef int		(__thiscall *PCallFn)				(void*, int, int, int);


			enum Event
			{
				RUNSTRINGEX,
				PCALL,
			};


			struct PCall_Info
			{
				bool			stopExecution;
				int				_return;

				global_State*	state;
				int				args;
				int				results;
				int				error;
			};

			struct RunStringEx_Info
			{
				bool			stopExecution;
				void*			_return;

				global_State*	state;
				char const*		filename;
				char const*		path;
				char const*		toRun;
				bool			run;
				bool			showErrors;
				bool			unknown1;
				bool			unknown2;
			};


			class State : public GKit::Event
			{
				public:
												State(void* _globalState);
												~State();

					void						HijackFunction(GKit::GLua::ILuaInterface::Event event);
					void						ReleaseFunction(GKit::GLua::ILuaInterface::Event event);

					GKit::Util::VTable*			GetVTable();
					void*						GetGlobalState();
					void*						GetMainThread();

				private:
					void						AllocateRTFunction(GKit::Util::RTFunction** pp_NewRT, void* baseFunc, size_t baseFuncSize, DWORD interfaceOffset, DWORD methodOffset);
					void						RemoveRTFunction(GKit::Util::RTFunction** pp_NewRT);


				private:
					GKit::Util::VTable*			vTable;
					GKit::GLua::lua_State*		mainThread;
					GKit::GLua::global_State*	globalState;

					// heap functions
					GKit::Util::RTFunction*		newPCall;
					GKit::Util::RTFunction*		newRunstringEx;
			};
		}
	}
}