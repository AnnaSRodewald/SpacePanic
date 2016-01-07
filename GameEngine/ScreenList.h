#pragma once
#include <vector>

namespace GameEngine {

	class IMainGame;
	class IGameScreen;

	class ScreenList
	{
	public:
		ScreenList(IMainGame* game);
		~ScreenList();

		IGameScreen* moveNext();
		IGameScreen* movePrevious();

		void setScreen(int nextScreenIndex);
		void addScreen(IGameScreen* newScreen);

		void destroy();

		IGameScreen* getCurrent();

	protected: 
		IMainGame* m_game = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = -1;

	};

}