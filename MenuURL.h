/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "GKit.h"
#include "GLua.h"
#include "Error.h"
#include "Menucommands.h"
#include "GLuaHelperFunctions.h"
#include "File.h"
#include "Resources.h"




namespace GKit
{
	namespace Tools
	{
		namespace MenuURL
		{
			void Init(GKit::GLua::MenuState* menu, GKit::GLua::ClientState* client, GKit::Util::File* files);
		}
	}
}