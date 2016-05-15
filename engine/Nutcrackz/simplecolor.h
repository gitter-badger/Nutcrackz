#pragma once

#include <GL/glew.h>
#include <string>
#include "math3d.h"
#include "shaderprogram.h"

class SimpleColorTechnique : public ShaderProgram
{
public:

	SimpleColorTechnique();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);

	std::string getError() { return error; }

private:
	std::string error;

	GLuint m_WVPLocation;
};