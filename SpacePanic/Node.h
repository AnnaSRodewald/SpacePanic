#pragma once

#include <string>
#include <vector>
#include <glm\glm.hpp>


class Node
{
public:
	Node(glm::vec2 position);
	~Node();

	void addNeighbor(Node& node) {
		m_neighbors.push_back(node);
	}

	int costTo(Node& otherNode);

	std::vector<Node&>& getNeighbors() { return m_neighbors; }

	glm::vec2 getPosition() { return m_position; }
	float getX() { return m_position.x; }
	float getY() { return m_position.y; }

private:
	glm::vec2 m_position;
	std::vector<Node&> m_neighbors;
};

