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
		class RTFunction
		{
			public:
						RTFunction(void* basfunc, size_t funcSize);
						~RTFunction();

				void*	GetFunc();

				void	Modify(DWORD offset, void* buffer, size_t bufferSize);


			private:
				BYTE*	newFunc;
				size_t	size;
				DWORD	ogAccess;
		};
	}
}