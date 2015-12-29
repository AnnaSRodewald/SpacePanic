#include "Level.h"

#include <GameEngine\Errors.h>
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
	file >> tmp >> _numHumans;

	while (std::getline(file, tmp)){
		_levelData.push_back(tmp);
	}

	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	// Render all the tiles
	for (int y = 0; y < _levelData.size(); y++)
	{
		for (int x = 0; x < _levelData[y].size(); x++)
		{
			// Grab the tile
			char tile = _levelData[y][x];
			
			// Get dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			// Process the tile
			switch (tile)
			{
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect, 
					uvRect, 
					GameEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, 
					0.0f,
					whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect,
					uvRect,
					GameEngine::ResourceManager::getTexture("Textures/glass.png").id,
					0.0f,
					whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect,
					uvRect,
					GameEngine::ResourceManager::getTexture("Textures/light_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.'; /// So we don't collide with the starting points (@) later in the game
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.'; /// So we don't collide with the starting points (Z) later in the game
				_startZombiePositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d, %e)", tile, x, y);
				break;
			}

		}
	
	}
	_spriteBatch.end();

}


Level::~Level()
{
}


void Level::draw(){
	_spriteBatch.renderBatch();
}