#include "GameWindow.h"
#include "Errors.h"

namespace GameEngine
{
	GameWindow::GameWindow()
	{
	}

	GameWindow::~GameWindow()
	{
	}

	int GameWindow::createWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int windowFlags)
	{
		Uint32 sdlWindowFlags = SDL_WINDOW_OPENGL;
		if (windowFlags & INVISIBLE) { sdlWindowFlags |= SDL_WINDOW_HIDDEN; }
		if (windowFlags & FULLSCREEN) { sdlWindowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }
		if (windowFlags & BORDERLESS) { sdlWindowFlags |= SDL_WINDOW_BORDERLESS; }

		//SDL functon to create a window with (window name, window x position, window y position, window width size, window height size, SDL_WINDOW_OPENGL we will learn later);
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, sdlWindowFlags);
		if (_sdlWindow == nullptr)
		{
			fatalError("SDL window could not be created!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr)
		{
			fatalError("SDL_GL Context could not be created!");
		}

		//Print the computer's OpenGL version. Only can be called after context is created
		printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			fatalError("Could not initialize glew!");
		}

		//Setting initial background color (R, G, B, 1.0f)
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		enableVSync(false);

		//Enable blending
		glEnable(GL_BLEND);

		//Using alpha channel blending.
		//source factor = GL_SRC_ALPHA will use alpha channel of the texture to blend.
		//destination factor = GL_ONE_MINUS_SRC_ALPHA will take whatever the background color - alpha value in that pixel.
		//This will give us transparency effect.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void GameWindow::swapBuffer()
	{
		//Swap buffer and draw everything to screen. Double buffering
		SDL_GL_SwapWindow(_sdlWindow);
	}
}