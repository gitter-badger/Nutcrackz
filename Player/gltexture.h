#pragma once

#include <GL\glew.h>
#include <string>

struct GLTexture
{
	std::string filePath = "";
	GLuint id;
	int width;
	int height;
};