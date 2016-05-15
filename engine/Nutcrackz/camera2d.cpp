#include "camera2d.h"

Camera2D::Camera2D() : m_position(0.0f, 0.0f), m_cameraMatrix(1.0f), m_orthoMatrix(1.0f), m_scale(1.0f), m_needsMatrixupdate(true),
m_width(500), m_height(500)
{

}

Camera2D::~Camera2D()
{

}

void Camera2D::init(int scrWidth, int scrHeight)
{
	m_width = scrWidth;
	m_height = scrHeight;

	m_aspectRatio = (float)m_width / (float)m_height;

	m_orthoMatrix = glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height);

	initialized = true;
}

void Camera2D::update()
{
	if (m_needsMatrixupdate)
	{
		glm::vec3 translate(-m_position.x + m_width / 2, -m_position.y + m_height / 2, 0.0f);
		m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

		glm::vec3 scale(m_scale, m_scale, 0.0f);
		m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

		m_needsMatrixupdate = false;
	}
}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords)
{
	//Invert Y coordinate
	screenCoords.y = m_height - screenCoords.y;
	//Make screen center 0
	screenCoords -= glm::vec2(m_width / 2, m_height / 2);
	//Scale the coordinates
	screenCoords /= m_scale;
	//Translate with camera _position
	screenCoords += m_position;

	return screenCoords;
}

bool Camera2D::isBoxInView(const glm::vec2 &position, const glm::vec2 &dimensions)
{
	glm::vec2 scaledScreenDimensions = glm::vec2(m_width, m_height) / (m_scale);

	const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
	const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

	//Center position of the 2D object
	glm::vec2 centerPos = position + dimensions / 2.0f;
	//Center position of the camera
	glm::vec2 centerCameraPos = m_position;
	//Vector from the input to the camera
	glm::vec2 distVec = centerPos - centerCameraPos;

	//Get the depth of the collision
	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	//if this is true then we are colliding
	if (xDepth > 0 && yDepth > 0)
	{
		//There was a collision here
		return true;
	}

	return false;
}