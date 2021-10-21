#include <iostream>
#include <SDL/SDL.h>

#include "Errors.h"

namespace GameEngine
{
	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl;

		SDL_Quit();

		system("PAUSE");

		exit(1);

		return;
	}
}