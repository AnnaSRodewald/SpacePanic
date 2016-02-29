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


void Player::init(GameEngine::InputManager* inputManager, const glm::vec2 position, const glm::vec2 drawDims, const glm::vec2 collisionDims, std::string textureFilePath, GameEngine::ColorRGBA8 color, float speed, GameEngine::SoundEffect digHoleSound, GameEngine::SoundEffect closeHoleSound, GameEngine::SoundEffect dyingSound){
	m_speed = speed;
	m_inputManager = inputManager;
	//	m_camera = camera;dww

	m_health = 150;

	//Load the texture
	GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture(textureFilePath);

	m_collisionBox.init(position, collisionDims, &texture, color, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f));

	m_digHoleSound = digHoleSound;
	m_closeHoleSound = closeHoleSound;
	m_dyingSound = dyingSound;
	m_collisionBox.setDrawDims(drawDims);

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

	m_consecutiveMonsterKills = 0;
	if (m_fallingThroughHoleStep == 0)
	{
		updateMovements(level, players, deltaTime);
		updateActions(level, players, monsters, deltaTime);
	}
	else
	{
		fallThroughHole(m_holeBox, deltaTime);
	}
}




bool Player::collideWithHalfHole(std::vector<Box>& levelBoxes){

	for (auto box : levelBoxes)
	{
		glm::vec4 penetrationDepth;
		if (collideWithBox(&box, penetrationDepth) == true)
		{
			handleCollisionWithUnmoveableObject(penetrationDepth);
			return true;
		}
		Box boxAboveGround = Box();
		boxAboveGround.m_dimensions = box.m_dimensions;
		boxAboveGround.m_position = glm::vec2(box.m_position.x, box.m_position.y + TILE_WIDTH);
		if (collideWithBox(&boxAboveGround, penetrationDepth) == true)
		{
			handleCollisionWithUnmoveableObject(penetrationDepth);
			return true;
		}


	}
	return false;
}

bool Player::collideWithHole(std::vector<Box>& levelBoxes){
	//may fall?
	for (auto box : levelBoxes)
	{
		glm::vec4 penetrationDepth;
		if (collideWithBox(&box, penetrationDepth) == true)
		{
			handleCollisionWithUnmoveableObject(penetrationDepth);
			return true;
		}
		Box boxAboveGround = Box();
		boxAboveGround.m_dimensions = box.m_dimensions;
		boxAboveGround.m_position = glm::vec2(box.m_position.x, box.m_position.y + TILE_WIDTH);
		if (collideWithBox(&boxAboveGround, penetrationDepth) == true)
		{
			handleCollisionWithUnmoveableObject(penetrationDepth);
			m_holeBox = box;
			m_fallingThroughHoleStep = 1;
			return true;
		}


	}
	return false;
}



void Player::draw(GameEngine::SpriteBatch& spriteBatch){
	glm::vec4 uvRect;

	int tileIndex;
	int numTiles;

	float animSpeed = 0.2f;

	if (m_isAlive == true)
	{
		//Calculate animation
		if (m_onGround == true)
		{
			if (m_isDigging == true)
			{
				numTiles = 4;
				tileIndex = 18;
				if (m_moveState != PlayerMoveState::DIGGING)
				{
					m_moveState = PlayerMoveState::DIGGING;
					m_animTime = 0.0f;
				}
			}
			else if (m_moved == true)
			{ //Running
				numTiles = 6;
				tileIndex = 10;
				animSpeed = 10 * 0.025f;
				if (m_moveState != PlayerMoveState::RUNNING)
				{
					m_moveState = PlayerMoveState::RUNNING;
					m_animTime = 0.0f;
				}
			}
			else
			{ //Standing still
				numTiles = 1;
				tileIndex = 0;
				m_moveState = PlayerMoveState::STANDING;
			}
		}
		else
		{
			//In the air
			numTiles = 1;
			if (m_onLadder)
			{
				//Climbing
				tileIndex = 16;
				m_moveState = PlayerMoveState::CLIMBING;
			}
			else
			{
				//Falling
				tileIndex = 17;
				m_moveState = PlayerMoveState::FALLING;
			}

		}
	}
	else
	{
		numTiles = 1;
		tileIndex = 19;
	}

	//Increment animation time
	//TODO: add deltaTime
	m_animTime += animSpeed;

	//Check for digging end
	if (m_animTime > numTiles)
	{
		m_isDigging = false;
	}

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

	m_collisionBox.setUVRect(uvRect);
	m_collisionBox.draw(spriteBatch);
}

void Player::kill(){
	m_isAlive = false;
	//TODO: trigger death animation and change sprite to "dead-state" sprite
}

void Player::updateMovements(Level& level, std::vector<Player*>& players, float deltaTime) {
	bool collidedWithLadder = collideWithLadder(level.getLadderBoxes());

	m_onGround = true;

	m_moved = false;

	if (collidedWithLadder) {

		if (m_inputManager->isKeyDown(SDLK_w) || m_inputManager->isKeyDown(SDLK_s)){
			m_moved = true;
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

			bool collidedWithLevel = collideWithLevel(level.getLevelBoxes());
			bool collidedWithHalfHole = collideWithHalfHole(level.getHalfHoleBoxes());
			bool collidedWithHole = collideWithHole(level.getHoleBoxes());

			if (collidedWithLevel == false && collidedWithHalfHole == false &&
				collidedWithHole == false)
			{
				if (isInAir(level.getLevelBoxes()))
				{
					m_onLadder = true;
					m_onGround = false;
				}
				else
				{
					m_onLadder = false;
				}
			}
			else if (collidedWithHole == false)
			{
				//TODO: FALLING
				m_onGround = false;
			}
			else
			{
				m_onLadder = false;
			}
		}
	}

	if (m_onLadder == false || isInAir(level.getLevelBoxes()) == false) {
		if (m_inputManager->isKeyDown(SDLK_a))
		{
			m_collisionBox.m_position.x -= m_speed * deltaTime;
			m_direction = glm::vec2(-1.0f, 0.0f);
			m_moved = true;
		}
		else if (m_inputManager->isKeyDown(SDLK_d))
		{
			m_collisionBox.m_position.x += m_speed * deltaTime;
			m_direction = glm::vec2(1.0f, 0.0f);
			m_moved = true;
		}
	}

	//Apply Collision Physics for player here
	collideWithLevel(level.getLevelBoxes());
	collideWithHalfHole(level.getHalfHoleBoxes());
	collideWithHole(level.getHoleBoxes());
}

void Player::updateActions(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) {

	if (m_inputManager->isKeyPressed(SDLK_SPACE)){
		Box& groundBox = Box();
		if (tryDigging(level, players, monsters, groundBox) == true){
			m_isDigging = true;
		}
	}
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
	else { //rechts
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

		for (size_t i = 0; i < holeBoxes.size();){
			if (foundGroundBox == false && collideBoxWithBox(groundBox, holeBoxes[i])){
				//Dig the hole over again
				foundGroundBox = true;
				groundBox = holeBoxes[i];

				bool monsterIsInHole = false;
				//If monster is in hole --> kill it!
				for (auto monster : monsters)
				{
					if (monster->isInHole() && monster->getHolteCounter() > 0 && isSameBox(&monster->getHole(), &groundBox)){
						//monster is stuck in the hole
						//now kill the monster
						monster->kill(this);
						playCloseHoleSound();
					}
					else if (monster->isInHole() && isSameBox(&monster->getHole(), &groundBox))
					{
						//monster is no longer stuck in the hole
						monsterIsInHole = true;
					}
					else if (monster->isInHole() && collideBoxWithBox(boxAboveGround, monster->getBox()))
					{
						//monster isn't stuck yet in the hole but going to in a few seconds
						monsterIsInHole = true;
					}

				}

				if (monsterIsInHole == false)
				{
					holeBoxes[i] = holeBoxes.back();
					holeBoxes.pop_back();

					groundBox.m_color = GameEngine::ColorRGBA8(255, 255, 255, 255);
					groundBox.m_textureID = GameEngine::ResourceManager::getTexture("Textures/red_bricks.png").id;
					groundBox.m_texture.texture = GameEngine::ResourceManager::getTexture("Textures/red_bricks.png");

					levelBoxes.push_back(groundBox);
					playCloseHoleSound();
				}

				break;
			}
			else{
				i++;
			}
		}

		for (size_t i = 0; i < halfHoleBoxes.size();)
		{
			if (foundGroundBox == false && collideBoxWithBox(groundBox, halfHoleBoxes[i]))
			{
				bool monsterIsInHole = false;
				for (auto monster : monsters)
				{
					if (monster->isInHalfHole() && collideBoxWithBox(boxAboveGround, monster->getBox())){
						monsterIsInHole = true;
					}
				}

				foundGroundBox = true;
				groundBox = halfHoleBoxes[i];

				if (monsterIsInHole == false)
				{
					halfHoleBoxes[i] = halfHoleBoxes.back();
					halfHoleBoxes.pop_back();

					groundBox.m_color = GameEngine::ColorRGBA8(255, 0, 0, 0);
					groundBox.m_textureID = GameEngine::ResourceManager::getTexture("Textures/red_bricks.png").id;
					groundBox.m_texture.texture = GameEngine::ResourceManager::getTexture("Textures/red_bricks.png");

					//halfHoleBoxes.erase(std::remove(halfHoleBoxes.begin(), halfHoleBoxes.end(), groundBox), halfHoleBoxes.end());
					holeBoxes.push_back(groundBox);
					playDiggingSound();
				}

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

				if (foundGroundBox == false && collideBoxWithBox(groundBox, levelBoxes[i]) && //and black color glass
					(levelBoxes[i].getColor().r != 0 || levelBoxes[i].getColor().g || 0 && levelBoxes[i].getColor().b || 0))
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
			else if (foundGroundBox == true)
			{
				groundBox.m_color = GameEngine::ColorRGBA8(0, 255, 0, 255);

				//levelBoxes.erase(std::remove(levelBoxes.begin(), levelBoxes.end(), groundBox), levelBoxes.end());
				groundBox.m_textureID = GameEngine::ResourceManager::getTexture("Textures/light_bricks.png").id;
				halfHoleBoxes.push_back(groundBox);
				playDiggingSound();
			}
		}
	}

	return foundGroundBox;
}

void Player::playDiggingSound(){
	std::printf("DIG");
	m_digHoleSound.play();
}

void Player::playCloseHoleSound(){
	std::printf("Close");
	m_closeHoleSound.play();
}

void Player::fallThroughHole(Box& holeBox, float deltaTime){
	glm::vec4 penetrationDepth;
	float speed = 4.0f;

	Box boxAboveHole = Box();
	boxAboveHole.m_dimensions = holeBox.m_dimensions;
	boxAboveHole.m_position = glm::vec2(holeBox.m_position.x, holeBox.m_position.y + TILE_WIDTH);

	//ground box 2 tiles below holeBox
	Box groundBoxBelowHole = Box();
	groundBoxBelowHole.m_dimensions = holeBox.m_dimensions;
	groundBoxBelowHole.m_position = glm::vec2(holeBox.m_position.x, holeBox.m_position.y - (TILE_WIDTH * 3));


	if (m_fallingThroughHoleStep == 1 && collideWithBox(&boxAboveHole, penetrationDepth) && (abs(penetrationDepth.z - penetrationDepth.x) >= TILE_WIDTH || abs(penetrationDepth.z - penetrationDepth.x) >= m_collisionBox.getDimensions().x))
	{
		m_fallingThroughHoleStep = 2;
	}
	else if (m_fallingThroughHoleStep == 1)
	{
		m_direction = glm::normalize(boxAboveHole.getPosition() - m_collisionBox.getPosition());
	}

	if (m_fallingThroughHoleStep == 2 && collideWithBox(&holeBox, penetrationDepth) && (abs(penetrationDepth.w - penetrationDepth.y) >= TILE_WIDTH || abs(penetrationDepth.w - penetrationDepth.y) >= m_collisionBox.getDimensions().y))
	{
		m_fallingThroughHoleStep = 3;
	}
	else if (m_fallingThroughHoleStep == 2){
		m_direction = glm::normalize(holeBox.getPosition() - m_collisionBox.getPosition());
	}

	if (m_fallingThroughHoleStep == 3 && collideWithBox(&groundBoxBelowHole, penetrationDepth))// && (abs(penetrationDepth.w - penetrationDepth.y) >= TILE_WIDTH || abs(penetrationDepth.w - penetrationDepth.y) >= m_collisionBox.getDimensions().y))
	{
		//landed on the ground
		m_fallingThroughHoleStep = 0;
	}
	else if (m_fallingThroughHoleStep == 3)
	{
		m_direction = glm::normalize(groundBoxBelowHole.getPosition() - m_collisionBox.getPosition());
	}

	if (m_fallingThroughHoleStep != 0)
	{
		m_collisionBox.m_position += m_direction * speed * deltaTime;
	}
	m_moved = true;
}