#include "Agent.h"
#include <GameEngine/ResourceManager.h>
#include "Level.h"
#include <algorithm>
#include <iostream>

Agent::Agent(){


}


Agent::~Agent(){

}

void Agent::draw(GameEngine::SpriteBatch& spriteBatch){

	glm::vec2 position = m_collisionBox.getPosition();

	glm::vec4 destRect(position.x, position.y, AGENT_WIDTH, AGENT_WIDTH);

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	spriteBatch.draw(destRect, uvRect, m_collisionBox.getTextureID(), 0.0f, m_collisionBox.m_color, m_direction);
}

bool Agent::collideWithLevel(std::vector<Box>& levelBoxes){

	bool collided = false;


	for (auto& box : levelBoxes){
		glm::vec4 penetrationDepth;
		if (collideWithBox(&box, penetrationDepth)){

			glm::vec2& position = m_collisionBox.m_position;

			const float agent_width = m_collisionBox.m_dimensions.x;
			//const float agent_height = m_collisionBox.m_dimensions.y;

			float xDepth = abs(penetrationDepth.z - penetrationDepth.x);
			float yDepth = abs(penetrationDepth.w - penetrationDepth.y);

			if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
				if ((position.x - penetrationDepth.x) < 0)
				{
					position.x -= xDepth;
				}
				else
				{
					position.x += xDepth;
				}
			}
			else{
				if ((position.y - penetrationDepth.y) < 0)
				{
					position.y -= yDepth;
				}
				else
				{
					position.y += yDepth;
				}
			}

			collided = true;
		}

	}

	return collided;

}

bool Agent::collideWithLadder(std::vector<Box>& ladderBoxes){

	bool collided = false;

	for (auto& box : ladderBoxes){
		glm::vec4 penetrationDepth;
		if (collideWithBox(&box, penetrationDepth)){
			float xDepth = abs(penetrationDepth.z - penetrationDepth.x);
			//if (xDepth <= 4)
			//{
				collided = true;
			//}
		}
	}
	return collided;
}

bool Agent::collideBoxWithBoxes(Box& box, std::vector<Box>& otherBoxes){
		bool collided = false;

		for (auto& otherBox : otherBoxes){
			if (collideBoxWithBox(box, otherBox)){
				collided = true;
			}
		}
		return collided;
	
}

Box* Agent::collideWithLadderAndGetLadderBox(std::vector<Box>& ladderBoxes){

	for (auto& box : ladderBoxes){
		glm::vec4 penetrationDepth;
		if (collideWithBox(&box, penetrationDepth)){
			
			return &box;
		}
	}
	return nullptr;
}

glm::vec4 Agent::collideWithLadderAndGetCollisionDepth(std::vector<Box>& ladderBoxes){

	for (auto& box : ladderBoxes){
		glm::vec4 penetrationDepth;
		if (collideWithBox(&box, penetrationDepth)){
			return penetrationDepth;
		}
	}
	return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}


bool Agent::collideWithLevel(const std::vector<std::string>& levelData){
	std::vector<glm::vec2> collideTilePositions;

	glm::vec2 position = m_collisionBox.getPosition();

	const float agent_width = m_collisionBox.m_dimensions.x;
	const float agent_height = m_collisionBox.m_dimensions.y;

	//Check the four corners
	//First corner
	checkTilePosition(levelData,
		collideTilePositions,
		position.x,
		position.y);

	//Second corner
	checkTilePosition(levelData,
		collideTilePositions,
		position.x + agent_width,
		position.y);

	//Third corner
	checkTilePosition(levelData,
		collideTilePositions,
		position.x,
		position.y + agent_height);

	//Fourth corner
	checkTilePosition(levelData,
		collideTilePositions,
		position.x + agent_width,
		position.y + agent_height);

	if (collideTilePositions.size() == 0)
	{
		return false;
	}

	//Do the collision
	for (int i = 0; i < collideTilePositions.size(); i++)
	{
		collideWithTile(collideTilePositions[i], glm::vec2(TILE_WIDTH, TILE_WIDTH));
	}

	return true;
}


bool Agent::collideWithAgent(Agent* agent, glm::vec4& penetrationDepth){
	return collideWithBox(&agent->m_collisionBox, penetrationDepth);
}


bool Agent::collideWithBox(const Box* otherBox, glm::vec4& penetrationDepth){
	glm::vec2 position = m_collisionBox.getPosition();
	const float agent_width = m_collisionBox.m_dimensions.x;
	const float agent_height = m_collisionBox.m_dimensions.y;

	//// Is the Agent too far away in the X direction to check?
	//if (agent->getBox().getPosition().x < position.x - agent_width) {
	//	return false;
	//}
	//else if (agent->getBox().getPosition().x > position.x + agent_width) {
	//	return false;
	//}

	//// Is the Agent too far away in the Y direction to check?
	//if (agent->getBox().getPosition().y < position.y - agent_height)      {
	//	return false;
	//}
	//else if (agent->getBox().getPosition().y > position.y + agent_height) {
	//	return false;
	//}

	float myLeft = position.x;
	float myTop = position.y + agent_height;
	float myRight = position.x + agent_width;
	float myBottom = position.y;

	//Box to collide with
	float right = otherBox->m_position.x + otherBox->m_dimensions.x;
	float left = otherBox->m_position.x;
	float bottom = otherBox->m_position.y;
	float top = otherBox->m_position.y + otherBox->m_dimensions.y;

	//Check collision
	if (myLeft <  right && myRight > left &&
		myTop > bottom && myBottom < top)
	{
		//collided - now calculate penetration depth
		penetrationDepth.x = std::max(myLeft, left);
		penetrationDepth.y = std::max(myBottom, bottom);
		penetrationDepth.z = std::min(myRight, right);
		penetrationDepth.w = std::min(myTop, top);
		return true;
	}

	return false;
}

bool Agent::collideBoxWithBox(Box& box, Box& otherBox){

	float myLeft = box.m_position.x;
	float myTop = box.m_position.y + box.m_dimensions.y;
	float myRight = box.m_position.x + box.m_dimensions.x;
	float myBottom = box.m_position.y;

	//Box to collide with
	float right = otherBox.m_position.x + otherBox.m_dimensions.x;
	float left = otherBox.m_position.x;
	float bottom = otherBox.m_position.y;
	float top = otherBox.m_position.y + otherBox.m_dimensions.y;

	//Check collision
	return myLeft <  right && myRight > left &&
		myTop > bottom && myBottom < top;
}


bool Agent::applyDamage(float damage){
	m_health -= damage;

	if (m_health <= 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y){

	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH));

	//If we are outside the world, just return
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].length() || cornerPos.y < 0 || cornerPos.y >= levelData.size()){
		return;
	}

	if (levelData[cornerPos.y][cornerPos.x] != '.'){
		collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
		std::cout << "collide " << x << " and " << y << std::endl;
	}
}


// AABB collision (Axis Aligned Bounding Box)
bool Agent::collideWithTile(glm::vec2 tilePosition, glm::vec2 tileDimensions){

	glm::vec2& position = m_collisionBox.m_position;
	const float agent_width = m_collisionBox.m_dimensions.x;
	const float agent_height = m_collisionBox.m_dimensions.y;

	//Box to collide with
	float right = (tilePosition.x + tileDimensions.x);
	float left = tilePosition.x;
	float bottom = tilePosition.y;
	float top = tilePosition.y + tileDimensions.y;

	//Check collision
	if (position.x <  right && position.x + agent_width > left &&
		position.y + agent_height > bottom && position.y < top)
	{
		//collided - now calculate penetration depth
		glm::vec4 penetrationDepth;
		penetrationDepth.x = std::max(position.x, left);
		penetrationDepth.y = std::max(position.y, bottom);
		penetrationDepth.z = std::min(position.x + agent_width, right);
		penetrationDepth.w = std::min(position.y + agent_height, top);

		float xDepth = abs(penetrationDepth.z - penetrationDepth.x);
		float yDepth = abs(penetrationDepth.w - penetrationDepth.y);

		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
			if ((position.x - penetrationDepth.x) < 0)
			{
				std::cout << "x " << position.x << " + " << agent_width << std::endl;
				position.x -= xDepth;
			}
			else
			{
				position.x += xDepth;
			}
		}
		else{
			if ((position.y - penetrationDepth.y) < 0)
			{
				position.y -= yDepth;
			}
			else
			{
				position.y += yDepth;
			}
		}

		//position.x += penetrationDepth.z - penetrationDepth.x;
		//position.y += penetrationDepth.w - penetrationDepth.y;

		return true;
	}

	return false;
}

// AABB collision (Axis Aligned Bounding Box)
bool Agent::collideWithTile(glm::vec2 tilePosition){

	glm::vec2 position = m_collisionBox.m_position;

	//// Is the tile too far away
	//if (tilePosition.x < position.x - TILE_WIDTH) {
	//	return false;
	//}
	//else if (tilePosition.x > position.x + TILE_WIDTH) {
	//	return false;
	//}
	//if (tilePosition.y < position.y - TILE_WIDTH) {
	//	return false;
	//}
	//else if (tilePosition.y > position.y + TILE_WIDTH) {
	//	return false;
	//}

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distanceVec = centerPlayerPos - tilePosition;

	float xDepth = MIN_DISTANCE - abs(distanceVec.x);
	float yDepth = MIN_DISTANCE - abs(distanceVec.y);

	//If this is true, we are colliding with something
	if (xDepth > 0 && yDepth > 0){
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
			if (distanceVec.x < 0)
			{
				position.x -= xDepth;
			}
			else
			{
				position.x += xDepth;
			}
		}
		else{
			if (distanceVec.y < 0)
			{
				position.y -= yDepth;
			}
			else
			{
				position.y += yDepth;
			}
		}
		return true;
	}

	return false;
}



