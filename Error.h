/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include <exception>
#include <fstream>
#include "Windows.h"
#include "Interface.h"




namespace GKit
{
	namespace Util
	{
		struct Exception : public std::exception
		{
			Exception(int _code);
			Exception(int _code, const char* _erString);
			Exception(int _code, int _erInt);
			Exception(int _code, const char* _erString, int _erInt);

			int				code;
			int				erInt;
			const char*		erString;
		};

		void ExceptionPopup(Exception* e);
		void ExceptionMenu(Exception* e);

		void SetMenu(void* _menu);
	}
}



#define GKIT_OK							0x0


//
//	State Error
//

#define GKIT_ERROR_SCRIPT_LOAD			0x1
#define GKIT_ERROR_SCRIPT_EXECUTION		0x2


//
//	Core Error
//


//
//	GLua Error
//

#define GKIT_ERROR_NOTHREAD				0x200
#define GKIT_ERROR_NOSTATE				0x201
#define GKIT_ERROR_SUSPEND_THREAD		0x202
#define GKIT_ERROR_RESUME_THREAD		0x203
#define GKIT_ERROR_OPEN_THREAD			0x204


//
//	Directory Error
//

#define GKIT_ERROR_OPEN_FILE			0x300
#define GKIT_ERROR_READ_FILE			0x301
#define GKIT_ERROR_WRITE_FILE			0x302
#define GKIT_ERROR_SIZE_FILE			0x303


//
//	File Error
//

#define GKIT_ERROR_FILE_TOO_LARGE		0x400
#define GKIT_ERROR_FILE_DOES_NOT_EXIST	0x400


//
//	Generic Error
//

#define GKIT_ERROR_MEMORY_ALLOCATION	0x500