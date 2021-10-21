#include <cstddef>
#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

namespace GameEngine
{
	Sprite::Sprite()
	{
		_vertexBufferObjectID = 0;
	}

	Sprite::~Sprite()
	{
		if (_vertexBufferObjectID != 0)
		{
			//Free v-ram after we're done with the buffer object
			glDeleteBuffers(1, &_vertexBufferObjectID);
		}
	}

	void Sprite::init(float X, float Y, float Width, float Height, std::string texturePath)
	{
		_x = X;
		_y = Y;
		_width = Width;
		_height = Height;

		_texture = ResourceManager::getTexture(texturePath);

		//Generate buffers and return the address to _vertexBufferObjectID
		if (_vertexBufferObjectID == 0)
		{
			glGenBuffers(1, &_vertexBufferObjectID);
		}

		Vertex vertexArray[MY_VERTEX_SIZE];

		//First 2D triangle (X,Y) normalized coordinates
		vertexArray[0].setPosition(X, Y);
		vertexArray[0].setUV(0.0f, 0.0f);

		vertexArray[1].setPosition(X, Y + Height);
		vertexArray[1].setUV(0.0f, 1.0f);

		vertexArray[2].setPosition(X + Width, Y + Height);
		vertexArray[2].setUV(1.0f, 1.0f);

		//Coloring the first 2D triangle's the vertices
		for (int i = 0; i < 3; i++)
		{
			vertexArray[i].setColor(255, 0, 255, 255);
		}

		//Second 2D triangle (X,Y) normalized coordinates
		vertexArray[3].setPosition(X, Y);
		vertexArray[3].setUV(0.0f, 0.0f);

		vertexArray[4].setPosition(X + Width, Y + Height);
		vertexArray[4].setUV(1.0f, 1.0f);

		vertexArray[5].setPosition(X + Width, Y);
		vertexArray[5].setUV(1.0f, 0.0f);

		//Coloring the second 2D triangle's the vertices
		for (int i = 3; i < 6; i++)
		{
			vertexArray[i].setColor(255, 0, 255, 255);
		}

		vertexArray[1].setColor(0, 0, 255, 255);

		vertexArray[5].setColor(0, 255, 0, 255);

		//Bind to buffer first
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);

		//Uploading vertexArray data to buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

		//Unbind to buffer after done
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return;
	}

	//draw() function uses a lot of openGL funtions.
	//This is the most alienated part to me.
	//Need to go through list of openGL functions to see what they can do.
	void Sprite::draw()
	{
		//Bind texture
		glBindTexture(GL_TEXTURE_2D, _texture.textureID);

		//Bind the buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);

		//Enable attributes
		//We use the 0th attribute for position.
		//We use the 1st attribute for color.
		//We use the 2nd attribute for sprite dimension.
		glEnableVertexAttribArray(VERTEX_ATTRIB_ARRAY_POSITION_INDEX);
		glEnableVertexAttribArray(VERTEX_ATTRIB_ARRAY_COLOR_INDEX);
		glEnableVertexAttribArray(VERTEX_ATTRIB_ARRAY_UV_INDEX);

		//Point opengl to the position data in our vertex buffer object.
		//offsetof(struct identifier, its' struct member) is part of the cstddef library.
		//It returns the struct's starting position of the assigned memory block
		glVertexAttribPointer(VERTEX_ATTRIB_ARRAY_POSITION_INDEX, MY_POSITION_DIMENSION, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//Point opengl to the color data in our vertex buffer object.
		glVertexAttribPointer(VERTEX_ATTRIB_ARRAY_COLOR_INDEX, MY_COLOR_DIMENSION, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//Point opengl to the uv data in our VBO
		glVertexAttribPointer(VERTEX_ATTRIB_ARRAY_UV_INDEX, MY_UV_DIMENSION, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//Telling GL to draw GL_TRIANGLES, starting from 0th index, MY_VERTEX_SIZE points (vertices) to the back buffer.
		glDrawArrays(GL_TRIANGLES, 0, MY_VERTEX_SIZE);

		//Disabling vertex attribute after glDrawArrays.
		//If we disable too soon, then we will not be using the attributes
		glDisableVertexAttribArray(VERTEX_ATTRIB_ARRAY_UV_INDEX);
		glDisableVertexAttribArray(VERTEX_ATTRIB_ARRAY_COLOR_INDEX);
		glDisableVertexAttribArray(VERTEX_ATTRIB_ARRAY_POSITION_INDEX);

		//Unbind to buffer after done
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Unbind texture. Actually we may not want to unbind them
		//glBindTexture(GL_TEXTURE_2D, 0);

		return;
	}
}