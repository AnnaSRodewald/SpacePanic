#pragma once
#include "Ball.h"
#include <vector>

struct Cell
{
	std::vector<Ball*> balls;


};



class Grid
{
	friend class BallController;
public:
	Grid(int width, int height, int cellSize);
	~Grid();

	//Adds a ball an determines which cell it belongs to
	void addBall(Ball* ball);
	//Adds a ball to the specified cell
	void addBall(Ball* ball, Cell* cell);

	//Get Cell based on cell coordinates
	Cell* getCell(int x, int y);
	//get Cell based on window coordinates
	Cell* getCell(const glm::vec2& position);

	void removeBallFromCell(Ball* ball);

private:
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
};

