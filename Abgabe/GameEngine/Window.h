#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace GameEngine{

	enum WindowFlags{ INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window(void);
		~Window(void);

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		int getScreenWidth() { return m_screenWidth; }
		int getScreenHeight() { return m_screenHeight; }

	private:
		SDL_Window* m_sdlWindow;
		int m_screenWidth, m_screenHeight;
	};


}