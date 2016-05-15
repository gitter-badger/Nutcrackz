#pragma once

#include "glslProgram.h"
#include "vertex.h"

#include <GLM\glm.hpp>
#include <vector>

#ifndef PI
#define PI 3.14159265358979323846
#endif // M_PI

class DebugRenderer
{
public:
	DebugRenderer();
	~DebugRenderer();

	void init();
	void end();

	void drawLine(const glm::vec2 &a, const glm::vec2 &b, const ColorRGBA8 color);
	void drawTriangle(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c, const ColorRGBA8 color);
	void drawPrimitiveBox(const glm::vec4 &destRect, const ColorRGBA8 &color, float angle);
	void drawBox(const glm::vec4 &destRect, const ColorRGBA8 &color, float angle);
	void drawCircle(const glm::vec2 &center, const ColorRGBA8 &color, float radius);
	void drawOctagon(const glm::vec2 &center, const ColorRGBA8 &color, float radius);

	void render(const glm::mat4 &projectionMatrix, float lineThickness);
	void dispose();

	struct DebugVertex
	{
		glm::vec2 position;
		ColorRGBA8 color;
	};

private:
	int m_numElements = 0;

	GLSLProgram m_program;

	std::vector<DebugVertex> m_verts;
	std::vector<GLuint> m_indices;
	Vertex vert1, vert2, vert3;

	GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
};