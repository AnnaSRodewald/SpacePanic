#pragma once

#include <SDL\SDL.h>
#include <glm/glm.hpp>
//#include <GameEngine\GLTexture.h>
#include <GameEngine\SpriteBatch.h>

const float AGENT_WIDTH = 60;

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

	void collideWithLevel(const std::vector<std::string>& levelData);



	virtual void getsHit(SDL_Event evnt) = 0;

	bool isAlive() {
		return _alive;
	};
	int getID() {
		return _id;
	};

	glm::vec2 getDirection(){
		return _direction;
	};

	

protected:
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
	GameEngine::Color _color;
	
	void checkTilePosition(const std::vector<std::string>& levelData, 
							std::vector<glm::vec2>& collideTilePosition, 
							float x, float y);

	void collideWithTile(glm::vec2 tilePosition);

	void kill() {
		_alive = false;
	};

private:
	bool _alive;
	int _id;




};

