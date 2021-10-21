#pragma once
#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/Sprite.h>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/GameWindow.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/Timing.h>

#include "Projectile.h"
#include "Level.h"
#include "Player.h"

class Enemy; //Forward declaration for pointers only. Use include otherwise.

enum class GameState { PLAY, EXIT };

const float PLAYER_SPEED = 5.0f;
const float HUMAN_SPEED = 2.0f;
const float ENEMY_SPEED = 2.4f;

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void updateAgents(float deltaTime);
	void updateProjectiles(float deltaTime);
	void processInput();
	void drawGame();
	void initLevel();
	void checkVictory();

	std::string _gameName;
	int _screenWidth;
	int _screenHeight;
	GameEngine::GameWindow _window;
	GameState _gameState;
	float _time;
	float _maxFPS;
	float _fps;
	int _currentLevel;

	//Need to use pointer because every time vector resizes from push_back,
	//it makes a copy of the original memory and delete them afterwards for array resizing.
	//Whenever vector deletes the memory (in this case an object), it calls the destructor.
	//The object destructor in this case deletes the VBO.
	//So we will use Sprite pointer and allocate the vector on the heap.
	//This way only the pointers (address of Sprite on the heap) gets copied and deleted.
	//So the Sprite object will NOT be deleted and the destructor will NOT be called.
	//std::vector<GameEngine::Sprite*> _sprites;
	//Using SpriteBatch now instead.

	GameEngine::GLSLProgram _textureProgram;
	GameEngine::GLTexture _playerTexture;
	GameEngine::Camera2D _camera;

	GameEngine::InputManager _inputManager;
	GameEngine::FpsLimiter _fpsLimiter;

	std::vector <Level*> _levels;
	GameEngine::SpriteBatch _agentSpriteBatch;
	std::vector <Enemy*> _enemies;
	std::vector <Human*> _humans;
	std::vector <Projectile> _projectiles;

	Player* _player;

	int _numHumansKilled;
	int _numEnemiesKilled;

};

