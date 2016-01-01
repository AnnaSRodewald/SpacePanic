#pragma once

#include <glm/glm.hpp>
#include "GameEngine/SpriteBatch.h"

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed);
	Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed, int lifeTime);
	~Bullet();

	void draw(GameEngine::SpriteBatch& spriteBatch);
	//Returns true when we are out of life or hit something --> delete bullet
	bool update(const std::vector<std::string>& levelData, float deltaTime);

	bool collideWithAgent(Agent* agent);

	float getDamage() const { return m_damage; }

private:
	bool collideWithWorld(const std::vector<std::string>& levelData);

	float m_damage; 
	glm::vec2 m_direction;
	glm::vec2 m_position;
	float m_speed;

	int m_lifeTime;


};

