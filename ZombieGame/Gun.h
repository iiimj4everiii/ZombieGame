#pragma once
#include <vector>
#include <string>

#include "Projectile.h"

class Gun
{
public:
	Gun(std::string name, int fireRate, int projectilesPerShot, float spread, float projectileSpeed, float projectileDamage);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector <Projectile>& projectiles, float deltaTime);

private:
	void fire(const glm::vec2& position, const glm::vec2& direction, std::vector <Projectile>& projectiles);

	std::string _name;
	int _fireRate;
	float _spread;
	int _projectilesPerShot;
	float _projectileSpeed;
	float _projectileDamage;
	float _frameCounter;
};

