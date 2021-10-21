#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>

class Agent;

const int PROJECTILE_RADIUS = 5;

class Projectile
{
public:
	Projectile(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Projectile();

	bool update(const std::vector <std::string>& levelData, float deltaTime);
	void draw(GameEngine::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* agent);

	float getDamage() const { return _damage; }

private:
	bool collideWithWorld(const std::vector <std::string>& levelData);

	glm::vec2 _position;
	glm::vec2 _direction;
	float _damage;
	float _speed;

};

