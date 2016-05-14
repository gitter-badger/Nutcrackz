#pragma once

#include <GL\glew.h>

struct Position2D
{
	float x;
	float y;
};

struct ColorRGBA8
{
	ColorRGBA8()
	{
		r = 255;
		g = 255;
		b = 255;
		a = 255;
	}

	ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}

	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct UV2D
{
	float u;
	float v;
};

struct Vertex2D
{
	// Sprite _position
	Position2D m_position;

	// Sprite _color
	ColorRGBA8 m_color;

	// Sprite UV texture coordinates
	UV2D m_uv;

	void setPosition(float x, float y)
	{
		m_position.x = x;
		m_position.y = y;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		m_color.r = r;
		m_color.g = g;
		m_color.b = b;
		m_color.a = a;
	}

	void setUV(float u, float v)
	{
		m_uv.u = u;
		m_uv.v = v;
	}
};