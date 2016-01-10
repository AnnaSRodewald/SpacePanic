#include "GameplayScreen.h"
#include <iostream>
#include <SDL\SDL.h>
#include <GameEngine\IMainGame.h>
#include <GameEngine\ResourceManager.h>
#include <random>

#define DEBUG_RENDER

GameplayScreen::GameplayScreen(GameEngine::Window* window) : m_window(window)
{
}


GameplayScreen::~GameplayScreen()
{
}


int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}


void GameplayScreen::build() {
}

void GameplayScreen::destroy() {

}


void GameplayScreen::onEntry() {
	b2Vec2 gravity(0.0f, -30.0);
	m_world = std::make_unique<b2World>(gravity);

	//Init debug renderer
	m_debugRenderer.init();

	//Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -25.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	//Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Make a punch of boxes
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);

	std::uniform_real_distribution<float> size(0.5f, 2.5f);

	std::uniform_int_distribution<int> color(0, 255);

	//Load the texture
	m_texture = GameEngine::ResourceManager::getTexture("Assets/bricks_top.png");

	const int NUM_BOXES = 50;

	for (size_t i = 0; i < NUM_BOXES; i++)
	{
		GameEngine::ColorRGBA8 randColor;
		randColor.r = color(randGenerator);
		randColor.g = color(randGenerator);
		randColor.b = color(randGenerator);
		randColor.a = 255;

		//float s = size(randGenerator);

		Box newBox;

		newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)), m_texture, randColor, false);
		m_boxes.push_back(newBox);
	}

	//Initialize the spriteBatch
	m_spriteBatch.init();

	//Shader init
	initShaders();

	//Init camera
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(24.0f);

	//Init player
	m_player.init(m_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(1.0f, 2.0f), "Assets/blue_ninja.png", GameEngine::ColorRGBA8(255, 255, 255, 255));

}

void GameplayScreen::onExit() {
	m_debugRenderer.dispose();
}


void GameplayScreen::update() {
	m_camera.update();
	checkInput();

	m_player.update(m_game->inputManager);

	//Update the physics simulation
	m_world->Step(1.0f / 60.0f, 6, 2);
}

void GameplayScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_textureProgram.use();

	//Upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_spriteBatch.begin();

	//Draw all the boxes
	for (auto& box : m_boxes)
	{
		box.draw(m_spriteBatch);
	}

	m_player.draw(m_spriteBatch);

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();

	m_textureProgram.unuse();

	//Debug rendering
	if (m_renderDebug)
	{
		glm::vec4 destRect;
		for (auto& box : m_boxes)
		{

			destRect.x = box.getBody()->GetPosition().x - box.getDimensions().x / 2.0f;
			destRect.y = box.getBody()->GetPosition().y - box.getDimensions().y / 2.0f;
			destRect.z = box.getDimensions().x;
			destRect.w = box.getDimensions().y;

			m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(255, 255, 255, 255), box.getBody()->GetAngle());

			/*	m_debugRenderer.drawCircle(glm::vec2(box.getBody()->GetPosition().x, box.getBody()->GetPosition().y), GameEngine::ColorRGBA8(255, 255, 255, 255), box.getDimensions().x / 2.0f);
			*/
		}

		//Render player
		auto box = m_player.getBox();
		destRect.x = box.getBody()->GetPosition().x - box.getDimensions().x / 2.0f;
		destRect.y = box.getBody()->GetPosition().y - box.getDimensions().y / 2.0f;
		destRect.z = box.getDimensions().x;
		destRect.w = box.getDimensions().y;
		m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(255, 255, 255, 255), box.getBody()->GetAngle());

		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
	}
}


void GameplayScreen::checkInput(){
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		m_game->onSDLEvent(evnt);
	}

}

void GameplayScreen::initShaders() {
	// Compile our color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}