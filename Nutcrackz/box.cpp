#include "box.h"

#include "resourcemanager.h"

#include <SDL\SDL.h>
#include <iostream>

Box::Box()
{
}

Box::~Box()
{
}

void Box::init(b2World *world, glm::vec2 &position, glm::vec2 &dimensions, GLTexture texture, ColorRGBA8 color, int boxNumber, bool isDynamic, bool fixedRotation, float angle, glm::vec4 uvRect)
{
	m_boxNumber = boxNumber + 1;
	m_dimensions = dimensions;
	m_position = position;
	m_texture = texture;
	m_color = color;

	m_uvRect = uvRect;
	
	m_fixedRotation = fixedRotation;
	m_isDynamic = isDynamic;

	//Make the body
	b2BodyDef bodyDef;
	if (isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}
	else
	{
		bodyDef.type = b2_staticBody;
	}
	bodyDef.position.Set(m_position.x, m_position.y);
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.angle = angle;

	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	m_fixture = m_body->CreateFixture(&fixtureDef);

	/*glm::vec4 rectX;

	rectX.x = position.x + 1.50f;
	rectX.y = position.y - 0.3f;
	rectX.z = 0.9f;
	rectX.w = 0.6f;

	glm::vec4 rectY;
	
	rectY.x = position.x - 0.45f;
	rectY.y = position.y + 1.65f;
	rectY.z = 0.6f;
	rectY.w = 0.9f;

	triX = new Triangle();
	triY = new Triangle();

	triX->init(world, position, dimensions, 1.5f, 0, rectX, Dystopia::ResourceManager::getTexture("Assets/Textures/blank.png"), Dystopia::ColorRGBA8(255, 0, 0, 200), 0.0f);
	triY->init(world, position, dimensions, 0, 1.5f, rectY, Dystopia::ResourceManager::getTexture("Assets/Textures/blank.png"), Dystopia::ColorRGBA8(0, 255, 0, 200), 0.0f);*/
}

void Box::destroy(b2World *world)
{
	world->DestroyBody(m_body);
}

void Box::draw(SpriteBatch &spriteBatch)
{
	glm::vec4 destRect;
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;

	spriteBatch.draw(destRect, m_uvRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());

	/*triX->draw(spriteBatch);
	triY->draw(spriteBatch);

	if (isCurrentlySelected)
	{
		triX->draw(spriteBatch);
		triY->draw(spriteBatch);
	}*/
}

void Box::move(glm::vec2 newPos, float angle)
{
	if (m_boxNumber != 0)
	{
		/*if (isCurrentlySelected)
		{
			std::cout << "m_boxNumber: " << m_boxNumber << std::endl;
		}*/

		if (isDragging)
		{
			//m_body->SetTransform(b2Vec2(newPos.x, m_body->GetPosition().y), angle); <- VIRKER!
			//m_body->SetTransform(b2Vec2(m_body->GetPosition().x, newPos.y), angle); <- VIRKER!
			m_body->SetTransform(b2Vec2(newPos.x, newPos.y), angle);
		}
	}
}

void Box::moveX(glm::vec2 newPos, float angle)
{
	if (m_boxNumber != 0)
	{
		if (isDragging)
		{
			m_body->SetTransform(b2Vec2(newPos.x, m_body->GetPosition().y), angle);
		}
	}
}

void Box::moveY(glm::vec2 newPos, float angle)
{
	if (m_boxNumber != 0)
	{
		if (isDragging)
		{
			//triY->isDragging = true;

			m_body->SetTransform(b2Vec2(m_body->GetPosition().x, newPos.y), angle);
		}
	}
}

void Box::setSelectedBox()
{
	if (m_boxNumber != 0)
	{
		isCurrentlySelected = true;
	}
}