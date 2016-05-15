#pragma once

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>

class Camera2D
{
public:
	Camera2D();
	~Camera2D();

	void init(int scrWidth, int scrHeight);

	void update();

	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

	bool isBoxInView(const glm::vec2 &position, const glm::vec2 &dimensions);

	void offsetPosition(const glm::vec2 &offset) { m_position += offset; m_needsMatrixupdate = true; }
	void offsetScale(float offset) { m_scale += offset; if (m_scale < 0.001f) m_scale = 0.001f; m_needsMatrixupdate = true; }

	void setPosition(const glm::vec2 &newPosition) { m_position = newPosition; m_needsMatrixupdate = true; }
	void setScale(float newScale) { m_scale = newScale; m_needsMatrixupdate = true; }

	void setAspectRatio(float ratio) { m_aspectRatio = ratio; }

	const glm::vec2 &getPosition() const { return m_position; }
	float getScale() const { return m_scale; }
	const glm::mat4 &getCameraMatrix() const { return m_cameraMatrix; }
	float getAspectRatio() const { return m_aspectRatio; }

private:
	bool m_needsMatrixupdate = true;

	float m_aspectRatio;

	int m_width;
	int m_height;

	float m_scale;

	glm::vec2 m_position;
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_orthoMatrix;
};