#include "Box.h"


Box::Box()
{
	//Empty
}


Box::~Box()
{
	//Empty
}


void Box::init(
	const glm::vec2 position,
	const glm::vec2 dimensions,
	GameEngine::GLTexture* texture,
	GameEngine::ColorRGBA8 color,
	glm::vec4 uvRect /* = glm::vec4 uvRect */){

	m_position = position;
	m_dimensions = dimensions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;
	m_textureID = texture->id;
}


void Box::draw(GameEngine::SpriteBatch& spriteBatch){
	glm::vec4 destRect;
	destRect.x = m_position.x;// -m_dimensions.x / 2.0f;
	destRect.y = m_position.y;// -m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;

	spriteBatch.draw(destRect, m_uvRect, m_textureID, 0.0f, m_color);
}