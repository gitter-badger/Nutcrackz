#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL\glew.h>
#include <GLM\glm.hpp>

#include <string>
#include <vector>
#include <map>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char *vsFilename, const char *fsFilename, GLuint programNumber);
	void use();
	void unuse();

	void setUniform(const GLchar *name, const glm::vec2 &v);
	void setUniform(const GLchar *name, const glm::vec3 &v);
	void setUniform(const GLchar *name, const glm::vec4 &v);

	void setUniform(std::string sName, float* fValues, int iCount);

	GLuint getProgram() const;
	GLint getUniformLocation(const GLchar *name);


private:
	std::string fileToString(const std::string &filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	
	GLuint m_handle;

	std::map<std::string, GLint> mUniformLocations;
};

#endif // !SHADER_PROGRAM_H