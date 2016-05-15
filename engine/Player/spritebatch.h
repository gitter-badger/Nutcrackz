#pragma once

#include "vertex.h"

#include <vector>
#include <GL\glew.h>
#include <GLM\glm.hpp>

// Determines how we should sort the glyphs
enum class GlyphSortType {
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

// A glyph is a single quad. These are added via SpriteBatch::draw
class Glyph {
public:
	Glyph() {};
	Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
	Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);

	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
private:
	// Rotates a point about (0,0) by angle
	glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
};

class Tri {
public:
	Tri() {};
	Tri(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
	Tri::Tri(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);

	GLuint texture;
	float depth;

	Vertex top;
	Vertex bottomLeft;
	Vertex bottomRight;
private:
	// Rotates a point about (0,0) by angle
	glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
};

// Each render batch is used for a single draw call
class RenderBatch {
public:
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
		numVertices(NumVertices), texture(Texture) {
	}
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};

// The SpriteBatch class is a more efficient way of drawing sprites
class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	// Initializes the spritebatch
	void init();
	void dispose();

	// Begins the spritebatch
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	void beginTris(GlyphSortType sortType = GlyphSortType::TEXTURE);

	// Ends the spritebatch
	void end();
	void endTris();

	// Adds a glyph to the spritebatch
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
	// Adds a glyph to the spritebatch with rotation
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
	// Adds a glyph to the spritebatch with rotation
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

	void drawTriangle(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
	void drawTriangle(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
	// Renders the entire SpriteBatch to the screen
	void renderBatch();

	Vertex topRightVertex() { Glyph glyph; glyph = _glyphs.back(); return glyph.topRight; }

private:
	// Creates all the needed RenderBatches
	void createRenderBatches();
	void createRenderBatchesTris();
	// Generates our VAO and VBO
	void createVertexArray();

	// Sorts glyphs according to _sortType
	void sortGlyphs();
	void sortGlyphTris();

	// Comparators used by sortGlyphs()
	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);

	static bool compareFrontToBackTris(Tri* a, Tri* b);
	static bool compareBackToFrontTris(Tri* a, Tri* b);
	static bool compareTextureTris(Tri* a, Tri* b);

	GLuint _vbo;
	GLuint _vao;

	GlyphSortType _sortType;

	std::vector<Glyph*> _glyphPointers; ///< This is for sorting
	std::vector<Glyph> _glyphs; ///< These are the actual glyphs
	std::vector<Tri*> _triPointers;
	std::vector<Tri> _tris;
	std::vector<RenderBatch> _renderBatches;
};