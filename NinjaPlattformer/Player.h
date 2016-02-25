#pragma once

#include <GameEngine\SpriteBatch.h>
#include <GameEngine\GLTexture.h>
#include <GameEngine\InputManager.h>
#include "Capsule.h"

class Player
{
public:


	void init(b2World* world, const glm::vec2 position, const glm::vec2 drawDims, const glm::vec2 collisionDims, std::string textureFilePath, GameEngine::ColorRGBA8 color);

	void draw(GameEngine::SpriteBatch& spriteBatch);

	void drawDebug(GameEngine::DebugRenderer& debugRenderer);

	void update(GameEngine::InputManager& inputManager);

	const Capsule& getCapsule() const { return m_capsule; }

private:
	glm::vec2 m_drawDims;
	Capsule m_capsule;
	GameEngine::GLTexture m_texture;
	GameEngine::ColorRGBA8 m_color;
};

