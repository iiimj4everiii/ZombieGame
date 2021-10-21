#include <iostream>
#include <stdio.h>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Errors.h>
#include <GameEngine/ImageLoader.h>
#include <GameEngine/ResourceManager.h>
#include <random>
#include <ctime>
#include <algorithm>

#include "MainGame.h"
#include "Enemy.h"
#include "Gun.h"

MainGame::MainGame() :
	_gameState(GameState::PLAY),
	_gameName("Zombie Game"),
	_screenWidth(1920),
	_screenHeight(1080),
	_fps(0),
	_maxFPS(60),
	_player(nullptr),
	_numHumansKilled(0),
	_numEnemiesKilled(0)
{
}

MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}

	for (int i = 0; i < _enemies.size(); i++)
	{
		delete _enemies[i];
	}

	for (int i = 0; i < _humans.size(); i++)
	{
		delete _humans[i];
	}
}

void MainGame::run()
{
	//Initial SDL, create a window, initialize shaders and levels
	initSystems();

	//Initialize level
	initLevel();

	//Run game loop
	gameLoop();

	return;
}

void MainGame::initSystems()
{
	GameEngine::init();

	//Create window
	_window.createWindow(_gameName, _screenWidth, _screenHeight, 0);

	//Set window background color
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	//Initialize shaders
	initShaders();

	//Initialize camera
	_camera.init(_screenWidth, _screenHeight);

	//Initialize sprite batch
	_agentSpriteBatch.init();

	return;
}

void MainGame::initLevel()
{
	//Initialize Level 1
	_levels.push_back(new Level("levels/level1.txt"));
	_currentLevel = 0;

	//Initialize _player and push back player in _humans vector
	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getPlayerStartPos(), &_inputManager, &_camera, &_projectiles);
	_humans.push_back(_player);

	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randXPos(2, _levels[_currentLevel]->getWidth() - 2);
	std::uniform_real_distribution<float> randYPos(2, _levels[_currentLevel]->getHeight() - 2);

	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++)
	{
		glm::vec2 humanPos(randXPos(randomEngine) * TILE_WIDTH, randYPos(randomEngine) * TILE_WIDTH);
		_humans.push_back(new Human);
		_humans.back()->init(HUMAN_SPEED, humanPos);
	}

	const std::vector <glm::vec2>& enemyPositions = _levels[_currentLevel]->getEnemyStartPos();
	
	for (int i = 0; i < enemyPositions.size(); i++)
	{
		_enemies.push_back(new Enemy);
		_enemies.back()->init(ENEMY_SPEED, enemyPositions[i]);
	}

	//Set up player gun
	const float BULLET_SPEED = 20;
	_player->addGun(new Gun("Pistol", 10, 1, 0.10f, BULLET_SPEED, 50.0f));
	_player->addGun(new Gun("Shot Gun", 30, 20, 0.26f, BULLET_SPEED, 8.0f));
	_player->addGun(new Gun("Machine Gun", 2, 1, 0.15f, BULLET_SPEED, 10.0f));

	return;
}

void MainGame::initShaders()
{
	//Compile Vertex and Fragment shaders
	_textureProgram.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");

	//Add attribute from the in vec2 vertexPosition
	_textureProgram.addAttribute("vertexPosition");

	//Add attribute from the in vec4 vertexColor
	_textureProgram.addAttribute("vertexColor");

	//Add attribute from the in vec4 vertexUV
	_textureProgram.addAttribute("vertexUV");

	//Link shaders
	_textureProgram.linkShaders();
}

void MainGame::gameLoop()
{
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FPS = 60;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;
	const float MAX_PHYSICS_STEPS = 6; //To avoid spiral of death by limiting time step simulation.

	//Limit FPS to _maxFPS (60 FPS)
	_fpsLimiter.setMaxFPS(_maxFPS);

	float previousTicks = SDL_GetTicks();

	while (_gameState == GameState::PLAY)
	{
		_fpsLimiter.begin();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();

		_inputManager.update();

		processInput();

		int physicsStepCount = 0;
		while (totalDeltaTime > 0.0f && physicsStepCount < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			updateAgents(deltaTime);

			updateProjectiles(deltaTime);

			totalDeltaTime -= deltaTime;
			physicsStepCount++;
		}

		_camera.setPosition(_player->getAgentPos()); //Making camera track player position
		_camera.update();

		drawGame();

		std::cout << _fps << std::endl;

		_fps = _fpsLimiter.end();
	}

	return;
}

void MainGame::updateAgents(float deltaTime)
{
	//Call update on all humans
	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _enemies, deltaTime);
	}

	//Call update on all enemies
	for (int i = 0; i < _enemies.size(); i++)
	{
		_enemies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _enemies, deltaTime);
	}

	//Update human collisions
	for (int i = 0; i < _humans.size(); i++)
	{
		//Collide with other humans
		for (int j = i + 1; j < _humans.size(); j++)
		{
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}

	//Update enemy collisions
	for (int i = 0; i < _enemies.size(); i++)
	{
		//Collide with other enemies
		for (int j = i + 1; j < _enemies.size(); j++)
		{
			_enemies[i]->collideWithAgent(_enemies[j]);
		}

		//Collide with humans
		for (int j = 1; j < _humans.size(); j++)
		{
			if (_enemies[i]->collideWithAgent(_humans[j]))
			{
				//Add a new enemy
				_enemies.push_back(new Enemy);
				_enemies.back()->init(ENEMY_SPEED, _humans[j]->getAgentPos());

				//Delete the human
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}

		//Collide with player
		if (_enemies[i]->collideWithAgent(_player))
		{
			//_gameState = GameState::EXIT;
			GameEngine::fatalError("Game Over!");
		}
	}

	return;
}

void MainGame::updateProjectiles(float deltaTime)
{
	for (int i = 0; i < _projectiles.size();)
	{
		//If collided with the world, remove the projecctile.
		if (_projectiles[i].update(_levels[_currentLevel]->getLevelData(), deltaTime))
		{
			//Set the current projectile to the last projectile.
			//Now we have 2 copies of last projectile. One at position i and one at the end.
			//Then we remove the projectile at the end.
			_projectiles[i] = _projectiles.back();
			_projectiles.pop_back();
		}
		else
		{
			i++;
		}
	}

	bool isProjectileRemoved;

	//Collide with humans and enemies
	for (int i = 0; i < _projectiles.size(); i++)
	{
		isProjectileRemoved = false;

		for (int j = 0; j < _enemies.size();)
		{
			//If projectile collides with enemies, damage health.
			if (_projectiles[i].collideWithAgent(_enemies[j]))
			{
				//Applying damage and check for 0 hp at the same time.
				if (_enemies[j]->applyDamage(_projectiles[i].getDamage()))
				{
					//If enemy dies, remove the enemy.
					delete _enemies[j];
					_enemies[j] = _enemies.back();
					_enemies.pop_back();
					_numEnemiesKilled++;
				}
				else
				{
					j++;
				}

				//Remove the projectile.
				_projectiles[i] = _projectiles.back();
				_projectiles.pop_back();
				i--;
				isProjectileRemoved = true;

				//Since projectile is gone, no need to loop through any more enemies
				break;
			}
			else
			{
				j++;

			}
		}

		//If the projectile is still around, then loop through humans
		if (isProjectileRemoved == false)
		{
			//Index starts with 1 so that we don't collide projectiles with the player.
			for (int j = 1; j < _humans.size();)
			{
				//If projectile collides with humans, damage health.
				if (_projectiles[i].collideWithAgent(_humans[j]))
				{
					//Applying damage and check for 0 hp at the same time.
					if (_humans[j]->applyDamage(_projectiles[i].getDamage()))
					{
						//If enemy dies, remove the enemy.
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}
					else
					{
						j++;
					}

					//Remove the projectile.
					_projectiles[i] = _projectiles.back();
					_projectiles.pop_back();
					i--;

					//Since projectile is gone, no need to loop through any more enemies
					break;
				}
				else
				{
					j++;
				}
			}
		}
	}

	return;
}

void MainGame::checkVictory()
{
	if (_enemies.empty())
	{
		printf("You win!\nNumber of enemies killed: %d.\nNumber of humans killed: %d.\nNumber of humans left: %d.\n", _numEnemiesKilled, _numHumansKilled, _humans.size());
		GameEngine::fatalError("");
	}
	return;
}

void MainGame::processInput()
{
	//Object that contains different kind of console inputs.
	SDL_Event event;

	//SDL function to poll event from console. Kind of like getch() from keyboard.
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << event.motion.x << " " << event.motion.y << std::endl;
			_inputManager.setMouseCoords(event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(event.button.button);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(event.key.keysym.sym);
			break;
		}
	}

	return;
}

void MainGame::drawGame()
{
	//Set base depth to 1.0
	glClearDepth(1.0);

	//Clear screen by clearing the color and depth buffer;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	//Using texture unit 0
	glActiveTexture(GL_TEXTURE0);

	GLint uniformTextureLocation = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(uniformTextureLocation, 0); //Make sure the shader uses texture unit 0

	//Grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint uniformOrthoProjectionLocation = _textureProgram.getUniformLocation("orthoProjection");
	glUniformMatrix4fv(uniformOrthoProjectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw the level
	_levels[_currentLevel]->draw();

	//Draw the humans, enemies, projectiles
	_agentSpriteBatch.begin();

	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _enemies.size(); i++)
	{
		_enemies[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _projectiles.size(); i++)
	{
		_projectiles[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();

	_textureProgram.unuse();

	_window.swapBuffer();
}