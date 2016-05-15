#include "dystopia.h"

#include <SDL\SDL.h>
#include <GL\glew.h>

int initialize()
{
	//init everything in SDL 2.0!
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return 0;
}