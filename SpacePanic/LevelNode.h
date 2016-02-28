#pragma once
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include <tuple>



class LevelNode
{
public:
	LevelNode(glm::vec2 position);
	LevelNode();
	~LevelNode();

	void addNeighbor(LevelNode& LevelNode) {
		m_neighbors.push_back(LevelNode);
	}

	int costTo(LevelNode& otherLevelNode);

	int compareTo(LevelNode node);

	std::vector<LevelNode>& getNeighbors() { return m_neighbors; }

	glm::vec2 getPosition() const { return m_position; }

	std::tuple<int, int> getTuple() const { return m_tuple; }

	float getX() { return m_position.x; }
	float getY() { return m_position.y; }

	bool operator== (const LevelNode& node) const;
	bool operator< (const LevelNode& node) const;
	bool operator<= (const LevelNode& node) const;
	bool operator> (const LevelNode& node) const;
	bool operator>= (const LevelNode& node) const;


private:
	glm::vec2 m_position;
	std::tuple<int, int> m_tuple;
	std::vector<LevelNode> m_neighbors;
};

