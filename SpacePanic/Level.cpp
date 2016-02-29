#include "Level.h"
#include <GameEngine\GameEngineErrors.h>
#include <fstream>


#include <GameEngine\ResourceManager.h>
#include "PathFinder.h"



Level::Level(const std::string fileName) : m_map(1, 1)
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

	SquareGrid map(m_levelData[0].size(), m_levelData.size());
	m_map = map;
	progressLevelData();

}

Level::~Level()
{
}

void Level::reload(){
	clear();
	progressLevelData();
}

void Level::update(){
	m_spriteBatch.begin();
	for (auto box : m_boxes)
	{
		box.draw(m_spriteBatch);
	}
	for (auto box : m_ladderBoxes)
	{
		box.draw(m_spriteBatch);
	}
	for (auto box : m_halfHoleBoxes)
	{
		box.draw(m_spriteBatch);
	}
	for (auto box : m_holeBoxes)
	{
		box.draw(m_spriteBatch);
	}
	m_spriteBatch.end();
}

void Level::draw(){
	m_spriteBatch.renderBatch();
}

void Level::clear(){
	m_boxes.clear();
	m_holeBoxes.clear();
	m_halfHoleBoxes.clear();
	m_ladderBoxes.clear();
}

void Level::progressLevelData(){

	m_spriteBatch.init();
	m_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	GameEngine::ColorRGBA8 blackColor(0,0,0,255);

	// Render all the tiles
	for (int y = 0; y < m_levelData.size(); y++)
	{
		for (int x = 0; x < m_levelData[y].size(); x++)
		{
			// Grab the tile
			char tile = m_levelData[y][x];

			// Get dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			Box newBox;

			// Process the tile
			switch (tile)
			{
			case 'B':
				break;
			case 'R':
				newBox.init(glm::vec2(x * TILE_WIDTH, y * TILE_WIDTH), glm::vec2(TILE_WIDTH, TILE_WIDTH), &GameEngine::ResourceManager::getTexture("Textures/red_bricks.png"), GameEngine::ColorRGBA8(0, 255, 255, 255), uvRect);
				m_boxes.push_back(newBox);

				m_map.walls.emplace(std::tie(x, y));

				//Draw the box
				newBox.draw(m_spriteBatch);
				break;
			case 'G':
				//ground
				newBox.init(glm::vec2(x * TILE_WIDTH, y * TILE_WIDTH), glm::vec2(TILE_WIDTH, TILE_WIDTH), &GameEngine::ResourceManager::getTexture("Textures/glass.png"), blackColor, uvRect);
				m_boxes.push_back(newBox);

				m_map.walls.emplace(std::tie(x, y));

				//Draw the box
				newBox.draw(m_spriteBatch);
				break;
			case 'L':
				newBox.init(glm::vec2(x * TILE_WIDTH, y * TILE_WIDTH), glm::vec2(TILE_WIDTH, TILE_WIDTH), &GameEngine::ResourceManager::getTexture("Textures/light_bricks.png"), GameEngine::ColorRGBA8(128, 0, 0, 255), uvRect);
				m_ladderBoxes.push_back(newBox);

				//Draw the box
				newBox.draw(m_spriteBatch);

				break;
			case '@':
				m_levelData[y][x] = '.'; /// So we don't collide with the starting points (@) later in the game
				m_startPlayerPos.x = x * TILE_WIDTH;
				m_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'M':
				m_levelData[y][x] = '.'; /// So we don't collide with the starting points (Z) later in the game
				m_startMonsterPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			case 'W': //wall
				newBox.init(glm::vec2(x * TILE_WIDTH, y * TILE_WIDTH), glm::vec2(TILE_WIDTH, TILE_WIDTH), &GameEngine::ResourceManager::getTexture("Textures/glass.png"), GameEngine::ColorRGBA8(0, 0, 0, 0), uvRect);
				m_boxes.push_back(newBox);

				m_map.walls.emplace(std::tie(x, y));

				//Draw the box
				newBox.draw(m_spriteBatch);
				break;
			case 'C':
				m_levelData[y][x] = '.';
				m_cameraPosition.x = x * TILE_WIDTH;
				m_cameraPosition.y = y * TILE_WIDTH;
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %e)", tile, x, y);
				break;
			}

			if (tile != 'R' && tile != 'G' &&  tile != 'W')
				{
					if (0 <= y - 1 < m_levelData.size() &&
						(m_levelData[y][x] != 'L' && m_levelData[y-1][x] != 'R' && m_levelData[y-1][x] != 'G'))
					{
						m_map.walls.emplace(std::tie(x, y));
					}
				}

		}

	}
	m_spriteBatch.end();

}
