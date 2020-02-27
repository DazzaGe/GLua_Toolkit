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
		class VTable
		{
			public:
								VTable();
								VTable(void* object);
								~VTable();

				void*			AddHook(void* newFunc, unsigned int offset);
				void			RemoveHook(unsigned int offset);

				void*			GetNew(unsigned int offset);
				void*			GetOld(unsigned int offset);


			private:
				void***			vTablePointer;
				void**			oldVTable;
				void**			newVTable;
				unsigned int	size;
		};

	}
}