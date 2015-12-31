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
	GameEngine::InputManager* _inputManager;
	GameEngine::Camera2D* _camera;

	std::vector<Gun*> _guns;
	int _currentGunIndex;
	std::vector<Bullet>* _bullets;
};

