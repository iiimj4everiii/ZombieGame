#include <iostream>

#include "Timing.h"

namespace GameEngine
{
	FpsLimiter::FpsLimiter()
	{

	}

	void FpsLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);

		return;
	}

	void FpsLimiter::setMaxFPS(float maxFPS)
	{
		_maxFPS = maxFPS;

		return;
	}

	void FpsLimiter::begin()
	{
		_startTick = SDL_GetTicks();

		return;
	}

	float FpsLimiter::end()
	{
		GLuint frameTime = SDL_GetTicks() - _startTick;

		//Setting FPS limiter to maxFps
		if (frameTime < 1000 / _maxFPS)
		{
			SDL_Delay(1000 / _maxFPS - frameTime);
		}

		calculateFPS();

		return _fps;
	}

	void FpsLimiter::calculateFPS()
	{
		static GLuint frameTimes[NUM_FPS_SAMPLES];
		static int frameCount = 0;

		GLuint currentTick = SDL_GetTicks();
		GLuint frameTime = currentTick - _startTick;
		frameTimes[frameCount % NUM_FPS_SAMPLES] = frameTime;

		frameCount++;

		float frameTimeSum = 0;
		for (int i = 0; i < NUM_FPS_SAMPLES; i++)
		{
			frameTimeSum += frameTimes[i];
		}

		float frameTimeAvg = frameTimeSum / NUM_FPS_SAMPLES;

		_startTick = currentTick;

		_fps = 1000.0f / frameTimeAvg;

		return;
	}
}