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

void Monster::init(float speed, glm::vec2 position, const glm::vec2 drawDims, const glm::vec2 collisionDims){
	m_speed = speed;
	m_collisionBox.init(position, collisionDims, &GameEngine::ResourceManager::getTexture("Assets/cvJmPda.png"), glm::ivec2(3, 2), GameEngine::ColorRGBA8(255, 255, 255, 255));
	m_collisionBox.setDrawDims(drawDims);
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

	static std::mt19937 randomEngine(time(nullptr));

	static std::uniform_int_distribution<int> randMov(0, 4);

	static std::uniform_int_distribution<int> randDir(0, 1);

	static std::uniform_int_distribution<int> randSteps(80, 150);

	static std::uniform_int_distribution<int> randFollowPlayer(-5, 210);


	float speed = 1;

	if (m_inHalfHole)
	{
		speed = m_speed / 1;
	}
	else if (m_inHole)
	{
		speed = m_speed / 2;
	}
	else{
		speed = m_speed;
	}

	if (isInAir(level.getLevelBoxes()))
	{
		m_inAir = true;
	}
	else
	{
		m_inAir = false;
	}

		if (m_inHoleCounter == 0){

			if (players[0] != nullptr && m_futurePath.size() == 0 && m_sawPlayer == true)
			{
				m_animTime = 0.0f;
				m_futurePath = determinePathToPlayer(level, monsters, *players[0]);
				m_calculatedNewPath = true;
				if (m_futurePath.size() != 0)
				{
					m_direction = glm::vec2(0.0f);
					if (changeDirectionToFuturePath())
					{
						m_directionSteps = randSteps(randomEngine);
						m_direction.y = 0;
						m_direction.x = randDir(randomEngine) * 2 - 1;
						if (m_direction.x == 0)
						{
							m_direction.x = 1;
						}
						m_sawPlayer = false;
					}
					m_collisionBox.m_position += glm::vec2(m_direction.x * speed * deltaTime, m_direction.y * speed * deltaTime);

					collideWithHalfHole(level.getHalfHoleBoxes());
					collideWithHole(level.getHoleBoxes());
					if (collideWithLevel(level.getLevelBoxes())){
					}

					if (m_wasOnLadder == true){
						m_wasOnLadder = false;
					}

				}
			}
			else if (m_futurePath.size() != 0 && m_sawPlayer == true)
			{
				if (changeDirectionToFuturePath())
				{
					m_directionSteps = randSteps(randomEngine);
					m_direction.y = 0;
					m_direction.x = randDir(randomEngine) * 2 - 1;
					if (m_direction.x == 0)
					{
						m_direction.x = 1;
					}
					m_sawPlayer = false;
					m_animTime = 0.0f;
				}
				m_collisionBox.m_position += glm::vec2(m_direction.x * speed * deltaTime, m_direction.y * speed * deltaTime);

				collideWithHalfHole(level.getHalfHoleBoxes());
				collideWithHole(level.getHoleBoxes());
				if (collideWithLevel(level.getLevelBoxes())){
				}

				if (m_wasOnLadder == true){
					m_wasOnLadder = false;
				}

			}
			else {

				if ((closestPlayer != nullptr && ((closestPlayer->getPosition().y == m_collisionBox.m_position.y && abs(closestPlayer->getPosition().x - m_collisionBox.m_position.x) <= 20
					) || m_sawPlayer == true) && m_onLadder == false)){
					m_directionSteps = 10; //
					m_direction = glm::normalize(closestPlayer->getPosition() - m_collisionBox.m_position);
					m_collisionBox.m_position += m_direction * speed * deltaTime;
					m_sawPlayer = false;
					m_animTime = 0.0f;
				}
				else {
					//otherwise move in random directions

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
							if (m_inAir == false && rand >= 4)
							{
								m_directionSteps = randSteps(randomEngine);
								m_direction.y = 0;
								m_direction.x = randDir(randomEngine) * 2 - 1;
								if (m_direction.x == 0)
								{
									m_direction.x = 1;
								}
								m_animTime = 0.0f;

							}
							else
								if (m_directionSteps <= 0)
								{
									m_directionSteps = randSteps(randomEngine);
									m_direction.x = 0;
									m_direction.y = randDir(randomEngine) * 2 - 1;
									if (m_direction.x == 0)
									{
										m_direction.x = 1;
									}
									m_animTime = 0.0f;
								}


						}
						else if (xDepth >= m_collisionBox.getDimensions().x && rand >= 4 && m_wasOnLadder == false)
						{
							m_directionSteps = randSteps(randomEngine);
							m_direction.x = 0;
							m_direction.y = randDir(randomEngine) * 2 - 1;
							if (m_direction.y == 0)
							{
								m_direction.y = 1;
							}
							m_animTime = 0.0f;
						}
						else if (m_directionSteps <= 0)
						{
							m_directionSteps = randSteps(randomEngine);
							m_direction.y = 0;
							m_direction.x = randDir(randomEngine) * 2 - 1;
						if (m_direction.x == 0)
							{
								m_direction.x = 1;
							}
							m_animTime = 0.0f;
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
						else
							if (collision)
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
								m_direction.x = randDir(randomEngine) * 2 - 1;
								if (m_direction.x == 0)
								{
									m_direction.x = 1;
								}
								m_animTime = 0.0f;
							}


					}
					else{

						if (m_directionSteps <= 0)
						{
							m_directionSteps = randSteps(randomEngine);
							m_direction.y = 0;
							m_direction.x = randDir(randomEngine) * 2 - 1;
							if (m_direction.x == 0)
							{
								m_direction.x = 1;
							}
							m_animTime = 0.0f;
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

				int followPRand = randFollowPlayer(randomEngine);
				if (followPRand  >= 200)
				{
					m_sawPlayer = true;
				}
			}
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
				m_holeBox = box;
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
				m_holeBox = box;
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
	m_directionSteps = 10;
}


void Monster::draw(GameEngine::SpriteBatch& spriteBatch){
	glm::vec4 uvRect;

	int tileIndex;
	int numTiles;

	float animSpeed = 0.2f;

	if (m_sawPlayer ==true)
	{
		tileIndex = 0;
	}
	else {
		//monsters are always running
		tileIndex = 3;
	}
	numTiles = 3;
	animSpeed = 10 * 0.025f;

	//Increment animation time
	//TODO: add deltaTime
	m_animTime += animSpeed;

	//Apply animation
	tileIndex = tileIndex + ((int)m_animTime % numTiles);

	//get the uv coordinates from the tile index
	uvRect = m_collisionBox.m_texture.getUVs(tileIndex);

	//Check direction
	if (m_direction == glm::vec2(-1.0f, 0.0f))
	{
		uvRect.x += 1.0f / m_collisionBox.m_texture.dims.x;
		uvRect.z *= -1;
	}

	//set uvRect
	m_collisionBox.setUVRect(m_collisionBox.m_texture.getUVs(0));

	//set destRect
	glm::vec4 destRect;

	glm::vec2 position = m_collisionBox.getPosition();
	glm::vec2 dimensions = m_collisionBox.getDimensions();

	glm::vec2 drawDims = m_collisionBox.getDrawDims();

	if (drawDims == glm::vec2(0.0f, 0.0f))
	{
		destRect.x = position.x;// -m_dimensions.x / 2.0f;
		destRect.y = position.y;// -m_dimensions.y / 2.0f;
		destRect.z = dimensions.x;
		destRect.w = dimensions.y;
	}
	else {
		destRect.x = position.x - drawDims.x / 8.0f;
		destRect.y = position.y;// - m_dimensions.y / 2.0f;
		destRect.z = drawDims.x;
		destRect.w = drawDims.y;
	}

	m_collisionBox.draw(spriteBatch, destRect);
}


void Monster::kill(Player* killedBy) {
	m_died = true;
	m_killedBy = killedBy;
}

void Monster::changeDirectionTo(Box& box){
	glm::vec2 diff = box.getPosition() - m_collisionBox.getPosition();
	m_direction = glm::normalize(diff);
}

std::vector<glm::vec2> Monster::determinePathToPlayer(Level& level, std::vector<Monster*>& monsters, Player& player){
	glm::vec2 playerTilePosition = glm::vec2(round(player.getBox().getPosition().x / TILE_WIDTH), round(player.getBox().getPosition().y / TILE_WIDTH));

	float restX = std::fmod(player.getBox().getPosition().x, TILE_WIDTH);
	float restY = std::fmod(player.getBox().getPosition().y, TILE_WIDTH);

	/*LevelNode start = LevelNode(playerTilePosition);
	LevelNode goal = LevelNode(glm::vec2(round(m_collisionBox.getPosition().x / TILE_WIDTH), round(m_collisionBox.getPosition().y / TILE_WIDTH)));
	*/
	std::vector<glm::vec2> result = determinePathTo(level, monsters,
		glm::vec2(round(player.getBox().getPosition().x / TILE_WIDTH), round(player.getBox().getPosition().y / TILE_WIDTH)),
		glm::vec2(round(m_collisionBox.getPosition().x / TILE_WIDTH), round(m_collisionBox.getPosition().y / TILE_WIDTH)));

	return result;
	//std::unordered_map<LevelNode, LevelNode> came_from;
	//std::unordered_map<LevelNode, int> cost_so_far;
	/*
		came_from = useAStarSearch(level.getLevelMap(), monsters, start, goal, came_from, cost_so_far);
		return reconstruct_path(start, goal, came_from);*/
}
//
////A* Pathfinding Algorithim
//std::unordered_map<LevelNode, LevelNode>& Monster::useAStarSearch(
//	std::vector<LevelNode>& levelMap,
//	std::vector<Monster*>& monsters,
//	LevelNode& start,
//	LevelNode& goal,
//	std::unordered_map<LevelNode, LevelNode>& came_from,
//	std::unordered_map<LevelNode, int>& cost_so_far)
//{
//	PriorityQueue frontier;
//	frontier.put(start, 0);
//
//	came_from[start] = start;
//	cost_so_far[start] = 0;
//
//	while (!frontier.empty())
//	{
//		auto& current = frontier.get();
//
//		if (current.getPosition() == goal.getPosition())
//		{
//			break;
//		}
//
//		std::cout << "Visiting " << current.getX() << ", " << current.getY() << std::endl;
//		for (auto& next : current.getNeighbors()) {
//			int new_cost = cost_so_far[current] + current.costTo(next);
//			if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
//				cost_so_far[next] = new_cost;
//				int priority = new_cost + heuristic(next.getPosition(), goal.getPosition());
//				frontier.put(next, priority);
//				came_from[next] = current;
//			}
//		}
//	}
//
//	return came_from;
//
//	//glm::vec2(round(m_collisionBox.getPosition().x / TILE_WIDTH), round(m_collisionBox.getPosition().y / TILE_WIDTH)));
//
//
//	//define neighbors
//
//}
//
//std::vector<LevelNode> Monster::reconstruct_path(
//	LevelNode& start,
//	LevelNode& goal,
//	std::unordered_map<LevelNode, LevelNode>& came_from)
//{
//	std::vector<LevelNode> path;
//	LevelNode& current = goal;
//	path.push_back(current);
//	while (current.getPosition() != start.getPosition()) {
//		current = came_from[current];
//		path.push_back(current);
//	}
//	std::reverse(path.begin(), path.end());
//	return path;
//}
//
//int heuristic(glm::vec2 a, glm::vec2 b){
//	return abs(a.x - b.x) + abs(a.y - b.y);
//}

std::vector<glm::vec2> Monster::determinePathTo(Level& level, std::vector<Monster*>& monsters, glm::vec2 startP, glm::vec2 goalP){

	SquareGrid::Location start = std::tie(startP.x, startP.y);
	SquareGrid::Location goal = std::tie(goalP.x, goalP.y);

	std::vector<glm::vec2> result;

	//std::vector<LevelNode> levelMap = level.getLevelMap();
	std::unordered_map<typename SquareGrid::Location, typename SquareGrid::Location> came_from;
	std::unordered_map<typename SquareGrid::Location, int> cost_so_far;

	came_from = pathfinder.a_star_search(level.getMap(), start, goal, came_from, cost_so_far);
	/*pathfinder.draw_grid(level.getMap(), 2, nullptr, &came_from);
	std::cout << std::endl;
	pathfinder.draw_grid(level.getMap(), 3, &cost_so_far, nullptr);
	std::cout << std::endl;
*/
	//std::vector<SquareGrid::Location> path = pathfinder.reconstruct_path(start, goal, came_from);
	//pathfinder.draw_grid(level.getMap(), 3, nullptr, nullptr, &path);

	result = pathfinder.reconstruct_path2(start, goal, came_from);
	return result;
}

bool Monster::changeDirectionToFuturePath(){
	glm::vec2 current = m_futurePath.back(); //.at(m_currentPathStep);
	//current.x = current.x *TILE_WIDTH;
	//current.y = current.y *TILE_WIDTH;

	glm::vec2 monsterPosition = glm::vec2((m_collisionBox.getPosition().x / TILE_WIDTH), (m_collisionBox.getPosition().y / TILE_WIDTH));

	//glm::vec2 next = m_futurePath.at(m_futurePath.size() - 2);

	if (current.x == monsterPosition.x && current.y == monsterPosition.y )
	{
		//	m_currentPathStep++;
		m_reachedNextStep = true;
		m_futurePath.pop_back();
		if (m_futurePath.size() == 0)
		{
			return true;
		}
		else{
			current = m_futurePath.back(); //.at(m_currentPathStep);
		/*	current.x = current.x *TILE_WIDTH;
			current.y = current.y *TILE_WIDTH;*/
		}
	}
	else{
		m_reachedNextStep = false;
	}

	
	glm::vec2 direction = glm::normalize(current - monsterPosition); //glm::vec2(round(m_collisionBox.getPosition().x / TILE_WIDTH), round(m_collisionBox.getPosition().y / TILE_WIDTH))); //m_collisionBox.getPosition());
		float x, y;
		if (direction.x > 0)
		{
			x = std::ceil(direction.x);
		}
		else {
			x = std::floor(direction.x);
		}
		if (direction.y > 0)
		{
			y = std::ceil(direction.y);
		}
		else {
			y = std::floor(direction.y);
		}

		if ((x!= 0 && m_direction.x == x*(-1)) || (y != 0 && m_direction.y == y*(-1)))
		{
			//stuttering problem 
			m_sawPlayer = false;
			m_futurePath.clear();
		}
		else{
			m_direction = glm::vec2(x, y);
		}

		return false;
	
}
