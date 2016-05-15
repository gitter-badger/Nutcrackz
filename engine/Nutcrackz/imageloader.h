#pragma once

#include "gltexture.h"

#include <string>

class ImageLoader
{
public:
	static GLTexture loadPNG(std::string filePath);
};