#pragma once
#include <map>
#include <string>
#include "GLTexture.h"

namespace GameEngine
{
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);

	private:
		//std::map <key, value>
		std::map <std::string, GLTexture> _textureMap;
	};
}