#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include "MainGame.h"

int main(int argc, char** argv)
{
	MainGame mainGame;

	mainGame.run();

	return 0;
}