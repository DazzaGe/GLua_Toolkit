/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Resources - Manages getting DLL resources

*/

#include "Resources.h"




const char* GKit::Util::GetLuaResource(int id)
{
	HRSRC hRes = FindResourceA(GKit::_this, MAKEINTRESOURCEA(id), "LUA");
	HGLOBAL hData = LoadResource(GKit::_this, hRes);
	LPVOID data = LockResource(hData);

	return (const char*)data;
}

