#include <random>
#include <ctime>
#include "Human.h"
#include <glm/gtx/rotate_vector.hpp>

Human::Human() :
	_frame(0)
{
}

Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos)
{
	_speed = speed;
	_position = pos;
	
	_color = GameEngine::ColorRGBA8(0, 185, 0, 255);
	//_color.r = 0;
	//_color.g = 185;
	//_color.b = 0;
	//_color.alpha = 255;

	static std::mt19937 randomEngine((unsigned int)time(nullptr));
	static std::uniform_real_distribution<float> randDirection(-1.0f, 1.0f);
	_direction = glm::vec2(randDirection(randomEngine), randDirection(randomEngine));
	
	if (_direction.length() != 0)
	{
		_direction = glm::normalize(_direction);
	}

	_health = 20;

	return;
}

void Human::update(const std::vector <std::string>& levelData, std::vector <Human*>& humans, std::vector <Enemy*>& enemies, float deltaTime)
{
	_position += _direction * _speed * deltaTime;

	static std::mt19937 randomEngine((unsigned int)time(nullptr));
	static std::uniform_real_distribution<float> randRotation(-0.5236f, 0.5236f); //Radian
	float angle = randRotation(randomEngine);

	if (_frame == 60)
	{
		//glm::rotate angle is in radian.
		_direction = glm::rotate(_direction, angle);
		_frame = 0;
	}

	_frame++;

	if (collideWithLevel(levelData))
	{
		_direction = glm::rotate(_direction, 3*angle);
	}

	return;
}