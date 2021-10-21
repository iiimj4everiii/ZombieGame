#include <iostream>
#include <fstream>
#include <GameEngine/Errors.h>
#include <GameEngine/ResourceManager.h>

#include "Level.h"

Level::Level(const std::string& filename)
{
	std::ifstream infile(filename.c_str());
	if (infile.fail())
	{
		GameEngine::fatalError("Unable to load level " + filename);
	}

	std::string tmp;
	infile >> tmp >> _numHumans;

	std::getline(infile, tmp); //Throw away the first line. Not calling push_back().

	//Read the rest of the level data
	while (std::getline(infile, tmp))
	{
		_levelData.push_back(tmp);
	}
	infile.close();

	_spriteBatch.init();
	_spriteBatch.begin();

	//Create uvRect
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	//Create color
	GameEngine::ColorRGBA8 whiteColor = GameEngine::ColorRGBA8(255, 255, 255, 255);
	//whiteColor.r = 255;
	//whiteColor.g = 255;
	//whiteColor.b = 255;
	//whiteColor.alpha = 255;

	for (int y = 0; y < _levelData.size(); y++)
	{
		for (int x = 0; x < _levelData[y].size(); x++)
		{
			//Grab the tile
			char tile = _levelData[y][x];

			//Get destination rectangle
			glm::vec4 destinationRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//Process the tile
			switch (tile)
			{
			case '.':
				break;
			case '#':
				_spriteBatch.draw(destinationRect, uvRect, GameEngine::ResourceManager::getTexture("textures/brick.png").textureID, 0.0f, whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destinationRect, uvRect, GameEngine::ResourceManager::getTexture("textures/glass.png").textureID, 0.0f, whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.'; //We should not be colliding with @
				_playerStartPos.x = x * TILE_WIDTH;
				_playerStartPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.'; //We should not be colliding with Z
				_enemyStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
				break;
			}
		}
	}

	_spriteBatch.end();
}

Level::~Level()
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();

	return;
}
