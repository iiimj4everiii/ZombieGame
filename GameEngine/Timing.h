#include <SDL/SDL.h>
#include <GL/glew.h>

#pragma once
namespace GameEngine
{
	const int NUM_FPS_SAMPLES = 10;

	class FpsLimiter
	{
	public:
		FpsLimiter();
		void init(float targetFPS);
		void setMaxFPS(float maxFPS);
		void begin();
		float end(); //Returns the FPS

	private:
		void calculateFPS();
		float _fps;
		float _maxFPS;
		GLuint _startTick;
		GLuint _currentTick;
	};
}