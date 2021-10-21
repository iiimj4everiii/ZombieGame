#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "ImageLoader.h"
#include "IOManager.h"
#include "picoPNG.h"
#include "Errors.h"

namespace GameEngine {
	ImageLoader::ImageLoader()
	{
		//ctor
	}

	GLTexture ImageLoader::loadPNG(std::string filePath)
	{
		GLTexture texture = {};

		std::vector <unsigned char> inImage;
		std::vector <unsigned char> outImage;
		unsigned long imageWidth, imageHeight;

		if (IOManager::readFileToBuffer(filePath, inImage) == false)
		{
			fatalError("Fail to load PNG file to vector!");
		}

		//Even though it is looking for a char* in the 4th argument, we can use &(array[0])
		int errorCode = decodePNG(outImage, imageWidth, imageHeight, &(inImage[0]), inImage.size());

		std::ostringstream os;
		os << errorCode;

		if (errorCode != 0)
		{
			fatalError("decodePNG failed to decode PNG file! Error code: " + os.str());
		}

		glGenTextures(1, &texture.textureID);
		glBindTexture(GL_TEXTURE_2D, texture.textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &outImage[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = imageWidth;
		texture.height = imageHeight;

		return texture;
	}
}