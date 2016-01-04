#pragma once

#include <SDL\SDL.h>
#include <glm/glm.hpp>
//#include <GameEngine\GLTexture.h>
#include <GameEngine\SpriteBatch.h>

const float AGENT_WIDTH = 60;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent
{
public:

	Agent();
	virtual ~Agent();
	/*explicit
		Agent(float speed, glm::vec2 position, glm::vec2 direction)
		: m_speed(speed), m_direction(direction), m_position(position)
		{}
		*/

	void draw(GameEngine::SpriteBatch& spriteBatch);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime) = 0;

	glm::vec2 getPosition() const { return m_position; }

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	//Return true if health is zero or less --> agent died
	bool applyDamage(float damage);

	int getID() {
		return m_id;
	};

	virtual float getHealth() {
		return m_health;
	};


protected:

	void checkTilePosition(const std::vector<std::string>& levelData,
		std::vector<glm::vec2>& collideTilePosition,
		float x, float y);

	bool collideWithTile(glm::vec2 tilePosition);

	glm::vec2 m_position;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);

	GameEngine::ColorRGBA8 m_color;

	float m_speed;
	float m_health;

	GLuint m_textureID;


private:
	int m_id;




};

