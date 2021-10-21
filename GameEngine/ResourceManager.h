#pragma once
#include <string>
#include "TextureCache.h"

namespace GameEngine
{
	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

		static GLuint boundTexture;

	private:
		static TextureCache _textureCache;
	};
}