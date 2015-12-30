#pragma once
#include "Agent.h"
#include <glm/glm.hpp>

const float DEG_TO_RAD = M_PI / 180.0f;
const float RAD_TO_DEG = 180.0f / M_PI;

class Human :
	public Agent
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 position);


	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans, std::vector<Zombie*>& zombies);
	
	glm::vec2 getDirection(){
		return _direction;
	};

private:
	glm::vec2 _direction;
	int _frames;
};

