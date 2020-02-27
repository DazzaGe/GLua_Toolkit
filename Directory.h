/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Windows.h"
#include <stdio.h>
#include "Error.h"




namespace GKit
{
	namespace Util
	{
		namespace Directory
		{
			void		Validate(const char* path);

			bool		Exists(char* path);

			long int	Size(char* path);

			size_t		Read(char* path, unsigned char* buffer, size_t bufferSize);
			void		Read(char* path, char* strBuffer, size_t bufferSize);

			void		Write(char* path, unsigned char* buffer, size_t bufferSize);
			void		Write(char* path, char* strBuffer);
		}
	}
}