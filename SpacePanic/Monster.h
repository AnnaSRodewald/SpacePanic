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

	virtual bool collideWithHalfHole(std::vector<Box>& levelBoxes) override;
	virtual bool collideWithHole(std::vector<Box>& levelBoxes) override;

	void setDirection(glm::vec2 newDirection);

	void draw(GameEngine::SpriteBatch& spriteBatch);

	void kill(Player* killedBy);

	float getHealth() override {
		return m_health;
	};

	bool isInHole() const {
		return (m_holeBox != nullptr);
	}

	Box* getHole() const {
		return m_holeBox;
	}


private:
	Player* getNearestPlayer(std::vector<Player*>& Player);

	int m_directionSteps = -1;
	bool m_sawPlayer = false;
	bool m_wasOnLadder = false;
	bool m_inHole = false;
	bool m_inHalfHole = false;
	int m_inHoleCounter = 0;
	Box* m_holeBox = nullptr;
	bool m_died = false;
	Player* m_killedBy = nullptr;
};

