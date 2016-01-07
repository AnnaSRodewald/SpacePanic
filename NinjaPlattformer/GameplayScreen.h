#pragma once
#include <GameEngine\IGameScreen.h>

class GameplayScreen : public GameEngine::IGameScreen
{
public:
	GameplayScreen();
	~GameplayScreen();

	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;

private:
	
};

