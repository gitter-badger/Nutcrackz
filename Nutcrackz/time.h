#pragma once

#include <SDL\SDL.h>
#include <algorithm>
#include <GL\freeglut.h>

class Time
{
public:
	Time() {}

	float deltaTime()
	{
		/*int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = timeSinceStart - oldTimeSinceStart;
		oldTimeSinceStart = timeSinceStart;*/

		double timeSinceStart = (double)SDL_GetTicks();
		double newTime = (double)SDL_GetTicks();
		float deltaTime = timeSinceStart - oldTimeSinceStart;
		oldTimeSinceStart = timeSinceStart;

		return deltaTime * 3 / 1000.0f;
	}

	float deltaTime2()
	{

	}

private:
	double oldTimeSinceStart;
};