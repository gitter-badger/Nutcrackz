#include "resourcemanager.h"

TextureCache ResourceManager::m_textureCache;

GLTexture ResourceManager::getTexture(std::string texturePath)
{
	return m_textureCache.GetTexture(texturePath);
}