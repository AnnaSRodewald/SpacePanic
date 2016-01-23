#include "Player.h"
#include <GameEngine\ResourceManager.h>


Player::Player()
{
	//Empty
}


Player::~Player()
{
	
}


void Player::init(GameEngine::InputManager* inputManager, const glm::vec2 position, const glm::vec2 dimensions, std::string textureFilePath, GameEngine::ColorRGBA8 color, float speed){
	m_speed = speed;
	m_inputManager = inputManager;
//	m_camera = camera;dww


	m_health = 150;

	//Load the texture
	GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture(textureFilePath);

	m_collisionBox.init(position, dimensions, &texture, color, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f));

}


void Player::update(const std::vector<std::string>& levelData, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime){

	if (m_inputManager->isKeyDown(SDLK_w))
	{
		m_collisionBox.m_position.y += m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_s))
	{
		m_collisionBox.m_position.y -= m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_a))
	{
		m_collisionBox.m_position.x -= m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_d))
	{
		m_collisionBox.m_position.x += m_speed * deltaTime;
	}


	collideWithLevel(levelData);
}

void Player::update(std::vector<Box>& levelBoxes, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime){

	if (m_inputManager->isKeyDown(SDLK_w))
	{
		m_collisionBox.m_position.y += m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_s))
	{
		m_collisionBox.m_position.y -= m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_a))
	{
		m_collisionBox.m_position.x -= m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_d))
	{
		m_collisionBox.m_position.x += m_speed * deltaTime;
	}


	collideWithLevel(levelBoxes);
}


void Player::update(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime){

	if (m_inputManager->isKeyDown(SDLK_w) && collideWithLadder(level.getLadderBoxes()))
	{
		m_collisionBox.m_position.y += m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_s) && collideWithLadder(level.getLadderBoxes()))
	{
		m_collisionBox.m_position.y -= m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_a))
	{
		m_collisionBox.m_position.x -= m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_d))
	{
		m_collisionBox.m_position.x += m_speed * deltaTime;
	}


	collideWithLevel(level.getLevelBoxes());
}

void Player::draw(GameEngine::SpriteBatch& spriteBatch){
	m_collisionBox.draw(spriteBatch);
}