#include "Bullet.h"
#include <GameEngine/GLTexture.h>
#include <GameEngine/ResourceManager.h>
#include "Level.h"
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed) : m_position(pos),
m_direction(dir),
m_damage(damage),
m_speed(speed),
m_lifeTime(100000) {

}


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed, int lifeTime){
	m_position = pos;
	m_direction = dir;
	m_damage = damage;
	m_speed = speed;
	m_lifeTime = lifeTime;
}


Bullet::~Bullet()
{
}


void Bullet::draw(GameEngine::SpriteBatch& spriteBatch){

	glm::vec4 destRect = glm::vec4(m_position.x - BULLET_RADIUS, m_position.y - BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	static GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/circle.png");

	GameEngine::ColorRGBA8 color;
	color.r = 75;
	color.g = 75;
	color.b = 75;
	color.a = 255;

	spriteBatch.draw(destRect, uvRect, texture.id, 0.0f, color);

}


bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime){
	m_position += m_direction * m_speed * deltaTime;
	m_lifeTime--;
	if (collideWithWorld(levelData))
	{
		return true;
	}
	return false;
}

bool Bullet::collideWithAgent(Agent* agent){
	const float MIN_DISTANCE = AGENT_RADIUS + float(BULLET_RADIUS);

	glm::vec2 centerPosA = m_position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth >= 0){
		//Bullet collided with the agent
		return true;
	}
	else{
		return false;
	}
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData){
	glm::ivec2 gridPosition;
	gridPosition.x = floor(m_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(m_position.y / (float)TILE_WIDTH);

	//If we are outside the world, just return
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length()
		|| gridPosition.y < 0 || gridPosition.y >= levelData.size()){
		return true;
	}

	return (levelData[gridPosition.y][gridPosition.x] != '.');
}