#pragma once

#include <GameEngine\SpriteBatch.h>
#include <GameEngine\TileSheet.h>
#include <GameEngine\InputManager.h>
#include "Capsule.h"


enum class PlayerMoveState { STANDING, RUNNING, PUNCHING, IN_AIR };

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
	GameEngine::TileSheet m_texture;
	GameEngine::ColorRGBA8 m_color;
	bool m_onGround = false;
	int m_direction = 1;
	float m_animTime = 0.0f;
	PlayerMoveState m_moveState = PlayerMoveState::STANDING;
	bool m_isPunching = false;
};

