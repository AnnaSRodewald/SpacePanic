#include "Agent.h"
#include <GameEngine/ResourceManager.h>
#include "Level.h"
#include <algorithm>


Agent::Agent(){


}


Agent::~Agent(){

}

void Agent::draw(GameEngine::SpriteBatch& spriteBatch){

	glm::vec4 destRect(m_position.x, m_position.y, AGENT_WIDTH, AGENT_WIDTH);

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color, m_direction);
}


bool Agent::collideWithLevel(const std::vector<std::string>& levelData){
	std::vector<glm::vec2> collideTilePositions;

	//Check the four corners
	//First corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x,
		m_position.y);

	//Second corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x + AGENT_WIDTH,
		m_position.y);

	//Third corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x,
		m_position.y + AGENT_WIDTH);

	//Fourth corner
	checkTilePosition(levelData,
		collideTilePositions,
		m_position.x + AGENT_WIDTH,
		m_position.y + AGENT_WIDTH);

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

	// Is the Agent too far away in the X direction to check?
	if (agent->m_position.x < m_position.x - AGENT_WIDTH) { 
		return false; 
	}
	else if (agent->m_position.x > m_position.x + AGENT_WIDTH) { 
		return false; 
	}

	// Is the Agent too far away in the Y direction to check?
	if (agent->m_position.y < m_position.y - AGENT_WIDTH)      { 
		return false; 
	}
	else if (agent->m_position.y > m_position.y + AGENT_WIDTH) { 
		return false; 
	}

	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	glm::vec2 centerPosA = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		m_position += collisionDepthVec / 2.0f;
		agent->m_position -= collisionDepthVec / 2.0f;
		return true;
	}

	return false;
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
	}
}


// AABB collision (Axis Aligned Bounding Box)
bool Agent::collideWithTile(glm::vec2 tilePosition){

	// Is the tile too far away
	if (tilePosition.x < m_position.x - TILE_WIDTH) {
		return false;
	}
	else if (tilePosition.x > m_position.x + TILE_WIDTH) {
		return false;
	}
	if (tilePosition.y < m_position.y - TILE_WIDTH) {
		return false;
	}
	else if (tilePosition.y > m_position.y + TILE_WIDTH) {
		return false;
	}

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distanceVec = centerPlayerPos - tilePosition;

	float xDepth = MIN_DISTANCE - abs(distanceVec.x);
	float yDepth = MIN_DISTANCE - abs(distanceVec.y);

	//If this is true, we are colliding with something
	if (xDepth > 0 && yDepth > 0){
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
			if (distanceVec.x < 0)
			{
				m_position.x -= xDepth;
			}
			else
			{
				m_position.x += xDepth;
			}
		}
		else{
			if (distanceVec.y < 0)
			{
				m_position.y -= yDepth;
			}
			else
			{
				m_position.y += yDepth;
			}
		}
	}
}