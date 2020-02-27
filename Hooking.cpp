/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Hooking - Manges VTable hooking

*/

#include "Hooking.h"




GKit::Util::VTable::VTable()
{
	this->size = 1;
	this->newVTable = NULL;
}


GKit::Util::VTable::VTable(void* object)
{
	this->vTablePointer = (void***)object;
	this->oldVTable = *(this->vTablePointer);

	//Find size of oldVTable
	int indices = 0;
	while (*(this->oldVTable + indices))
	{
		indices++;
	}
	this->size = sizeof(int) * (indices + 4);

	//Create new VTable
	this->newVTable = new void*[this->size];

	//Copy contents from old VTable to new VTable
	int i = 0;
	while (i < indices)
	{
		newVTable[i] = (void*)(this->oldVTable)[i];
		i++;
	}

	//Set vTable pointer to point to our new VTable
	*vTablePointer = newVTable;
}


GKit::Util::VTable::~VTable()
{
	delete[this->size] this->newVTable;
}


void* GKit::Util::VTable::AddHook(void* newFunction, unsigned int offset)
{
	this->newVTable[offset] = newFunction;

	return this->oldVTable[offset];
}


void GKit::Util::VTable::RemoveHook(unsigned int offset)
{
	this->newVTable[offset] = this->oldVTable[offset];
}


void* GKit::Util::VTable::GetNew(unsigned int offset)
{
	return newVTable[offset];
}

void* GKit::Util::VTable::GetOld(unsigned int offset)
{
	return oldVTable[offset];
}