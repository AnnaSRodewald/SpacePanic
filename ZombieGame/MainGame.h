#pragma once

#include <GameEngine/Window.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>

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

    /// Member Variables
    GameEngine::Window _window; ///< The game window
    
    GameEngine::GLSLProgram _textureProgram; ///< The shader program

    GameEngine::InputManager _inputManager; ///< Handles input

    GameEngine::Camera2D _camera; ///< Main Camera

	GameEngine::SpriteBatch _agentSpriteBatch; ///< Draws all agents

	std::vector<Level*> _levels; ///< Vector of all levels

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

	float _fps;

	int _currentLevel; 

	Player* _player;
	std::vector<Human*> _humans; ///< Vector of all humans
	std::vector<Zombie*> _zombies; ///< Vector of all zombies
	std::vector<Bullet> _bullets; ///< Vector of bullets

	int _numHumansKilled; ///< Humans killed by player
	int _numZombiesKilled; ///< Zombies killed by player
};

