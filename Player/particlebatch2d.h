#pragma once

#include "vertex.h"
#include "gltexture.h"
#include "spritebatch.h"

#include <functional>
#include <GLM\glm.hpp>

class Particle2D
{
public:
	float lifeTime = 0.0f;
	float width = 0.0f;
	float height = 0.0f;

	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);

	ColorRGBA8 color;
};

inline void defaultParticleUpdate(Particle2D &particle, float deltaTime)
{
	particle.position += particle.velocity * deltaTime;
}

class ParticleBatch2D
{
public:
	ParticleBatch2D();
	~ParticleBatch2D();

	void init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D &, float)> updateFunc = defaultParticleUpdate);

	void update(float deltaTime);
	void draw(SpriteBatch *spriteBatch);

	void addParticle(const glm::vec2 &position, const glm::vec2 &velocity, const ColorRGBA8 &color, float width, float height);

private:
	int findFreeParticle();

	int m_maxParticles = 0;
	int m_lastFreeParticle = 0;

	float m_decayRate = 0.1f;
		
	std::function<void(Particle2D &, float)> m_updateFunc;

	GLTexture m_texture;

	Particle2D *m_particles = nullptr;
};