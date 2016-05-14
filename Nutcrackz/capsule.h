#pragma once

#include "debugrenderer.h"
#include "vertex.h"

#include <Box2D\Box2D.h>
#include <GLM\glm.hpp>

class Capsule
{
public:
	void init(b2World *world, glm::vec2 &position, glm::vec2 &dimensions, ColorRGBA8 &color, float density, float friction, bool fixedRotation = false);
	void destroy(b2World *world);

	void drawDebug(DebugRenderer &debugRenderer);

	b2Body *getBody() const { return m_body; }
	b2Fixture *getFixture(int index) const { return m_fixtures[index]; }

	const glm::vec2 &getDimensions() const { return m_dimensions; }

private:
	b2Body *m_body = nullptr;
	b2Fixture *m_fixtures[3];

	ColorRGBA8 m_color;

	glm::vec2 m_dimensions;
};