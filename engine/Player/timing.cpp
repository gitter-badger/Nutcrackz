#include "timing.h"

#include <SDL\SDL.h>

FpsLimiter::FpsLimiter()
{
}

FpsLimiter::~FpsLimiter()
{
}

void FpsLimiter::init(float maxFPS)
{
	setMaxFPS(maxFPS);
}

void FpsLimiter::setMaxFPS(float maxFPS)
{
	m_maxFPS = maxFPS;
}

void FpsLimiter::beginFrame()
{
	m_startTicks = SDL_GetTicks();
}

float FpsLimiter::endFrame()
{
	calculateFPS();

	Uint32 frameTicks = SDL_GetTicks() - m_startTicks;

	//Limit the fps to the max fps
	if (1000.0f / m_maxFPS > frameTicks)
	{
		SDL_Delay(1000 / (Uint32)m_maxFPS - frameTicks);
	}

	return m_fps;
}

void FpsLimiter::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static Uint32 frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static Uint32 prevTicks = SDL_GetTicks();

	Uint32 currentTicks;

	currentTicks = SDL_GetTicks();
	m_frameTime = (float)(currentTicks - prevTicks);
	frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

	prevTicks = currentTicks;
		
	int count;

	currentFrame++;

	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}

	float frameTimeAvg = 0;

	for (int i = 0; i < count; i++)
	{
		frameTimeAvg += frameTimes[i];
	}

	frameTimeAvg /= count;
		
	if (frameTimeAvg > 0)
	{
		m_fps = 1000.0f / frameTimeAvg;
	}
	else
	{
		m_fps = 60.0f;
	}
}