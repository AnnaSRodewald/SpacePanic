#include <SDL/SDL.h>
#include <GL/glew.h>

#include "GameEngine.h"

namespace GameEngine{

	int init(){
		//Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		//Tell SDL that we want a doible buffered window so we dont get any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // one window is cleared, on the other the stuff is drawn -->prevents the window from flickering --> smoother :)

		return 0;
		}
	}