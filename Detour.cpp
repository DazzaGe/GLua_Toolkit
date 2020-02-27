/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Detour - Functions to allow detouring C funcs

*/

#include "Detour.h"




GKit::Util::Detour::Detour(void* oldFunc, void* newFunc, unsigned int size_)
{
	size = size_;
	location = oldFunc;
	access = 0;

	//Set .text location to be editable (no access to write to this location by default)
	VirtualProtect(location, size, PAGE_EXECUTE_READWRITE, &access);

	//Save old function code
	oldCode = new BYTE[size_];
	CopyFunction(oldFunc, size_, oldCode);

	//Clear function region so no stray bytes
	//ClearFunctionSpace();

	//Get relative address between the newfunc and jmp (that we will insert)
	DWORD relativeAddress = GetRelativeAddress(newFunc, oldFunc, JMP_INSTRUCTION_SIZE);

	//Replace original function location with a jmp to our new function
	*(BYTE*)oldFunc = 0xE9;
	*(DWORD*)((DWORD)oldFunc + 1) = relativeAddress;

	//Reset memory permissions
	ResetAccess();
}


GKit::Util::Detour::~Detour()
{
	SetAccess(PAGE_EXECUTE_READWRITE);
	ClearFunctionSpace();

	for (unsigned int i = 0; i < size; i++)
	{
		*(BYTE*)((BYTE*)location + i) = oldCode[i];
	}

	delete[] oldCode;
	ResetAccess();
}


void GKit::Util::Detour::SetAccess(DWORD newAccess)
{
	DWORD temp;
	VirtualProtect(location, size, newAccess, &temp);
}

void GKit::Util::Detour::ResetAccess()
{
	DWORD temp;
	VirtualProtect(location, size, access, &temp);
}


void GKit::Util::Detour::ClearFunctionSpace()
{
	memset(location, 0xCC, size);
}


void GKit::Util::Detour::CopyFunction(void* func, unsigned int size, void* buffer)
{
	for (unsigned int i = 0; i < size; i++)
	{
		*((BYTE*)buffer + i) = *(BYTE*)((BYTE*)func + i);
	}
}


DWORD GKit::Util::Detour::GetRelativeAddress(void* targetFunc, void* baseFunc, DWORD instructionSize)
{
	DWORD relativeAddress = (DWORD)targetFunc - ((DWORD)baseFunc + instructionSize);
	return relativeAddress;
}
