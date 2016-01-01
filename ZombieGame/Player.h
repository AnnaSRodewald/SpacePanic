#pragma once
#include "Human.h"
#include <GameEngine\InputManager.h>
#include "Gun.h"
#include <GameEngine\Camera2D.h>


class Player :
	public Human
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime) override;

private:
	GameEngine::InputManager* m_inputManager;
	GameEngine::Camera2D* m_camera;

	std::vector<Gun*> m_guns;
	int m_currentGunIndex;
	std::vector<Bullet>* m_bullets;
};

