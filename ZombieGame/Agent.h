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
		: _speed(speed), _direction(direction), _position(position)
	{}
*/

	void draw(GameEngine::SpriteBatch& spriteBatch);

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans, std::vector<Zombie*>& zombies) = 0;

	glm::vec2 getPosition() const { return _position; }

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	//Return true if health is zero or less --> agent died
	bool applyDamage(float damage);

	int getID() {
		return _id;
	};


	

protected:
	float _speed;
	glm::vec2 _position;
	GameEngine::Color _color;
	
	void checkTilePosition(const std::vector<std::string>& levelData, 
							std::vector<glm::vec2>& collideTilePosition, 
							float x, float y);

	void collideWithTile(glm::vec2 tilePosition);

	float _health;


private:
	int _id;




};

