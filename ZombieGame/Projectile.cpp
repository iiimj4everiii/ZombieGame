#include <GameEngine/ResourceManager.h>

#include "Projectile.h"
#include "Agent.h"
#include "Level.h"

Projectile::Projectile(glm::vec2 position, glm::vec2 direction, float damage, float speed) :
	_position(position),
	_direction(direction),
	_damage(damage),
	_speed(speed)

{
}

Projectile::~Projectile()
{
}

bool Projectile::update(const std::vector <std::string>& levelData, float deltaTime)
{
	_position += _direction * _speed * deltaTime;

	return collideWithWorld(levelData);
}

void Projectile::draw(GameEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destinationRect(_position.x + PROJECTILE_RADIUS, _position.y + PROJECTILE_RADIUS, PROJECTILE_RADIUS * 2, PROJECTILE_RADIUS * 2);
	const glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	GameEngine::ColorRGBA8 color = GameEngine::ColorRGBA8(75, 75, 75, 255);
	//color.r = 75;
	//color.g = 75;
	//color.b = 75;
	//color.alpha = 255;
	
	spriteBatch.draw(destinationRect, uvRect, GameEngine::ResourceManager::getTexture("textures/circle.png").textureID, 0, color);

	return;
}

bool Projectile::collideWithAgent(Agent* agent)
{
	//Move agent reference point to center
	glm::vec2 centerPosA = _position + glm::vec2(PROJECTILE_RADIUS);
	glm::vec2 centerPosB = agent->getAgentPos() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;
	const float MIN_DISTANCE = AGENT_RADIUS + PROJECTILE_RADIUS;

	float collisionDepth = MIN_DISTANCE - glm::length(distVec); //glm::length() gives magnitude of distVec

	//Condition for a circle collision
	if (collisionDepth > 0)
	{
		return true;
	}

	return false;
}

bool Projectile::collideWithWorld(const std::vector <std::string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	//If we are outside of the world, return true;
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() || gridPosition.y < 0 || gridPosition.y >= levelData.size())
	{
		return true;
	}

	//if (levelData[gridPosition.y][gridPosition.x] != '.')
	//{
	//	return true;
	//}

	//return false;

	return (levelData[gridPosition.y][gridPosition.x] != '.');
}