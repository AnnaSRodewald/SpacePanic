#pragma once

#include <glm/glm.hpp>
#include "GameEngine/SpriteBatch.h"

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void draw(GameEngine::SpriteBatch& spriteBatch);
	//Returns true when we are out of life
	bool update();


private:
	int m_lifeTime;
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;


};

