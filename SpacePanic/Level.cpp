#include "Level.h"

#include <GameEngine\GameEngineErrors.h>
#include <fstream>
#include <iostream>

#include <GameEngine\ResourceManager.h>



Level::Level(const std::string fileName)
{


	std::fstream file;
	file.open(fileName);

	//Error checking
	if (file.fail())
	{
		GameEngine::fatalError("Failed to open " + fileName);
	}

	//Throw away the first sting in tap
	std::string tmp;
	file >> tmp >> m_numPlayer;

	std::getline(file, tmp); //Throw away the rest of the first line

	//Read the level data
	while (std::getline(file, tmp)){
		m_levelData.push_back(tmp);
	}

	m_spriteBatch.init();
	m_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	// Render all the tiles
	for (int y = 0; y < m_levelData.size(); y++)
	{
		for (int x = 0; x < m_levelData[y].size(); x++)
		{
			// Grab the tile
			char tile = m_levelData[y][x];

			// Get dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			// Process the tile
			switch (tile)
			{
			case 'B':
			case 'R':
				m_spriteBatch.draw(destRect,
					uvRect,
					GameEngine::ResourceManager::getTexture("Textures/red_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case 'G':
				m_spriteBatch.draw(destRect,
					uvRect,
					GameEngine::ResourceManager::getTexture("Textures/glass.png").id,
					0.0f,
					whiteColor);
				break;
			case 'L':
				m_spriteBatch.draw(destRect,
					uvRect,
					GameEngine::ResourceManager::getTexture("Textures/light_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case '@':
				m_levelData[y][x] = '.'; /// So we don't collide with the starting points (@) later in the game
				m_startPlayerPos.x = x * TILE_WIDTH;
				m_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				m_levelData[y][x] = '.'; /// So we don't collide with the starting points (Z) later in the game
				m_startMonsterPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %e)", tile, x, y);
				break;
			}

		}

	}
	m_spriteBatch.end();

}


Level::~Level()
{
}


void Level::draw(){
	m_spriteBatch.renderBatch();
}