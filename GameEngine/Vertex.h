#pragma once
#include <GL/glew.h>

namespace GameEngine
{
	struct Position
	{
		float x;
		float y;
	};

	struct ColorRGBA8
	{
		ColorRGBA8() : r(0), g(0), b(0), alpha(0) {} //Constructor initialized with initialization list.

		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte Alpha) : r(R), g(G), b(B), alpha(Alpha) {} //Constructor initialized with initialization list.

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte alpha;
	};

	struct UV
	{
		float u;
		float v;
	};

	//For alignment issues, we want the size of Vertex struct to be a multiple of 4 bytes.
	//Right now float position[2] holds 8 bytes and GLubyte color RGB holds 3 bytes.
	//We will add another byte to color to make Vertex struct size 3x4 = 12 bytes.
	//We will use this last byte for transparency.

	struct Vertex
	{
		//position[2];
		//GLubyte color[4];

		Position position;
		ColorRGBA8 color;
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte alpha)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.alpha = alpha;

			return;
		}

		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;

			return;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;

			return;
		}
	};
}