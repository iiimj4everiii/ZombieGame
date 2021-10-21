#pragma once
#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>

namespace GameEngine
{
	//For bitwise operation.
	enum WindowFlag
	{
		INVISIBLE = 0x1,
		FULLSCREEN = 0x2,
		BORDERLESS = 0x4
	};

	class GameWindow
	{
	public:
		GameWindow();
		~GameWindow();
		int createWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int windowFlags);
		int getScreenWidth() { return _screenWidth; }
		int getScreenHeight() { return _screenHeight; }
		void enableVSync(bool enable = true) { SDL_GL_SetSwapInterval(enable); }
		void swapBuffer();
	private:
		int _screenWidth;
		int _screenHeight;
		SDL_Window* _sdlWindow;

	};
}
