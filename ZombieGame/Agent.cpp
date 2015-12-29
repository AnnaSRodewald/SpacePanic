#include "Agent.h"
#include <GameEngine/ResourceManager.h>
#include "Level.h"

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


void Agent::collideWithLevel(const std::vector<std::string>& levelData){
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

	//Do the collision
	for (int i = 0; i < collideTilePositions.size(); i++)
	{
		collideWithTile(collideTilePositions[i]);
	}
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y){

	glm::vec2 cornerPos1 = glm::vec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH));

	if (levelData[cornerPos1.y][cornerPos1.x] != '.'){
		collideTilePositions.push_back(cornerPos1 * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}


//AAB collision
void Agent::collideWithTile(glm::vec2 tilePosition){

	const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 distanceVec = _position - tilePosition;

	float xDepth = MIN_DISTANCE - distanceVec.x;
	float yDepth = MIN_DISTANCE - distanceVec.y;

	float absXDepth = abs(xDepth);
	float absYDepth = abs(yDepth);
	//If this is true, we are colliding with something
	if (absXDepth > 0 || absYDepth > 0){
		if (xDepth < yDepth){
			_position += xDepth;
		}
		else{
			_position += yDepth;
		}
	}
}