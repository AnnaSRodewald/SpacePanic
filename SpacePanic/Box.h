#pragma once
//#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <GameEngine\Vertex.h>
#include <GameEngine\SpriteBatch.h>
#include <GameEngine\GLTexture.h>

class Box
{
public:
	friend class Agent;
	friend class Player;
	friend class Monster;

	Box();
	~Box();

	void init(
		const glm::vec2 position,
		const glm::vec2 dimensions,
		GameEngine::GLTexture* texture,
		GameEngine::ColorRGBA8 color,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	void draw(GameEngine::SpriteBatch& spriteBatch);


	const glm::vec2& getPosition() const { return m_position; }
	const glm::vec2& getDimensions() const { return m_dimensions; }
	const GameEngine::ColorRGBA8 getColor() const {
		return m_color;
	}

	const GLuint getTextureID() const { return m_textureID; }

	void collideWithBox(Box box);

private:
	glm::vec4 m_uvRect;
	GameEngine::ColorRGBA8 m_color;
	glm::vec2 m_position;
	glm::vec2 m_dimensions;
	GameEngine::GLTexture* m_texture;
	GLuint m_textureID;

};

