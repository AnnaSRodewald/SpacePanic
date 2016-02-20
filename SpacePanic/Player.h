#pragma once

#include "Box.h"
#include <GameEngine\SpriteBatch.h>
#include <GameEngine\GLTexture.h>
#include <GameEngine\InputManager.h>

#include "Agent.h"
#include "Monster.h"
#include "Level.h"

class Player :
	public Agent
{
public:
	Player();
	~Player();

	void init(GameEngine::InputManager* inputManager, const glm::vec2 position, const glm::vec2 dimensions, std::string textureFilePath, GameEngine::ColorRGBA8 color, float speed);

	virtual void update(const std::vector<std::string>& levelData, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;

	virtual void update(std::vector<Box>& levelBoxes, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;
	virtual void update(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;

	void draw(GameEngine::SpriteBatch& spriteBatch);


private:

	void updateMovements(Level& level, std::vector<Player*>& players, float deltaTime);
	void updateActions(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime);
	bool tryDigging(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, Box& groundBox);
	void playDiggingSound();


	GameEngine::InputManager* m_inputManager;
};

