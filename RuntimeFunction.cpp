/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	RuntimeFunction - Class creates a new function at runtime (in the heap), allowing function variables to be changed at runtime

*/

#include "RuntimeFunction.h"




GKit::Util::RTFunction::RTFunction(void* basfunc, size_t funcSize)
{
	// Create new function

	size = funcSize;
	newFunc = new BYTE[funcSize];

	memcpy(newFunc, basfunc, funcSize);

	// Allow newfunction (in heap) to be executed
	VirtualProtect(newFunc, funcSize, PAGE_EXECUTE_READWRITE, &ogAccess);
}

GKit::Util::RTFunction::~RTFunction()
{
	// Reset protection
	DWORD temp;
	VirtualProtect(newFunc, size, ogAccess, &temp);

	delete[] newFunc;
}


void* GKit::Util::RTFunction::GetFunc()
{
	return newFunc;
}


void GKit::Util::RTFunction::Modify(DWORD offset, void* buffer, size_t bufferSize)
{
	memcpy(newFunc + offset, buffer, bufferSize);
}