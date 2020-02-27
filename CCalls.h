/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "GLua.h"
#include "Error.h"
#include "Menucommands.h"
#include "GLuaHelperFunctions.h"
#include "File.h"




namespace GKit
{
	namespace Tools
	{
		namespace CCalls
		{
			enum Call_Type
			{
				NONE,
				HOOK_CALL,
				HOOK_RUN,
				CONCOMMAND_RUN
			};


			void Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* files);
		}
	}
}