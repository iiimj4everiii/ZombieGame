#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <GameEngine/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	Level(const std::string& filename); //Load the level
	~Level();

	void draw();

	glm::vec2 getPlayerStartPos() { return _playerStartPos; }
	const std::vector <glm::vec2>& getEnemyStartPos() { return _enemyStartPos; }

	const std::vector <std::string> getLevelData() const { return _levelData; }
	int getNumHumans() const { return _numHumans; }
	int getHeight() const { return _levelData.size(); }
	int getWidth() const { return _levelData[0].size(); }

private:
	int _numHumans;
	std::vector <std::string> _levelData;
	GameEngine::SpriteBatch _spriteBatch;
	glm::vec2 _playerStartPos;
	std::vector <glm::vec2> _enemyStartPos;
};

