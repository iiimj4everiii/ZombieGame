#include <SDL/SDL.h>
#include <GL/glew.h>

#include "GameEngine.h"

namespace GameEngine
{
	int init()
	{
		//SDL function to initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		//Use double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}