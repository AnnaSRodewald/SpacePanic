#include "MainGame.h"


#include <iostream>
#include <string>

#include <GameEngine/Errors.h>
#include <GameEngine/ResourceManager.h>




MainGame::MainGame(void) : 
	m_screenWidth(1024),   //Initialzation list
	m_screenHeight(768), 
	m_time(0),
	m_gameState(GameState::PLAY),
	m_maxFPS(60.0f)
	{
	m_camera.init(m_screenWidth, m_screenHeight);
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

	m_window.create("Game Engine", m_screenWidth, m_screenHeight, 0);

	initShaders();

	m_spriteBatch.init();

	m_fpsLimiter.init(m_maxFPS);
	}


void MainGame::initShaders(){
	m_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_colorProgram.addAttribute("vertexPosition");
	m_colorProgram.addAttribute("vertexColor");
	m_colorProgram.addAttribute("vertexUV");
	m_colorProgram.linkShaders();
	}


void MainGame::gameLoop(){

	//loop until the game state is EXIT
	while (m_gameState != GameState::EXIT ){
		//Used for frame time measuring
		//	float startTicks = SDL_GetTicks();
		m_fpsLimiter.beginFrame();

		processInput();
		m_time += 0.01;

		m_camera.update();

		//Update all bullets
		for (size_t i = 0; i < m_bullets.size();)
		{
			if (m_bullets[i].update() == true){
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
			} else{
				i++;
			}
		}

		drawGame();

		m_fps = m_fpsLimiter.end();	

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000){
			std::cout << m_fps << std::endl;
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
				m_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				m_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				m_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				m_inputManager.releaseKey(evnt.button.button);
				break;
			} 
		}

	if (m_inputManager.isKeyDown(SDLK_w))
		{
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
		}
	if (m_inputManager.isKeyDown(SDLK_s)){
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
		}
	if (m_inputManager.isKeyDown(SDLK_a)){
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
		}
	if (m_inputManager.isKeyDown(SDLK_d)){
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
		}
	if (m_inputManager.isKeyDown(SDLK_q)){
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
		}
	if (m_inputManager.isKeyDown(SDLK_e)){
		m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
		}

	if (m_inputManager.isKeyDown(SDL_BUTTON_LEFT))
		{
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertScreenToWorld(mouseCoords);
		//std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		m_bullets.emplace_back(playerPosition, direction, 5.00f, 1000);
		}

	}

void MainGame::drawGame() {

	//Set the base depth to 1.0
	glClearDepth(1.0);
	//clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // same as callinglClear fot both variables


	m_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = m_colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//set the camera matrix
	GLuint pLocation = m_colorProgram.getUniformLocation("P");
	glm::mat4& cameraMatrix = m_camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/JimmyJump/PNG/CharacterRight_Standing.png");
	GameEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	m_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	for (size_t i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i].draw(m_spriteBatch);
	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);


	m_colorProgram.unuse();

	//Swap our buffer and draw everything to the screen
	m_window.swapBuffer();
	}


