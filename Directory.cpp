/*

	GLua Toolkit - Minimalist toolkit to interface with GLua
	Created by - DazzaGe

	Directory - FileIO helper functions for a directory

*/

#include "Directory.h"




void GKit::Util::Directory::Validate(const char* path)
{
	CreateDirectory(path, NULL);
}


bool GKit::Util::Directory::Exists(char* path)
{
	DWORD attributes = GetFileAttributesA(path);

	return (attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}


long int GKit::Util::Directory::Size(char* path)
{
	// Open File
	FILE* file = nullptr;
	file = fopen(path, "rb");
	if (file == nullptr)
		throw GKit::Util::Exception(GKIT_ERROR_OPEN_FILE, errno);

	// Get Size
	fseek(file, 0, SEEK_END);
	long int size = ftell(file);

	// Cleanup
	fclose(file);
	return size;
}


size_t GKit::Util::Directory::Read(char* path, unsigned char* buffer, size_t bufferSize)
{
	// Open File
	FILE* file = nullptr;
	file = fopen(path, "rb");
	if (file == nullptr)
		throw GKit::Util::Exception(GKIT_ERROR_OPEN_FILE, errno);

	// Read File
	size_t readLength = fread(buffer, sizeof(unsigned char), bufferSize, file);
	if (ferror(file) != 0)
	{
		fclose(file);
		throw GKit::Util::Exception(GKIT_ERROR_READ_FILE, ferror(file));
	}

	// Cleanup
	//data->pop_back();	// remove eof
	fclose(file);
	return readLength;
}

void GKit::Util::Directory::Read(char* path, char* strBuffer, size_t bufferSize)
{
	if (bufferSize < 1)
		return;

	// Open File
	FILE* file = nullptr;
	file = fopen((char const*)path, "rb");
	if (file == nullptr)
		throw GKit::Util::Exception(GKIT_ERROR_OPEN_FILE, errno);

	// Read File
	fread(strBuffer, sizeof(char), bufferSize - 1, file);
	if (ferror(file) != 0)
	{
		fclose(file);
		throw GKit::Util::Exception(GKIT_ERROR_READ_FILE, ferror(file));
	}

	// Cleanup
	strBuffer[bufferSize - 1] = '\0';
	fclose(file);
}


void GKit::Util::Directory::Write(char* path, unsigned char* buffer, size_t bufferSize)
{
	// Open File
	FILE* file = nullptr;
	file = fopen(path, "wb");
	if (file == nullptr)
		throw GKit::Util::Exception(GKIT_ERROR_OPEN_FILE, errno);

	// Write File
	fwrite(buffer, sizeof(unsigned char), bufferSize, file);
	if (ferror(file) != 0)
	{
		fclose(file);
		throw GKit::Util::Exception(GKIT_ERROR_WRITE_FILE, ferror(file));
	}

	// Cleanup
	fclose(file);
}

void GKit::Util::Directory::Write(char* path, char* strBuffer)
{
	// Open File
	FILE* file = nullptr;
	file = fopen(path, "wb");
	if (file == nullptr)
		throw GKit::Util::Exception(GKIT_ERROR_OPEN_FILE, errno);

	// Write File
	fwrite(strBuffer, sizeof(char), strlen(strBuffer), file);
	if (ferror(file) != 0)
	{
		fclose(file);
		throw GKit::Util::Exception(GKIT_ERROR_WRITE_FILE, ferror(file));
	}

	// Cleanup
	fclose(file);
}