#pragma once
#include <Box2D\Box2D.h>
#include <GameEngine\IGameScreen.h>
#include <GameEngine\SpriteBatch.h>
#include <GameEngine\GLSLProgram.h>
#include <GameEngine\Camera2D.h>
#include <GameEngine\GLTexture.h>
#include <GameEngine\Window.h>
#include <GameEngine\DebugRenderer.h>

#include "Box.h"
#include "Player.h"

class GameplayScreen : public GameEngine::IGameScreen
{
public:
	GameplayScreen(GameEngine::Window* window);
	~GameplayScreen();

	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;

private:
	void checkInput();
	void initShaders();

	GameEngine::SpriteBatch m_spriteBatch;
	GameEngine::GLSLProgram m_textureProgram;
	GameEngine::Camera2D m_camera;
	GameEngine::GLTexture m_texture;
	GameEngine::Window* m_window;
	GameEngine::DebugRenderer m_debugRenderer;

	bool m_renderDebug = true;

	Player m_player;

	std::unique_ptr<b2World> m_world;

	std::vector<Box> m_boxes;


	
};

