#pragma once
#include <string>
#include <GL/glew.h>

#include "GLTexture.h"

namespace GameEngine {
	class ImageLoader
	{
	public:
		ImageLoader();

		static GLTexture loadPNG(std::string filePath);
	};
}