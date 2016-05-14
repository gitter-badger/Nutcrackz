#include "lightingprogram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

LightingProgram::LightingProgram()
{
}

bool LightingProgram::Init()
{
	/*if (!ShaderProgram::use)
	{
		error = "Technique failed to init.";

		return false;
	}*/

	if (!loadShaders("Shaders/3D/lighting.vert", "Shaders/3D/lighting.frag", 1))
	{
		error = "Failed to load lighting shaders.";

		return false;
	}

	/*if (!AddShader(GL_FRAGMENT_SHADER, "lighting.fs"))
	{
		error = "Failed to add fragment shader.";

		return false;
	}

	if (!Finalize())
	{
		error = "Failed to finalize.";

		return false;
	}*/

	//m_samplerLocation = getUniformLocation("texture_diffuse1");

	// Get a handle for our "MVP" uniform
	m_WorldMatrixLocation = getUniformLocation("gWorld");
	m_samplerLocation = getUniformLocation("texture_diffuse1");
	//m_dirLightColorLocation = getUniformLocation("gDirectionalLight.Color");
	//m_dirLightAmbientIntensityLocation = getUniformLocation("gDirectionalLight.AmbientIntensity");
	m_dirLightLocation.Color = getUniformLocation("gDirectionalLight.Color");
	m_dirLightLocation.AmbientIntensity = getUniformLocation("gDirectionalLight.AmbientIntensity");
	m_dirLightLocation.Direction = getUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = getUniformLocation("gDirectionalLight.DiffuseIntensity");

	if (m_dirLightLocation.AmbientIntensity == 0xFFFFFFFF)
	{
		error = "Failed gDirectionalLight.AmbientIntensity.";

		return false;
	}

	if (m_WorldMatrixLocation == 0xFFFFFFFF)
	{
		error = "Failed gWorld.";

		return false;
	}

	if (m_samplerLocation == 0xFFFFFFFF)
	{
		error = "Failed texture_diffuse1.";

		return false;
	}

	/*if (m_dirLightColorLocation == 0xFFFFFFFF)
	{
		error = "Failed gDirectionalLight.Color.";

		return false;
	}*/


	if (m_dirLightLocation.Color == 0xFFFFFFFF)
	{
		error = "Failed gDirectionalLight.Color.";

		return false;
	}

	if (m_dirLightLocation.DiffuseIntensity == 0xFFFFFFFF)
	{
		error = "Failed gDirectionalLight.DiffuseIntensity.";

		return false;
	}

	if (m_dirLightLocation.Direction == 0xFFFFFFFF)
	{
		error = "Failed gDirectionalLight.Direction.";

		return false;
	}

	return true;
}

void LightingProgram::worldMatrix(glm::mat4 view, glm::mat4 projection)
{
	m_view = view;
	m_projection = projection;
}

void LightingProgram::SetWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}

void LightingProgram::SetWorldMatrix(const Matrix4f& WorldInverse)
{
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
}

void LightingProgram::SetTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_samplerLocation, TextureUnit);
}

void LightingProgram::SetDirectionalLight(const DirectionalLight& Light)
{
	//glUniform3f(m_dirLightColorLocation, Light.Color.x, Light.Color.y, Light.Color.z);
	//glUniform1f(m_dirLightAmbientIntensityLocation, Light.AmbientIntensity);

	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	Vector3f Direction = Light.Direction;
	Direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}