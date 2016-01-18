#pragma once
#include <memory>
#include <GameEngine\IMainGame.h>
#include "GameplayScreen.h"

class App : public GameEngine::IMainGame
{
public:
	App();
	~App();

	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;

private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
};

