/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	File - Interface for interacting with user files

*/

#include "File.h"




GKit::Util::File::File(const char* _path)
{
	path = _path;
	GKit::Util::Directory::Validate(path);

	char* startup = GetFullPath((char*)GKIT_STARTUP_FILE);

	// Check startup.txt otherwise create one
	if (!GKit::Util::Directory::Exists(startup))
	{
		GKit::Util::Directory::Write(startup, nullptr, 0);
	}

	delete[] startup;
}

GKit::Util::File::~File()
{
}


void* GKit::Util::File::OpenFile(char* name, Open_Mode mode)
{
	char* fullPath = GetFullPath((char*)name);
	void* info = nullptr;

	if (!GKit::Util::Directory::Exists(fullPath))
		throw GKit::Util::Exception(GKIT_ERROR_FILE_DOES_NOT_EXIST);

	switch (mode)
	{
		case Open_Mode::NORMAL:
		{
			info = OpenNormal(fullPath);
			break;
		}

		case Open_Mode::BINARY:
		{
			info = OpenBinary(fullPath);
			break;
		}
	}

	delete[] fullPath;
	return info;
}

void GKit::Util::File::CloseFile(void* file)
{
	File_Info* info = (File_Info*)file;
	delete[] (unsigned char*)info->data;

	delete info;
}


size_t GKit::Util::File::ReadFile(void* file, unsigned char** data)
{
	*data = (unsigned char*)((File_Info*)file)->data;

	return ((File_Info*)file)->size;
}

void GKit::Util::File::ReadFile(void* file, char** data)
{
	*data = (char*)((File_Info*)file)->data;
}


char* GKit::Util::File::GetFullPath(char* name)
{
	// risky adding dynamic memory and depending on caller to delete (however since this is private function I don't have to worry about forgetting)
	char* fullPath = new char[MAX_PATH];
	sprintf(fullPath, "%s%s", path, name);

	return fullPath;
}


void* GKit::Util::File::OpenNormal(char* fullPath)
{
	// Get Size
	size_t size = GKit::Util::Directory::Size(fullPath);

	// Create Buffer
	if (size > MAX_FILE_SIZE)
		throw GKit::Util::Exception(GKIT_ERROR_FILE_TOO_LARGE);

	char* buffer = nullptr;
	try
	{
		buffer = new char[size + 1];
	}
	catch (std::bad_alloc&)
	{
		throw GKit::Util::Exception(GKIT_ERROR_MEMORY_ALLOCATION);
	}

	// Read File into Buffer
	GKit::Util::Directory::Read(fullPath, buffer, size + 1);

	File_Info* info = new File_Info;
	info->data = buffer;
	info->size = size;

	return info;
}

void* GKit::Util::File::OpenBinary(char* fullPath)
{
	// Get Size
	size_t size = GKit::Util::Directory::Size(fullPath);

	// Create Buffer
	if (size > MAX_FILE_SIZE)
		throw GKit::Util::Exception(GKIT_ERROR_FILE_TOO_LARGE);

	unsigned char* buffer = nullptr;
	try
	{
		buffer = new unsigned char[size];
	}
	catch (std::bad_alloc&)
	{
		throw GKit::Util::Exception(GKIT_ERROR_MEMORY_ALLOCATION);
	}

	// Read File into Buffer
	GKit::Util::Directory::Read(fullPath, buffer, size);
	File_Info* info = new File_Info;
	info->data = buffer;
	info->size = size;

	return info;
}