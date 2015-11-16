#include "MainGame.h"


#include <iostream>
#include <string>

#include "Errors.h"


MainGame::MainGame(void) : 
		_screenWidth(1024),   //Initialzation list
		_screenHeight(768), 
		_time(0),
		_window(nullptr),
		_gameState(GameState::PLAY)  
	{

	}


MainGame::~MainGame(void)
	{
	}

void MainGame::run(){
	initSystems();

	//fix it later
	_sprite.init(-1.0f, -1.0f, 2.0f, 2.0f);

	gameLoop();
	}



void MainGame::initSystems(){
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Set up our OpenGl Context
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if ( _window == nullptr){
			fatalError("SDL Window could not be created!");
		}

	//Set up glew (optional but recommended)
	//glewExperimental = true; // may/should not need it
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr){
		fatalError("SDL_GL context could not be created!");
		}
		
	GLenum error = glewInit();

	if (error != GLEW_OK ){
		fatalError("Could not initialize glew!");
		}

	//Tell SDL that we want a doible buffered window so we dont get any flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // one window is cleared, on the other the stuff is drawn -->prevents the window from flickering --> smoother :)

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	initShaders();
	}


void MainGame::initShaders(){
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.linkShaders();
	}


void MainGame::gameLoop(){

	while (_gameState != GameState::EXIT ){
		processInput();
		_time += 0.01;
		drawGame();
		}

	}


void MainGame::processInput(){
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt) == true){
		switch (evnt.type){
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				std::cout << evnt.motion.x << " " << evnt.motion.y  << std::endl;
				break;
			}
		}
	}

void MainGame::drawGame() {

	//Set the base depth to 1.0
	glClearDepth(1.0);
	//clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // same as callinglClear fot both variables
	
	_colorProgram.use();

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	//Draw our sprite!
	_sprite.draw();

	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen
	SDL_GL_SwapWindow(_window);
	}
	
