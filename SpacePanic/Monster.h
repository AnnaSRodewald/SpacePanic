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
	virtual void update(std::vector<Box>& levelBoxes, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;

	virtual void update(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;

	void draw(GameEngine::SpriteBatch& spriteBatch);

	float getHealth() override {
		return m_health;
	};
private:
	Player* getNearestPlayer(std::vector<Player*>& Player);

	int m_directionSteps = -1;
	bool m_sawPlayer = false;
	bool m_wasOnLadder = false;
};

