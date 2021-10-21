#pragma once
#include <GameEngine/InputManager.h>
#include <GameEngine/Camera2D.h>

#include "Human.h"
#include "Projectile.h"

class Gun; //Forward declaration for pointers only. Use include otherwise.

class Player : public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector <Projectile>* projectiles);
	void addGun(Gun* gun);
	void update(const std::vector <std::string>& levelData, std::vector <Human*>& humans, std::vector <Enemy*>& enemies, float deltaTime); //No need to be virtual as there are no class deriving from Player

private:
	GameEngine::Camera2D* _camera;
	GameEngine::InputManager* _inputManager;
	std::vector <Gun*> _guns;
	int _currentGunIndex;
	std::vector <Projectile>* _projectiles;
};

