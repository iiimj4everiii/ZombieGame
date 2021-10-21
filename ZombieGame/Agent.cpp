#include <GameEngine/ResourceManager.h>
#include <algorithm>

#include "Agent.h"
#include "Level.h"

Agent::Agent()
{
}

Agent::~Agent()
{
}

void Agent::update(const std::vector <std::string>& levelData, std::vector <Human*>& humans, std::vector <Enemy*>& enemies, float deltaTime)
{
	return;
}

bool Agent::collideWithAgent(Agent* agent)
{
	//Move agent reference point to center
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getAgentPos() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;
	const float MIN_DISTANCE = AGENT_RADIUS + AGENT_RADIUS;

	float collisionDepth = MIN_DISTANCE - glm::length(distVec); //glm::length() gives magnitude of distVec

	//Condition for a circle collision
	if (collisionDepth > 0)
	{
		glm::vec2 collisionVec = glm::normalize(distVec) * collisionDepth;

		//Push each circle out by half the distance
		_position += collisionVec / 2.0f;
		agent->_position -= collisionVec / 2.0f;

		return true;
	}

	return false;
}

bool Agent::collideWithLevel(const std::vector <std::string>& levelData)
{
	std::vector <glm::vec2> collideTilePos;

	//Check Agents' four corners
	checkCollidableTilePos(levelData, collideTilePos, _position.x,				 _position.y); 					//1st corner
	checkCollidableTilePos(levelData, collideTilePos, _position.x + AGENT_WIDTH, _position.y); 					//2nd corner
	checkCollidableTilePos(levelData, collideTilePos, _position.x,				 _position.y + AGENT_WIDTH);	//3rd corner
	checkCollidableTilePos(levelData, collideTilePos, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH); 	//4th corner

	if (collideTilePos.empty())
	{
		return false;
	}

	for (int i = 0; i < collideTilePos.size(); i++)
	{
		collideWithTile(collideTilePos[i]);
	}

	return true;
}

void Agent::checkCollidableTilePos(const std::vector <std::string>& levelData, std::vector <glm::vec2>& collideTilePos, float x, float y)
{
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

	//If we are outside of the world, just return;
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() || cornerPos.y < 0 || cornerPos.y >= levelData.size())
	{
		return;
	}

	//Getting a vector of collidable tiles
	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		//Move collidable tile reference point to center and adding tiles to a vector 
		collideTilePos.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f, (float)TILE_WIDTH / 2.0f));
	}

	return;
}

void Agent::collideWithTile(glm::vec2 tilePos)
{
	//Axis Aligned Bounding Box (AABB) Collision

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	//Move player reference point to center
	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	
	glm::vec2 distVec = centerPlayerPos - tilePos;
	
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	//Condition for a box collision
	if (xDepth > 0 && yDepth > 0)
	{
		//Only do collision if depth is positive
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{
			if (distVec.x < 0)
			{
				_position.x -= xDepth;
			}
			else
			{
				_position.x += xDepth;

			}
		}
		else
		{
			if (distVec.y < 0)
			{
				_position.y -= yDepth;
			}
			else
			{
				_position.y += yDepth;
			}
		}
	}

	return;
}

void Agent::draw(GameEngine::SpriteBatch& spriteBatch)
{	
	static GLuint textureID = GameEngine::ResourceManager::getTexture("textures/circle.png").textureID;

	glm::vec4 destinationRect = glm::vec4(_position, AGENT_WIDTH, AGENT_WIDTH);

	const glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	spriteBatch.draw(destinationRect, uvRect, textureID, 0.0f, _color);

	return;
}

bool Agent::applyDamage(float damage)
{
	_health -= damage;

	if (_health <= 0)
	{
		return true;
	}

	return false;
}