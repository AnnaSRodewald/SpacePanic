#include "Node.h"


Node::Node(glm::vec2 position)
{
	m_position = position;
}


Node::~Node()
{
}


int Node::costTo(Node& otherNode){
	if (std::find(m_neighbors.begin(), m_neighbors.end(), otherNode) != m_neighbors.end())
	{
		return 1;
	}

	//infinite if not neigbor
	return 9999999999;
}