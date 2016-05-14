#include "player.h"

#include "resourcemanager.h"

#include <SDL\SDL.h>
#include <iostream>

void Player::init(b2World *world, glm::vec2 &position, glm::vec2 &drawDims, glm::vec2 &collisionDims, ColorRGBA8 color)
{
	GLTexture texture = ResourceManager::getTexture("Assets/Textures/kisi.png");
	m_color = color;
	m_drawDims = drawDims;
	m_collisionDims = collisionDims;

	m_capsule.init(world, position, collisionDims, color, 1.0f, 0.1f, true);

	m_texture.init(texture, glm::ivec2(4, 9));
	//m_collisionBox.init(world, position, dimensions, texture, color, true, glm::vec4(0.25f, 0.9f, 0.25f, 0.1f));
}

void Player::destroy(b2World *world)
{
	m_capsule.destroy(world);
}

void Player::update(InputManager &inputManager)
{
	b2Body *body = m_capsule.getBody();

	if (inputManager.isKeyDown(SDLK_LEFT) || inputManager.isKeyDown(SDLK_a))
	{
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
		m_direction = -1;
	}
	else if (inputManager.isKeyDown(SDLK_RIGHT) || inputManager.isKeyDown(SDLK_d))
	{
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
		m_direction = 1;
	}
	else
	{
		//Apply damping
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.9f, body->GetLinearVelocity().y));
	}

	float maxSpeed = 5.5f;

	if (body->GetLinearVelocity().x < -maxSpeed)
	{
		body->SetLinearVelocity(b2Vec2(-maxSpeed, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > maxSpeed)
	{
		body->SetLinearVelocity(b2Vec2(maxSpeed, body->GetLinearVelocity().y));
	}

	//Always set onGround to false
	m_onGround = false;

	//Loop through all the contact points
	for (b2ContactEdge *ce = body->GetContactList(); ce != nullptr; ce = ce->next)
	{
		b2Contact *cp = ce->contact;

		if (cp->IsTouching())
		{
			b2WorldManifold manifold;
			cp->GetWorldManifold(&manifold);

			//Check if the points are below
			bool isBelow = false;

			for (int i = 0; i < b2_maxManifoldPoints; i++)
			{
				if (manifold.points[i].y < body->GetPosition().y - m_capsule.getDimensions().y / 2.0f + m_capsule.getDimensions().x / 2.0f + 0.01f)
				isBelow = true;
				break;
			}

			if (isBelow)
			{
				m_onGround = true;

				//Now we can jump!
				if (inputManager.isKeyPressed(SDLK_SPACE))
				{
					body->ApplyLinearImpulse(b2Vec2(0.0f, 45.0f), b2Vec2(0.0f, 0.0f), true);
					isBelow = false;
					break;
				}
			}
		}
	}
}

void Player::draw(SpriteBatch &spriteBatch, EditorMode editMode)
{
	glm::vec4 destRect;
	b2Body *body = m_capsule.getBody();

	destRect.x = body->GetPosition().x - m_drawDims.x / 2.0f;
	destRect.y = body->GetPosition().y - m_capsule.getDimensions().y / 2.0f;
	destRect.z = m_drawDims.x;
	destRect.w = m_drawDims.y;

	int tileIndex;
	int numTiles;

	float animSpeed = 0.2f;
	
	glm::vec2 velocity;
	velocity.x = body->GetLinearVelocity().x;
	velocity.y = body->GetLinearVelocity().y;

	if (editMode == EditorMode::EDIT)
	{
		//Is falling
		numTiles = 1;
		tileIndex = 26;

		animSpeed = 0.2f;

		if (m_moveState != MoveState::IN_AIR)
		{
			m_moveState = MoveState::IN_AIR;
			m_animTime = 0.0f;
		}

		m_direction = 1;
	}
	else if (editMode == EditorMode::PLAY)
	{
		//Calculate animation
		if (m_onGround)
		{
			//On ground
			if (abs(velocity.x) > 1.0f && ((velocity.x > 0.0f && m_direction > 0.0f) || (velocity.x < 0.0f && m_direction < 0.0f)))
			{
				//Running
				numTiles = 4;
				tileIndex = 28;
				animSpeed = abs(velocity.x) * 0.075f;

				if (m_moveState != MoveState::RUNNING)
				{
					m_moveState = MoveState::RUNNING;
					m_animTime = 0.0f;
				}
			}
			else
			{
				//Standing
				numTiles = 4;
				//tileIndex = 32 for standing anim
				//tileIndex = 0 for knocked out anim
				tileIndex = 32;

				//animSpeed = 0.125f for standing anim
				//animSpeed = 0.11f for knocked out anim
				animSpeed = 0.125f;

				if (m_moveState != MoveState::IDLE)
				{
					m_moveState = MoveState::IDLE;
					m_animTime = 0.0f;
				}
			}
		}
		else
		{
			//In the air
			if (abs(velocity.x) > 20.0f)
			{
				numTiles = 1;
				tileIndex = 25;

				animSpeed = 0.2f;

				if (m_moveState != MoveState::IN_AIR)
				{
					m_moveState = MoveState::IN_AIR;
					m_animTime = 0.0f;
				}
			}
			else if (velocity.y <= 0.0f)
			{
				//Is falling
				numTiles = 1;
				tileIndex = 26;

				animSpeed = 0.2f;

				if (m_moveState != MoveState::IN_AIR)
				{
					m_moveState = MoveState::IN_AIR;
					m_animTime = 0.0f;
				}
			}
			else
			{
				//Is rising
				numTiles = 1;
				tileIndex = 25;

				animSpeed = 0.2f;

				if (m_moveState != MoveState::IN_AIR)
				{
					m_moveState = MoveState::IN_AIR;
					m_animTime = 0.0f;
				}
			}
		}
	}

	//Increment animation time
	m_animTime += animSpeed;

	// Check for punch end
	//if (m_animTime > numTiles)
	//{
		//m_isPunching = false;
	//}

	if (tileIndex < tileIndex + numTiles)
	{
		//Apply animation
		tileIndex = tileIndex + (int)m_animTime % numTiles;
	}

	//Get uv coords from tile index
	glm::vec4 uvRect = m_texture.getUVs(tileIndex);

	//Check direction
	if (m_direction == -1)
	{
		uvRect.x += 1.0f / m_texture.m_dims.x;
		uvRect.z *= -1;
	}

	//Draw the sprite
	spriteBatch.draw(destRect, uvRect, m_texture.m_texture.id, 0.0f, m_color, body->GetAngle());

	//m_collisionBox.draw(spriteBatch);
}

void Player::drawDebug(DebugRenderer &debugRenderer)
{
	m_capsule.drawDebug(debugRenderer);
}