#pragma once

#include "dystopia.h"
#include "window.h"
#include "inputmanager.h"

#include <memory>

class ScreenList;
class IGameScreen;

class IMainGame
{
public:
	IMainGame();
	virtual ~IMainGame();

	void setNothing();

	void run(std::string windowTitle, int width, int height, unsigned int currentFlags);
	void exitGame();

	virtual void onInit() = 0;
	virtual void addScreens() = 0;
	virtual void onExit() = 0;

	const float getFps() const { return m_fps; }

	void onSDLEvent(SDL_Event &evnt);

	InputManager inputManager;

	Window getWindow() { return m_window; }

	const IMainGame &getThis() { return *this; }

protected:
	virtual void update();
	virtual void draw();
		
	bool init(std::string windowTitle, int width, int height, unsigned int currentFlags);
	bool initSystems(std::string windowTitle, int width, int height, unsigned int currentFlags);

	bool m_isRunning = false;

	float m_fps = 0.0f;

	std::unique_ptr<ScreenList> m_screenList = nullptr;

	IGameScreen *m_currentScreen = nullptr;

	Window m_window;
	SDL_Event evnt;

	bool isRightClicking;
};