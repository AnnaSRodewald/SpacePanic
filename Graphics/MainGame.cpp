#include "MainGame.h"


#include <iostream>
#include <string>

#include <GameEngine/Errors.h>
#include <GameEngine/ResourceManager.h>




MainGame::MainGame(void) : 
	_screenWidth(1024),   //Initialzation list
	_screenHeight(768), 
	_time(0),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
	{
	_camera.init(_screenWidth, _screenHeight);
	}


MainGame::~MainGame(void)
	{
	}

void MainGame::run(){
	initSystems();

	gameLoop();
	}



void MainGame::initSystems(){
	GameEngine::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();

	_spriteBatch.init();

	_fpsLimiter.init(_maxFPS);
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
		//	float startTicks = SDL_GetTicks();
		_fpsLimiter.beginFrame();

		processInput();
		_time += 0.01;

		_camera.update();

		drawGame();

		_fps = _fpsLimiter.end();	

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10){
			std::cout << _fps << std::endl;
			frameCounter = 0;
			}
		}

	}


void MainGame::processInput(){
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while(SDL_PollEvent(&evnt) == true){
		switch (evnt.type){
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				//std::cout << evnt.motion.x << " " << evnt.motion.y  << std::endl;
				break;
			case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			}
		}

	if (_inputManager.isKeyPressed(SDLK_w))
		{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
		}
	if (_inputManager.isKeyPressed(SDLK_s)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
		}
	if (_inputManager.isKeyPressed(SDLK_a)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
		}
	if (_inputManager.isKeyPressed(SDLK_d)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
		}
	if (_inputManager.isKeyPressed(SDLK_q)){
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
		}
	if (_inputManager.isKeyPressed(SDLK_e)){
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
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

	//set the camera matrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4& cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/JimmyJump/PNG/CharacterRight_Standing.png");
	GameEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	/*for (int i = 0; i < 1000; i++)
	{*/
	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
	_spriteBatch.draw(pos + glm::vec4(50.0f, 0.0f, 0.0f, 0.0f), uv, texture.id, 0.0f, color);
	/*	}*/


	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);


	_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen
	_window.swapBuffer();
	}


