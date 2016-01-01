#include "Human.h"
#include <SDL\SDL.h>
#include <glm/glm.hpp>
#include <random>
#include <ctime>
#include <glm\gtx\rotate_vector.hpp>

const float DEG_TO_RAD = M_PI / 180.0f;
const float RAD_TO_DEG = 180.0f / M_PI;


//
//Human::Human(int speed, glm::vec2 position, glm::vec2 direction)
//{
//}

Human::Human() : m_frames(0) {
	m_health = 20;
}


Human::~Human()
{
}


void Human::init(float speed, glm::vec2 position){

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	m_color.r = 200;
	m_color.g = 0;
	m_color.b = 200;
	m_color.a = 255;

	m_speed = speed;
	m_position = position;
	//Get random direction
	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));

	//Make sure direction isn't zero
	if (m_direction.length() == 0){
		m_direction = glm::vec2(1.0f, 0.0f);
	}

	m_direction = glm::normalize(m_direction);
}


void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime){

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f * DEG_TO_RAD, 40.0f * DEG_TO_RAD);
	m_position += m_direction * m_speed * deltaTime; 

	//Randomly change direction every 20 frames
	if (m_frames == 20){
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
		m_frames = 0;
	}
	else
	{
		m_frames++;
	}

	if (collideWithLevel(levelData)){
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
	}
}