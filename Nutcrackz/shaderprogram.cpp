#include "shaderprogram.h"

#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram() : m_handle(0)
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_handle);
}

bool ShaderProgram::loadShaders(const char *vsFilename, const char *fsFilename, GLuint programNumber)
{
	std::string vsString = fileToString(vsFilename);
	std::string fsString = fileToString(fsFilename);

	const GLchar *vertexShader = vsString.c_str();
	const GLchar *fragmentShader = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vertexShader, NULL);
	glShaderSource(fs, 1, &fragmentShader, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);
	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);

	m_handle = programNumber;

	m_handle = glCreateProgram();
	glAttachShader(m_handle, vs);
	glAttachShader(m_handle, fs);
	glLinkProgram(m_handle);

	checkCompileErrors(m_handle, PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);

	mUniformLocations.clear();

	return true;
}

void ShaderProgram::use()
{
	if (m_handle > 0)
	{
		glUseProgram(m_handle);
	}	
}

void ShaderProgram::unuse()
{
	glUseProgram(0);
}

std::string ShaderProgram::fileToString(const std::string &filename)
{
	std::stringstream ss;
	std::ifstream file;
	//std::string srcCode = "";

	file.open(filename, std::ios::in);

	try
	{
		if (!file.fail())
		{
			ss << file.rdbuf();
			//std::cout << filename.c_str() << std::endl;
		}

		file.close();
	}
	catch (std::exception ex)
	{
		std::cerr << "Error reading shader filename!" << std::endl;
	}
	
	return ss.str();
	//return srcCode;
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;

	if (type == PROGRAM)
	{
		glGetProgramiv(m_handle, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);

			std::string errorLog(length, ' ');
			glGetProgramInfoLog(m_handle, length, &length, &errorLog[0]);

			std::cerr << "Error! Program failed to link = " << errorLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			std::string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);

			std::cerr << "Error! Shader failed to compile = " << errorLog << std::endl;

			glDeleteShader(shader);
		}
	}
}

GLuint ShaderProgram::getProgram() const
{
	return m_handle;
}

GLint ShaderProgram::getUniformLocation(const GLchar *name)
{
	std::map<std::string, GLint>::iterator it = mUniformLocations.find(name);

	if (it == mUniformLocations.end())
	{
		mUniformLocations[name] = glGetUniformLocation(m_handle, name);
	}

	return mUniformLocations[name];
}

void ShaderProgram::setUniform(const GLchar *name, const glm::vec2 &v)
{
	GLint loc = getUniformLocation(name);

	glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::setUniform(const GLchar *name, const glm::vec3 &v)
{
	GLint loc = getUniformLocation(name);

	glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const GLchar *name, const glm::vec4 &v)
{
	GLint loc = getUniformLocation(name);

	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(std::string sName, float* fValues, int iCount)
{
	int iLoc = glGetUniformLocation(m_handle, sName.c_str());
	glUniform1fv(iLoc, iCount, fValues);
}