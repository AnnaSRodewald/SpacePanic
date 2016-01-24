#include "Monster.h"

#include "Player.h"

#include <GameEngine\ResourceManager.h>

Monster::Monster()
{
	m_health = 150;
}


Monster::~Monster()
{

}

void Monster::init(float speed, glm::vec2 position, glm::vec2 dimensions){
	m_speed = speed;
	m_collisionBox.init(position, dimensions, &GameEngine::ResourceManager::getTexture("Textures/zombie.png"), GameEngine::ColorRGBA8(255, 255, 255, 255));
}

void Monster::update(const std::vector<std::string>& levelData,
	std::vector<Player*>& players, std::vector<Monster*>& zombies, float deltaTime){

	Player* closestPlayer = getNearestPlayer(players);

	if (closestPlayer != nullptr){
		m_direction = glm::normalize(closestPlayer->getPosition() - m_collisionBox.m_position);
		m_collisionBox.m_position += m_direction * m_speed * deltaTime;
	}

	collideWithLevel(levelData);

}

void Monster::update(std::vector<Box>& levelBoxes, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime){
	Player* closestPlayer = getNearestPlayer(players);

	if (closestPlayer != nullptr){
		m_direction = glm::normalize(closestPlayer->getPosition() - m_collisionBox.m_position);
		m_collisionBox.m_position += m_direction * m_speed * deltaTime;
	}

	collideWithLevel(levelBoxes);
}

void Monster::update(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime){
	Player* closestPlayer = getNearestPlayer(players);

	if (closestPlayer != nullptr){
		m_direction = glm::normalize(closestPlayer->getPosition() - m_collisionBox.m_position);
		m_collisionBox.m_position += m_direction * m_speed * deltaTime;
	}

	collideWithLevel(level.getLevelBoxes());
}

Player* Monster::getNearestPlayer(std::vector<Player*>& Players){
	Player* closestPlayer = nullptr;
	float smallestDistance = 999999999999.0f;

	for (int i = 0; i < Players.size(); i++)
	{
		glm::vec2 distVec = Players[i]->getPosition() - m_collisionBox.m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance){
			smallestDistance = distance;
			closestPlayer = Players[i];
		}
	}

	return closestPlayer;
}

void Monster::draw(GameEngine::SpriteBatch& spriteBatch){
	m_collisionBox.draw(spriteBatch);
}


