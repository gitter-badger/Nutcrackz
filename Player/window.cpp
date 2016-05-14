#include "window.h"
#include "commonerrors.h"

Window::Window()
{
}

Window::~Window()
{
}

int Window::create(std::string windowTitle, int width, int height, unsigned int currentFlags)
{
	//m_windowWidth = windowWidth;
	//m_windowHeight = windowHeight - 62;

	//m_scrWidth = width;
	//m_scrHeight = height;

	Uint32 flags = SDL_WINDOW_OPENGL;

	/*if (currentFlags & INVISIBLE)
	{
		flags |= SDL_WINDOW_HIDDEN;
	}

	if (currentFlags & FULLSCREEN)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if (currentFlags & BORDERLESS)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}
		
	if (currentFlags & RESIZABLE)
	{
		flags |= SDL_WINDOW_RESIZABLE;
	}*/

	//SDL_Init(SDL_INIT_EVERYTHING);

	// Setup window
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	/*SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);*/

	m_sdlWindow = SDL_CreateWindow(windowTitle.c_str(), 0, 0, width, height, flags);

	if (m_sdlWindow == nullptr)
	{
		fatalError("SDL Window could not be create.");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);

	if (glContext == nullptr)
	{
		fatalError("SDL GL context could not be created.");
	}

	GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		fatalError("Could not initialize glew.");
	}

	//Check the version of opengl
	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

	//glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	//Set VSYNC!
	SDL_GL_SetSwapInterval(0);

	//Enable alpha blending
	glViewport(0, 0, width, height);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer()
{
	SDL_GL_SwapWindow(m_sdlWindow);
}