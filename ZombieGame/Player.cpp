#include "Player.h"
#include <GameEngine\ResourceManager.h>

//currently no gun (-1)
Player::Player() : m_currentGunIndex(-1)
{
	//Empty
}


Player::~Player()
{
	for (int i = 0; i < m_guns.size(); i++)
	{
		delete m_guns[i];
	}
}


void Player::init(float speed, glm::vec2 position, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector<Bullet>* bullets){
	m_speed = speed;
	m_position = position;
	m_inputManager = inputManager;
	m_camera = camera;
	m_bullets = bullets;

	m_color.r = 255;
	m_color.g = 255;
	m_color.b = 255;
	m_color.a = 255;

	m_health = 150;

	m_textureID = GameEngine::ResourceManager::getTexture("Textures/player.png").id;

}

void Player::addGun(Gun* gun){
	//Add the gun to the players inventory
	m_guns.push_back(gun);

	//If no gun equipped, equip gun;
	if (m_currentGunIndex == -1)
	{
		m_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){
	if (m_inputManager->isKeyDown(SDLK_w))
	{
		m_position.y += m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_s))
	{
		m_position.y -= m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_a))
	{
		m_position.x -= m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_d))
	{
		m_position.x += m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_1) && m_guns.size() >= 0)
	{
		m_currentGunIndex = 0;
	}
	else if (m_inputManager->isKeyDown(SDLK_2) && m_guns.size() >= 1)
	{
		m_currentGunIndex = 1;
	}
	else if (m_inputManager->isKeyDown(SDLK_3) && m_guns.size() >= 2)
	{
		m_currentGunIndex = 2;
	}

	glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
	mouseCoords = m_camera->convertScreenToWorld(mouseCoords);

	glm::vec2 centerPlayerPosition = m_position + glm::vec2(AGENT_RADIUS);

	//Mouse direction
	m_direction = glm::normalize(mouseCoords - centerPlayerPosition);


	if (m_currentGunIndex != -1)
	{
		m_guns[m_currentGunIndex]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT), centerPlayerPosition, m_direction, *m_bullets, deltaTime);
	}

	collideWithLevel(levelData);
}