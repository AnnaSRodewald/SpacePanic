#include "MainGame.h"


#include <iostream>
#include <string>

#include "Errors.h"




MainGame::MainGame(void) : 
	_screenWidth(1024),   //Initialzation list
	_screenHeight(768), 
	_time(0),
	_window(nullptr),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
	{

	}


MainGame::~MainGame(void)
	{
	}

void MainGame::run(){
	initSystems();

	//fix it later
	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/JimmyJump/PNG/CharacterRight_Standing.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/JimmyJump/PNG/CharacterRight_Standing.png");

	//for (int i = 0; i < 1000; i++){
	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "Textures/JimmyJump/PNG/CharacterRight_Standing.png");
	//	}


	//_playerTexture = ImageLoader::loadPNG("Textures/JimmyJump/PNG/CharacterRight_Standing.png");

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
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
	}


void MainGame::gameLoop(){

	//loop until the game state is EXIT
	while (_gameState != GameState::EXIT ){
		//Used for frame time measuring
		float startTicks = SDL_GetTicks();

		processInput();
		_time += 0.01;
		drawGame();
		calculateFPS();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10){
			std::cout << _fps << std::endl;
			frameCounter = 0;
			}

		float frameTicks = SDL_GetTicks() - startTicks;
		//Limit the FPS to the max FPS value 
		if (1000.0f / _maxFPS > frameTicks){
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
			}
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
				//std::cout << evnt.motion.x << " " << evnt.motion.y  << std::endl;
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
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	//Draw our sprites!
	for (int i = 0; i < _sprites.size(); i++)
		{
		_sprites[i]->draw();
		}

	glBindTexture(GL_TEXTURE_2D, 0);


	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen
	SDL_GL_SwapWindow(_window);
	}


void MainGame::calculateFPS(){
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();

	float currentTicks;
	currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	int count; 

	currentFrame++;
	if (currentFrame < NUM_SAMPLES){
		count = currentFrame;
		} else {
			count = NUM_SAMPLES;
		}

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++)
		{
		frameTimeAverage += frameTimes[i];
		}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0){
		_fps = 1000.0f / frameTimeAverage;
		} else {
			_fps =  60.0f;
		}

	}