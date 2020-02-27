/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Error - Error handling for GKit

*/

#include "Error.h"
#include "GLua.h"




namespace
{
	GKit::GLua::MenuState* menu = nullptr;


	void OutputFormat_IIS(int _int1, int _int2, const char* _str)
	{
		unsigned int stringlen = strlen(_str);

		char* message = new char[stringlen + 8 + 8];
		char* title = new char[32 + 8];

		sprintf(message, "%s: %d\n\0", _str, _int2);
		sprintf(title, "GLUA TOOLKIT ERROR: %X", _int1);

		MessageBoxA(NULL, message, title, MB_OK|MB_ICONEXCLAMATION);

		delete[] message;
		delete[] title;
	}

	void MenuFormat_IIS(GKit::GLua::MenuState* _menu, int _int1, int _int2, const char* _str)
	{
		unsigned int stringlen = strlen(_str);

		char* script = new char[stringlen + 32];

		sprintf(script, "print([[GKIT ERROR %X: (%s) %d]])", _int1, _str, _int2);

		try
		{
			if (_menu->Active())
				_menu->LoadBuffer(script, strlen(script), "");
		}
		catch (GKit::Util::Exception& e)
		{
			GKit::Util::ExceptionPopup(&e);
		}

		delete[] script;
	}
}



GKit::Util::Exception::Exception(int _code)
{
	code = _code;
	erString = "";
	erInt = 0;
}

GKit::Util::Exception::Exception(int _code, const char* _erString)
{
	code = _code;
	erString = _erString;
	erInt = 0;
}

GKit::Util::Exception::Exception(int _code, int _erInt)
{
	code = _code;
	erString = "";
	erInt = _erInt;
}

GKit::Util::Exception::Exception(int _code, const char* _erString, int _erInt)
{
	code = _code;
	erString = _erString;
	erInt = _erInt;
}



void GKit::Util::ExceptionPopup(Exception* e)
{
	OutputFormat_IIS(e->code, e->erInt, e->erString);
}

void GKit::Util::ExceptionMenu(Exception* e)
{
	MenuFormat_IIS(menu, e->code, e->erInt, e->erString);
}


void GKit::Util::SetMenu(void* _menu)
{
	menu = (GKit::GLua::MenuState*)_menu;
}
