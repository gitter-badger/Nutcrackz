#include "camera3d.h"
#include <glm\gtx\transform.hpp>

const float Camera3D::MOVEMENT_SPEED = 0.8f / 1000.0f;
const float Camera3D::MOVEMENT_SPEEDX2 = 0.7f / 1000.0f;

Camera3D::Camera3D() : viewDirection(0.0f, 0.0f, -1.0f), position(0.0f, 0.0f, 0.0f), UP(0.0f, 1.0f, 0.0f)
{
}

void Camera3D::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;

	if (glm::length(mouseDelta) >= 25.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}

	const float ROTATIONAL_SPEED = 0.005f;

	strafeDirection = glm::cross(viewDirection, UP);

	glm::mat4 rotator = glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, UP) * glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, strafeDirection);

	viewDirection = glm::mat3(rotator) * viewDirection;

	oldMousePosition = newMousePosition;
}

glm::mat4 Camera3D::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void Camera3D::moveForward()
{
	position += MOVEMENT_SPEED / deltaTime * viewDirection;
}

void Camera3D::moveBackward()
{
	position += -MOVEMENT_SPEED / deltaTime * viewDirection;
}

void Camera3D::strafeLeft()
{
	position += (-MOVEMENT_SPEED * 1.5f) / deltaTime * strafeDirection;
}

void Camera3D::strafeRight()
{
	position += (MOVEMENT_SPEED * 1.5f) / deltaTime * strafeDirection;
}

void Camera3D::moveUp()
{
	position += MOVEMENT_SPEED / deltaTime * UP;
}

void Camera3D::moveDown()
{
	position += -MOVEMENT_SPEED / deltaTime * UP;
}

void Camera3D::moveForwardX2()
{
	position += MOVEMENT_SPEEDX2 / deltaTime * viewDirection;
}

void Camera3D::moveBackwardX2()
{
	position += -MOVEMENT_SPEEDX2 / deltaTime * viewDirection;
}

void Camera3D::strafeLeftX2()
{
	position += (-MOVEMENT_SPEEDX2 * 1.5f) / deltaTime * strafeDirection;
}

void Camera3D::strafeRightX2()
{
	position += (MOVEMENT_SPEEDX2 * 1.5f) / deltaTime * strafeDirection;
}

void Camera3D::moveUpX2()
{
	position += MOVEMENT_SPEEDX2 / deltaTime * UP;
}

void Camera3D::moveDownX2()
{
	position += -MOVEMENT_SPEEDX2 / deltaTime * UP;
}

glm::vec2 Camera3D::convertScreenToWorld(glm::vec2 screenCoords, int w, int h, float scale)
{
	//Invert Y coordinate
	screenCoords.y = h - screenCoords.y;
	//Make screen center 0
	screenCoords -= glm::vec2(w / 2, h / 2);
	//Scale the coordinates
	screenCoords /= scale;
	//Translate with camera _position
	//screenCoords += m_position;

	return screenCoords;
}
