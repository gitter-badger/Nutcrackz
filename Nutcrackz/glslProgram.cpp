#include "glslProgram.h"
#include "commonerrors.h"
#include "iomanager.h"

#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram()
{
}

GLSLProgram::~GLSLProgram()
{

}

void GLSLProgram::compileShaders(const std::string &vertFilepath, const std::string &fragFilepath)
{
	std::string vertSource;
	std::string fragSource;

	IOManager::readFileToBuffer(vertFilepath, vertSource);
	IOManager::readFileToBuffer(fragFilepath, fragSource);

	compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
}

void GLSLProgram::compileShadersFromSource(const char *vertexSource, const char *fragmentSource)
{
	m_programID = glCreateProgram();

	m_vertexID = glCreateShader(GL_VERTEX_SHADER);

	if (m_vertexID == 0)
	{
		//fatalError("Vertex shader could not be created.");
	}

	m_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	if (m_fragmentID == 0)
	{
		//fatalError("Fragment shader could not be created.");
	}

	compileShader(vertexSource, "Vertex shader", m_vertexID);

	compileShader(fragmentSource, "Fragment shader", m_fragmentID);
}

void GLSLProgram::linkShaders()
{
	glAttachShader(m_programID, m_vertexID);
	glAttachShader(m_programID, m_fragmentID);

	glLinkProgram(m_programID);

	GLint isLinked = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);

		glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(m_programID);

		glDeleteShader(m_vertexID);
		glDeleteShader(m_fragmentID);

		printf("%s\n", &(errorLog[0]));
		//fatalError("Shaders failed to link.");
	}

	glDetachShader(m_programID, m_vertexID);
	glDetachShader(m_programID, m_fragmentID);

	glDeleteShader(m_vertexID);
	glDeleteShader(m_fragmentID);
}

void GLSLProgram::addAttribute(const std::string &attributeName)
{
	glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
}

GLint GLSLProgram::getUniformLocation(const std::string &uniformName)
{
	GLint location = glGetUniformLocation(m_programID, uniformName.c_str());

	if (location == GL_INVALID_INDEX)
	{
		//fatalError("Uniform " + uniformName + " not found in shader.");
	}

	return location;
}

void GLSLProgram::use()
{
	glUseProgram(m_programID);

	for (int i = 0; i < m_numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse()
{
	glUseProgram(0);

	for (unsigned int i = 0; i < m_numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}

	m_numAttributes = 0; //new line
}

void GLSLProgram::compileShader(const char *source, const std::string &name, GLuint id)
{
	glShaderSource(id, 1, &source, nullptr);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);

		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(id);

		printf("%s\n", &(errorLog[0]));
		//fatalError("Shader: " + name + " failed to compile.");
	}
}

void GLSLProgram::dispose()
{
	if (m_programID)
	{
		glDeleteProgram(m_programID);
	}
}