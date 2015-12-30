#include "Player.h"

//currently no gun (-1)
Player::Player() : _currentGunIndex(-1)
{
	//Empty
}


Player::~Player()
{
	for (int i = 0; i < _guns.size(); i++)
	{
		delete _guns[i];
	}
}


void Player::init(float speed, glm::vec2 position, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector<Bullet>* bullets){
	_speed = speed;
	_position = position;
	_inputManager = inputManager;
	_camera = camera;
	_bullets = bullets;

	_color.r = 0;
	_color.g = 0;
	_color.b = 185;
	_color.a = 255;

	_health = 150;

}

void Player::addGun(Gun* gun){
	//Add the gun to the players inventory
	_guns.push_back(gun);

	//If no gun equipped, equip gun;
	if (_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies){
	if (_inputManager->isKeyDown(SDLK_w))
	{
		_position.y += _speed;
	}
	else if (_inputManager->isKeyDown(SDLK_s))
	{
		_position.y -= _speed;
	}

	if (_inputManager->isKeyDown(SDLK_a))
	{
		_position.x -= _speed;
	}
	else if (_inputManager->isKeyDown(SDLK_d))
	{
		_position.x += _speed;
	}

	if (_inputManager->isKeyDown(SDLK_1) && _guns.size()>=0)
	{
		_currentGunIndex = 0;
	}
	else if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1)
	{
		_currentGunIndex = 1;
	}
	else if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2)
	{
		_currentGunIndex = 2;
	}

	if (_currentGunIndex != -1)
	{
		glm::vec2 mouseCoords = _inputManager->getMouseCoords();
		mouseCoords = _camera->convertScreenToWorld(mouseCoords);

		glm::vec2 centerPlayerPosition = _position + glm::vec2(AGENT_RADIUS);

		glm::vec2 mouseDirection = glm::normalize(mouseCoords - centerPlayerPosition);

		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT), centerPlayerPosition, mouseDirection, *_bullets);
	}

	collideWithLevel(levelData);
}