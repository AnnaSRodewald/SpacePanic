#include "Bullet.h"
#include <GameEngine/GLTexture.h>
#include <GameEngine/ResourceManager.h>
#include "Level.h"
#include "Agent.h"


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed) : _position(pos),
_direction(dir),
_damage(damage),
_speed(speed),
_lifeTime(100000) {

}


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed, int lifeTime){
	_position = pos;
	_direction = dir;
	_damage = damage;
	_speed = speed;
	_lifeTime = lifeTime;
}


Bullet::~Bullet()
{
}


void Bullet::draw(GameEngine::SpriteBatch& spriteBatch){

	glm::vec4 destRect = glm::vec4(_position.x - BULLET_RADIUS, _position.y - BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/circle.png");

	GameEngine::Color color;
	color.r = 75;
	color.g = 75;
	color.b = 75;
	color.a = 255;

	spriteBatch.draw(destRect, uvRect, texture.id, 0.0f, color);

}


bool Bullet::update(const std::vector<std::string>& levelData){
	_position += _direction * _speed;
	_lifeTime--;
	if (collideWithWorld(levelData) || _lifeTime == 0)
	{
		return true;
	}
	return false;
}

bool Bullet::collideWithAgent(Agent* agent){
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0){
		//Bullet collided with the agent
		return true;
	}
	else{
		return false;
	}
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData){
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	//If we are outside the world, just return
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length()
		|| gridPosition.y < 0 || gridPosition.y >= levelData.size()){
		return true;
	}

	return (levelData[gridPosition.y][gridPosition.x] != '.');
}