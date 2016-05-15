#include "texturecache.h"
#include "imageloader.h"

#include <iostream>

TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}

GLTexture TextureCache::GetTexture(std::string texturePath)
{
	//lookup the texture and see if it is in the map
	auto mit = m_textureMap.find(texturePath);

	//check if texture is not in map
	if (mit == m_textureMap.end())
	{
		//load the texture
		GLTexture newTexture = ImageLoader::loadPNG(texturePath);

		//insert the new texture into the map
		m_textureMap.insert(make_pair(texturePath, newTexture));
			
		return newTexture;
	}

	return mit->second;
}