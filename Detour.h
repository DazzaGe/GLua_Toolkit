/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Windows.h"




#define JMP_INSTRUCTION_SIZE 5



namespace GKit
{
	namespace Util
	{
		class Detour
		{
			public:
								Detour(void*, void*, unsigned int);
								~Detour();
				void			SetAccess(DWORD);
				void			ResetAccess();
				void			ClearFunctionSpace();
				static void		CopyFunction(void*, unsigned int, void*);
				static DWORD	GetRelativeAddress(void*, void*, DWORD);


			public:
				BYTE*			oldCode;

			private:
				unsigned int	size;
				void*			location;
				DWORD			access;
		};

	}
}
