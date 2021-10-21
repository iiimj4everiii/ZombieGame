#pragma once
#include "Agent.h"

class Human : public Agent
{
public:
	Human();
	//If Human pointer gets deleted by the destructor, it will not delete the inherted members.
	//Virtual destructor will also call the inherted members' destructor.
	virtual ~Human();

	void init(float speed, glm::vec2 pos);

	virtual void update(const std::vector <std::string>& levelData, std::vector <Human*>& humans, std::vector <Enemy*>& enemies, float deltaTime) override;

private:
	int _frame;
};

