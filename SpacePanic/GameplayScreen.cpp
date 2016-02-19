#include "GameplayScreen.h"
#include <iostream>
#include <SDL\SDL.h>
#include <GameEngine\IMainGame.h>
#include <GameEngine\ResourceManager.h>
#include <random>
#include <ctime>
#include <GameEngine\GameEngineErrors.h>

#define DEBUG_RENDER

const float PLAYER_SPEED = 5.0f;
const float MONSTER_SPEED = 2.5f;

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


	//Init debug renderer
	m_debugRenderer.init();


	//Make a punch of boxes
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);

	std::uniform_real_distribution<float> size(0.5f, 2.5f);

	std::uniform_int_distribution<int> color(0, 255);

	//Load the texture
	m_texture = GameEngine::ResourceManager::getTexture("Assets/bricks_top.png");

	const int NUM_BOXES = 50;

	//for (size_t i = 0; i < NUM_BOXES; i++)
	//{
	//	GameEngine::ColorRGBA8 randColor;
	//	randColor.r = color(randGenerator);
	//	randColor.g = color(randGenerator);
	//	randColor.b = color(randGenerator);
	//	randColor.a = 255;

	//	//float s = size(randGenerator);

	//	Box newBox;

	//	newBox.init(glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)), m_texture, randColor);
	//	m_boxes.push_back(newBox);
	//}




	//Initialize the spriteBatch
	m_spriteBatch.init();

	//Shader init
	initShaders();

	//Init camera
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(0.5f);

	//Init level
	initLevel();

}

void GameplayScreen::onExit() {
	m_debugRenderer.dispose();

	////Delete levels, players and monsters
	//for (int i = 0; i < m_levels.size(); i++)
	//{
	//	delete m_levels[i];
	//}
	//for (int i = 0; i < m_players.size(); i++)
	//{
	//	delete m_players[i];
	//}
	//for (int i = 0; i < m_monsters.size(); i++)
	//{
	//	delete m_monsters[i];
	//}
}


void GameplayScreen::update() {
	m_camera.setPosition(m_player.getPosition());
	m_camera.update();
	checkInput();

	updateAgents(1.0f);

}

void GameplayScreen::updateAgents(float deltaTime){
	//update(const std::vector<std::string>& levelData, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime
	//m_levels[m_currentLevel]->getLevelData();
	//m_player.update(m_levels[m_currentLevel]->getLevelData(), m_players, m_monsters, deltaTime);
	
	for (auto player : m_players)
	{
		player->update(*m_levels[m_currentLevel], m_players, m_monsters, deltaTime);
	}

	for (size_t i = 0; i < m_monsters.size(); i++)
	{
		m_monsters[i]->update(*m_levels[m_currentLevel], m_players, m_monsters, deltaTime);
		for (auto player : m_players)
		{
			if (m_monsters[i]->collideWithAgent(player)){
				std::printf("");
				GameEngine::fatalError("YOU LOSE");
			}
		}
		for (size_t a = i+1; a < m_monsters.size(); a++)
		{
			glm::vec4 penetrationDepth;
			if (m_monsters[i]->collideWithAgent(m_monsters[a], penetrationDepth))
			{
				handleMonsterCollisionBehaviour(m_monsters[i], m_monsters[a], penetrationDepth);
			}
		}
	}

}

void GameplayScreen::handleMonsterCollisionBehaviour(Monster* a, Monster* b, glm::vec4 penetrationDepth){

	float xDepth = abs(penetrationDepth.z - penetrationDepth.x);
	float yDepth = abs(penetrationDepth.w - penetrationDepth.y);

	glm::vec2 newPosition = a->getPosition();
	glm::vec2 newPosition2 = b->getPosition();

	if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
		if ((newPosition.x - penetrationDepth.x) < 0)
		{
			newPosition.x -= xDepth/2;
			newPosition2.x += xDepth / 2;
		}
		else
		{
			newPosition.x += xDepth/2;
			newPosition2.x -= xDepth / 2;
		}
	}
	else{
		if ((newPosition.y - penetrationDepth.y) < 0)
		{
			newPosition.y -= yDepth/2;
			newPosition2.y += yDepth / 2;
		}
		else
		{
			newPosition.y += yDepth/2;
			newPosition2.y -= yDepth / 2;
		}
	}

	a->setPosition(newPosition);
	b->setPosition(newPosition2);

	a->setDirection(glm::vec2(a->getDirection().x*(-1), a->getDirection().y*(-1)));
	b->setDirection(glm::vec2(b->getDirection().x*(-1), b->getDirection().y*(-1)));

}


void GameplayScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	m_textureProgram.use();

	//Upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw the level
	m_levels[m_currentLevel]->draw();

	m_spriteBatch.begin();

	////Draw all the boxes
	//for (auto& box : m_boxes)
	//{
	//	box.draw(m_spriteBatch);
	//}

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	//Draw the players
	for (int i = 0; i < m_players.size(); i++)
	{
		if (m_camera.isBoxInView(m_players[i]->getPosition(), agentDims))
		{
			m_players[i]->draw(m_spriteBatch);
		}
	}

	//Draw the monsters
	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (m_camera.isBoxInView(m_monsters[i]->getPosition(), agentDims))
		{
			m_monsters[i]->draw(m_spriteBatch);
		}
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

			destRect.x = box.getPosition().x;// -box.getDimensions().x / 2.0f;
			destRect.y = box.getPosition().y;// -box.getDimensions().y / 2.0f;
			destRect.z = box.getDimensions().x;
			destRect.w = box.getDimensions().y;

			m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);

			/*	m_debugRenderer.drawCircle(glm::vec2(box.getBody()->GetPosition().x, box.getBody()->GetPosition().y), GameEngine::ColorRGBA8(255, 255, 255, 255), box.getDimensions().x / 2.0f);
			*/
		}

		std::vector<Box> levelBoxes = m_levels[m_currentLevel]->getLevelBoxes();

		for (auto& box : levelBoxes)
		{
			destRect.x = box.getPosition().x;// -box.getDimensions().x / 2.0f;
			destRect.y = box.getPosition().y;// -box.getDimensions().y / 2.0f;
			destRect.z = box.getDimensions().x;
			destRect.w = box.getDimensions().y;

			m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);

			/*	m_debugRenderer.drawCircle(glm::vec2(box.getBody()->GetPosition().x, box.getBody()->GetPosition().y), GameEngine::ColorRGBA8(255, 255, 255, 255), box.getDimensions().x / 2.0f);
			*/
		}

		//Render player
		auto box = m_player.getBox();
		destRect.x = box.getPosition().x;// -box.getDimensions().x / 2.0f;
		destRect.y = box.getPosition().y;// -box.getDimensions().y / 2.0f;
		destRect.z = box.getDimensions().x;
		destRect.w = box.getDimensions().y;
		m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);

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

void GameplayScreen::initLevel(){
	// Level 1
	m_levels.push_back(new Level("Levels/level3.txt"));
	m_currentLevel = 0;

	//Init player
	//"Assets/blue_ninja.png"
	//"Textures/player.png"
	m_player.init(&m_game->inputManager, m_levels[m_currentLevel]->getStartPlayerPos(), glm::vec2(55.0f, 128.0f), "Assets/blue_ninja.png", GameEngine::ColorRGBA8(0, 255, 255, 255), PLAYER_SPEED);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() - 2);

	//Add player
	m_players.push_back(&m_player);

	for (auto& monsterPosition : m_levels[m_currentLevel]->getStartMonsterPositions()){
		m_monsters.push_back(new Monster);
		m_monsters.back()->init(MONSTER_SPEED, monsterPosition, glm::vec2(60.0f, 128.0f));
		//m_monsters.push_back(&monster);
		////m_boxes.push_back(monster.getBox());
	}

	//m_boxes.push_back(m_player.getBox());

	////Add all the monsters
	//const std::vector<glm::vec2> monsterPositions = m_levels[m_currentLevel]->getStartMonsterPositions();
	//for (int i = 0; i < monsterPositions.size(); i++)
	//{
	//	m_monsters.push_back(new Monster);
	//	m_monsters.back()->init(MONSTER_SPEED, monsterPositions[i], glm::vec2(AGENT_WIDTH, AGENT_WIDTH));
	//}

}
