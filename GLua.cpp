/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	GLua - GLua interface for GKit 

*/

#include "GLua.h"
#include <time.h>




namespace
{
	//
	//	Creation Callbacks
	//

	// For Menu
	void GettopMenuCallback(void* infoStruct, void* arg)
	{
		GKit::GLua::ILuaShared::GetTop_Info* info = (GKit::GLua::ILuaShared::GetTop_Info*)infoStruct;
		GKit::GLua::MenuState* state = (GKit::GLua::MenuState*)arg;

		state->NewState((GKit::GLua::global_State*)((GKit::GLua::lua_State*)info->state)->luabase, GetCurrentThreadId());
	}


	// For Client
	void CreateLuaInterfaceCallback(void* infoStruct, void* arg)
	{
		GKit::GLua::ClientState* state = (GKit::GLua::ClientState*)arg;
		GKit::GLua::ILuaShared::New_State_Info* info = (GKit::GLua::ILuaShared::New_State_Info*)infoStruct;

		if (info->type != GKit::GLua::ILuaShared::State::CLIENT_STATE)
			return;

		state->NewState((GKit::GLua::global_State*)info->state, GetCurrentThreadId());
	}

	// For Client
	void CloseLuaInterfaceCallback(void* infoStruct, void* arg)
	{
		GKit::GLua::ClientState* state = (GKit::GLua::ClientState*)arg;

		state->DeleteState();
	}


	//
	//	Callbacks
	//

	void PCallCallback(void* infoStruct, void* arg)
	{
		GKit::GLua::LuaState* state = (GKit::GLua::LuaState*)arg;

		state->DoEvents(GKit::GLua::Event::PCALL, infoStruct);

		GKit::GLua::global_State* globState = ((GKit::GLua::ILuaInterface::PCall_Info *)infoStruct)->state;

		if (state->IsSeized())
			state->SeizeCallback(globState->mainThread);
	}


	void RunStringExCallback_C(void* infoStruct, void* arg)
	{
		GKit::GLua::ClientState* state = (GKit::GLua::ClientState*)arg;

		state->DoEvents(GKit::GLua::Event::RUNSTRINGEX, infoStruct);

		if (!state->HasAutorun())
		{
			state->OnAutorun(GetCurrentThreadId());
			state->DoEvents(GKit::GLua::Event::STARTUP, infoStruct);
		}
	}
}



/*
	LuaState
*/

GKit::GLua::LuaState::LuaState(GKit::GLua::ILuaShared::LuaCore* _core) : GKit::Event(NUMBER_OF_STATE_EVENTS, MAX_STATE_EVENT_HOOKS)
{
	// this class does not find the state, need ClientState or MenuState
	core = _core;
	state = nullptr;
	seized = false;
	seizeInfo = nullptr;
	threadID = NULL;
}

GKit::GLua::LuaState::~LuaState()
{
	delete state;
}


void GKit::GLua::LuaState::LoadBuffer(const char* buffer, size_t bufferSize, const char* dbgStr)
{
	if (!Active())
		throw GKit::Util::Exception(GKIT_ERROR_NOSTATE);

	// If thread calling this is the same as the thread running the state in this, then the state thread is frozen obviously so dont need to seize
	// If we seized this thread then since callback uses same thread as this it waits, the callback will never call and gmod freezes
	if (GetCurrentThreadId() == threadID)
	{
		GKit::GLua::GState::LoadBuffer((GKit::GLua::lua_State*)state->GetMainThread(), buffer, bufferSize, dbgStr);
	}
	else
	{
		LoadBuffer_Info info;
		info.bufferSize = bufferSize;
		info.buffer = buffer;
		info.dbgStr = dbgStr;

		Seize(Seize_Type::LOAD_BUFFER, &info);
	}
}

void GKit::GLua::LuaState::ExecuteCLua(GState::CLuaFunction func)
{
	if (!Active())
		throw GKit::Util::Exception(GKIT_ERROR_NOSTATE);

	if (GetCurrentThreadId() == threadID)
	{
		GKit::GLua::GState::ExecuteCLua((GKit::GLua::lua_State*)state->GetMainThread(), func);
	}
	else
	{
		CLua_Info info;
		info.func = func;

		Seize(Seize_Type::CLUA, &info);
	}
}


bool GKit::GLua::LuaState::Active()
{
	return state != nullptr;
}


bool GKit::GLua::LuaState::IsSeized()
{
	return seized;
}

void GKit::GLua::LuaState::SeizeCallback(GKit::GLua::lua_State* _state)
{
	switch (seizeType)
	{
		case Seize_Type::CLUA:
		{
			CLua_Info* info = (CLua_Info*)seizeInfo;
			GKit::GLua::GState::ExecuteCLua((GKit::GLua::lua_State*)_state, info->func);

			break;
		}


		case Seize_Type::LOAD_BUFFER:
		{
			LoadBuffer_Info* info = (LoadBuffer_Info*)seizeInfo;
			GKit::GLua::GState::LoadBuffer((GKit::GLua::lua_State*)_state, info->buffer, info->bufferSize, info->dbgStr);

			break;
		}
	}

	seizeInfo = nullptr;
	seized = false;
}


void GKit::GLua::LuaState::NewState(global_State* newState, DWORD newthreadid)
{
	threadID = newthreadid;
	state = new GKit::GLua::ILuaInterface::State(newState);

	// Hook PCall
	state->HookEvent(GKit::GLua::ILuaInterface::Event::PCALL, (GKit::CallbackFunc)PCallCallback, this);
	state->HijackFunction(GKit::GLua::ILuaInterface::Event::PCALL);
}

void GKit::GLua::LuaState::DeleteState()
{
	delete state;

	state = nullptr;
}


void GKit::GLua::LuaState::Seize(Seize_Type type, void* info)
{
	seizeInfo = info;
	seizeType = type;
	seized = true;

	while (seized == true)
	{
		Sleep(100);
	}
}



/*
	ClientState
*/

GKit::GLua::ClientState::ClientState(GKit::GLua::ILuaShared::LuaCore* core) : LuaState(core)
{
	hasAutorun = false;

	//
	//	Obtaining Client State
	//
	
	// Generate Lua Interface Name
	GenerateRandomString();

	// Hook CreateLuaInterface and CloseLuaInterface
	core->HookEvent(GKit::GLua::ILuaShared::Event::NEW_STATE, (CallbackFunc)CreateLuaInterfaceCallback, this);
	core->HookEvent(GKit::GLua::ILuaShared::Event::DELETE_STATE, (CallbackFunc)CloseLuaInterfaceCallback, this);

	core->HijackVTable(GKit::GLua::ILuaShared::Event::NEW_STATE);
	core->HijackVTable(GKit::GLua::ILuaShared::Event::DELETE_STATE);
}

GKit::GLua::ClientState::~ClientState()
{
	// remove core hooks once i implement remove hook

	core->ReleaseVTable(GKit::GLua::ILuaShared::Event::NEW_STATE);
	core->ReleaseVTable(GKit::GLua::ILuaShared::Event::DELETE_STATE);
}


void GKit::GLua::ClientState::LoadBuffer(const char* buffer, size_t bufferSize, const char* dbgStr)
{
	/*
	// Have to use runstringex to allow glua (not just lua) scripts to run
	GKit::GLua::ILuaInterface::RunStringExFn runstringex = (GKit::GLua::ILuaInterface::RunStringExFn)(state->GetVTable())->GetOld(RUNSTRINGEX_OFFSET);
	runstringex(state->GetGlobalState(), dbgStr, "", buffer, true, true, 1, 1);
	*/
	GKit::GLua::LuaState::LoadBuffer(buffer, bufferSize, dbgStr);
}


bool GKit::GLua::ClientState::HasAutorun()
{
	return hasAutorun;
}


const char* GKit::GLua::ClientState::GetRandomString()
{
	return randomString;
}


void GKit::GLua::ClientState::NewState(global_State* newState, DWORD newthreadid)
{
	GKit::GLua::LuaState::NewState(newState, newthreadid);

	// Hook RunStringEx
	state->HookEvent(GKit::GLua::ILuaInterface::Event::RUNSTRINGEX, (GKit::CallbackFunc)RunStringExCallback_C, this);
	state->HijackFunction(GKit::GLua::ILuaInterface::Event::RUNSTRINGEX);
}

void GKit::GLua::ClientState::DeleteState()
{
	delete state;

	state = nullptr;

	hasAutorun = false;
}


void GKit::GLua::ClientState::OnAutorun(DWORD newThread)
{
	hasAutorun = true;
}


void GKit::GLua::ClientState::GenerateRandomString()
{
	srand(time(nullptr));

	for (unsigned int i = 0; i < RANDOM_STRING_LENGTH; i++)
	{
		randomString[i] = char((rand() % 0x5F) + 0x20);
	}

	randomString[RANDOM_STRING_LENGTH] = '\0';
}



/*
	MenuState
*/

GKit::GLua::MenuState::MenuState(GKit::GLua::ILuaShared::LuaCore* core) : LuaState(core)
{
	bufferFunction = nullptr;

	//
	//	Obtaining Menu State
	//

	// Hook gettop
	core->HookEvent(GKit::GLua::ILuaShared::Event::GET_TOP, (CallbackFunc)GettopMenuCallback, this);
	core->HijackExports(GKit::GLua::ILuaShared::Event::GET_TOP);

	// For Convenience wait till state created
	while (!Active())
		Sleep(100);
}

GKit::GLua::MenuState::~MenuState()
{
}


void GKit::GLua::MenuState::NewState(global_State* newState, DWORD newthreadid)
{
	GKit::GLua::LuaState::NewState(newState, newthreadid);

	// Remove gettop hook
	core->ReleaseExports(GKit::GLua::ILuaShared::Event::GET_TOP);
}

void GKit::GLua::MenuState::DeleteState()
{
	GKit::GLua::LuaState::DeleteState();
}