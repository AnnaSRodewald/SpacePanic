#include "Agent.h"
#include <GameEngine/ResourceManager.h>
#include "Level.h"
#include <algorithm>


Agent::Agent(){


}


Agent::~Agent(){

}

void Agent::draw(GameEngine::SpriteBatch& spriteBatch){

	static int textureID = GameEngine::ResourceManager::getTexture("Textures/circle.png").id;

	glm::vec4 destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}


bool Agent::collideWithLevel(const std::vector<std::string>& levelData){
	std::vector<glm::vec2> collideTilePositions;

	//Check the four corners
	//First corner
	checkTilePosition(levelData,
		collideTilePositions,
		_position.x,
		_position.y);

	//Second corner
	checkTilePosition(levelData,
		collideTilePositions,
		_position.x + AGENT_WIDTH,
		_position.y);

	//Third corner
	checkTilePosition(levelData,
		collideTilePositions,
		_position.x,
		_position.y + AGENT_WIDTH);

	//Fourth corner
	checkTilePosition(levelData,
		collideTilePositions,
		_position.x + AGENT_WIDTH,
		_position.y + AGENT_WIDTH);

	if (collideTilePositions.size() == 0)
	{
		return false;
	}

	//Do the collision
	for (int i = 0; i < collideTilePositions.size(); i++)
	{
		collideWithTile(collideTilePositions[i]);
	}

	return true;
}

//circular collision
bool Agent::collideWithAgent(Agent* agent){

	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		_position += collisionDepthVec / 2.0f;
		agent->_position -= collisionDepthVec / 2.0f;
		return true;
	}

	return false;
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
	}
}


//AAB collision
void Agent::collideWithTile(glm::vec2 tilePosition){

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distanceVec = centerPlayerPos - tilePosition;

	float xDepth = MIN_DISTANCE - abs(distanceVec.x);
	float yDepth = MIN_DISTANCE - abs(distanceVec.y);

	//If this is true, we are colliding with something
	if (xDepth > 0 || yDepth > 0){
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
			if (distanceVec.x < 0)
			{
				_position.x -= xDepth;
			}
			else
			{
				_position.x += xDepth;
			}
		}
		else{
			if (distanceVec.y < 0)
			{
				_position.y -= yDepth;
			}
			else
			{
				_position.y += yDepth;
			}
		}
	}
}