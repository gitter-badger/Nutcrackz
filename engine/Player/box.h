#pragma once

#include "vertex.h"
#include "spritebatch.h"
#include "gltexture.h"

#include <Box2D\Box2D.h>
#include <GLM\glm.hpp>

class Box
{
public:
	void init(b2World *world, glm::vec2 &position, glm::vec2 &dimensions, GLTexture texture, ColorRGBA8 color, bool isDynamic, bool fixedRotation = false, float angle = 0.0f, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	void destroy(b2World *world);

	void draw(SpriteBatch &spriteBatch);

	bool isDynamic() const { return m_body->GetType() == b2_dynamicBody; }

	//Test if a point is inside the box
	bool testPoint(float x, float y) const { return m_fixture->TestPoint(b2Vec2(x, y)); }

	b2Body *getBody() const { return m_body; }
	b2Fixture *getFixture() const { return m_fixture; }
	glm::vec2 getPosition() const { return glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y); }
	const glm::vec2 &getDimensions() const { return m_dimensions; }
	const ColorRGBA8 getColor() const { return m_color; }
	glm::vec4 getUvRect() const { return m_uvRect; }
	float getAngle() const { return m_body->GetAngle(); }
	const GLTexture &getTexture() const { return m_texture; }
	const bool &getFixedRotation() const { return m_fixedRotation; }
	const bool &getIsDynamic() const { return m_isDynamic; }

private:
	bool m_fixedRotation;
	bool m_isDynamic;

	b2Body *m_body = nullptr;
	b2Fixture *m_fixture = nullptr;

	glm::vec2 m_dimensions;
	glm::vec4 m_uvRect;

	ColorRGBA8 m_color;
	GLTexture m_texture;
};