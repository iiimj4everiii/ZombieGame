#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Vertex.h"

namespace GameEngine
{
	enum class GlyphSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph
	{
	public:
		Glyph() {}
		Glyph(const glm::vec4& destinationRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& Color) :
			texture(Texture),
			depth(Depth)
		{
			topLeft.color = Color;
			topLeft.setPosition(destinationRect.x, destinationRect.y + destinationRect.w);
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = Color;
			bottomLeft.setPosition(destinationRect.x, destinationRect.y);
			bottomLeft.setUV(uvRect.x, uvRect.y);

			bottomRight.color = Color;
			bottomRight.setPosition(destinationRect.x + destinationRect.z, destinationRect.y);
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

			topRight.color = Color;
			topRight.setPosition(destinationRect.x + destinationRect.z, destinationRect.y + destinationRect.w);
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}

		GLuint texture;
		float depth;

		Vertex bottomLeft;
		Vertex topLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset),
			numVertices(NumVertices),
			texture(Texture)
		{}

		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();
		//void draw(glm::vec2 position, glm::vec2 dimension);
		//Passing by reference will speed up the process since there is no need to make copies of the parameters.
		//const will make these parameters immune to change by accident.
		void draw(const glm::vec4& destinationRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* glyph1, Glyph* glyph2);
		static bool compareBackToFront(Glyph* glyph1, Glyph* glyph2);
		static bool compareTexture(Glyph* glyph1, Glyph* glyph2);

		//Vertex buffer object
		GLuint _vbo;

		//Vertex array object
		GLuint _vao;

		GlyphSortType _sortType;

		////Since Glyph is a big struct, it will be unwise to simple sort such a thing.
		////Instead, sort the pointers. This way the sorting process will be much faster.
		//std::vector <Glyph*> _glyphs;
		//std::vector <RenderBatch> _renderBatches;

		//Since calling new and delete slows down the game when we call draw, we will use vector to handle creating and clearing new glyphs.
		//We also wants to have fast sorting, so we will still be sorting pointers and not the glyphs themselves.
		std::vector <Glyph> _glyphs;			//This is for actual glyphs
		std::vector <Glyph*> _glyphPointers;	//This is for sorting
		std::vector <RenderBatch> _renderBatches;
	};
}
