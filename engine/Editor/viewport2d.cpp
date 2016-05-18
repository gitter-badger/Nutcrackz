#include "viewport2d.h"

#include <iostream>
#include "ResourceManager.h"
#include "IOManager.h"
//#include <fstream>

const int MOUSE_LEFT = 0;
const int MOUSE_RIGHT = 1;
const float LIGHT_SELECT_RADIUS = 0.5f;

const b2Vec2 GRAVITY(0.0f, -25.0);

BEGIN_EVENT_TABLE(Viewport2D, wxGLCanvas)
EVT_IDLE(Viewport2D::OnIdle)
EVT_PAINT(Viewport2D::render)
EVT_ERASE_BACKGROUND(Viewport2D::OnEraseBackground)
EVT_MOTION(Viewport2D::OnMouseMoved)
EVT_LEFT_DOWN(Viewport2D::OnMouseDown)
EVT_LEFT_UP(Viewport2D::OnMouseReleased)
EVT_RIGHT_DOWN(Viewport2D::OnRightClick)
EVT_RIGHT_UP(Viewport2D::OnMouseReleased2)
EVT_LEAVE_WINDOW(Viewport2D::OnMouseLeftWindow)
EVT_SIZE(Viewport2D::resized)
EVT_MOUSEWHEEL(Viewport2D::OnMouseWheelMoved)
EVT_ENTER_WINDOW(Viewport2D::OnMouseEvent)
EVT_KEY_DOWN(Viewport2D::OnKeyDown)
EVT_KEY_UP(Viewport2D::OnKeyUp)
//EVT_TIMER(wxID_ANY, BasicGLPane::OnTimer)
END_EVENT_TABLE()

Viewport2D::Viewport2D(wxPanel* parent, int* args, long style) :
	wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, style | wxWANTS_CHARS | wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);
	
	InitOpenGl();

	//prepare2DViewport(0, 0, getWidth(), getHeight());

	//rotate = 0.0f;

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	
	m_spriteFont.init("Assets/Fonts/Suplexmentary_comic.ttf", 32);

	m_mouseButtons[MOUSE_LEFT] = false;
	m_mouseButtons[MOUSE_RIGHT] = false;
	// Init camera

	m_camera.init(getWidth(), getHeight());
	m_camera.setScale(32.0f);

	//m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	//m_camera.setScale(32.0f);
	// UI camera has scale 1.0f
	//m_uiCamera.init(m_window->getWidth(), m_window->getHeight());
	//m_uiCamera.setScale(1.0f);

	m_debugRenderer.init();

	//initUI();
	m_spriteBatch.init();

	// Set up box2D stuff
	m_world = std::make_unique<b2World>(GRAVITY);

	// Shader init
	// Compile our texture shader
	m_textureProgram.compileShaders("Shaders/2D/textureShading.vert", "Shaders/2D/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
	// Compile our light shader
	m_lightProgram.compileShaders("Shaders/2D/lightShading.vert", "Shaders/2D/lightShading.frag");
	m_lightProgram.addAttribute("vertexPosition");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();

	m_blankTexture = ResourceManager::getTexture("assets/textures/blank.png");
	tmpTexture = ResourceManager::getTexture("assets/textures/selection.png");
}

Viewport2D::~Viewport2D()
{
	m_textureProgram.dispose();
	m_spriteFont.dispose();
	m_spriteBatch.dispose();

	clearLevel();
	m_world.reset();

	delete m_context;
}

void Viewport2D::InitOpenGl()
{
	if (InitGL) return;
	//
	while (!IsShown()) {};  // Force the Shown
	wxGLCanvas::SetCurrent(*m_context);

	/// Init OpenGL
	glLoadIdentity();
	GLenum err = glewInit();
	if (err != GL_NO_ERROR)
	{
		wxMessageBox(wxString("GLEW Error: ") + wxString(glewGetErrorString(err)), _("OpenGl ERROR"), wxOK | wxICON_EXCLAMATION);
		exit(4001);
	}
	InitGL = true;
}

void Viewport2D::message(wxStyledTextCtrl &output, wxString msg)
{
	output.SetReadOnly(false);

	output.AppendText(msg + "\n");

	output.SetReadOnly(true);
}

void Viewport2D::update()
{
	m_camera.update();
	//m_uiCamera.update();

	minus = m_camera.convertScreenToWorld(glm::vec2(325.0f, 330.0f));

	checkInput();

	if (m_editMode == EditorMode::PLAY)
	{
		m_player.update(m_inputManager);

		//Update the physics simulation
		m_world->Step(1.0f / 60.0f, 6, 2);

		m_camera.setPosition(glm::vec2(m_player.getPosition().x, m_player.getPosition().y + 10.0f));
		m_camera.setScale(32.0f);
	}

	// Check for deleting an object
	/*if (m_inputManager.isKeyPressed(SDLK_DELETE)) {
		if (m_selectedLight != NO_LIGHT) {
			m_lights.erase(m_lights.begin() + m_selectedLight);
			m_selectedLight = NO_LIGHT;
		}

		for (int i = 0; i < m_boxes.size(); i++)
		{
			if (m_boxes[i].getBoxNumber() <= m_boxes.size())
			{
				if (m_boxes[i].isCurrentlySelected)
				{
					m_boxes[i].destroy(m_world.get());
					m_boxes.erase(m_boxes.begin() + i);
				}
			}
		}
	}*/
}

void Viewport2D::resized(wxSizeEvent& evt)
{
	Refresh();
}

/** Inits the OpenGL viewport for drawing in 2D. */
void Viewport2D::prepare2DViewport(int topleft_x, int topleft_y, int bottomright_x, int bottomright_y)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Black Background
	glEnable(GL_TEXTURE_2D);   // textures
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(topleft_x, topleft_y, bottomright_x - topleft_x, bottomright_y - topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		//exit(1);
		//fatalError("Could not initialize glew.");
	}*/

	/*gluOrtho2D(topleft_x, bottomright_x, bottomright_y, topleft_y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
}

int Viewport2D::getWidth()
{
	return GetSize().x;
}

int Viewport2D::getHeight()
{
	return GetSize().y;
}


void Viewport2D::render(wxPaintEvent& evt)
{
	if (!IsShown()) return;

	if (checkKeys())
	{
		GetGrandParent()->Close(true);
	}

	wxGLCanvas::SetCurrent(*m_context);
	//wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

	update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

	drawWorld();
	drawUI();
	// ------------- draw some 2D ----------------
	prepare2DViewport(0, 0, getWidth(), getHeight());
	//prepare2DViewport(-getWidthOverTwo(), -getHeightOverTwo(), getWidth(), getHeight());
	glLoadIdentity();

	// white background
	/*glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(getWidth(), 0, 0);
	glVertex3f(getWidth(), getHeight(), 0);
	glVertex3f(0, getHeight(), 0);
	glEnd();*/
	
	/*if (!documentChanged && (rotate > getRotation() || rotate < getRotation()))
	{
		documentChanged = true;
	}*/

	//glRotatef(rotate, 0, 0, 1);

	//draw();

	// red square
	//glColor4f(1, 0, 0, 1);
	/*glBegin(GL_QUADS);

	glVertex2f(getWidth() / 2, getHeight() / 3);
	glVertex2f(getWidth() * 3 / 2, getHeight() / 3);
	glVertex2f(getWidth() * 3 / 2, getHeight() * 2 / 3);
	glVertex2f(getWidth() / 2, getHeight() * 2 / 3);*/

	/*glVertex3f(getWidth() / 4, getHeight() / 3, 0);
	glVertex3f(getWidth() * 3 / 4, getHeight() / 3, 0);
	glVertex3f(getWidth() * 3 / 4, getHeight() * 2 / 3, 0);
	glVertex3f(getWidth() / 4, getHeight() * 2 / 3, 0);*/
	//glEnd();

	glFlush();
	SwapBuffers();

	//DisplayFPS();

	wxPaintDC(this);
}

void Viewport2D::OnIdle(wxIdleEvent &evnt)
{
	Refresh();
}

void Viewport2D::OnTimer(wxTimerEvent &evnt)
{
	Refresh();
}

void Viewport2D::OnEraseBackground(wxEraseEvent &evnt)
{
	//Do nothing here to prevent flickering!
}

// some useful events to use
void Viewport2D::OnMouseMoved(wxMouseEvent& event)
{
	const wxPoint pt = event.GetPosition();//wxGetMousePosition();
	mouseX = pt.x;
	mouseY = pt.y;

	wxGetMousePosition(&mouseX, &mouseY);

	relX = pt.x - this->GetSize().x;
	relY = pt.y - this->GetSize().y;

	glm::vec2 mousePos = m_camera.convertScreenToWorld(glm::vec2(mouseX, mouseY));
	glm::vec2 mousePosRel = m_camera.convertScreenToWorld(glm::vec2(relX, relY));

	// If right button is down, drag camera
	const float SPEED = 0.00015f;

	if (m_editMode == EditorMode::EDIT)
	{
		//message(*m_output, "MouseMove is currently being used!");

		m_camPosRel = glm::vec2(mousePos.x, mousePos.y * (m_camera.getAspectRatio()) * (SPEED * 0.05f));
			//m_camPosRel = glm::vec2(-mouseX, mouseY) /** (m_camera.getAspectRatio()*/ * SPEED;

		if (m_mouseButtons[MOUSE_RIGHT])
		{
			//if (mousePos2.x > 0 && mousePos2.y > 0)
			//{
				//m_camPosRel = glm::vec2(-evnt.motion.xrel, evnt.motion.yrel) * (m_camera.getAspectRatio() * SPEED);
				m_camera.offsetPosition(glm::vec2(mousePos.x, mousePos.y * m_camera.getAspectRatio()) * (SPEED * 0.05f));
			//}
			/*if (mousePos.x < 0 && mousePos.y > 0)
			{
				m_camera.offsetPosition(glm::vec2(-mousePos.x, mousePos.y * m_camera.getAspectRatio()) * (SPEED * 0.05f));
			}
			if (mousePos.x < 0 && mousePos.y < 0)
			{
				//m_camPosRel = glm::vec2(-evnt.motion.xrel, evnt.motion.yrel) * (m_camera.getAspectRatio() * SPEED);
				m_camera.offsetPosition(glm::vec2(-mousePos.x, -mousePos.y * m_camera.getAspectRatio()) * (SPEED * 0.05f));
			}
			if (mousePos.x > 0 && mousePos.y < 0)
			{
				m_camera.offsetPosition(glm::vec2(mousePos.x, -mousePos.y * m_camera.getAspectRatio()) * (SPEED * 0.05f));
			}*/

			//wxString myX = std::to_string(mousePos.x);
			//wxString myY = std::to_string(mousePos.y);

			wxString myX = std::to_string(mousePosRel.x);
			wxString myY = std::to_string(mousePosRel.y);
			
			message(*m_output, "mouse pos is currently =" + myX + ", " + myY + "!");
				//m_camera.offsetPosition(glm::vec2(-mouseX, mouseY /** m_camera.getAspectRatio()*/) * SPEED);
		}

		//glm::vec2 mousePos = m_camera.convertScreenToWorld(m_inputManager.getMouseCoords());

		if (m_selectMode == SelectionMode::SELECT)
		{
			glm::vec2 mouseCoords = m_camera.convertScreenToWorld(glm::vec2(mousePos.x, mousePos.y));
			m_mousePos2 = mouseCoords;
		}

		if (m_mouseButtons[MOUSE_LEFT])
		{
			if (m_selectMode == SelectionMode::SELECT)
			{
				if (m_boxes.size() == 0)
				{
					m_isDraggingLeftMouse = true;
				}
				else
				{
					for (int i = 0; i < m_boxes.size(); i++)
					{
						if (m_boxes[i].isCurrentlySelected)
						{
							/*if (m_boxes[i].getBoxNumber() <= m_boxes.size())
							{
							//NYT PROBLEM! det ser ud til at koden her ikke virker! det koden gør er at tage en box og sætte den til en anden box's position så de overlapper 100%
							//LØSNING MANGLES!

							//m_boxes[i].move(m_boxes[i].getPosition(), m_rotationSpinner->getCurrentValue());
							std::cout << "i = " << i << std::endl;
							}*/
							if (m_boxes[i].isDragging)
							{
								if (m_boxes[i].getBoxNumber() <= i)
								{
									m_isDraggingLeftMouse = false;
								}

								glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(mousePos.x, mousePos.y)) - m_boxes[i].selectOffset;
								refreshSelectedBox(pos);
							}
						}
					}
				}
			}
		}

		// Dragging stuff around
		if (m_isDragging && m_mouseButtons[MOUSE_LEFT])
		{
			// Light
			if (m_selectedLight != NO_LIGHT) {
				glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(mousePos.x, mousePos.y)) - m_selectOffset;
				refreshSelectedLight(pos);
			}
			// Box
			/*if (m_selectedBox != NO_BOX) {
			glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(evnt.motion.x, evnt.motion.y)) - m_selectOffset;
			refreshSelectedBox(pos);
			}*/
		}
	}

	if (m_selectMode == SelectionMode::SELECT)
	{
		m_inputManager.setMouseCoords(mousePos.x, mousePos.y);
	}
}

void Viewport2D::OnMouseDown(wxMouseEvent& event)
{
	const wxPoint pt = wxGetMousePosition();
	mouseX = pt.x;
	mouseY = pt.y;

	// Texture for boxes. Its here because lazy.
	static GLTexture texture = ResourceManager::getTexture("assets/textures/light_bricks.png");
	glm::vec2 pos;
	glm::vec4 uvRect;

	if (event.LeftDown())
	{
		if (m_editMode == EditorMode::EDIT)
		{
			//wxLogMessage("You pressed '%i'", event.LeftDown());

			m_mouseButtons[MOUSE_LEFT] = true;

			message(*m_output, "Left mouse button is currently being clicked!");

			// Don't place objects or select things when clicking on UI.
			//if (isMouseInUI()) break;
			if (m_selectMode == SelectionMode::SELECT)
			{
				//int mx;
				//int my;

				//SDL_GetMouseState(&mx, &my);
				//glm::vec2 minus = m_camera.convertScreenToWorld(glm::vec2(325.0f, 330.0f));
				// Select mode
				pos = m_camera.convertScreenToWorld(glm::vec2(mouseX, mouseY));

				glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
				m_mousePos = m_camera.convertScreenToWorld(glm::vec2(mouseCoords.x, mouseCoords.y));

				// Lights have selection priority, so check lights first
				// If a light is already selected, test to see if we just clicked it again
				if (m_selectedLight != NO_LIGHT && inLightSelect(m_lights[m_selectedLight], pos + minus))
				{
					// We selected ourself, do nothing.
				}
				else
				{
					// Unselect
					m_selectedLight = NO_LIGHT;
					// Find the box that we are selecting
					for (size_t i = 0; i < m_lights.size(); i++)
					{
						if (inLightSelect(m_lights[i], pos + minus))
						{
							m_selectedLight = i;
							break;
						}
					}
				}
				// If we selected a light
				if (m_selectedLight != NO_LIGHT)
				{
					// Get the offset from the center so we can drag correctly
					m_selectOffset = pos - m_lights[m_selectedLight].position;
					m_selectedBox = NO_BOX;
					m_isDragging = true;
					// Set variables first so refreshing the controls works
					m_colorPickerRed = m_lights[m_selectedLight].color.r;
					m_colorPickerGreen = m_lights[m_selectedLight].color.g;
					m_colorPickerBlue = m_lights[m_selectedLight].color.b;
					m_colorPickerAlpha = m_lights[m_selectedLight].color.a;
					m_lightSize = m_lights[m_selectedLight].size;
					// Set widget values
					/*m_rSlider->setCurrentValue(m_lights[m_selectedLight].color.r);
					m_gSlider->setCurrentValue(m_lights[m_selectedLight].color.g);
					m_bSlider->setCurrentValue(m_lights[m_selectedLight].color.b);
					m_aSlider->setCurrentValue(m_lights[m_selectedLight].color.a);
					m_sizeSpinner->setCurrentValue(m_lights[m_selectedLight].size);
					m_lightRadioButton->setSelected(true);*/
					m_objectMode = ObjectMode::LIGHT;
				}

				for (auto& b : m_boxes)
				{
					glm::vec2 boxDimension = glm::vec2(b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f - minus.x, b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f - minus.y);

					if (pos.x <= (boxDimension.x + b.getDimensions().x) && pos.y <= (boxDimension.y + b.getDimensions().y) && pos.x >= boxDimension.x && pos.y >= boxDimension.y)
					{
						m_isDraggingLeftMouse = false;
						//refreshSelectedBox(m_mousePos);
						b.hasSelectedBox = true;
						b.isDragging = true;
						b.setSelectedBox();
					}
					else
					{
						m_isDraggingLeftMouse = true;
						b.isDragging = false;
						b.hasSelectedBox = false;
						b.isCurrentlySelected = false;
					}

					// If a box is already selected, test to see if we just clicked it again
					if (b.isCurrentlySelected && b.testPoint(pos.x, pos.y))
					{
						// We selected ourself, do nothing.
					}
					/*else {
					// Unselect
					//m_selectedBox = NO_BOX;
					// Find the box that we are selecting
					for (size_t i = 0; i < m_boxes.size(); i++) {
					if (m_boxes[i].testPoint(pos.x, pos.y)) {
					b.hasSelectedBox = false;
					//m_selectedBox = i;
					break;
					}
					}
					if (b.hasSelectedBox)
					{
					b.hasSelectedBox = false;
					}
					}*/
					// If we selected a box
					/*if (m_selectedBox != NO_BOX) {
					// Get the offset from the center so we can drag correctly
					m_selectOffset = pos - m_boxes[m_selectedBox].getPosition();
					m_selectedLight = NO_LIGHT;
					m_isDragging = true;
					// Set variables first so refreshing the controls works
					m_rotation = m_boxes[m_selectedBox].getBody()->GetAngle();
					m_boxDims.x = m_boxes[m_selectedBox].getDimensions().x;
					m_boxDims.y = m_boxes[m_selectedBox].getDimensions().y;
					m_colorPickerRed = m_boxes[m_selectedBox].getColor().r;
					m_colorPickerGreen = m_boxes[m_selectedBox].getColor().g;
					m_colorPickerBlue = m_boxes[m_selectedBox].getColor().b;
					// Set widget values
					m_rotationSpinner->setCurrentValue(m_boxes[m_selectedBox].getBody()->GetAngle());
					m_widthSpinner->setCurrentValue(m_boxes[m_selectedBox].getDimensions().x);
					m_heightSpinner->setCurrentValue(m_boxes[m_selectedBox].getDimensions().y);
					m_rSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().r);
					m_gSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().g);
					m_bSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().b);
					if (m_boxes[m_selectedBox].isDynamic()) {
					m_dynamicRadioButton->setSelected(true);
					m_physicsMode = PhysicsMode::DYNAMIC;
					}
					else {
					m_rigidRadioButton->setSelected(true);
					m_physicsMode = PhysicsMode::RIGID;
					}
					m_platformRadioButton->setSelected(true);
					m_objectMode = ObjectMode::PLATFORM;
					*/

					/*b.selectOffset = m_mousePos;

					std::cout << b.selectOffset.x << ", " << b.selectOffset.y << std::endl;*/

					if (b.isCurrentlySelected)
					{
						// Get the offset from the center so we can drag correctly
						b.selectOffset = m_mousePos - b.getPosition();//glm::vec2(b.getBody()->GetPosition().x, b.getBody()->GetPosition().y);
																	  //m_selectedLight = NO_LIGHT;
						m_isDraggingLeftMouse = false;
						//b.isDragging = true;
						// Set variables first so refreshing the controls works
						m_rotation = b.getBody()->GetAngle();
						//m_boxDims.x = b.getDimensions().x;
						//m_boxDims.y = b.getDimensions().y;
						//m_colorPickerRed = b.getColor().r;
						//m_colorPickerGreen = b.getColor().g;
						//m_colorPickerBlue = b.getColor().b;
						// Set widget values
						//m_rotationSpinner->setCurrentValue(b.getBody()->GetAngle());
						//m_widthSpinner->setCurrentValue(m_boxes[m_selectedBox].getDimensions().x);
						//m_heightSpinner->setCurrentValue(m_boxes[m_selectedBox].getDimensions().y);
						//m_rSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().r);
						//m_gSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().g);
						//m_bSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().b);
						/*if (b.isDynamic()) {
						m_dynamicRadioButton->setSelected(true);
						m_physicsMode = PhysicsMode::DYNAMIC;
						}
						else {
						m_rigidRadioButton->setSelected(true);
						m_physicsMode = PhysicsMode::RIGID;
						}*/
						//m_platformRadioButton->setSelected(true);
						//m_objectMode = ObjectMode::PLATFORM;
					}
				}
			}
			else
			{
				// Place mode
				Box *newBox;
				Light newLight;
				//WayPoint newWaypoint;
				// Place
				//glm::vec2 minus = m_camera.convertScreenToWorld(glm::vec2(325.0f, 330.0f));
				if (m_objectMode == ObjectMode::PLATFORM)
				{
					newBox = new Box();
					pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					uvRect.x = pos.x;
					uvRect.y = pos.y;
					uvRect.z = m_boxDims.x;
					uvRect.w = m_boxDims.y;
					newBox->init(m_world.get(), pos + minus, m_boxDims, texture, ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255), m_boxes.size(),
						false, false, m_rotation, uvRect);
					m_boxes.push_back(*newBox);
				}
				if (m_objectMode == ObjectMode::PLATFORMPHYSICS)
				{
					newBox = new Box();
					pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					uvRect.x = pos.x;
					uvRect.y = pos.y;
					uvRect.z = m_boxDims.x;
					uvRect.w = m_boxDims.y;
					newBox->init(m_world.get(), pos + minus, m_boxDims, texture, ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255), m_boxes.size(),
						true, false, m_rotation, uvRect);
					m_boxes.push_back(*newBox);
				}
				if (m_objectMode == ObjectMode::LIGHT)
				{
					newLight.position = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y)) + minus;
					newLight.size = m_lightSize;
					newLight.color = ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, (GLubyte)m_colorPickerAlpha);
					m_lights.push_back(newLight);
				}
				if (m_objectMode == ObjectMode::PLAYER)
				{
					// Just remove the player, easiest way.
					m_player.destroy(m_world.get());
					// Re-init the player
					pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					m_player.init(m_world.get(), pos + minus, glm::vec2(1.9f, 2.5f), glm::vec2(1.3f, 2.2f), ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255));
					m_hasPlayer = true;
				}

				switch (m_objectMode)
				{
					/*case ObjectMode::FINISH:
					// TODO: Implement
					newWaypoint.position = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					newWaypoint.init(m_world.get(), pos + minus, glm::vec2(1.0f, 1.0f), ColorRGBA8(0, 0, 255, 255));
					m_waypoints.push_back(newWaypoint);
					break;*/
				}
			}
		}
	}

	SetFocus();

	//wxLogMessage("Mouse X Position = %i", x);
	//wxLogMessage("Mouse Y Position = %i", y);
}

void Viewport2D::OnMouseWheelMoved(wxMouseEvent& event)
{
	if (event.GetEventType() == wxEVT_MOUSEWHEEL)
	{
		//event.getw

		event.m_wheelAxis = wxMOUSE_WHEEL_VERTICAL;

		//event.GetWheelDelta();

		// Which way are we scrolling 
		bool scrollDown = event.GetWheelRotation() <= 0;
		bool scrollUp = event.GetWheelRotation() >= 0;

		if (m_editMode == EditorMode::EDIT)
		{
			// Linear scaling sucks for mouse wheel zoom so we multiply by getScale() instead.
			if (scrollUp)
			{
					m_camera.offsetScale(m_camera.getScale() * /*evnt.wheel.y*/ event.GetWheelRotation() * ((0.1f * 0.15f) * 0.1f));

					//float offset = m_camera.getScale() * /*evnt.wheel.y*/ event.GetWheelRotation() * (0.1f * 0.15f);
					float offset = event.GetWheelRotation();

					wxString myString = std::to_string(offset);

					message(*m_output, "mouse wheel is currently +=" + myString + "!");
			}
			else if (scrollDown)
			{
					m_camera.offsetScale(-m_camera.getScale() * /*evnt.wheel.y*/ -event.GetWheelRotation() * ((-0.1f * -0.15f) * 0.1f));

					float offset = event.GetWheelRotation();

					wxString myString = std::to_string(offset);

					message(*m_output, "mouse wheel is currently -=" + myString + "!");
			}
		}
	}
}

void Viewport2D::OnMouseReleased(wxMouseEvent& event)
{
	if (m_editMode == EditorMode::EDIT)
	{
		m_mouseButtons[MOUSE_LEFT] = false;
		m_isDraggingLeftMouse = false;
	}
}

void Viewport2D::OnMouseReleased2(wxMouseEvent& event)
{
	if (m_editMode == EditorMode::EDIT)
	{
		m_mouseButtons[MOUSE_RIGHT] = false;
	}
}

void Viewport2D::OnRightClick(wxMouseEvent& event)
{
	if (event.RightDown())
	{
		if (m_objectMode == ObjectMode::PLAYER)
		{
			std::cout << "PLAYER" << std::endl;
		}

		//rightMouseButtonPressed = true;

		m_mouseButtons[MOUSE_RIGHT] = true;

		message(*m_output, "Right mouse button is currently being clicked!");
	}

	SetFocus();
}

void Viewport2D::OnMouseLeftWindow(wxMouseEvent& event)
{

}

void Viewport2D::OnMouseEvent(wxMouseEvent &ev)
{

}

GLboolean Viewport2D::checkKeys()
{
	static long lastTime = (long)wxGetLocalTimeMillis().GetLo();

	const GLfloat speed = 1.0f;
	const long updateTime = 10;

	/*if (keys[WXK_ESCAPE])
	return true;*/

	long newTime = (long)wxGetLocalTimeMillis().GetLo();

	if (newTime - lastTime > updateTime)
	{
		if (keys[WXK_LEFT])
		{
			rotate -= speed;
		}
		if (keys[WXK_RIGHT])
		{
			rotate += speed;
		}
		/*if (keys[WXK_UP])
		{
			rotateX -= speed;
		}
		if (keys[WXK_DOWN])
		{
			rotateX += speed;
		}*/
	}

	return false;
}

void Viewport2D::OnKeyDown(wxKeyEvent &evnt)
{
	long key = evnt.GetKeyCode();

	keys[key] = true;

	evnt.Skip();
}

void Viewport2D::OnKeyUp(wxKeyEvent &evnt)
{
	long key = evnt.GetKeyCode();

	keys[key] = false;

	evnt.Skip();
}

wxString Viewport2D::getPositionX()
{
	std::string x = std::to_string(position.x);
	std::string tmpString = "Position X: " + x;
	return tmpString;
}

wxString Viewport2D::getPositionY()
{
	std::string y = std::to_string(position.y);
	std::string tmpString = "Position Y: " + y;
	return tmpString;
}

wxString Viewport2D::getRotation()
{
	std::string tmpRot = std::to_string(rotate);
	std::string tmpString = "Rotation: " + tmpRot;
	return tmpString;
}

wxString Viewport2D::getScale()
{
	std::string tmpScl = std::to_string(scale);
	std::string tmpString = "Scale: " + tmpScl;
	return tmpString;
}

wxString Viewport2D::getObject()
{
	std::string x = std::to_string(position.x);
	std::string tmpString1 = "Position X: " + x + "\n";

	std::string y = std::to_string(position.y);
	std::string tmpString2 = "Position Y: " + y + "\n";

	std::string tmpRot = std::to_string(rotate);
	std::string tmpString3 = "Rotation: " + tmpRot + "\n";

	std::string tmpScl = std::to_string(scale);
	std::string tmpString4 = "Scale: " + tmpScl + "\n";

	std::string endString = tmpString1 + tmpString2 + tmpString3 + tmpString4;

	return endString;
}

void Viewport2D::drawUI() 
{
	if (m_editMode == EditorMode::EDIT)
	{
		// Outlines
		if (m_selectMode == SelectionMode::PLACE /*&& !isMouseInUI()*/)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(x, y));
			//glm::vec2 minus = m_camera.convertScreenToWorld(glm::vec2(325.0f, 330.0f));
			// Draw the object placement outlines
			if (m_objectMode == ObjectMode::PLATFORM || m_objectMode == ObjectMode::PLATFORMPHYSICS)
			{
				m_debugRenderer.drawPrimitiveBox(glm::vec4(pos + minus - m_boxDims * 0.5f, m_boxDims), ColorRGBA8(255, 255, 255, 200), m_rotation);
				m_debugRenderer.end();
				m_debugRenderer.render(m_camera.getCameraMatrix(), 2.0f);
			}
			else if (m_objectMode == ObjectMode::LIGHT)
			{
				// Make temporary light to render
				Light tmpLight;
				tmpLight.position = pos + minus;
				tmpLight.color = ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, (GLubyte)m_colorPickerAlpha);
				tmpLight.size = m_lightSize;

				// Draw light
				m_lightProgram.use();
				GLint pUniform = m_textureProgram.getUniformLocation("P");
				glUniformMatrix4fv(pUniform, 1, GL_FALSE, &m_camera.getCameraMatrix()[0][0]);
				// Additive blending
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				m_spriteBatch.begin();
				tmpLight.draw(m_spriteBatch);
				m_spriteBatch.end();
				m_spriteBatch.renderBatch();
				m_lightProgram.unuse();
				// Restore alpha blending
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				// Selection radius
				m_debugRenderer.drawCircle(pos + minus, ColorRGBA8(255, 255, 255, 255), LIGHT_SELECT_RADIUS);
				// Outer radius
				m_debugRenderer.drawCircle(pos + minus, tmpLight.color, tmpLight.size);
				m_debugRenderer.end();
				m_debugRenderer.render(m_camera.getCameraMatrix(), 2.0f);
			}
			else if (m_objectMode == ObjectMode::FINISH)
			{
				// radius
				m_debugRenderer.drawOctagon(pos + minus, ColorRGBA8(255, 255, 255, 200), 0.25f);
				m_debugRenderer.end();
				m_debugRenderer.render(m_camera.getCameraMatrix(), 2.0f);
			}
		}
		else 
		{
			for (auto& b : m_boxes)
			{
				glm::vec4 destRect;

				//glm::vec2 minus = m_camera.convertScreenToWorld(glm::vec2(325.0f, 330.0f));

				destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f - minus.x;
				destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f - minus.y;
				destRect.z = b.getDimensions().x;
				destRect.w = b.getDimensions().y;

				if (m_isDraggingLeftMouse && m_mouseButtons[MOUSE_LEFT] || b.isDragging)
				{
					//if (!b.hasSelectedBox)
					//{
					if (m_mousePos.x < (destRect.x + b.getDimensions().x) && m_mousePos.y < (destRect.y + b.getDimensions().y))
					{
						if (m_mousePos2.x >(destRect.x + b.getDimensions().x) && m_mousePos2.y >(destRect.y + b.getDimensions().y))
						{
							b.hasSelectedBox = true;
							b.setSelectedBox();
						}
					}

					if (m_mousePos.x > destRect.x && m_mousePos.y > destRect.y)
					{
						if (m_mousePos2.x < destRect.x && m_mousePos2.y < destRect.y)
						{
							b.hasSelectedBox = true;
							b.setSelectedBox();
						}
					}

					if (m_mousePos.x > destRect.x && m_mousePos.y < (destRect.y + b.getDimensions().y))
					{
						if (m_mousePos2.x < destRect.x && m_mousePos2.y >(destRect.y + b.getDimensions().y))
						{
							b.hasSelectedBox = true;
							b.setSelectedBox();
						}
					}

					if (m_mousePos.x < (destRect.x + b.getDimensions().x) && m_mousePos.y > destRect.y)
					{
						if (m_mousePos2.x > (destRect.x + b.getDimensions().x) && m_mousePos2.y < destRect.y)
						{
							b.hasSelectedBox = true;
							b.setSelectedBox();
						}
					}
					//}
				}
			}

			// Draw selected light
			if (m_selectedLight != NO_LIGHT)
			{
				// Selection radius
				m_debugRenderer.drawCircle(m_lights[m_selectedLight].position, ColorRGBA8(255, 255, 0, 255), LIGHT_SELECT_RADIUS);
				// Outer radius
				m_debugRenderer.drawCircle(m_lights[m_selectedLight].position, m_lights[m_selectedLight].color, m_lights[m_selectedLight].size);
				m_debugRenderer.end();
				m_debugRenderer.render(m_camera.getCameraMatrix(), 2.0f);
			}
			// Draw selected box
			/*if (m_selectedBox != NO_BOX) {
			const Box& b = m_boxes[m_selectedBox];
			glm::vec4 destRect;
			destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
			destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
			destRect.z = b.getDimensions().x;
			destRect.w = b.getDimensions().y;

			m_debugRenderer.drawBox(destRect, Dystopia::ColorRGBA8(255, 255, 0, 255), b.getBody()->GetAngle());
			m_debugRenderer.end();
			m_debugRenderer.render(m_camera.getCameraMatrix(), 2.0f);
			}*/

			for (auto& b : m_boxes)
			{
				if (b.hasSelectedBox)
				{
					glm::vec4 destRect;

					destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
					destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
					destRect.z = b.getDimensions().x;
					destRect.w = b.getDimensions().y;

					m_debugRenderer.drawBox(destRect, ColorRGBA8(255, 0, 0, 255), b.getBody()->GetAngle());

					float x = b.getBody()->GetPosition().x;
					float y = b.getBody()->GetPosition().y;

					/*float mx;
					float my;

					int m_x;
					int m_y;

					SDL_GetMouseState(&m_x, &m_y);

					mx = (float)m_x;
					my = (float)m_y;

					if (mx >= x + 1.5f && mx <= x + 1.85f && my >= y - 0.15f && my <= y + 0.15f)
					return true;*/

					m_textureProgram.use();

					// Camera matrix
					glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
					GLint pUniform = m_textureProgram.getUniformLocation("P");
					glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

					m_spriteBatch.beginTris();

					glm::vec4 destRectX;
					// Sorry for this
					destRectX.x = x + 1.50f;
					destRectX.y = y - 0.3f;
					destRectX.z = 0.9f;
					destRectX.w = 0.6f;

					m_spriteBatch.drawTriangle(destRectX, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_blankTexture.id, 0.0f, ColorRGBA8(255, 0, 0, 200), 0);

					glm::vec4 destRectY;
					// Sorry for this
					destRectY.x = x - 0.45f;
					destRectY.y = y + 1.65f;
					destRectY.z = 0.9f;
					destRectY.w = 0.6f;

					m_spriteBatch.drawTriangle(destRectY, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_blankTexture.id, 0.0f, ColorRGBA8(0, 255, 0, 200), 1.57f);

					m_spriteBatch.endTris();
					m_spriteBatch.renderBatch();
					m_textureProgram.unuse();

					m_debugRenderer.drawLine(glm::vec2(x, y), glm::vec2(x + 1.5f, y), ColorRGBA8(255, 0, 0, 200));
					//m_debugRenderer.drawTriangle(glm::vec2(x + 1.5f, y - 0.15f), glm::vec2(x + 1.5f, y + 0.15f), glm::vec2(x + 1.85f, y), Dystopia::ColorRGBA8(255, 0, 0, 200));

					m_debugRenderer.drawLine(glm::vec2(x, y), glm::vec2(x, y + 1.5f), ColorRGBA8(0, 255, 0, 200));
					//m_debugRenderer.drawTriangle(glm::vec2(x - 0.15f, y + 1.5f), glm::vec2(x + 0.15f, y + 1.5f), glm::vec2(x, y + 1.85f), Dystopia::ColorRGBA8(0, 255, 0, 200));

					m_debugRenderer.end();
					m_debugRenderer.render(m_camera.getCameraMatrix(), 2.0f);
				}
			}
		}
	}

	m_textureProgram.use();

	// Camera matrix
	glm::mat4 projectionMatrix = m_uiCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	if (m_editMode == EditorMode::EDIT)
	{
		m_spriteBatch.begin();

		/*{ // Draw the color picker quad
		const float QUAD_SIZE = 75.0f;

		glm::vec4 destRect;
		// Sorry for this
		destRect.x = m_aSlider->getXPosition().d_scale * m_window->getWidth() + 8.0f - m_window->getWidth() / 2.0f + QUAD_SIZE / 2.0f;
		destRect.y = m_window->getHeight() / 2.0f - m_aSlider->getYPosition().d_scale * m_window->getHeight() -
		m_aSlider->getHeight().d_scale * m_window->getHeight() * 0.75f - QUAD_SIZE / 4.0f;
		destRect.z = QUAD_SIZE;
		destRect.w = QUAD_SIZE;

		if (m_showGui)
		{
		m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_blankTexture.id, 0.0f, Dystopia::ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255));

		// Draw color text
		std::string colorText;
		if (m_objectMode == ObjectMode::LIGHT) {
		colorText = "RGBA[" + std::to_string((int)m_colorPickerRed) + "," + std::to_string((int)m_colorPickerGreen) + "," + std::to_string((int)m_colorPickerBlue) + "," + std::to_string((int)m_colorPickerAlpha) + "]";
		}
		else {
		colorText = "RGB[" + std::to_string((int)m_colorPickerRed) + "," + std::to_string((int)m_colorPickerGreen) + "," + std::to_string((int)m_colorPickerBlue) + "]";
		}

		m_spriteFont.draw(m_spriteBatch, colorText.c_str(), glm::vec2(destRect.x + destRect.z * 0.5f, destRect.y + destRect.w), glm::vec2(0.55f), 0.0f, Dystopia::ColorRGBA8(255, 255, 255, 255), Dystopia::Justification::MIDDLE);
		}
		}*/
		// Draw custom labels for widgets	
		//for (auto& l : m_widgetLabels) l.draw(m_spriteBatch, m_spriteFont, m_window);

		m_spriteBatch.end();
		m_spriteBatch.renderBatch();
		m_textureProgram.unuse();
	}

	if (m_editMode == EditorMode::EDIT)
	{
		//m_gui.draw();
	}
	else if (m_editMode == EditorMode::PLAY)
	{
		//m_playGui.draw();
	}
}

void Viewport2D::drawWorld() {
	m_textureProgram.use();

	// Upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	// Camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	{ // Draw all the boxes and the player
		m_spriteBatch.begin();
		for (auto& b : m_boxes) 
		{
			b.draw(m_spriteBatch);
		}
		if (m_hasPlayer) m_player.draw(m_spriteBatch, m_editMode);

		m_spriteBatch.end();
		m_spriteBatch.renderBatch();
		m_textureProgram.unuse();
	}

	{ // Draw lights
		m_lightProgram.use();
		pUniform = m_textureProgram.getUniformLocation("P");
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		// Additive blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		m_spriteBatch.begin();

		for (auto& l : m_lights) l.draw(m_spriteBatch);

		m_spriteBatch.end();
		m_spriteBatch.renderBatch();

		m_lightProgram.unuse();

		// Restore alpha blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if (m_selectMode == SelectionMode::SELECT)
	{
		if (m_isDraggingLeftMouse && m_mouseButtons[MOUSE_LEFT])
		{
			if (m_boxes.size() > 0)
			{
				for (auto& b : m_boxes)
				{
					glm::vec2 boxDimension = glm::vec2(b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f, b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f);

					if (!b.isDragging)
					{
						float textureWidth = m_mousePos.x - m_camPosRel.x;//m_mousePos2.x;
						float textureHeight = m_mousePos.y - m_camPosRel.y;//m_mousePos2.y;
																		   //glm::vec2 minus = m_camera.convertScreenToWorld(glm::vec2(325.0f, 330.0f));

						glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

						selectionMarkerRect.x = m_mousePos.x + m_camPosRel.x;
						selectionMarkerRect.y = m_mousePos.y + m_camPosRel.y;
						selectionMarkerRect.z = -textureWidth;
						selectionMarkerRect.w = -textureHeight;

						m_textureProgram.use();

						textureUniform = m_textureProgram.getUniformLocation("mySampler");
						glUniform1i(textureUniform, 0);
						glActiveTexture(GL_TEXTURE0);

						pUniform = m_textureProgram.getUniformLocation("P");
						glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

						m_spriteBatch.begin();

						m_spriteBatch.draw(selectionMarkerRect, uvRect, tmpTexture.id, 0.0f, ColorRGBA8(), 0.0f);

						m_spriteBatch.end();
						m_spriteBatch.renderBatch();

						m_textureProgram.unuse();
					}
				}
			}
			else
			{
				float textureWidth = m_mousePos.x - m_mousePos2.x;
				float textureHeight = m_mousePos.y - m_mousePos2.y;

				glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

				selectionMarkerRect.x = m_mousePos.x + minus.x;
				selectionMarkerRect.y = m_mousePos.y + minus.y;

				//glm::vec2 minus2 = m_camera.convertScreenToWorld(glm::vec2(selectionMarkerRect.x, selectionMarkerRect.y));
				selectionMarkerRect.z = -textureWidth;
				selectionMarkerRect.w = -textureHeight;

				m_textureProgram.use();

				textureUniform = m_textureProgram.getUniformLocation("mySampler");
				glUniform1i(textureUniform, 0);
				glActiveTexture(GL_TEXTURE0);

				pUniform = m_textureProgram.getUniformLocation("P");
				glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

				m_spriteBatch.begin();

				m_spriteBatch.draw(selectionMarkerRect, uvRect, tmpTexture.id, 0.0f, ColorRGBA8(), 0.0f);
				//m_spriteBatch.draw(glm::vec4(minus2.x, minus2.y, selectionMarkerRect.z, selectionMarkerRect.w), uvRect, tmpTexture.id, 0.0f, Dystopia::ColorRGBA8(), 0.0f);

				m_spriteBatch.end();
				m_spriteBatch.renderBatch();

				m_textureProgram.unuse();
			}
		}
	}

	// Debug rendering
	//if (debugPressed)
	//{
		// Player collision
		if (m_hasPlayer) m_player.drawDebug(m_debugRenderer);
		// Render dynamic box borders
		glm::vec4 destRect;
		for (auto& b : m_boxes)
		{
			destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
			destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
			destRect.z = b.getDimensions().x;
			destRect.w = b.getDimensions().y;
			// Dynamic is green, static is red.
			if (b.isDynamic()) {
				m_debugRenderer.drawBox(destRect, ColorRGBA8(0, 255, 0, 255), b.getBody()->GetAngle());
			}
			else {
				m_debugRenderer.drawBox(destRect, ColorRGBA8(255, 0, 0, 255), b.getBody()->GetAngle());
			}
		}
		// Render magenta light selection radius
		for (auto& l : m_lights)
		{
			m_debugRenderer.drawCircle(l.position, ColorRGBA8(255, 0, 255, 255), LIGHT_SELECT_RADIUS);
		}
		// Draw axis
		// +X axis
		m_debugRenderer.drawLine(glm::vec2(0.0f), glm::vec2(100000.0f, 0.0f), ColorRGBA8(255, 0, 0, 200));
		// -X axis
		m_debugRenderer.drawLine(glm::vec2(0.0f), glm::vec2(-100000.0f, 0.0f), ColorRGBA8(255, 0, 0, 100));
		// +Y axis
		m_debugRenderer.drawLine(glm::vec2(0.0f), glm::vec2(0.0f, 100000.0f), ColorRGBA8(0, 255, 0, 200));
		// -Y axis
		m_debugRenderer.drawLine(glm::vec2(0.0f), glm::vec2(0.0f, -100000.0f), ColorRGBA8(0, 255, 0, 100));

		m_debugRenderer.end();
		m_debugRenderer.render(m_camera.getCameraMatrix(), 2.0f);

		/*for (auto& w : m_waypoints) {
			destRect.x = w.getBody()->GetPosition().x - w.getDimensions().x / 2.0f;
			destRect.y = w.getBody()->GetPosition().y - w.getDimensions().y / 2.0f;
			destRect.z = w.getDimensions().x;
			destRect.w = w.getDimensions().y;
			// Dynamic is green, static is red.
			m_debugRenderer.drawOctagon(w.position, Dystopia::ColorRGBA8(0, 128, 255, 255), 0.25f);
		}*/
	//}
}

void Viewport2D::clearLevel()
{
	m_boxes.clear();
	m_lights.clear();
	m_hasPlayer = false;
	m_world.reset();
	m_world = std::make_unique<b2World>(GRAVITY);
}

void Viewport2D::checkInput() 
{
	SDL_Event evnt;

	m_inputManager.update();

	while (SDL_PollEvent(&evnt)) 
	{
		switch (evnt.type) 
		{
		case SDL_QUIT:
			//m_currentState = Dystopia::ScreenState::EXIT_APPLICATION;
			break;
		case SDL_MOUSEBUTTONDOWN:
			//uMouseDown(evnt);
			break;
		case SDL_MOUSEBUTTONUP:
			//uMouseUp(evnt);
			break;
		case SDL_MOUSEMOTION:
			//uMouseMotion(evnt);

			/*if (m_selectMode == SelectionMode::SELECT)
			{
				m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			}*/
			break;
		case SDL_MOUSEWHEEL:
			/*if (m_editMode == EditorMode::EDIT)
			{
				// Linear scaling sucks for mouse wheel zoom so we multiply by getScale() instead.
				m_camera.offsetScale(m_camera.getScale() * evnt.wheel.y * 0.1f);
			}*/
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		}
	}
}

bool Viewport2D::inLightSelect(const Light& l, const glm::vec2& pos)
{
	return (glm::length(pos - l.position) <= LIGHT_SELECT_RADIUS);
}

void Viewport2D::uMouseDown(const SDL_Event& evnt)
{
	// Texture for boxes. Its here because lazy.
	static GLTexture texture = ResourceManager::getTexture("assets/textures/light_bricks.png");
	glm::vec2 pos;
	glm::vec4 uvRect;

	if (m_editMode == EditorMode::EDIT)
	{
		switch (evnt.button.button)
		{
		case SDL_BUTTON_LEFT:
			m_mouseButtons[MOUSE_LEFT] = true;
			// Don't place objects or select things when clicking on UI.
			//if (isMouseInUI()) break;
			if (m_selectMode == SelectionMode::SELECT)
			{
				//int mx;
				//int my;

				//SDL_GetMouseState(&mx, &my);
				//glm::vec2 minus = m_camera.convertScreenToWorld(glm::vec2(325.0f, 330.0f));
				// Select mode
				pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));

				glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
				m_mousePos = m_camera.convertScreenToWorld(glm::vec2(mouseCoords.x, mouseCoords.y));

				// Lights have selection priority, so check lights first
				// If a light is already selected, test to see if we just clicked it again
				if (m_selectedLight != NO_LIGHT && inLightSelect(m_lights[m_selectedLight], pos + minus))
				{
					// We selected ourself, do nothing.
				}
				else
				{
					// Unselect
					m_selectedLight = NO_LIGHT;
					// Find the box that we are selecting
					for (size_t i = 0; i < m_lights.size(); i++)
					{
						if (inLightSelect(m_lights[i], pos + minus))
						{
							m_selectedLight = i;
							break;
						}
					}
				}
				// If we selected a light
				if (m_selectedLight != NO_LIGHT)
				{
					// Get the offset from the center so we can drag correctly
					m_selectOffset = pos - m_lights[m_selectedLight].position;
					m_selectedBox = NO_BOX;
					m_isDragging = true;
					// Set variables first so refreshing the controls works
					m_colorPickerRed = m_lights[m_selectedLight].color.r;
					m_colorPickerGreen = m_lights[m_selectedLight].color.g;
					m_colorPickerBlue = m_lights[m_selectedLight].color.b;
					m_colorPickerAlpha = m_lights[m_selectedLight].color.a;
					m_lightSize = m_lights[m_selectedLight].size;
					// Set widget values
					/*m_rSlider->setCurrentValue(m_lights[m_selectedLight].color.r);
					m_gSlider->setCurrentValue(m_lights[m_selectedLight].color.g);
					m_bSlider->setCurrentValue(m_lights[m_selectedLight].color.b);
					m_aSlider->setCurrentValue(m_lights[m_selectedLight].color.a);
					m_sizeSpinner->setCurrentValue(m_lights[m_selectedLight].size);
					m_lightRadioButton->setSelected(true);*/
					m_objectMode = ObjectMode::LIGHT;
					break;
				}

				for (auto& b : m_boxes)
				{
					glm::vec2 boxDimension = glm::vec2(b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f - minus.x, b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f - minus.y);

					if (pos.x <= (boxDimension.x + b.getDimensions().x) && pos.y <= (boxDimension.y + b.getDimensions().y) && pos.x >= boxDimension.x && pos.y >= boxDimension.y)
					{
						m_isDraggingLeftMouse = false;
						//refreshSelectedBox(m_mousePos);
						b.hasSelectedBox = true;
						b.isDragging = true;
						b.setSelectedBox();
					}
					else
					{
						m_isDraggingLeftMouse = true;
						b.isDragging = false;
						b.hasSelectedBox = false;
						b.isCurrentlySelected = false;
					}

					// If a box is already selected, test to see if we just clicked it again
					if (b.isCurrentlySelected && b.testPoint(pos.x, pos.y))
					{
						// We selected ourself, do nothing.
					}
					/*else {
					// Unselect
					//m_selectedBox = NO_BOX;
					// Find the box that we are selecting
					for (size_t i = 0; i < m_boxes.size(); i++) {
					if (m_boxes[i].testPoint(pos.x, pos.y)) {
					b.hasSelectedBox = false;
					//m_selectedBox = i;
					break;
					}
					}
					if (b.hasSelectedBox)
					{
					b.hasSelectedBox = false;
					}
					}*/
					// If we selected a box
					/*if (m_selectedBox != NO_BOX) {
					// Get the offset from the center so we can drag correctly
					m_selectOffset = pos - m_boxes[m_selectedBox].getPosition();
					m_selectedLight = NO_LIGHT;
					m_isDragging = true;
					// Set variables first so refreshing the controls works
					m_rotation = m_boxes[m_selectedBox].getBody()->GetAngle();
					m_boxDims.x = m_boxes[m_selectedBox].getDimensions().x;
					m_boxDims.y = m_boxes[m_selectedBox].getDimensions().y;
					m_colorPickerRed = m_boxes[m_selectedBox].getColor().r;
					m_colorPickerGreen = m_boxes[m_selectedBox].getColor().g;
					m_colorPickerBlue = m_boxes[m_selectedBox].getColor().b;
					// Set widget values
					m_rotationSpinner->setCurrentValue(m_boxes[m_selectedBox].getBody()->GetAngle());
					m_widthSpinner->setCurrentValue(m_boxes[m_selectedBox].getDimensions().x);
					m_heightSpinner->setCurrentValue(m_boxes[m_selectedBox].getDimensions().y);
					m_rSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().r);
					m_gSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().g);
					m_bSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().b);
					if (m_boxes[m_selectedBox].isDynamic()) {
					m_dynamicRadioButton->setSelected(true);
					m_physicsMode = PhysicsMode::DYNAMIC;
					}
					else {
					m_rigidRadioButton->setSelected(true);
					m_physicsMode = PhysicsMode::RIGID;
					}
					m_platformRadioButton->setSelected(true);
					m_objectMode = ObjectMode::PLATFORM;
					*/

					/*b.selectOffset = m_mousePos;

					std::cout << b.selectOffset.x << ", " << b.selectOffset.y << std::endl;*/

					if (b.isCurrentlySelected)
					{
						// Get the offset from the center so we can drag correctly
						b.selectOffset = m_mousePos - b.getPosition();//glm::vec2(b.getBody()->GetPosition().x, b.getBody()->GetPosition().y);
																	  //m_selectedLight = NO_LIGHT;
						m_isDraggingLeftMouse = false;
						//b.isDragging = true;
						// Set variables first so refreshing the controls works
						m_rotation = b.getBody()->GetAngle();
						//m_boxDims.x = b.getDimensions().x;
						//m_boxDims.y = b.getDimensions().y;
						//m_colorPickerRed = b.getColor().r;
						//m_colorPickerGreen = b.getColor().g;
						//m_colorPickerBlue = b.getColor().b;
						// Set widget values
						//m_rotationSpinner->setCurrentValue(b.getBody()->GetAngle());
						//m_widthSpinner->setCurrentValue(m_boxes[m_selectedBox].getDimensions().x);
						//m_heightSpinner->setCurrentValue(m_boxes[m_selectedBox].getDimensions().y);
						//m_rSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().r);
						//m_gSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().g);
						//m_bSlider->setCurrentValue(m_boxes[m_selectedBox].getColor().b);
						/*if (b.isDynamic()) {
						m_dynamicRadioButton->setSelected(true);
						m_physicsMode = PhysicsMode::DYNAMIC;
						}
						else {
						m_rigidRadioButton->setSelected(true);
						m_physicsMode = PhysicsMode::RIGID;
						}*/
						//m_platformRadioButton->setSelected(true);
						//m_objectMode = ObjectMode::PLATFORM;
					}
				}
			}
			else 
			{
				// Place mode
				Box *newBox;
				Light newLight;
				//WayPoint newWaypoint;
				// Place
				//glm::vec2 minus = m_camera.convertScreenToWorld(glm::vec2(325.0f, 330.0f));
				if (m_objectMode == ObjectMode::PLATFORM)
				{
					newBox = new Box();
					pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					uvRect.x = pos.x;
					uvRect.y = pos.y;
					uvRect.z = m_boxDims.x;
					uvRect.w = m_boxDims.y;
					newBox->init(m_world.get(), pos + minus, m_boxDims, texture, ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255), m_boxes.size(),
						false, false, m_rotation, uvRect);
					m_boxes.push_back(*newBox);
				}
				if (m_objectMode == ObjectMode::PLATFORMPHYSICS)
				{
					newBox = new Box();
					pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					uvRect.x = pos.x;
					uvRect.y = pos.y;
					uvRect.z = m_boxDims.x;
					uvRect.w = m_boxDims.y;
					newBox->init(m_world.get(), pos + minus, m_boxDims, texture, ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255), m_boxes.size(),
						true, false, m_rotation, uvRect);
					m_boxes.push_back(*newBox);
				}
				if (m_objectMode == ObjectMode::LIGHT)
				{
					newLight.position = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y)) + minus;
					newLight.size = m_lightSize;
					newLight.color = ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, (GLubyte)m_colorPickerAlpha);
					m_lights.push_back(newLight);
				}
				if (m_objectMode == ObjectMode::PLAYER)
				{
					// Just remove the player, easiest way.
					m_player.destroy(m_world.get());
					// Re-init the player
					pos = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					m_player.init(m_world.get(), pos + minus, glm::vec2(1.9f, 2.5f), glm::vec2(1.3f, 2.2f), ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255));
					m_hasPlayer = true;
				}

				switch (m_objectMode) 
				{
				/*case ObjectMode::FINISH:
					// TODO: Implement
					newWaypoint.position = m_camera.convertScreenToWorld(glm::vec2(evnt.button.x, evnt.button.y));
					newWaypoint.init(m_world.get(), pos + minus, glm::vec2(1.0f, 1.0f), ColorRGBA8(0, 0, 255, 255));
					m_waypoints.push_back(newWaypoint);
					break;*/
				}
			}
			break;
		case SDL_BUTTON_RIGHT:
			if (m_objectMode == ObjectMode::PLAYER)
			{
				std::cout << "PLAYER" << std::endl;
			}

			//rightMouseButtonPressed = true;

			m_mouseButtons[MOUSE_RIGHT] = true;
			break;
		}
	}
}

void Viewport2D::uMouseUp(const SDL_Event& evnt)
{
	if (m_editMode == EditorMode::EDIT)
	{
		switch (evnt.button.button)
		{
		case SDL_BUTTON_LEFT:
			m_mouseButtons[MOUSE_LEFT] = false;
			m_isDraggingLeftMouse = false;
			break;
		case SDL_BUTTON_RIGHT:
			m_mouseButtons[MOUSE_RIGHT] = false;
			break;
		}

		/*for (auto& b : m_boxes)
		{
		if (!b.isCurrentlySelected)
		b.isDragging = false;
		}*/
	}
}

void Viewport2D::uMouseMotion(const SDL_Event& evnt)
{
	// If right button is down, drag camera
	const float SPEED = 0.015f;

	if (m_editMode == EditorMode::EDIT)
	{
		m_camPosRel = glm::vec2(-evnt.motion.xrel, evnt.motion.yrel) * (m_camera.getAspectRatio() * SPEED);

		if (m_mouseButtons[MOUSE_RIGHT]) {
			//m_camPosRel = glm::vec2(-evnt.motion.xrel, evnt.motion.yrel) * (m_camera.getAspectRatio() * SPEED);
			m_camera.offsetPosition(glm::vec2(-evnt.motion.xrel, evnt.motion.yrel * m_camera.getAspectRatio()) * SPEED);
		}

		//glm::vec2 mousePos = m_camera.convertScreenToWorld(m_inputManager.getMouseCoords());

		if (m_selectMode == SelectionMode::SELECT)
		{
			glm::vec2 mouseCoords = m_camera.convertScreenToWorld(glm::vec2(evnt.motion.x, evnt.motion.y));
			m_mousePos2 = mouseCoords;
		}

		if (m_mouseButtons[MOUSE_LEFT])
		{
			if (m_selectMode == SelectionMode::SELECT)
			{
				if (m_boxes.size() == 0)
				{
					m_isDraggingLeftMouse = true;
				}
				else
				{
					for (int i = 0; i < m_boxes.size(); i++)
					{
						if (m_boxes[i].isCurrentlySelected)
						{
							/*if (m_boxes[i].getBoxNumber() <= m_boxes.size())
							{
							//NYT PROBLEM! det ser ud til at koden her ikke virker! det koden gør er at tage en box og sætte den til en anden box's position så de overlapper 100%
							//LØSNING MANGLES!

							//m_boxes[i].move(m_boxes[i].getPosition(), m_rotationSpinner->getCurrentValue());
							std::cout << "i = " << i << std::endl;
							}*/
							if (m_boxes[i].isDragging)
							{
								if (m_boxes[i].getBoxNumber() <= i)
								{
									m_isDraggingLeftMouse = false;
								}

								glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(evnt.motion.x, evnt.motion.y)) - m_boxes[i].selectOffset;
								refreshSelectedBox(pos);
							}
						}
					}
				}
			}
		}

		// Dragging stuff around
		if (m_isDragging && m_mouseButtons[MOUSE_LEFT])
		{
			// Light
			if (m_selectedLight != NO_LIGHT) {
				glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(evnt.motion.x, evnt.motion.y)) - m_selectOffset;
				refreshSelectedLight(pos);
			}
			// Box
			/*if (m_selectedBox != NO_BOX) {
			glm::vec2 pos = m_camera.convertScreenToWorld(glm::vec2(evnt.motion.x, evnt.motion.y)) - m_selectOffset;
			refreshSelectedBox(pos);
			}*/
		}
	}
}

void Viewport2D::refreshSelectedBox() 
{
	for (auto& b : m_boxes)
	{
		//if (!b.isDragging)
		if (b.getBoxNumber() == 0)
		{
			return;
		}

		refreshSelectedBox(b.getPosition());
	}
	//if (m_selectedBox == NO_BOX) return;
	//refreshSelectedBox(m_boxes[m_selectedBox].getPosition());
}

void Viewport2D::refreshSelectedBox(glm::vec2& newPosition)
{
	for (int i = 0; i < m_boxes.size(); i++)
	{
		if (m_boxes[i].getBoxNumber() <= m_boxes.size())
		{
			//NYT PROBLEM! det ser ud til at koden her ikke virker! det koden gør er at tage en box og sætte den til en anden box's position så de overlapper 100%
			//LØSNING MANGLES!

			if (m_boxes[i].isCurrentlySelected /*&& !isMouseInUI()*/)
			{
				m_isDraggingLeftMouse = false;
				m_boxes[i].move(newPosition, 0);//m_rotationSpinner->getCurrentValue());
				std::cout << "i = " << i << std::endl;
			}
		}
	}

	/*for (auto& b : m_boxes)
	{
	//if (!b.isDragging)
	if (b.getBoxNumber() == 0)
	{
	return;
	}
	b.move(newPosition, m_rotation);*/
	// Texture for boxes. Its here because lazy.
	/*static Dystopia::GLTexture texture = Dystopia::ResourceManager::getTexture("Assets/Textures/light_bricks.png");
	glm::vec4 uvRect;
	uvRect.x = newPosition.x;
	uvRect.y = newPosition.y;
	uvRect.z = m_boxDims.x;
	uvRect.w = m_boxDims.y;
	Box newBox;*/

	//newBox.init(m_world.get(), newPosition, m_boxDims, texture, Dystopia::ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255),
	//m_physicsMode == PhysicsMode::DYNAMIC, false, m_rotation, uvRect);
	// Destroy old box and replace with new one
	//b.destroy(m_world.get());
	//b = newBox;
	//}
}

void Viewport2D::refreshSelectedLight()
{
	if (m_selectedLight == NO_LIGHT) return;
	refreshSelectedLight(m_lights[m_selectedLight].position);
}

void Viewport2D::refreshSelectedLight(glm::vec2& newPosition)
{
	if (m_selectedLight == NO_LIGHT) return;
	Light newLight;
	newLight.position = newPosition;
	newLight.size = m_lightSize;
	newLight.color = ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, (GLubyte)m_colorPickerAlpha);
	m_lights[m_selectedLight] = newLight;
}