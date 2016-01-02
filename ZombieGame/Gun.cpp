#include "Gun.h"
#include <random>
#include <ctime>
#include <SDL\SDL.h>
#include <glm\gtx\rotate_vector.hpp>

const float DEG_TO_RAD = M_PI / 180.0f;
const float RAD_TO_DEG = 180.0f / M_PI;

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage, GameEngine::SoundEffect fireEffect) : m_name(name),
m_fireRate(fireRate),
m_bulletsPerShot(bulletsPerShot),
m_spread(spread),
m_bulletSpeed(bulletSpeed),
m_bulletDamage(bulletDamage),
m_frameCounter(0),
m_fireEffect(fireEffect)
{
	//Empty
}


Gun::~Gun()
{
	//Empty
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime){
	m_frameCounter+= 1.0f * deltaTime;
	if (m_frameCounter >= m_fireRate && isMouseDown)
	{
		fire(position, direction, bullets);
		m_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets){
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-m_spread * DEG_TO_RAD, m_spread * DEG_TO_RAD);

	m_fireEffect.play();

	for (int i = 0; i < m_bulletsPerShot; i++)
	{
		bullets.emplace_back(position,
							glm::rotate(direction, randRotate(randomEngine)),
							m_bulletDamage,
							m_bulletSpeed);
	}
}