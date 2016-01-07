#include "GameplayScreen.h"
#include <iostream>


GameplayScreen::GameplayScreen()
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
	std::cout << "OnEntry\n";
}

void GameplayScreen::onExit() {

}


void GameplayScreen::update() {
	std::cout << "Update\n";
}

void GameplayScreen::draw() {
	std::cout << "Draw\n";
}
