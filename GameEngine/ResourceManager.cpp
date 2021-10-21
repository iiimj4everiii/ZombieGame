#include "ResourceManager.h"

namespace GameEngine
{
	TextureCache ResourceManager::_textureCache;

	GLTexture ResourceManager::getTexture(std::string texturePath)
	{
		//Although the same name getTexture, but this getTexture is call with TextureCache object.
		//So this getTexture belongs to TextureCache class function.
		return _textureCache.getTexture(texturePath);
	}
}