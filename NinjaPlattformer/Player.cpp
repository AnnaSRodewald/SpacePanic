#include "Player.h"
#include <GameEngine\ResourceManager.h>
#include <SDL\SDL.h>
#include <iostream>



void Player::init(b2World* world, const glm::vec2 position, const glm::vec2 drawDims, const glm::vec2 collisionDims, std::string textureFilePath, GameEngine::ColorRGBA8 color){
	//Load the texture
	m_texture = GameEngine::ResourceManager::getTexture(textureFilePath);
	m_color = color;
	m_drawDims = drawDims;

	m_capsule.init(world, position, collisionDims, 1.0f, 0.1f, true);
	//m_capsule.init(world, position, dimensions, texture, color, true, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f));
}

void Player::draw(GameEngine::SpriteBatch& spriteBatch){
	glm::vec4 destRect;
	b2Body* body = m_capsule.getBody();
	destRect.x = body->GetPosition().x - m_drawDims.x / 2.0f;
	destRect.y = body->GetPosition().y - m_capsule.getDimensions().y / 2.0f;
	destRect.z = m_drawDims.x;
	destRect.w = m_drawDims.y;

	spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f), m_texture.id, 0.0f, m_color, body->GetAngle());

}

void Player::drawDebug(GameEngine::DebugRenderer& debugRenderer){
	m_capsule.drawDebug(debugRenderer);
}

void Player::update(GameEngine::InputManager& inputManager){
	b2Body* body = m_capsule.getBody();
	if (inputManager.isKeyDown(SDLK_a))
	{
		body->ApplyForceToCenter(b2Vec2(-100.0, 0.0), true);
	}
	else if (inputManager.isKeyDown(SDLK_d))
	{
		body->ApplyForceToCenter(b2Vec2(100.0, 0.0), true);
	}
	else{
		//Apply damping
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95, body->GetLinearVelocity().y));
	}

	float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x < (-MAX_SPEED))
	{
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED)
	{
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}

	///Detect if player is standing on the ground
	//Loop through all the contact points
	for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next)
	{
		b2Contact* c = ce->contact;
		if (c->IsTouching())
		{
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);

			//Check if the points are below
			bool below = false;
			for (size_t i = 0; i < b2_maxManifoldPoints; i++)
			{
				if (manifold.points[i
				].y < body->GetPosition().y - m_capsule.getDimensions().y / 2.0f + 0.01f)
				{
					below = true;
					break;
				}
			}
			
			if (below)
			{
				//We can jump
				if (inputManager.isKeyPressed(SDLK_w))
				{
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}