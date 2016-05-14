#pragma once

#include "shaderprogram.h"
#include <GL\glew.h>
#include "math3d.h"

struct DirectionalLight
{
	Vector3f Color;
	float AmbientIntensity;
	Vector3f Direction;
	float DiffuseIntensity;
};

class LightingProgram : public ShaderProgram
{
public:

	LightingProgram();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetWorldMatrix(const Matrix4f& WVP);
	void SetTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight &Light);
	
	void worldMatrix(glm::mat4 view, glm::mat4 projection);

	std::string getError() { return error; }

private:

	std::string error;

	GLuint m_WVPLocation;
	GLuint m_samplerLocation;
	GLuint m_WorldMatrixLocation;
	//GLuint m_dirLightColorLocation;
	//GLuint m_dirLightAmbientIntensityLocation;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	GLuint MatrixID;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint Direction;
		GLuint DiffuseIntensity;
	} m_dirLightLocation;
};