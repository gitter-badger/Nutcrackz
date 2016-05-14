#pragma once

#include "gltexture.h"

#include <string>
#include <GL\glew.h>

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height, std::string texturePath, int r = 255, int g = 255, int b = 255, int a = 255);
	void draw();

private:
	float m_x;
	float m_y;
	float m_width;
	float m_height;

	GLuint m_vboID = 0;
	GLTexture m_texture;
};