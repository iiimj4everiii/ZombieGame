#pragma once
#include <glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>

#include "Agent.h"

class Enemy : public Agent
{
public:
	Enemy();
	~Enemy();

	void init(float speed, glm::vec2 pos);

	virtual void update(const std::vector <std::string>& levelData, std::vector <Human*>& humans, std::vector <Enemy*>& enemies, float deltaTime) override;

private:
	Human* getNearestHuman(std::vector <Human*>& humans);

};