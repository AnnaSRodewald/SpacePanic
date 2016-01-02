#pragma once

#include <GameEngine/Window.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/SpriteFont.h>
#include <GameEngine/AudioEngine.h>
#include <GameEngine/ParticleEngine2D.h>
#include <GameEngine/ParticleBatch2D.h>

#include "Level.h"
#include "Player.h"

class Zombie;

enum class GameState{
	PLAY,
	EXIT
};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

	/// Initializes the level and sets up everything
	void initLevel();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

	///Updates all agents
	void updateAgents(float deltaTime);

	///Update all bullets
	void updateBullets(float deltaTime);

	///This checks the victory conditions
	void checkVictory();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

	/// Draws the HUD
	void drawHUD();

	//Adds blood to the particle engine
	void addBlood(glm::vec2& position, int numParticles);

    /// Member Variables
    GameEngine::Window m_window; ///< The game window
    
    GameEngine::GLSLProgram m_textureProgram; ///< The shader program

    GameEngine::InputManager m_inputManager; ///< Handles input

    GameEngine::Camera2D m_camera; ///< Main Camera
	GameEngine::Camera2D m_hudCamera; ///< HUD Camera

	GameEngine::SpriteBatch m_agentSpriteBatch; ///< Draws all agents
	GameEngine::SpriteBatch m_hudSpriteBatch;

	GameEngine::SpriteFont* m_spriteFont;

	GameEngine::AudioEngine m_audioEngine;

	GameEngine::ParticleEngine2D m_particleEngine;
	GameEngine::ParticleBatch2D* m_bloodParticleBatch;

	std::vector<Level*> m_levels; ///< Vector of all levels

	int m_screenWidth = 1024;
	int m_screenHeight = 768;

	GameState m_gameState;

	float m_fps;

	int m_currentLevel; 

	Player* m_player;
	std::vector<Human*> m_humans; ///< Vector of all humans
	std::vector<Zombie*> m_zombies; ///< Vector of all zombies
	std::vector<Bullet> m_bullets; ///< Vector of bullets

	int m_numHumansKilled; ///< Humans killed by player
	int m_numZombiesKilled; ///< Zombies killed by player

};

