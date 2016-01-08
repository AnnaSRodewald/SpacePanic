#pragma once
#include "Box.h"
#include <GameEngine\SpriteBatch.h>
#include <GameEngine\GLTexture.h>
#include <GameEngine\InputManager.h>

class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, const glm::vec2 position, const glm::vec2 dimensions, std::string textureFilePath, GameEngine::ColorRGBA8 color);

	void draw(GameEngine::SpriteBatch& spriteBatch);

	void update(GameEngine::InputManager& inputManager);

	const Box& getBox() const { return m_collisionBox; }

private:
	Box m_collisionBox;

};

