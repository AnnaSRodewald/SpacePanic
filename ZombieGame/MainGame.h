#pragma once

#include <GameEngine/Window.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>

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

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

    /// Member Variables
    GameEngine::Window _window; ///< The game window
    
    GameEngine::GLSLProgram _textureProgram; ///< The shader program

    GameEngine::InputManager _inputManager; ///< Handles input

    GameEngine::Camera2D _camera; ///< Main Camera
};

