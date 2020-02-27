/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include "Directory.h"
#include <stdio.h>




#define	GKIT_DIRECTORY				"C:\\GKit\\"
#define GKIT_STARTUP_FILE			"startup.txt"

#define MAX_STARTUP_LINE_LENGTH		128
#define MAX_STARTUP_NUM_COMMANDS	128
#define MAX_FILE_SIZE				1000000



namespace GKit
{
	namespace Util
	{
		struct File_Info
		{
			void*			data;
			size_t			size;
		};


		enum Open_Mode
		{
			NORMAL,
			BINARY
		};


		class File
		{
			public:
										File(const char* _path);
										~File();

				void*					OpenFile(char* name, Open_Mode mode);
				void					CloseFile(void* file);

				size_t					ReadFile(void* file, unsigned char** data);
				void					ReadFile(void* file, char** data);

			private:
				char*					GetFullPath(char* name);

				void*					OpenNormal(char* fullPath);
				void*					OpenBinary(char* fullPath);


			private:
				const char*				path;
		};
	}
}
