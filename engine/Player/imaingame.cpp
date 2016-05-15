#include "imaingame.h"
#include "screenlist.h"
#include "igamescreen.h"
#include "timing.h"

#include <stdio.h>
#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>

static int windowWidth;
static int windowHeight;

IMainGame::IMainGame()
{
	m_screenList = std::make_unique<ScreenList>(this);
}

IMainGame::~IMainGame()
{
}

void IMainGame::setNothing()
{
	m_currentScreen->setObjectMode(0);

	m_currentScreen->createBox = false;
	m_currentScreen->createPhysicsBox = false;
	m_currentScreen->createPointLight = false;
	m_currentScreen->createPlayer = false;

	std::cout << "TEST!" << std::endl;
}

void IMainGame::run(std::string windowTitle, int width, int height, unsigned int currentFlags)
{
	/*int w;
	int h;

	w = GetSystemMetrics(SM_CXSCREEN);
	h = GetSystemMetrics(SM_CYSCREEN) - 72;

	windowWidth = w - 960;
	windowHeight = h - 406;*/

	if (!init(windowTitle, width, height, currentFlags))
		return;

	bool show_menu_bar = true;
	bool show_test_window = true;
	bool show_another_window = false;

	FpsLimiter limiter;
	limiter.setMaxFPS(60.0f);

	m_isRunning = true;

	while (m_isRunning)
	{
		limiter.beginFrame();

		inputManager.update();
		update();
			
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				m_isRunning = false;
		}

		if (m_isRunning)
		{
			draw();

			m_fps = limiter.endFrame();

			m_window.swapBuffer();
		}
	}
}

void IMainGame::exitGame()
{
	m_currentScreen->onExit();

	if (m_screenList)
	{
		m_screenList->destroy();
		m_screenList.reset();
	}

	SDL_Quit();
	exit(1);
	//m_isRunning = false;
}

void IMainGame::onSDLEvent(SDL_Event &evnt)
{
	switch (evnt.type)
	{
	case SDL_QUIT:
		exitGame();
		break;
	case SDL_MOUSEMOTION:
		inputManager.setMouseCoords(float(evnt.motion.x), float(evnt.motion.y));
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

bool IMainGame::init(std::string windowTitle, int width, int height, unsigned int currentFlags)
{
	initialize();

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	if (!initSystems(windowTitle, width, height, currentFlags))
		return false;

	onInit();
	addScreens();

	m_currentScreen = m_screenList->getCurrent();
	m_currentScreen->onEntry();
	m_currentScreen->setRunning();

	return true;
}

bool IMainGame::initSystems(std::string windowTitle, int width, int height, unsigned int currentFlags)
{
	m_window.create(windowTitle, width, height, currentFlags);

	return true;
}

void IMainGame::update()
{
	if (m_currentScreen)
	{
		switch (m_currentScreen->getState())
		{
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
			exitGame();
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

void IMainGame::draw()
{
	//glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());

	if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING)
	{
		m_currentScreen->draw();
	}
}