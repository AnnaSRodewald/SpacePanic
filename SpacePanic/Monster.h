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
		return m_inHole;
	}
	bool isInHalfHole() const {
		return m_inHalfHole;
	}

	Box& getHole() const {
		return m_holeBox;
	}

	bool isAlive(){
		return !m_died;
	}

	Player* getKiller() const { return m_killedBy; }

	int getHolteCounter() const { return m_inHoleCounter; }


private:
	Player* getNearestPlayer(std::vector<Player*>& Player);

	int m_directionSteps = -1;
	bool m_sawPlayer = false;
	bool m_wasOnLadder = false;
	bool m_inHole = false;
	bool m_inHalfHole = false;
	int m_inHoleCounter = 0;
	Box& m_holeBox = Box();
	//m_holeBox.init(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), GameEngine::ResourceManager::getTexture("Textures/red_bricks.png"), GameEngine::ColorRGBA8(255, 255, 255, 255));
	bool m_died = false;
	Player* m_killedBy;
};

