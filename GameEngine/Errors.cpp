#include "Errors.h"

#include <cstdlib>

#include <SDL/SDL.h>
#include <iostream>
#include <string>


namespace GameEngine{

	void fatalError(std::string errorString){
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit(); //quits all the SDL systems
		exit(69); //exists the game
		}

	}