#pragma once

#include <string>
#include <vector>

#include <GameEngine\SpriteBatch.h>


const float TILE_WIDTH = 64.0f;

class Level
{
public:
	//Load and build the level
	Level(const std::string fileName);
	~Level();

	void draw();

	//Getters
	const std::vector<std::string>& getLevelData(){ return m_levelData; }

	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	const std::vector<glm::vec2> getStartZombiePositions() const { return m_startZombiePositions; }

	int getNumHumans() const { return m_numHumans; }

	int getWidth() const {
		return m_levelData[0].size();
	}
	int getHeight() const {
		return m_levelData.size();
	}

private:
	std::vector<std::string> m_levelData;
	int m_numHumans;
	GameEngine::SpriteBatch m_spriteBatch;

	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_startZombiePositions;
};

