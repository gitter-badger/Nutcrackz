#pragma once

class FpsLimiter
{
public:
	FpsLimiter();
	~FpsLimiter();

	void init(float maxFPS);
	void setMaxFPS(float maxFPS);
	void beginFrame();

	//end function will return current FPS!
	float endFrame();

private:
	void calculateFPS();

	float m_fps;
	float m_frameTime;
	float m_maxFPS;

	unsigned int m_startTicks;
};