/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	LuaInterface - Manages interaction with the luainterface (global states)

*/

#include "LuaInterface.h"
#include <stdio.h>



namespace
{
	/*
		Hijacks
	*/

	#pragma optimize( "", off ) // don't change our function with your optimizations, we need this to be constant (as variables depend on it's size/structure)


	int __fastcall hooked_PCall(void* state, void*, int args, int results, int error)
	{
		// Do Callback
		GKit::GLua::ILuaInterface::PCall_Info info;
		info.stopExecution = false;
		info._return = 0;

		info.state = (GKit::GLua::global_State*)state;
		info.args = args;
		info.results = results;
		info.error = error;

		((GKit::GLua::ILuaInterface::State*)0x12345678)->DoEvents(GKit::GLua::ILuaInterface::Event::PCALL, &info);

		// Return Normal
		if (info.stopExecution)
			return info._return;
		else
			return ((GKit::GLua::ILuaInterface::PCallFn)0x81234567)(state, args, results, error);
	}


	void * __fastcall hooked_RunStringEx(void* state, void*, char const* filename, char const* path, char const* torun, bool run, bool showerrors, bool idk, bool idk2)
	{
		// Do Callback
		GKit::GLua::ILuaInterface::RunStringEx_Info info;
		info.stopExecution = false;
		info._return = 0x0;

		info.state = (GKit::GLua::global_State*)state;
		info.filename = filename;
		info.path = path;
		info.toRun = torun;
		info.run = run;
		info.showErrors = showerrors;
		info.unknown1 = idk;
		info.unknown2 = idk2;

		((GKit::GLua::ILuaInterface::State*)0x12345678)->DoEvents(GKit::GLua::ILuaInterface::Event::RUNSTRINGEX, &info);

		// Return Normal
		if (info.stopExecution)
			return info._return;
		else
			return ((GKit::GLua::ILuaInterface::RunStringExFn)0x81234567)(state, filename, path, torun, run, showerrors, idk, idk2);
	}


	#pragma optimize( "", on ) 
}



/*
	Public Functions
*/

GKit::GLua::ILuaInterface::State::State(void* _globalState) : GKit::Event(NUMBER_OF_EVENTS, MAX_EVENT_HOOKS)
{
	globalState = (GKit::GLua::global_State*)_globalState;

	vTable = new GKit::Util::VTable(globalState);

	newRunstringEx = nullptr;
	newPCall = nullptr;
}

GKit::GLua::ILuaInterface::State::~State()
{
	if (vTable != nullptr)
		delete this->vTable;

	// remove heap functions
	RemoveRTFunction(&newRunstringEx);
	RemoveRTFunction(&newPCall);
}


void GKit::GLua::ILuaInterface::State::HijackFunction(GKit::GLua::ILuaInterface::Event event)
{
	switch (event)
	{
		case GKit::GLua::ILuaInterface::Event::RUNSTRINGEX:
		{
			// create rt func
			AllocateRTFunction(&newRunstringEx, hooked_RunStringEx, HOOKED_RUNSTRINGEX_BASE_SIZE, HOOKED_RUNSTRINGEX_INTERFACE_OFFSET, HOOKED_RUNSTRINGEX_CLASS_CALL_OFFSET);

			void* oldFunc = vTable->GetOld(RUNSTRINGEX_OFFSET);
			newRunstringEx->Modify(HOOKED_RUNSTRINGEX_ORIG_CALL_OFFSET, (BYTE*)&oldFunc, 4);

			// update vtable
			vTable->AddHook(newRunstringEx->GetFunc(), RUNSTRINGEX_OFFSET);

			break;
		}

		case GKit::GLua::ILuaInterface::Event::PCALL:
		{
			// create rt func
			AllocateRTFunction(&newPCall, hooked_PCall, HOOKED_PCALL_BASE_SIZE, HOOKED_PCALL_INTERFACE_OFFSET, HOOKED_PCALL_CLASS_CALL_OFFSET);

			void* oldFunc = vTable->GetOld(PCALL_OFFSET);
			newPCall->Modify(HOOKED_PCALL_ORIG_CALL_OFFSET, (BYTE*)&oldFunc, 4);

			// update vtable
			vTable->AddHook(newPCall->GetFunc(), PCALL_OFFSET);

			break;
		}
	}
}

void GKit::GLua::ILuaInterface::State::ReleaseFunction(GKit::GLua::ILuaInterface::Event event)
{
	switch (event)
	{
		case GKit::GLua::ILuaInterface::Event::RUNSTRINGEX:
		{
			vTable->RemoveHook(RUNSTRINGEX_OFFSET);
			RemoveRTFunction(&newRunstringEx);
		}

		case GKit::GLua::ILuaInterface::Event::PCALL:
		{
			vTable->RemoveHook(PCALL_OFFSET);
			RemoveRTFunction(&newPCall);
		}
	}
}


GKit::Util::VTable* GKit::GLua::ILuaInterface::State::GetVTable()
{
	return vTable;
}

void* GKit::GLua::ILuaInterface::State::GetGlobalState()
{
	return globalState;
}

void* GKit::GLua::ILuaInterface::State::GetMainThread()
{
	// mainThread will not exist if state just created
	return globalState->mainThread;
}



/*
	Private Functions
*/

void GKit::GLua::ILuaInterface::State::AllocateRTFunction(GKit::Util::RTFunction** pp_NewRT, void* baseFunc, size_t baseFuncSize, DWORD interfaceOffset, DWORD methodOffset)
{
	RemoveRTFunction(pp_NewRT);

	*pp_NewRT = new GKit::Util::RTFunction(baseFunc, baseFuncSize);

	// change object address
	GKit::GLua::ILuaInterface::State* _this = this;
	(*pp_NewRT)->Modify(interfaceOffset, (BYTE*)&_this, 4);

	// change class function call offset
	auto eventMethod = &GKit::GLua::ILuaInterface::State::DoEvents;
	DWORD offset = (DWORD)((void*&)eventMethod) - ((DWORD)(*pp_NewRT)->GetFunc() + methodOffset + 4);
	(*pp_NewRT)->Modify(methodOffset, (BYTE*)&offset, 4);
}

void GKit::GLua::ILuaInterface::State::RemoveRTFunction(GKit::Util::RTFunction** pp_NewRT)
{
	if (*pp_NewRT == nullptr)
		return;

	delete *pp_NewRT;
	*pp_NewRT = nullptr;
}
