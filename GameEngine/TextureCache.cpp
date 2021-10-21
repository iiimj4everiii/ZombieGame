#include <iostream>
#include "TextureCache.h"
#include "ImageLoader.h"

namespace GameEngine
{
	TextureCache::TextureCache()
	{
		//ctor
	}

	TextureCache::~TextureCache()
	{
		//dtor
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//Since the iterator mit has the same type as _textureMap, we can use auto to save some typing.
		//std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath);
		//Lookup the texture and see if it is in the map.
		auto mit = _textureMap.find(texturePath);

		//Check if its not in the map.
		//We will only cache texture is new/not cached before.
		if (mit == _textureMap.end())
		{
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			//std::pair<std::string, GLTexture> texturePair(texturePath, newTexture);
			//_textureMap.insert(texturePair);

			_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "Texture cached!\n";

			return newTexture;
		}

		//std::cout << "Texture loaded!\n";

		//Otherwise cache the new texture.
		//The second psition in std::map mit holds the GLTexture.
		return mit->second;
	}
}