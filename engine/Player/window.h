#pragma once

#include <iostream>
#include <string>
#include <SDL\SDL.h>
#include <GL\glew.h>

enum WindowFlags
{
	INVISIBLE = 0x1,
	FULLSCREEN = 0x2,
	BORDERLESS = 0x4,
	RESIZABLE = 0x8
};

class Window
{
public:
	Window();
	~Window();

	int create(std::string windowTitle, int width, int height, unsigned int currentFlags);

	void swapBuffer();

	//int getScreenWidth() { return m_scrWidth; }
	//int getScreenHeight() { return m_scrHeight; }

	SDL_Window *getWindow() const { return m_sdlWindow; }

	//int getWidth() { return m_windowWidth; }
	//int getHeight() { return m_windowHeight; }

private:
	//int m_scrWidth;
	//int m_scrHeight;
		
	//int m_windowWidth;
	//int m_windowHeight;

	SDL_Window *m_sdlWindow;
};