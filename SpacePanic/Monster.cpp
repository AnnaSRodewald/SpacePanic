#include "Monster.h"

#include "Player.h"

#include <GameEngine\ResourceManager.h>
#include <random>
#include <ctime>

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

	if (closestPlayer != nullptr && ((closestPlayer->getPosition().y == m_collisionBox.m_position.y //&& abs(closestPlayer->getPosition().x - m_collisionBox.m_position.x) <= 50
		) || m_sawPlayer == true)){
		m_directionSteps = 10; //
		m_direction = glm::normalize(closestPlayer->getPosition() - m_collisionBox.m_position);
		m_collisionBox.m_position += m_direction * m_speed * deltaTime;
	}
	else {
		//otherwise move in random directions

		static std::mt19937 randomEngine(time(nullptr));

		static std::uniform_int_distribution<int> randMov(0, 3);

		static std::uniform_int_distribution<int> randDir(-1, 1);

		static std::uniform_int_distribution<int> randSteps(80, 150);

		glm::vec4 collisionWithLadderDepth = collideWithLadderAndGetCollisionDepth(level.getLadderBoxes());

		bool collidedWithLadder = (collisionWithLadderDepth != glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		//bool collidedWithLadder = collideWithLadder(level.getLadderBoxes());

		if (collidedWithLadder) {
			int rand = randMov(randomEngine);
			float xDepth = abs(collisionWithLadderDepth.z - collisionWithLadderDepth.x);

			if (m_onLadder == true){
				/*m_directionSteps = randSteps(randomEngine);
				m_direction.x = 0;
				m_direction.y = randDir(randomEngine);*/
			}
			else if (xDepth >= m_collisionBox.getDimensions().x && rand >= 1 && m_wasOnLadder == false)
			{
				m_directionSteps = randSteps(randomEngine);
				m_direction.x = 0;
				m_direction.y = randDir(randomEngine);
				if (m_direction.y == 0)
				{
					m_direction.y = 1;
				}

			}
			else if (m_directionSteps <= 0)
			{
				m_directionSteps = randSteps(randomEngine);
				m_direction.y = 0;
				m_direction.x = randDir(randomEngine);
				if (m_direction.x == 0)
				{
					m_direction.x = 1;
				}

				m_wasOnLadder = false;
			}


			//	m_direction = glm::normalize(m_direction);

			m_collisionBox.m_position += m_direction *  m_speed * deltaTime;

			bool collision = collideWithLevel(level.getLevelBoxes());
			if (collision == false && m_direction.y != 0)
			{
				m_onLadder = true;
				m_wasOnLadder = false;
			}
			else if (collision)
			{
				if (m_onLadder){
					m_wasOnLadder = true;
				}
				else {
					m_wasOnLadder = false;
				}
				m_onLadder = false;
				m_direction.y = m_direction.y * (-1.0);
				m_direction.x = m_direction.x * (-1.0);

			}


		}
		else{

			if (m_directionSteps <= 0)
			{
				m_directionSteps = randSteps(randomEngine);
				m_direction.y = 0;
				m_direction.x = randDir(randomEngine);
				if (m_direction.x == 0)
				{
					m_direction.x = 1;
				}
			}

			//	m_direction = glm::normalize(m_direction);

			m_collisionBox.m_position += m_direction *  m_speed * deltaTime;

			//Apply Physics for player here
			if (collideWithLevel(level.getLevelBoxes())){
				m_directionSteps = randSteps(randomEngine);
				m_direction.x = m_direction.x * (-1.0);
			}

			if (m_wasOnLadder == true){
				m_wasOnLadder = false;
			}

		}

		m_directionSteps--;

	}
}

Player* Monster::getNearestPlayer(std::vector<Player*>& players){
	Player* closestPlayer = nullptr;
	float smallestDistance = 999999999999.0f;

	for (int i = 0; i < players.size(); i++)
	{
		float distance;
		/*glm::vec2 distVec = players[i]->getPosition() - m_collisionBox.m_position;
		float distance = glm::length(distVec);*/

		if (abs(players[i]->getPosition().y - m_collisionBox.m_position.y) <= 64){
			distance = abs(players[i]->getPosition().x - m_collisionBox.m_position.x);

			if (distance < smallestDistance){
				smallestDistance = distance;
				closestPlayer = players[i];
			}
		}
	}

	return closestPlayer;
}

void Monster::draw(GameEngine::SpriteBatch& spriteBatch){
	m_collisionBox.draw(spriteBatch);
}


