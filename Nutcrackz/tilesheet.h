#pragma once

#include "gltexture.h"

#include <GLM\glm.hpp>

class TileSheet
{
public:
	void init(const GLTexture &texture, const glm::ivec2 &tileDims)
	{
		m_texture = texture;
		m_dims = tileDims;
	}

	glm::vec4 getUVs(int index)
	{
		int xTile = index % m_dims.x;
		int yTile = index / m_dims.x;

		glm::vec4 uv;

		uv.x = xTile / (float)m_dims.x;
		uv.y = yTile / (float)m_dims.y;
		uv.z = 1.0f / m_dims.x;
		uv.w = 1.0f / m_dims.y;

		return uv;
	}

	GLTexture m_texture;
	glm::ivec2 m_dims;
};