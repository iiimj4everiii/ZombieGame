#include <SDL/SDL.h>

#include "Player.h"
#include "Gun.h"

Player::Player() :
	_currentGunIndex(-1)
{
}

Player::~Player()
{
}

void Player::init(float speed, glm::vec2 pos, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector <Projectile>* projectiles)
{
	_speed = speed;
	_position = pos;
	_inputManager = inputManager;
	_camera = camera;
	_projectiles = projectiles;
	_color.r = 0;
	_color.g = 0;
	_color.b = 185;
	_color.alpha = 255;

	return;
}

void Player::addGun(Gun* gun)
{
	_guns.push_back(gun);

	//If no gun equipped, equip gun
	if (_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}

	return;
}


void Player::update(const std::vector <std::string>& levelData, std::vector <Human*>& humans, std::vector <Enemy*>& enemies, float deltaTime)
{
	if (_inputManager->isKeyDown(SDLK_w) || _inputManager->isKeyDown(SDLK_UP))
	{
		_position.y += _speed * deltaTime;
		_direction.y += 0.1;
		_direction = glm::normalize(_direction);
	}
	else if (_inputManager->isKeyDown(SDLK_a) || _inputManager->isKeyDown(SDLK_LEFT))
	{
		_position.x -= _speed * deltaTime;
		_direction.x -= 0.1;
		_direction = glm::normalize(_direction);
	}
	else if (_inputManager->isKeyDown(SDLK_s) || _inputManager->isKeyDown(SDLK_DOWN))
	{
		_position.y -= _speed * deltaTime;
		_direction.y -= 0.1;
		_direction = glm::normalize(_direction);
	}
	else if (_inputManager->isKeyDown(SDLK_d) || _inputManager->isKeyDown(SDLK_RIGHT))
	{
		_position.x += _speed * deltaTime;
		_direction.x += 0.1;
		_direction = glm::normalize(_direction);
	}
	
	if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0)
	{
		_currentGunIndex = 0;
	}
	else if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1)
	{
		_currentGunIndex = 1;
	}
	else if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2)
	{
		_currentGunIndex = 2;
	}

	if (_currentGunIndex != -1)
	{
		glm::vec2 mouseCoords = _inputManager->getMouseCoords();
		mouseCoords = _camera->convertScreenToWorldCoords(mouseCoords);
		glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
		glm::vec2 shotVector = mouseCoords - centerPosition;
		glm::vec2 shotDirection = glm::normalize(shotVector);
		_guns[_currentGunIndex]->update((_inputManager->isKeyDown(SDL_BUTTON_LEFT) || _inputManager->isKeyDown(SDLK_SPACE)), centerPosition, shotDirection, *_projectiles, deltaTime);
	}

	collideWithLevel(levelData);

	return;
}