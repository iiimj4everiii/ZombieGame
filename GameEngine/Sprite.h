#pragma once
#include <windows.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
#include "GLTexture.h"

namespace GameEngine
{
	const int MY_VERTEX_SIZE = 6;
	const int MY_POSITION_DIMENSION = 2;
	const int MY_COLOR_DIMENSION = 4;
	const int MY_UV_DIMENSION = 2;

	enum
	{
		VERTEX_ATTRIB_ARRAY_POSITION_INDEX,
		VERTEX_ATTRIB_ARRAY_COLOR_INDEX,
		VERTEX_ATTRIB_ARRAY_UV_INDEX
	};

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float X, float Y, float Width, float Height, std::string texturePath);
		void draw();

	private:
		float _x;
		float _y;
		float _width;
		float _height;
		GLuint _vertexBufferObjectID;
		GLTexture _texture;
	};
}