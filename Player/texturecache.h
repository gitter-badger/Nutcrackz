#pragma once

#include "gltexture.h"

#include <map>

class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	GLTexture GetTexture(std::string texturePath);

private:
	std::map<std::string, GLTexture> m_textureMap;
};