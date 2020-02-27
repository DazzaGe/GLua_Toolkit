/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Main - Central file manages the DLL

*/

#include "SourceEngine.h"




void* GKit::Util::Source::CreateInterface(LPCSTR module, const char* interfaceName)
{
	//Function to interface with (get pointer to) the interface class
	CreateInterfaceFn interfaceFunction = (CreateInterfaceFn)GetProcAddress(GetModuleHandle(module), "CreateInterface");

	return (void*)interfaceFunction(interfaceName, 0);
}

void* GKit::Util::Source::CreateInterface(HMODULE module, const char* interfaceName)
{
	//Function to interface with (get pointer to) the interface class
	CreateInterfaceFn interfaceFunction = (CreateInterfaceFn)GetProcAddress(module, "CreateInterface");

	return (void*)interfaceFunction(interfaceName, 0);
}

