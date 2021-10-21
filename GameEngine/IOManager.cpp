#include <fstream>

#include "IOManager.h"
#include "Errors.h"

namespace GameEngine
{
	bool IOManager::readFileToBuffer(const std::string filePath, std::vector<unsigned char> &buffer)
	{
		std::ifstream infile(filePath, std::ios::binary);
		if (infile.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//{ Start: Getting the size of the file to be readin.

		//Put the cursor at the end of the file.
		infile.seekg(0, std::ios::end);

		//Get the file size. tellg() returns streampos type (can be converted to int)
		std::streampos fileSize = infile.tellg();

		//Put the cursor back at the beginning of the file.
		infile.seekg(0, std::ios::beg);

		//Removing the size of the header bytes that might be present.
		fileSize -= infile.tellg();

		//} End: Getting the size of the file to be read in.

		buffer.resize((unsigned int) fileSize);

		//The address of the first element in the vector is the pointer to a char that's at the beginning of the string
		infile.read((char *)&(buffer[0]), fileSize);

		infile.close();

		return true;
	}
}