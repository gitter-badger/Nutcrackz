#pragma once

#include <string>
#include <GL\glew.h>

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string &vertFilepath, const std::string &fragFilepath);
	void compileShadersFromSource(const char *vertexSource, const char *fragmentSource);
	void linkShaders();
	void addAttribute(const std::string &attributeName);
	void use();
	void unuse();

	void dispose();

	GLint getUniformLocation(const std::string &uniformName);

private:
	void compileShader(const char *source, const std::string &name, GLuint id);

	int m_numAttributes = 0;

	GLuint m_programID = 0;
	GLuint m_vertexID = 0;
	GLuint m_fragmentID = 0;
};