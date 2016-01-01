#pragma once

#include <Windows.h>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <vector>

#include <GameEngine/GameEngine.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/Sprite.h>
#include <GameEngine/Window.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/Timing.h>

#include "Bullet.h"


enum class GameState{PLAY, EXIT};


class MainGame
	{
	public:
		MainGame(void);
		~MainGame(void);

		void run();

	
	private:
		void initSystems();
		void initShaders();
		void gameLoop();
		void processInput();
		void drawGame();

		GameEngine::Window m_window;
		int m_screenWidth;
		int m_screenHeight;

		GameState m_gameState;

		GameEngine::GLSLProgram m_colorProgram;
		GameEngine::GLTexture m_playerTexture;

		GameEngine::Camera2D m_camera;

		GameEngine::SpriteBatch m_spriteBatch;

		GameEngine::InputManager m_inputManager;

		GameEngine::FpsLimiter m_fpsLimiter;

		std::vector<Bullet> m_bullets;

		float m_fps;
		float m_maxFPS;

		float m_time;
	};

