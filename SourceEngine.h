/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Windows.h"




namespace GKit
{
	namespace Util
	{
		namespace Source
		{
			void* CreateInterface(LPCSTR module, const char* interfaceName);
			void* CreateInterface(HMODULE module, const char* interfaceName);
		}
	}
}


typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);