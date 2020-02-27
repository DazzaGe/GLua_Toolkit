/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

*/

#pragma once

#include <unordered_map>
#include "Windows.h"




namespace GKit
{
	namespace Util
	{
		namespace ThreadSwitch
		{
			void ChangeResponsibility(int val, int* responsibility);
			void WaitForResponsibility(int val, int* responsibility);
			bool HasResponsibility(int val, int* responsibility);
		}
	}
}