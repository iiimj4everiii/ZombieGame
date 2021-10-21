#pragma once
#include <glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>

const float AGENT_WIDTH = 60; //slightly smaller the TILE_WIDTH
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

//Forward declaration of the derived class instead of using include is to avoid circular includes
class Human;
class Enemy;

class Agent
{
public:
	Agent();
	//If Agent pointer gets deleted by the destructor, it will not delete the inherted members.
	//Virtual destructor will also call the inherted members' destructor.
	virtual ~Agent();
	
	//Pure virtual function.
	//Derived classes must have this function
	virtual void update(const std::vector <std::string>& levelData, std::vector <Human*>& humans, std::vector <Enemy*>& enemies, float deltaTime);

	bool collideWithLevel(const std::vector <std::string>& levelData);
	bool collideWithAgent(Agent* agent);

	void draw(GameEngine::SpriteBatch& spriteBatch);

	bool applyDamage(float damage); //Returns true if health drops below 0

	glm::vec2 getAgentPos() const { return _position; }

protected:
	void checkCollidableTilePos(const std::vector <std::string>& levelData, std::vector <glm::vec2>& collideTilePos, float x, float y);
	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 _direction;
	glm::vec2 _position;
	float _speed;
	GameEngine::ColorRGBA8 _color;
	float _health;
};