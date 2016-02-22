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

	float speed = 1;

	if (m_inHalfHole)
	{
		speed = m_speed / 1.5;
	}
	else if (m_inHole)
	{
		speed = m_speed / 2;
	}
	else{
		speed = m_speed;
	}

	if (m_inHoleCounter == 0){

		if ((closestPlayer != nullptr && ((closestPlayer->getPosition().y == m_collisionBox.m_position.y && abs(closestPlayer->getPosition().x - m_collisionBox.m_position.x) <= 500
			) || m_sawPlayer == true) && m_onLadder == false)){
			m_directionSteps = 10; //
			m_direction = glm::normalize(closestPlayer->getPosition() - m_collisionBox.m_position);
			m_collisionBox.m_position += m_direction * speed * deltaTime;
		}
		else {
			//otherwise move in random directions

			static std::mt19937 randomEngine(time(nullptr));

			static std::uniform_int_distribution<int> randMov(0, 2);

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
				else if (xDepth >= m_collisionBox.getDimensions().x && rand >= 2 && m_wasOnLadder == false)
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

				m_collisionBox.m_position += m_direction *  speed * deltaTime;

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
					//	m_direction.y = m_direction.y * (-1.0);
					m_direction.y = 0;
					m_direction.x = randDir(randomEngine);
					if (m_direction.x == 0)
					{
						m_direction.x = 1;
					}
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

				m_collisionBox.m_position += m_direction *  speed * deltaTime;

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
		collideWithHalfHole(level.getHalfHoleBoxes());
		collideWithHole(level.getHoleBoxes());

	}
	else{
		m_inHoleCounter--;
	}

}

bool Monster::collideWithHalfHole(std::vector<Box>& levelBoxes){

	for (auto box : levelBoxes)
	{
		glm::vec4 penetrationDepth;
		if (collideWithBox(&box, penetrationDepth) == true)
		{
			handleCollisionWithUnmoveableObject(penetrationDepth);
			m_inHalfHole = false;
			return true;
		}
		Box boxAboveGround = Box();
		boxAboveGround.m_dimensions = box.m_dimensions;
		boxAboveGround.m_position = glm::vec2(box.m_position.x, box.m_position.y + TILE_WIDTH);
		if (collideWithBox(&boxAboveGround, penetrationDepth) == true)
		{
			if (abs(penetrationDepth.z - penetrationDepth.x) >= m_collisionBox.m_dimensions.x && m_inHoleCounter == 0){
				m_inHoleCounter = 20;
			}
			m_inHalfHole = true;
			return true;
		}


	}

	m_inHalfHole = false;
	return false;
}

bool Monster::collideWithHole(std::vector<Box>& levelBoxes){
	for (auto box : levelBoxes)
	{
		glm::vec4 penetrationDepth;
		if (collideWithBox(&box, penetrationDepth) == true)
		{
			handleCollisionWithUnmoveableObject(penetrationDepth);
			m_inHole = false;
			return true;
		}
		Box boxAboveGround = Box();
		boxAboveGround.m_dimensions = box.m_dimensions;
		boxAboveGround.m_position = glm::vec2(box.m_position.x, box.m_position.y + TILE_WIDTH);
		if (collideWithBox(&boxAboveGround, penetrationDepth) == true)
		{
			if (abs(penetrationDepth.z - penetrationDepth.x) >= m_collisionBox.m_dimensions.x && m_inHoleCounter == 0){
				m_inHoleCounter = 100;
				m_holeBox = &box;
			}
			m_inHole = true;
			return true;
		}


	}

	m_inHole = false;
	return false;
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

void Monster::setDirection(glm::vec2 newDirection){
	m_direction = newDirection;
	m_directionSteps = 1;
}


void Monster::draw(GameEngine::SpriteBatch& spriteBatch){
	m_collisionBox.draw(spriteBatch);
}


void Monster::kill(Player* killedBy) {
	m_died = true;
	m_killedBy = killedBy;
}