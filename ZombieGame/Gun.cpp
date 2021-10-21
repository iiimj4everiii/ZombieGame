#include <random>
#include <ctime>
#include <GameEngine/GLTexture.h>
#include <GameEngine/ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>

#include "Gun.h"

Gun::Gun(std::string name, int fireRate, int projectilesPerShot, float spread, float projectileSpeed, float projectileDamage) :
	_name(name),
	_fireRate(fireRate),
	_projectilesPerShot(projectilesPerShot),
	_spread(spread),
	_projectileSpeed(projectileSpeed),
	_projectileDamage(projectileDamage),
	_frameCounter(0.0f)
{
}

Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector <Projectile>& projectiles, float deltaTime)
{
	_frameCounter += 1.0f * deltaTime;
	if (isMouseDown && _frameCounter >= _fireRate)
	{
		fire(position, direction, projectiles);
		_frameCounter = 0.0f;
	}
}

void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector <Projectile>& projectiles)
{
	static std::mt19937 randomEngine((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> randRotation(-_spread, _spread); //Radian

	for (int i = 0; i < _projectilesPerShot; i++)
	{
		float angle = randRotation(randomEngine);
		projectiles.emplace_back(position, glm::rotate(direction, angle), _projectileDamage, _projectileSpeed);
	}

	return;
}