#include "LevelNode.h"


LevelNode::LevelNode(glm::vec2 position)
{
	m_position = position;
	m_tuple = std::tie(position.x, position.y);
}

LevelNode::LevelNode(){
	m_position = glm::vec2(0.0f, 0.0f);
	m_tuple = std::tie(m_position.x, m_position.y);
}

LevelNode::~LevelNode()
{
}


int LevelNode::costTo(LevelNode& otherLevelNode){
	for (auto neighbor : m_neighbors)
	{
		if (neighbor.getPosition() == otherLevelNode.getPosition())
		{
			return 1;
		}
	}

	//infinite if not neigbor
	return 9999999;
}

int LevelNode::compareTo(LevelNode node){
	if (m_position == node.getPosition())
	{
		return 0;
	}
	else if (m_position.x >= node.getPosition().x || m_position.y >= node.getPosition().y)
	{
		return 1;
	}
	else
	{
		return -1;
	}

}


bool LevelNode::operator== (const LevelNode& node) const{
	return m_tuple == node.getTuple();
}
bool LevelNode::operator< (const LevelNode& node) const{
	return m_tuple < node.getTuple();
}
bool LevelNode::operator<= (const LevelNode& node) const{
	return m_tuple <= node.getTuple();
}
bool LevelNode::operator>(const LevelNode& node) const{
	return m_tuple > node.getTuple();
}
bool LevelNode::operator>= (const LevelNode& node) const{
	return m_tuple >= node.getTuple();
}