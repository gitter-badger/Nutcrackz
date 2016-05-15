
#pragma once
#include <glm\glm.hpp>

#include "time.h"

class Camera3D
{
public:
	Camera3D();
	void mouseUpdate(const glm::vec2& newMousePosition);
	glm::mat4 getWorldToViewMatrix() const;

	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();
	
	void moveForwardX2();
	void moveBackwardX2();
	void strafeLeftX2();
	void strafeRightX2();
	void moveUpX2();
	void moveDownX2();
	
	glm::vec3 getPosition() const { return position; }
	glm::vec3 getViewDirection() const { return viewDirection; }
	glm::vec3 getUP() const { return UP; }

	void setPosition(glm::vec3 value) { position = value; }

	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords, int w, int h, float scale);

	void setCameraDeltaTime(float value) { deltaTime = value; }
	float getCameraDeltaTime() { return deltaTime; }
	
private:
	float deltaTime;
	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 UP;
	glm::vec2 oldMousePosition;
	static const float MOVEMENT_SPEED;
	static const float MOVEMENT_SPEEDX2;
	glm::vec3 strafeDirection;

	float pitch = 0;
};