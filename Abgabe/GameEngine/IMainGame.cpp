#include "IMainGame.h"
#include "Timing.h"
#include "ScreenList.h"
#include "IGameScreen.h"

namespace GameEngine {

	IMainGame::IMainGame()
	{
		m_screenList = std::make_unique<ScreenList>(this);
	}


	IMainGame::~IMainGame()
	{
		//Empty
	}


	void IMainGame::run(){
		if (init() == false){
			return;
		}

		FpsLimiter limiter;
		limiter.setMaxFPS(60.0f);

		m_isRunning = true;

		while (m_isRunning)
		{
			limiter.beginFrame();

			inputManager.update();

			//Call the custom update and draw method
			update();

			if (m_isRunning)
			{
				draw();

				m_fps = limiter.end();
				m_window.swapBuffer();
			}
		}
	}

	void IMainGame::exitGame(){
		onExit();
		if (m_currentScreen != nullptr){
			m_currentScreen->onExit();
		}
		if (m_screenList)
		{
			m_screenList->destroy();
			m_screenList.reset();
		}
		m_isRunning = false;
	}

	void IMainGame::onSDLEvent(SDL_Event& evnt){
		switch (evnt.type) {
		case SDL_QUIT:
			exitGame();
			break;
		case SDL_MOUSEMOTION:
			inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	void IMainGame::update(){
		if (m_currentScreen) // != nullptr
		{
			switch (m_currentScreen->getScreenState()) {
			case ScreenState::RUNNING:
				m_currentScreen->update();
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen)
				{
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
				if (m_currentScreen)
				{
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::EXIT_APPLICATION:
				//m_currentScreen->onExit();
				exitGame();
				break;
			case ScreenState::NONE:
				//Empty
				break;
			default:
				break;
			}
		}
		else
		{
			exitGame();
		}
	}

	void IMainGame::draw(){
		glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());

		if (m_currentScreen && m_currentScreen->getScreenState() == ScreenState::RUNNING)
		{
			m_currentScreen->draw();
		}
	}


	bool IMainGame::init(){
		GameEngine::init();

		//Initialize sound, must happen after GameEngine::init()
		audioEngine.init();

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		if (initSystems() == false){
			return false;
		}

		onInit();
		addScreens();

		m_currentScreen = m_screenList->getCurrent();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();

		return true;
	}

	bool IMainGame::initSystems(){
		m_window.create("Default2", 1280, 720, 0);

		return true;
	}

}