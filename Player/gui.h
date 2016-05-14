#pragma once

#include <SDL\SDL_events.h>
#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules\OpenGL\GL3Renderer.h>
#include <GLM\glm.hpp>

class GUI
{
public:
	GUI();
	~GUI();

	void init(const std::string &resourceDirectory);
	void destroy();

	void draw();
	void update();

	void setMouseCursor(const std::string &imageFile);
	void showMouseCursor();
	void hideMouseCursor();

	void onSDLEvent(SDL_Event &evnt);

	void loadScheme(const std::string &schemeFile);
	void setFont(const std::string &fontFile);

	CEGUI::Window *createWidget(const std::string &type, const glm::vec4 &destRectPerc, const glm::vec4 &destRectPix, const std::string &name = "");
	CEGUI::Window *createWidget(CEGUI::Window *parent, const std::string &type, const glm::vec4 &destRectPerc, const glm::vec4 &destRectPix, const std::string &name = "");

	static void setWidgetDestRect(CEGUI::Window *widget, const glm::vec4 &destRectPerc, const glm::vec4 &destRectPix);

	static CEGUI::OpenGL3Renderer *getRenderer() { return m_renderer; }
	const CEGUI::GUIContext *getContect() { return m_context; }
	//bool getInitialized() const { return hasInitialized; }

private:
	bool hasInitialized = false;

	unsigned int m_lastTime = 0;

	static CEGUI::OpenGL3Renderer *m_renderer;
	CEGUI::GUIContext *m_context = nullptr;
	CEGUI::Window *m_root = nullptr;
};