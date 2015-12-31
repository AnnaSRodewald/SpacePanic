#pragma once
#include "Agent.h"

class Zombie :
	public Agent
{
public:
	Zombie();
	~Zombie();

	void init(float speed, glm::vec2 position);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime) override;

	float getHealth() override {
		return _health;
	};
private:
	Human* getNearestHuman(std::vector<Human*>& humans);
};

