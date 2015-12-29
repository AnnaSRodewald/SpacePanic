#pragma once
#include "Human.h"
#include <GameEngine\InputManager.h>


class Player :
	public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, GameEngine::InputManager* inputManager);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans, std::vector<Zombie*>& zombies);

private:
	GameEngine::InputManager* _inputManager;
};

