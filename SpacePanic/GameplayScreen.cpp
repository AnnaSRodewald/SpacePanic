#include "GameplayScreen.h"
#include <SDL\SDL.h>
#include <GameEngine\IMainGame.h>
#include <GameEngine\ResourceManager.h>
#include <random>
#include <ctime>
#include <GameEngine\GameEngineErrors.h>

#define DEBUG_RENDER

const float PLAYER_SPEED = 5.0f;
const float MONSTER_SPEED = 4.0f;

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

	//Load the texture
	m_texture = GameEngine::ResourceManager::getTexture("Assets/bricks_top.png");

	//Initialize the spriteBatch
	m_spriteBatch.init();
	m_hudSpriteBatch.init();

	//Initialize sprite font
	m_spriteFont = new GameEngine::SpriteFont("Fonts/chintzy_cpu_brk/chintzy.ttf", 64);

	//Shader init
	initShaders();

	//Init camera
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(0.5f);
	m_hudCamera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_hudCamera.setPosition(glm::vec2(m_window->getScreenWidth() / 2, m_window->getScreenHeight() / 2));


	//Load the levels and init the first one level
	loadLevels();

	////Start playing music
	//GameEngine::Music music = m_game->audioEngine.loadMusic("Music/Electrix_NES.mp3");
	//music.play(-1);

	m_currentLevelState = LevelState::INPROGRESS;

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

	if (m_currentLevelState != LevelState::INIT)
	{
		m_camera.update();
		m_hudCamera.update();
		checkInput();
		processInput();

		if (!checkWinCondition() && m_currentLevelState != LevelState::GAMEOVER && m_currentLevelState != LevelState::COMPLETED)
		{ //The player hasn't won yet..
			if (m_levels[m_currentLevel]->getCameraPosition() != glm::vec2(0.0f, 0.0f))
			{
				m_camera.setPosition(m_levels[m_currentLevel]->getCameraPosition());
			}
			else if (m_players.size() > 0)
			{
				m_camera.setPosition(m_players[0]->getPosition());
			}
			else{
				m_camera.setPosition(glm::vec2(m_window->getScreenWidth() / 2, m_window->getScreenHeight() / 2));
			}

			updateAgents(1.0f);
			updateLevel(*m_levels[m_currentLevel]);
		}
		else if (checkWinCondition())
		{ //The player won!
			m_currentLevelState = LevelState::COMPLETED;
		}
	}

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


	//m_player.draw(m_spriteBatch);

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();

	//Render the heads up display
	drawHUD();

	m_textureProgram.unuse();

	//Debug rendering
	if (m_renderDebug)
	{
		glm::vec4 destRect;
		//for (auto& box : m_boxes)
		//{

		//	destRect.x = box.getPosition().x;// -box.getDimensions().x / 2.0f;
		//	destRect.y = box.getPosition().y;// -box.getDimensions().y / 2.0f;
		//	destRect.z = box.getDimensions().x;
		//	destRect.w = box.getDimensions().y;

		//	m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);

		//	/*	m_debugRenderer.drawCircle(glm::vec2(box.getBody()->GetPosition().x, box.getBody()->GetPosition().y), GameEngine::ColorRGBA8(255, 255, 255, 255), box.getDimensions().x / 2.0f);
		//	*/
		//}

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

		std::vector<Box> holeBoxes = m_levels[m_currentLevel]->getHoleBoxes();

		for (auto& box : holeBoxes)
		{
			destRect.x = box.getPosition().x;// -box.getDimensions().x / 2.0f;
			destRect.y = box.getPosition().y;// -box.getDimensions().y / 2.0f;
			destRect.z = box.getDimensions().x;
			destRect.w = box.getDimensions().y;

			m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(0, 255, 255, 255), 0.0f);

			/*	m_debugRenderer.drawCircle(glm::vec2(box.getBody()->GetPosition().x, box.getBody()->GetPosition().y), GameEngine::ColorRGBA8(255, 255, 255, 255), box.getDimensions().x / 2.0f);
			*/
		}

		std::vector<Box> halfHoleBoxes = m_levels[m_currentLevel]->getHalfHoleBoxes();

		for (auto& box : halfHoleBoxes)
		{
			destRect.x = box.getPosition().x;// -box.getDimensions().x / 2.0f;
			destRect.y = box.getPosition().y;// -box.getDimensions().y / 2.0f;
			destRect.z = box.getDimensions().x;
			destRect.w = box.getDimensions().y;

			m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(255, 0, 255, 255), 0.0f);

			/*	m_debugRenderer.drawCircle(glm::vec2(box.getBody()->GetPosition().x, box.getBody()->GetPosition().y), GameEngine::ColorRGBA8(255, 255, 255, 255), box.getDimensions().x / 2.0f);
			*/
		}

		for (auto player : m_players)
		{
			//Render player
			auto box = player->getBox();
			destRect.x = box.getPosition().x;// -box.getDimensions().x / 2.0f;
			destRect.y = box.getPosition().y;// -box.getDimensions().y / 2.0f;
			destRect.z = box.getDimensions().x;
			destRect.w = box.getDimensions().y;
			m_debugRenderer.drawBox(destRect, GameEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);
		}

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

void GameplayScreen::processInput(){

	GameEngine::InputManager& inputManager = m_game->inputManager;

	if (inputManager.isKeyPressed(SDLK_ESCAPE)) {
		m_currentState = GameEngine::ScreenState::EXIT_APPLICATION;
	}
	if ((m_currentLevelState == LevelState::GAMEOVER || (m_currentLevelState == LevelState::COMPLETED && m_currentLevel < m_levels.size() - 1)) && inputManager.isKeyPressed(SDLK_SPACE))
	{
		//Clean up the current level
		cleanLevel();

		//Reload the level or init the next one
		if (m_currentLevelState == LevelState::GAMEOVER) {
			//Reload current level
			m_levels[m_currentLevel]->reload();
		} 
		else if (m_currentLevelState == LevelState::COMPLETED && m_currentLevel < m_levels.size() - 1) {
			//Init the next level
			m_currentLevel++;
		}

		initLevel(m_levels[m_currentLevel]);
		m_currentLevelState = LevelState::INPROGRESS;
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

void GameplayScreen::loadLevels(){
	// Level 1
	m_levels.push_back(new Level("Levels/level6.txt"));
	initLevel(m_levels.back());
	m_currentLevel = 0;

	m_levels.push_back(new Level("Levels/level5.txt"));
	m_levels.push_back(new Level("Levels/level4.txt"));
}

void GameplayScreen::initLevel(Level* level){
	m_currentLevelState = LevelState::INIT;
	//Add player
	m_players.push_back(new Player);
	//Init player
	//"Assets/blue_ninja.png"
	//"Textures/player.png"
	m_players.back()->init(
		&m_game->inputManager, 
		level->getStartPlayerPos(),
		glm::vec2(110.0f, 140.0f),
		glm::vec2(55.0f, 128.0f), 
		"Assets/blue_ninja.png", 
		GameEngine::ColorRGBA8(0, 255, 255, 255), 
		PLAYER_SPEED, 
		m_game->audioEngine.loadSoundEffect("Sound/Digging/Shovel_Into_Dirt_Louder.wav"), 
		m_game->audioEngine.loadSoundEffect("Sound/Digging/DirtShovelOnCoffin.wav"), 
		m_game->audioEngine.loadSoundEffect("Sound/shots/pistol.wav"));

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, level->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, level->getHeight() - 2);

	//m_boxes.push_back(m_player.getBox());

	//Add all the monsters
	for (auto& monsterPosition : level->getStartMonsterPositions()){
		m_monsters.push_back(new Monster);
		m_monsters.back()->init(MONSTER_SPEED, monsterPosition, glm::vec2(60.0f, 128.0f));
		//m_monsters.push_back(&monster);
		////m_boxes.push_back(monster.getBox());
	}

	m_playersDead = 0;
}

bool GameplayScreen::checkWinCondition(){

	if (m_monsters.size() == 0 && m_players.size() > 0)
	{
		return true;
	}

	return false;
}

void GameplayScreen::updateAgents(float deltaTime){

	int killPoints = 0;
	Player* latestPlayerToKillMonster = nullptr; // TODO: change into vector
	glm::vec4 penetrationDepth;

	//Update the players
	for (auto player : m_players)
	{
		if (player->isAlive())
		{
			player->update(*m_levels[m_currentLevel], m_players, m_monsters, deltaTime);
		}
	}

	//Update the monsters
	for (size_t i = 0; i < m_monsters.size(); i++)
	{
		if (m_monsters[i]->isAlive())
		{ //The Monster is alive and kicking
			m_monsters[i]->update(*m_levels[m_currentLevel], m_players, m_monsters, deltaTime);

			for (auto player : m_players)
			{
				if (m_monsters[i]->collideWithAgent(player, penetrationDepth) && abs(penetrationDepth.z - penetrationDepth.x) >= 20){
					std::printf("A player died :(");
					//Mark the player as dead
					player->kill();
					m_playersDead++;

					//Check if all players are dead
					if (m_playersDead >= m_players.size())
					{
						//change into game over state
						m_currentLevelState = LevelState::GAMEOVER;
						break;
					}
				}
			}

			for (size_t a = i + 1; a < m_monsters.size(); a++)
			{
				if (m_monsters[i]->collideWithAgent(m_monsters[a], penetrationDepth))
				{
					handleMonsterCollisionBehaviour(m_monsters[i], m_monsters[a], penetrationDepth);
				}
			}
		}
		else{ //Monster was killed by a player
			//Add points to the player who killed the monster
			killPoints += 500;

			if (latestPlayerToKillMonster == nullptr){
				latestPlayerToKillMonster = m_monsters[i]->getKiller();
			}
			else if (latestPlayerToKillMonster != m_monsters[i]->getKiller()) {
				//BONUS points if two player killed 2 monster at the same time ~ highly unlikely at the moment
				killPoints = killPoints * 2;
				latestPlayerToKillMonster->addPoints(killPoints);
				latestPlayerToKillMonster = m_monsters[i]->getKiller();
			}
			latestPlayerToKillMonster->addConsecutiveMonsterKills();
			latestPlayerToKillMonster->addPoints(killPoints);

			killPoints = 0;

			//Delete the monster from the vector
			delete m_monsters[i];
			m_monsters[i] = m_monsters.back();
			m_monsters.pop_back();
		}
	}

}

void GameplayScreen::updateLevel(Level& level){
	level.update();
}


void GameplayScreen::handleMonsterCollisionBehaviour(Monster* a, Monster* b, glm::vec4 penetrationDepth){

	float xDepth = abs(penetrationDepth.z - penetrationDepth.x);
	float yDepth = abs(penetrationDepth.w - penetrationDepth.y);

	glm::vec2 newPosition = a->getPosition();
	glm::vec2 newPosition2 = b->getPosition();

	if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)){
		if ((newPosition.x - penetrationDepth.x) < 0)
		{
			newPosition.x -= xDepth / 2;
			newPosition2.x += xDepth / 2;
		}
		else
		{
			newPosition.x += xDepth / 2;
			newPosition2.x -= xDepth / 2;
		}
	}
	else{
		if ((newPosition.y - penetrationDepth.y) < 0)
		{
			newPosition.y -= yDepth / 2;
			newPosition2.y += yDepth / 2;
		}
		else
		{
			newPosition.y += yDepth / 2;
			newPosition2.y -= yDepth / 2;
		}
	}

	a->setPosition(newPosition);
	b->setPosition(newPosition2);

	a->setDirection(glm::vec2(a->getDirection().x*(-1), a->getDirection().y*(-1)));
	b->setDirection(glm::vec2(b->getDirection().x*(-1), b->getDirection().y*(-1)));

	a->deactivateSawPlayer();
	b->deactivateSawPlayer();

}

void GameplayScreen::drawHUD(){
	char buffer[256];

	//Grab the hud camera matrix
	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	if (m_currentLevelState == LevelState::COMPLETED)
	{
		sprintf_s(buffer, "YOU WIN!");
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_window->getScreenWidth() / 4, m_window->getScreenHeight() / 2), glm::vec2(2.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));

		sprintf_s(buffer, "Score: %d", m_players[0]->getPoints());
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_window->getScreenWidth() / 8, m_window->getScreenHeight() / 3), glm::vec2(2.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));

		if (m_currentLevel < m_levels.size() - 1)
		{
			sprintf_s(buffer, "Enter the next level");
			m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_window->getScreenWidth() / 10, m_window->getScreenHeight() / 4), glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));
			sprintf_s(buffer, "by pressing SPACE");
			m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_window->getScreenWidth() / 10, m_window->getScreenHeight() / 6), glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));
		}
		else{
			sprintf_s(buffer, "Wow, you completed all levels!");
			m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_window->getScreenWidth() / 10, m_window->getScreenHeight() / 4), glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));
			sprintf_s(buffer, "You ROCK!");
			m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_window->getScreenWidth() / 10, m_window->getScreenHeight() / 6), glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));
		}

	}
	else if (m_currentLevelState == LevelState::GAMEOVER)
	{
		sprintf_s(buffer, "YOU LOSE!");
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_window->getScreenWidth() / 4, m_window->getScreenHeight() / 2), glm::vec2(2.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));

		sprintf_s(buffer, "Try again by pressing SPACE");
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(m_window->getScreenWidth() / 10, m_window->getScreenHeight() / 3), glm::vec2(1.0), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));

	}
	else {
		sprintf_s(buffer, "Num Humans %d", m_players.size());
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0), glm::vec2(0.5), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));

		sprintf_s(buffer, "Num Zombies %d", m_monsters.size());
		m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 36), glm::vec2(0.5), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));

		if (m_players.size() > 0)
		{
			sprintf_s(buffer, "Score P1 %d", m_players[0]->getPoints());
			m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 72), glm::vec2(0.5), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));

		}
	}

	m_hudSpriteBatch.end();

	m_hudSpriteBatch.renderBatch();
}

void GameplayScreen::cleanLevel() {
	//Clean up the current level
	for (size_t i = 0; i < m_monsters.size(); i++)
	{
		//Delete all monsters
		delete m_monsters[i];
		m_monsters[i] = m_monsters.back();
		m_monsters.pop_back();
	}
	if (m_monsters.size() != 0)
	{
		delete m_monsters[0];
		m_monsters[0] = m_monsters.back();
		m_monsters.pop_back();
	}

	for (size_t i = 0; i < m_players.size(); i++)
	{
		delete m_players[i];
		m_players[i] = m_players.back();
		m_players.pop_back();
	}
}