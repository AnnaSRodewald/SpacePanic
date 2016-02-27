#pragma once

#include <string>
#include <vector>

#include <GameEngine\SpriteBatch.h>
#include "Box.h"
#include "Node.h"


const float TILE_WIDTH = 64.0f;



class Level
{
public:
	//Load and build the level
	Level(const std::string fileName);
	~Level();

	void reload();

	void update();

	void draw();

	//Getters
	const std::vector<std::string>& getLevelData(){ return m_levelData; }

	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	const std::vector<glm::vec2> getStartMonsterPositions() const { return m_startMonsterPositions; }

	int getNumPlayer() const { return m_numPlayer; }

	int getWidth() const {
		return m_levelData[0].size();
	}
	int getHeight() const {
		return m_levelData.size();
	}

	std::vector<Box>& getLevelBoxes() { return m_boxes; }
	std::vector<Box>& getLadderBoxes() { return m_ladderBoxes; }
	std::vector<Box>& getHalfHoleBoxes() { return m_halfHoleBoxes; }
	std::vector<Box>& getHoleBoxes() { return m_holeBoxes; }

	glm::vec2 getCameraPosition() const { return m_cameraPosition; }

	std::vector<Node&> getLevelMap() const { return m_levelMap; }

private:

	void progressLevelData();

	void defineNeighbors(Node& node);
	bool isInBounds(glm::vec2 position);
	bool isInBounds(float x, float y);

	std::vector<std::string> m_levelData;
	int m_numPlayer;
	GameEngine::SpriteBatch m_spriteBatch;
	std::vector<Box> m_boxes;
	std::vector<Box> m_ladderBoxes;
	std::vector<Box> m_halfHoleBoxes;
	std::vector<Box> m_holeBoxes;
	std::vector<Node&> m_levelMap;

	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_startMonsterPositions;
	glm::vec2 m_cameraPosition = glm::vec2(0.0f, 0.0f);
};

