#include "Gun.h"
#include <random>
#include <ctime>
#include <SDL\SDL.h>
#include <glm\gtx\rotate_vector.hpp>

const float DEG_TO_RAD = M_PI / 180.0f;
const float RAD_TO_DEG = 180.0f / M_PI;

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage) : _name(name),
_fireRate(fireRate),
_bulletsPerShot(bulletsPerShot),
_spread(spread),
_bulletSpeed(bulletSpeed),
_bulletDamage(bulletDamage),
_frameCounter(0)
{
	//Empty
}


Gun::~Gun()
{
	//Empty
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets){
	_frameCounter++;
	if (_frameCounter >= _fireRate && isMouseDown)
	{
		fire(position, direction, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets){
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-_spread * DEG_TO_RAD, _spread * DEG_TO_RAD);

	for (int i = 0; i < _bulletsPerShot; i++)
	{
		bullets.emplace_back(position,
							glm::rotate(direction, randRotate(randomEngine)),
							_bulletDamage,
							_bulletSpeed);
	}
}