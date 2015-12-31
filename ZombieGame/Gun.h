#pragma once

#include <string>
#include <glm\glm.hpp>
#include <vector>
#include "Bullet.h"


class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);

private:
	std::string _name;

	int _fireRate; ///< fire rate in terms of frames

	int _bulletsPerShot; ///< how many bullets are fired at a time

	float _spread; ///< accuracy

	float _bulletSpeed; 

	float _bulletDamage;

	float _frameCounter;

	void fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);

};

