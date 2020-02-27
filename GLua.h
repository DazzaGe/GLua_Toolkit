/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Core.h"
#include "LuaInterface.h"
#include "GState.h"
#include "ThreadSwitch.h"




// Menustate has to use the gmod thread for printing to console for some reason (else it doesn't print until you join game)
#define MAX_STATE_EVENT_HOOKS				128
#define NUMBER_OF_STATE_EVENTS				3

#define RANDOM_STRING_LENGTH				64



namespace GKit
{
	namespace GLua
	{
		enum Seize_Type
		{
			LOAD_BUFFER,
			CLUA,
		};


		enum Event
		{
			STARTUP,
			RUNSTRINGEX,
			PCALL,
		};


		struct LoadBuffer_Info
		{
			const char*				buffer;
			unsigned int			bufferSize;
			const char*				dbgStr;
			bool					showErrors;
		};

		struct CLua_Info
		{
			GState::CLuaFunction	func;
		};


		class LuaState : public GKit::Event
		{
			public:
								LuaState(GKit::GLua::ILuaShared::LuaCore* core);
								~LuaState();

				virtual void	LoadBuffer(const char* buffer, size_t bufferSize, const char* dbgStr);
				void			ExecuteCLua(GState::CLuaFunction func);

				bool			Active();

				// FOR INTERNAL USE
				bool			IsSeized();
				void			SeizeCallback(GKit::GLua::lua_State* _state);

				virtual void	NewState(global_State* newState, DWORD newthreadid);
				virtual void	DeleteState();

			private:
				void			Seize(Seize_Type type, void* info);


			protected:
				GKit::GLua::ILuaShared::LuaCore*	core;
				GKit::GLua::ILuaInterface::State*	state;

			private:
				bool								seized;
				Seize_Type							seizeType;
				void*								seizeInfo;

				DWORD								threadID;
		};


		class ClientState : public LuaState
		{
			public:
								ClientState(GKit::GLua::ILuaShared::LuaCore* core);
								~ClientState();
								
				void			LoadBuffer(const char* buffer, size_t bufferSize, const char* dbgStr);

				bool			HasAutorun();

				const char*		GetRandomString();

				// FOR INTERNAL USE
				virtual void	NewState(global_State* newState, DWORD newthreadid);
				virtual void	DeleteState();
				void			OnAutorun(DWORD threadid);

			private:
				void			GenerateRandomString();

			private:
				bool			hasAutorun;
				char			randomString[RANDOM_STRING_LENGTH + 1];
		};


		class MenuState : public LuaState
		{
			public:
								MenuState(GKit::GLua::ILuaShared::LuaCore* core);
								~MenuState();

				// FOR INTERNAL USE
				virtual void	NewState(global_State* newState, DWORD newthreadid);
				virtual void	DeleteState();


			private:
				void* bufferFunction;
		};
	}
}