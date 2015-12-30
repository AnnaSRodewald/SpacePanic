#pragma once

#include <string>
#include <vector>

#include <GameEngine\SpriteBatch.h>


const int TILE_WIDTH = 64;

class Level
{
public:
	//Load and build the level
	Level(const std::string fileName);
	~Level();

	void draw();

	//Getters
	const std::vector<std::string>& getLevelData(){ return _levelData; }

	glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
	const std::vector<glm::vec2> getStartZombiePositions() const { return _startZombiePositions; }

	int getNumHumans() const { return _numHumans; }

	int getWidth() const {
		return _levelData[0].size();
	}
	int getHeight() const {
		return _levelData.size();
	}

private:
	std::vector<std::string> _levelData;
	int _numHumans;
	GameEngine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _startZombiePositions;
};

