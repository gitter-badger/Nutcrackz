#include "particlebatch2d.h"

ParticleBatch2D::ParticleBatch2D()
{
}

ParticleBatch2D::~ParticleBatch2D()
{
	delete[] m_particles;
}

void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D &, float)> updateFunc)
{
	m_maxParticles = maxParticles;

	m_particles = new Particle2D[m_maxParticles];

	m_decayRate = decayRate;

	m_texture = texture;

	m_updateFunc = updateFunc;
}

void ParticleBatch2D::update(float deltaTime)
{
	for (int i = 0; i < m_maxParticles; i++)
	{
		//Check if particle is active
		if (m_particles[i].lifeTime > 0.0f)
		{
			m_updateFunc(m_particles[i], deltaTime);
			m_particles[i].lifeTime -= m_decayRate * deltaTime;
		}
	}
}

void ParticleBatch2D::draw(SpriteBatch *spriteBatch)
{
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	for (int i = 0; i < m_maxParticles; i++)
	{
		auto &p = m_particles[i];
		//Check if particle is active
		if (p.lifeTime > 0.0f)
		{
			glm::vec4 destRect(p.position.x, p.position.y, p.width, p.height);

			spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.color);
		}
	}
}

void ParticleBatch2D::addParticle(const glm::vec2 &position, const glm::vec2 &velocity, const ColorRGBA8 &color, float width, float height)
{
	int particleIndex = findFreeParticle();

	auto &p = m_particles[particleIndex];

	p.lifeTime = 1.0f;
	p.position = position;
	p.velocity = velocity;
	p.color = color;
	p.width = width;
	p.height = height;
}

int ParticleBatch2D::findFreeParticle()
{
	for (int i = m_lastFreeParticle; i < m_maxParticles; i++)
	{
		if (m_particles[i].lifeTime <= 0.0f)
		{
			m_lastFreeParticle = i;

			return i;
		}
	}

	for (int i = 0; i < m_lastFreeParticle; i++)
	{
		if (m_particles[i].lifeTime <= 0.0f)
		{
			m_lastFreeParticle = i;

			return i;
		}
	}

	//No particles are free, overwrite very first particle!
	return 0;
}