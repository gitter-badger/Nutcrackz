#pragma once

#include "texturecache.h"

#include <string>

class ResourceManager
{
public:
	static GLTexture getTexture(std::string texturePath);

private:
	static TextureCache m_textureCache;
};