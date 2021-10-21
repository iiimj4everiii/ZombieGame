#include <algorithm>

#include "SpriteBatch.h"
#include "Sprite.h" //For pre-defined constants and enum only

namespace GameEngine
{
	SpriteBatch::SpriteBatch() :
		_vbo(0),
		_vao(0)
	{}

	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::createVertexArray()
	{
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		//When binding, any change made in the openGL state will be stored in the _vao
		glBindVertexArray(_vao);

		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//Enable attributes
		//We use the 0th attribute for position.
		//We use the 1st attribute for color.
		//We use the 2nd attribute for sprite dimension.
		glEnableVertexAttribArray(GameEngine::VERTEX_ATTRIB_ARRAY_POSITION_INDEX);
		glEnableVertexAttribArray(GameEngine::VERTEX_ATTRIB_ARRAY_COLOR_INDEX);
		glEnableVertexAttribArray(GameEngine::VERTEX_ATTRIB_ARRAY_UV_INDEX);

		//Point opengl to the position data in our vertex buffer object.
		//offsetof(struct identifier, its' struct member) is part of the cstddef library.
		//It returns the struct's starting position of the assigned memory block
		glVertexAttribPointer(GameEngine::VERTEX_ATTRIB_ARRAY_POSITION_INDEX, GameEngine::MY_POSITION_DIMENSION, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//Point opengl to the color data in our vertex buffer object.
		glVertexAttribPointer(GameEngine::VERTEX_ATTRIB_ARRAY_COLOR_INDEX, GameEngine::MY_COLOR_DIMENSION, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//Point opengl to the uv data in our VBO
		glVertexAttribPointer(GameEngine::VERTEX_ATTRIB_ARRAY_UV_INDEX, GameEngine::MY_UV_DIMENSION, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//Trick to disable all the glEnableVertexAttribArray and unbind _vbo
		glBindVertexArray(0);

		return;
	}

	void SpriteBatch::begin(GlyphSortType sortType /* = GlyphSortType::TEXTURE*/)
	{
		_sortType = sortType;

		_renderBatches.clear();

		//No longer need delete since we are not using new and delete to create new glyphs anymore.
		//We will use vector to manage creating new glyphs for better performance.
		//for (int i = 0; i < _glyphs.size(); i++)
		//{
		//	delete _glyphs[i];
		//}

		_glyphs.clear();

		return;
	}

	void SpriteBatch::end()
	{
		//Setting up _glyphPointers for sorting.
		_glyphPointers.resize(_glyphs.size());

		//Now _glyphPointers is like an array now with a set size.
		for (int i = 0; i < _glyphs.size(); i++)
		{
			_glyphPointers[i] = &_glyphs[i]; //Match actual _glyphs index with its corresponding address in the _glyphPointers.
		}

		sortGlyphs();
		createRenderBatches();

		return;
	}

	void SpriteBatch::sortGlyphs()
	{
		switch (_sortType)
		{
		case GlyphSortType::NONE:
			break;
		case GlyphSortType::FRONT_TO_BACK:
			//Sorts the elements into ascending order, AND also preserves the relative order of the elements with equivalent values.
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
			break;
		default:
			break;
		}

		return;
	}

	//Compare front to back compares the depth of Glyph struct
	bool SpriteBatch::compareFrontToBack(Glyph* glyph1, Glyph* glyph2)
	{
		return (glyph1->depth < glyph2->depth);
	}

	//Compare back to front compares the depth of Glyph struct
	bool SpriteBatch::compareBackToFront(Glyph* glyph1, Glyph* glyph2)
	{
		return (glyph1->depth > glyph2->depth);
	}

	//Compare back to front compares the texture of Glyph struct
	bool SpriteBatch::compareTexture(Glyph* glyph1, Glyph* glyph2)
	{
		return (glyph1->texture < glyph2->texture);
	}

	void SpriteBatch::draw(const glm::vec4& destinationRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color)
	{
		//This is inefficient as we are creating a newGlyph and then creating another copy of the newGlyph to the _glyphs vector.
		//We can optimize this by creating a constructor and use emplace_back() instead.

		//Glyph newGlyph;
		//newGlyph.texture = texture;
		//newGlyph.depth = depth;

		//newGlyph.topLeft.color = color;
		//newGlyph.topLeft.setPosition(destinationRect.x, destinationRect.y + destinationRect.w);
		//newGlyph.topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		//newGlyph.bottomLeft.color = color;
		//newGlyph.bottomLeft.setPosition(destinationRect.x, destinationRect.y);
		//newGlyph.bottomLeft.setUV(uvRect.x, uvRect.y);

		//newGlyph.bottomRight.color = color;
		//newGlyph.bottomRight.setPosition(destinationRect.x + destinationRect.z, destinationRect.y);
		//newGlyph.bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		//newGlyph.topRight.color = color;
		//newGlyph.topRight.setPosition(destinationRect.x + destinationRect.z, destinationRect.y + destinationRect.w);
		//newGlyph.topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		//_glyphs.push_back(newGlyph);

		_glyphs.emplace_back(destinationRect, uvRect, texture, depth, color);

		return;
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector <Vertex> vertices;
		vertices.resize(_glyphPointers.size() * 6);

		if (_glyphPointers.empty())
		{
			return;
		}

		//0th index will be our reference that the next index will be compared with.
		//If the next index contains different texture, then the next index will be the new reference.
		int cv = 0; // current vertex
		int offset = 0;
		//RenderBatch myBatch(0, GameEngine::MY_VERTEX_SIZE, _glyphs[0]->texture);
		//_renderBatches.push_back(myBatch);
		//We can remove the intermediate step of myBatch initialization by using emplace_back.
		_renderBatches.emplace_back(0, GameEngine::MY_VERTEX_SIZE, _glyphPointers[0]->texture);

		//vertices[cv++] = _glyphPointers[0]->topLeft;
		//is the same as
		//vertices[cv] = _glyphPointers[0]->topLeft; cv++;
		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->bottomLeft;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;

		offset += 6;

		for (int cg = 1; cg < _glyphPointers.size(); cg++)
		{
			//If current glyph texture not equal to the previous glyph texture after sorting. 
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture)
			{
				_renderBatches.emplace_back(offset, GameEngine::MY_VERTEX_SIZE, _glyphPointers[cg]->texture);
			}
			else
			{
				_renderBatches.back().numVertices += 6;
			}

			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;

			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//vertices.data() returns the address of the first element of vertices.
		//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
		//We will instead orphan the buffer then reupload vertex data to the buffer.
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), nullptr, GL_DYNAMIC_DRAW);

		//Uploading vertex data.
		//vertices.data() returns the address of the first element of vertices.
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());

		//Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return;
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);

		//Loop through all the different textures
		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}
}