#pragma once
#include "Agent.h"
#include <glm/glm.hpp>

class Human :
	public Agent
{
public:
	Human();
	virtual ~Human();

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans, std::vector<Zombie*>& zombies);
	void getsHit(SDL_Event evnt);


};

