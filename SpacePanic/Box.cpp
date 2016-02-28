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
	m_uvRect = uvRect;
	m_textureID = texture->id;

	m_texture.init(*texture, glm::ivec2(10, 2));
}

void Box::init(
	const glm::vec2 position,
	const glm::vec2 dimensions,
	GameEngine::GLTexture* texture,
	glm::ivec2 tileDimns,
	GameEngine::ColorRGBA8 color,
	glm::vec4 uvRect /* = glm::vec4 uvRect */){
	m_position = position;
	m_dimensions = dimensions;
	m_color = color;
	m_uvRect = uvRect;
	m_textureID = texture->id;

	m_texture.init(*texture, tileDimns);
}

void Box::draw(GameEngine::SpriteBatch& spriteBatch){
	glm::vec4 destRect;
	if (m_drawDims == glm::vec2(0.0f, 0.0f))
	{
		destRect.x = m_position.x;// -m_dimensions.x / 2.0f;
		destRect.y = m_position.y;// -m_dimensions.y / 2.0f;
		destRect.z = m_dimensions.x;
		destRect.w = m_dimensions.y;
	}
	else {
		destRect.x = m_position.x - m_drawDims.x / 4.0f;
		destRect.y = m_position.y;// - m_dimensions.y / 2.0f;
		destRect.z = m_drawDims.x;
		destRect.w = m_drawDims.y;
	}

	spriteBatch.draw(destRect, m_uvRect, m_textureID, 0.0f, m_color);
}

void Box::draw(GameEngine::SpriteBatch& spriteBatch, glm::vec4 destRect){
	spriteBatch.draw(destRect, m_uvRect, m_textureID, 0.0f, m_color);
}