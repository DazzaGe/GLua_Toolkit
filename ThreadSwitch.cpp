/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	ThreadSwitch - Simple thread control management

*/

#include "ThreadSwitch.h"




void GKit::Util::ThreadSwitch::ChangeResponsibility(int val, int* responsibility)
{
	*responsibility = val;
}

void GKit::Util::ThreadSwitch::WaitForResponsibility(int val, int* responsibility)
{
	while (true)
	{
		if (HasResponsibility(val, responsibility))
			break;

		Sleep(100);
	}
}

bool GKit::Util::ThreadSwitch::HasResponsibility(int val, int* responsibility)
{
	if (*responsibility == 0 || *responsibility == val)
		return true;

	return false;
}

