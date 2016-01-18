#pragma once
#include "Agent.h"
#include "Player.h"
class Monster :
	public Agent
{
public:
	Monster();
	~Monster();

	void init(float speed, glm::vec2 position, glm::vec2 dimensions);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;

	float getHealth() override {
		return m_health;
	};
private:
	Player* getNearestPlayer(std::vector<Player*>& Player);
};

