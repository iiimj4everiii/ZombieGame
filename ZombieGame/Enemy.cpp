#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include "Enemy.h"

//In order to do something with Human, we still have to include "Human.h"
//even though we included "Agent.h" in the Enemy class. Need to look into this.
#include "Human.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::init(float speed, glm::vec2 pos)
{
	_speed = speed;
	_position = pos;

	_color = GameEngine::ColorRGBA8(185, 0, 0, 255);
	//_color.r = 185;
	//_color.g = 0;
	//_color.b = 0;
	//_color.alpha = 255;

	_health = 100;

	return;
}

void Enemy::update(const std::vector <std::string>& levelData, std::vector <Human*>& humans, std::vector <Enemy*>& enemies, float deltaTime)
{
	static std::mt19937 randomEngine((unsigned int)time(nullptr));
	static std::uniform_real_distribution<float> randRotation(-0.5236f, 0.5236f); //Radian
	float angle = randRotation(randomEngine);

	Human* nearestHuman = getNearestHuman(humans);
	if (nearestHuman != nullptr)
	{
		glm::vec2 distVec = nearestHuman->getAgentPos() - _position;
		glm::vec2 direction = glm::normalize(distVec);
		_position += direction * _speed * deltaTime;
	}

	collideWithLevel(levelData);

	return;
}

Human* Enemy::getNearestHuman(std::vector <Human*>& humans)
{
	Human* nearestHuman = nullptr;
	float nearestDist = 1000000;
	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getAgentPos() - _position;
		float distance = glm::length(distVec);
		if (distance < nearestDist)
		{
			nearestDist = distance;
			nearestHuman = humans[i];
		}
	}

	return nearestHuman;
}