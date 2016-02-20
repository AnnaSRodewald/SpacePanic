#include "Player.h"
#include <GameEngine\ResourceManager.h>
#include <vector> // the general-purpose vector container
#include <iostream>
#include <algorithm> // remove and remove_if


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

	updateMovements(level, players, deltaTime);
	updateActions(level, players, monsters, deltaTime);


}

void Player::updateMovements(Level& level, std::vector<Player*>& players, float deltaTime) {
	bool collidedWithLadder = collideWithLadder(level.getLadderBoxes());

	if (collidedWithLadder) {

		if (m_inputManager->isKeyDown(SDLK_w) || m_inputManager->isKeyDown(SDLK_s)){
			if (m_inputManager->isKeyDown(SDLK_w))
			{
				m_collisionBox.m_position.y += m_speed * deltaTime;
				m_direction = glm::vec2(0.0f, 1.0f);
			}
			else if (m_inputManager->isKeyDown(SDLK_s))
			{
				m_collisionBox.m_position.y -= m_speed * deltaTime;
				m_direction = glm::vec2(0.0f, -1.0f);
			}

			if (collideWithLevel(level.getLevelBoxes()) == false)
			{
				m_onLadder = true;
			}
			else
			{
				m_onLadder = false;
			}
		}
	}

	if (m_onLadder == false) {
		if (m_inputManager->isKeyDown(SDLK_a))
		{
			m_collisionBox.m_position.x -= m_speed * deltaTime;
			m_direction = glm::vec2(-1.0f, 0.0f);
		}
		else if (m_inputManager->isKeyDown(SDLK_d))
		{
			m_collisionBox.m_position.x += m_speed * deltaTime;
			m_direction = glm::vec2(1.0f, 0.0f);
		}
	}

	//Apply Physics for player here

	collideWithLevel(level.getLevelBoxes());
}

void Player::updateActions(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) {
	/*if (collideWithLadder(level.getLadderBoxes()) == false)
	{*/
	if (m_inputManager->isKeyPressed(SDLK_SPACE)){
		Box& groundBox = Box();
		if (tryDigging(level, players, monsters, groundBox) == true){
			playDiggingSound();
		}
	}
	/*}*/
}

bool Player::tryDigging(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, Box& groundBox){
	std::vector<Box>& levelBoxes = level.getLevelBoxes();
	std::vector<Box>& ladderBoxes = level.getLadderBoxes();
	std::vector<Box>& halfHoleBoxes = level.getHalfHoleBoxes();
	std::vector<Box>& holeBoxes = level.getHoleBoxes();

	if (m_direction == glm::vec2(-1.0f, 0.0f)) // links
	{
		groundBox.m_position.x = m_collisionBox.m_position.x - TILE_WIDTH;
		groundBox.m_position.y = m_collisionBox.m_position.y - TILE_WIDTH;
	}
	else {
		groundBox.m_position.x = m_collisionBox.m_position.x + TILE_WIDTH;
		groundBox.m_position.y = m_collisionBox.m_position.y - TILE_WIDTH;
	}
	groundBox.m_dimensions = glm::vec2(TILE_WIDTH, TILE_WIDTH);

	Box boxAboveGround = Box();
	boxAboveGround.m_dimensions = groundBox.m_dimensions;
	boxAboveGround.m_position = glm::vec2(groundBox.m_position.x, groundBox.m_position.y + TILE_WIDTH);

	bool wallAboveGround = false;
	bool foundGroundBox = false;

	if (collideBoxWithBoxes(boxAboveGround, ladderBoxes) == false){

		for (size_t i = 0; i < halfHoleBoxes.size();)
		{
			if (foundGroundBox == false && collideBoxWithBox(groundBox, halfHoleBoxes[i]))
			{
				foundGroundBox = true;
				groundBox = halfHoleBoxes[i];

				halfHoleBoxes[i] = halfHoleBoxes.back();
				halfHoleBoxes.pop_back();

				groundBox.m_color = GameEngine::ColorRGBA8(255, 0, 0, 255);
				groundBox.m_textureID = GameEngine::ResourceManager::getTexture("Textures/glass.png").id;

				//halfHoleBoxes.erase(std::remove(halfHoleBoxes.begin(), halfHoleBoxes.end(), groundBox), halfHoleBoxes.end());
				holeBoxes.push_back(groundBox);
				break;
			}
			else{
				i++;
			}
		}

		if (foundGroundBox == false)
		{
			for (size_t i = 0; i < levelBoxes.size();)
			{
				if (collideBoxWithBox(boxAboveGround, levelBoxes[i]))
				{
					wallAboveGround = true;
					foundGroundBox = false;
					break;
				}

				if (foundGroundBox == false && collideBoxWithBox(groundBox, levelBoxes[i]))
				{
					groundBox = levelBoxes[i];
					foundGroundBox = true;
					levelBoxes[i] = levelBoxes.back();
					levelBoxes.pop_back();
				}
				else{
					i++;
				}
			}

			if (wallAboveGround == true){
				foundGroundBox = false;
				levelBoxes.push_back(groundBox); // add groundBox back to level boxes
			}
			else{
				groundBox.m_color = GameEngine::ColorRGBA8(0, 255, 0, 255);

				//levelBoxes.erase(std::remove(levelBoxes.begin(), levelBoxes.end(), groundBox), levelBoxes.end());
				groundBox.m_textureID = GameEngine::ResourceManager::getTexture("Textures/light_bricks.png").id;
				halfHoleBoxes.push_back(groundBox);
			}
		}
	}

	return foundGroundBox;
}

void Player::playDiggingSound(){
	std::printf("DIG");
}

void Player::draw(GameEngine::SpriteBatch& spriteBatch){
	m_collisionBox.draw(spriteBatch);
}