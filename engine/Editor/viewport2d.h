#pragma once

#include "box.h"
#include "light.h"
#include "player.h"
//#include "waypoint.h"
#include "inputmanager.h"
#include "camera2d.h"
#include "commonerrors.h"

#include "debugrenderer.h"
#include "glslprogram.h"
#include "gltexture.h"
#include "spritebatch.h"
#include "spritefont.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/stc/stc.h>
#include <GLM/glm.hpp>

#include <GL/glu.h>
#include <GL/gl.h>

#include <vector>
//#include <fstream>
#include <string>

const int NO_BOX = -1;
const int NO_LIGHT = -1;

enum class PhysicsMode {
	RIGID,
	DYNAMIC
};

enum class ObjectMode {
	NONE,
	PLAYER,
	PLATFORM,
	PLATFORMPHYSICS,
	LIGHT,
	FINISH
};

enum class SelectionMode {
	SELECT,
	PLACE
};

class Viewport2D : public wxGLCanvas
{
	wxGLContext*	m_context;

public:
	Viewport2D(wxPanel* parent, int* args, long style = 0);
	virtual ~Viewport2D();

	void InitOpenGl();

/*	void build();
	void destroy();
	void onEntry();
	void onExit();*/
	void update();
	//void draw();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	int getWidthOverTwo() { return GetSize().x / 2; }
	int getHeightOverTwo() { return GetSize().y / 2; }

	void render(wxPaintEvent& evt);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomright_x, int bottomright_y);
	GLboolean checkKeys();

	void setOutput(wxStyledTextCtrl &output) { m_output = &output; }
	void message(wxStyledTextCtrl &output, wxString msg);

	//Getters
	bool get2DModelSelected() { return model2DSelected; }
	wxString getPositionX();
	wxString getPositionY();
	wxString getRotation();
	wxString getScale();
	wxString getObject();

	//Setters
	//void setPositionX(glm::vec2 newPos) { position = newPos; }
	void setPositionXY(glm::vec2 newPos) { position = newPos; }
	void setRotation(float newRot) { rotate = newRot; }
	void setScale(float newScale) { scale = newScale; }

	// events
	void OnIdle(wxIdleEvent &evnt);
	void OnEraseBackground(wxEraseEvent &evnt);
	void OnTimer(wxTimerEvent &evnt);
	void OnMouseMoved(wxMouseEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseWheelMoved(wxMouseEvent& event);
	void OnMouseReleased(wxMouseEvent& event);
	void OnMouseReleased2(wxMouseEvent& event);
	void OnRightClick(wxMouseEvent& event);
	void OnMouseLeftWindow(wxMouseEvent& event);
	void OnMouseEvent(wxMouseEvent &ev);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);

private:
	void drawUI();
	void drawWorld();

	void clearLevel();

	void initUI();
	void checkInput();
	void uMouseDown(const SDL_Event& evnt);
	void uMouseUp(const SDL_Event& evnt);
	void uMouseMotion(const SDL_Event& evnt);
	void refreshSelectedBox();
	void refreshSelectedBox(glm::vec2& newPosition);
	void refreshSelectedLight();
	void refreshSelectedLight(glm::vec2& newPosition);
	bool isMouseInUI();
	bool inLightSelect(const Light& l, const glm::vec2& pos);

	void setPlatformWidgetVisibility(bool visible);
	void setLightWidgetVisibility(bool visible);

	/************************************************************************/
	/* Event Handlers                                                       */
	/************************************************************************/

	/************************************************************************/
	/* Member Variables                                                     */
	/************************************************************************/
	float m_colorPickerRed = 255.0f;
	float m_colorPickerGreen = 255.0f;
	float m_colorPickerBlue = 255.0f;
	float m_colorPickerAlpha = 255.0f;
	float m_rotation = 0.0f;
	float m_lightSize = 10.0f;
	glm::vec2 m_boxDims = glm::vec2(1.0f);
	bool m_mouseButtons[2];
	bool m_debugRender = false;
	bool m_showGui = true;
	bool m_isEditing = true;

	SpriteBatch m_spriteBatch;
	SpriteFont m_spriteFont;
	GLSLProgram m_textureProgram;
	GLSLProgram m_lightProgram;
	DebugRenderer m_debugRenderer;

	GLTexture m_blankTexture;

	PhysicsMode m_physicsMode = PhysicsMode::RIGID;
	ObjectMode m_objectMode = ObjectMode::NONE;
	SelectionMode m_selectMode = SelectionMode::SELECT;

	InputManager m_inputManager;

	int selectedBoxNo = 0;
	bool m_hasPlayer = false;
	Player m_player;
	std::vector<Box> m_boxes;
	std::vector<Light> m_lights;
	//std::vector<WayPoint> m_waypoints;
	int m_selectedBox = NO_BOX;
	int m_selectedLight = NO_LIGHT;
	bool m_isDragging = false;
	bool m_isDraggingLeftMouse = false;
	glm::vec2 m_selectOffset;
	glm::vec2 m_mousePos;
	glm::vec2 m_mousePos2;
	glm::vec2 m_mousePos3;
	glm::vec2 minus;
	glm::vec2 m_camPosRel;
	glm::vec4 selectionMarkerRect;
	GLTexture tmpTexture;
	EditorMode m_editMode = EditorMode::EDIT;
	Camera2D m_camera;
	Camera2D m_uiCamera;
	std::unique_ptr<b2World> m_world;
	bool isPlaying = false;
	bool debugPressed;
	bool InitGL;
	SDL_Event evnt;
	wxStyledTextCtrl *m_output;

private:
	bool model2DSelected;
	bool keys[500];

	int mouseX;
	int mouseY;

	int relX;
	int relY;

	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	float rotate;
	float scale;

	bool documentChanged;

	//InputManager m_input;
	Camera2D camera;

	DECLARE_EVENT_TABLE()
};