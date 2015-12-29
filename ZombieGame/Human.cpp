#include "Human.h"
#include <SDL\SDL.h>
#include <glm/glm.hpp>

//
//Human::Human(int speed, glm::vec2 position, glm::vec2 direction)
//{
//}

Human::Human(){

}


Human::~Human()
{
}


void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies){
	//_position += _direction * _speed;
	/*if (isAlive())
	{
		return true;
	}
	return false;*/
}

void Human::getsHit(SDL_Event evnt){

}