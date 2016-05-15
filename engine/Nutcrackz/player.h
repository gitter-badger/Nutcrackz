#pragma once

#include "spritebatch.h"
#include "tilesheet.h"
#include "inputmanager.h"
#include "editmode.h"

#include "capsule.h"

enum class MoveState
{
	IDLE,
	RUNNING,
	IN_AIR
};

class Player
{
public:
	void init(b2World *world, glm::vec2 &position, glm::vec2 &drawDims, glm::vec2 &collisionDims, ColorRGBA8 color);
	void destroy(b2World *world);

	void update(InputManager &inputManager);
	void draw(SpriteBatch &spriteBatch, EditorMode editMode);
	void drawDebug(DebugRenderer &debugRenderer);

	const Capsule &getCapsule() const { return m_capsule; }
	glm::vec2 getPosition() const { glm::vec2 rv; rv.x = m_capsule.getBody()->GetPosition().x; rv.y = m_capsule.getBody()->GetPosition().y; return rv; }

	const glm::vec2& getDrawDims() const { return m_drawDims; }
	const glm::vec2& getCollisionDims() const { return m_collisionDims; }
	const ColorRGBA8& getColor() const { return m_color; }

private:
	bool m_onGround = false;

	int m_direction = 1; //1 or -1

	float m_animTime = 0.0f;

	MoveState m_moveState = MoveState::IDLE;

	Capsule m_capsule;

	TileSheet m_texture;
	ColorRGBA8 m_color;

	glm::vec2 m_drawDims;
	glm::vec2 m_collisionDims;
};