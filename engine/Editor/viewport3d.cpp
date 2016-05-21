#include "viewport3d.h"

#include <GL/glew.h>

#include "pipeline.h"

BEGIN_EVENT_TABLE(Viewport3D, wxGLCanvas)
EVT_IDLE(Viewport3D::OnIdle)
EVT_PAINT(Viewport3D::render)
EVT_ERASE_BACKGROUND(Viewport3D::OnEraseBackground)
EVT_MOTION(Viewport3D::OnMouseMoved)
EVT_LEFT_DOWN(Viewport3D::OnMouseDown)
EVT_LEFT_UP(Viewport3D::OnMouseReleased)
EVT_RIGHT_DOWN(Viewport3D::OnRightClick)
EVT_RIGHT_UP(Viewport3D::OnRightReleased)
EVT_LEAVE_WINDOW(Viewport3D::OnMouseLeftWindow)
EVT_SIZE(Viewport3D::resized)
EVT_MOUSEWHEEL(Viewport3D::OnMouseWheelMoved)
EVT_ENTER_WINDOW(Viewport3D::OnMouseEvent)
EVT_KEY_DOWN(Viewport3D::OnKeyDown)
EVT_KEY_UP(Viewport3D::OnKeyUp)
END_EVENT_TABLE()

Viewport3D::Viewport3D(wxPanel* parent, int* args, long style) :
	wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, style | wxWANTS_CHARS | wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);

	InitOpenGl();

	prepare3DViewport(0, 0, getWidth(), getHeight());

	rotation.x = 45.0f;
	rotation.y = 45.0f;

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	camera.setPosition(glm::vec3(0.0f, 1.5f, 10.0f));
	
	aEngine.init();

	m_ambientIntensity = 0.25f;
	m_diffuseIntensity = 1.0f;

	m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = m_ambientIntensity;
	m_directionalLight.DiffuseIntensity = m_diffuseIntensity;
	m_directionalLight.Direction = Vector3f(0.0f, -0.5f, -1.0f);

	m_persProjInfo.FOV = 45.0f;
	m_persProjInfo.Height = getWidth();
	m_persProjInfo.Width = getHeight();
	m_persProjInfo.zNear = 0.1f;
	m_persProjInfo.zFar = 1000.0f;

	skyboxShader.loadShaders("Shaders/3D/skybox.vert", "Shaders/3D/skybox.frag", 6);
	
	//normal_mapping
	//shader.loadShaders("Shaders/3D/normal_mapping.vert", "Shaders/3D/normal_mapping.frag", 0);

	camera.mouseUpdate(glm::vec2(0.0f, 0.0f));

	hasStarted = true;
}

Viewport3D::~Viewport3D()
{
	for (int j = 0, i = this->gameObjects.size(); j < i; j++)
	{
		GameObject3D *gos = this->gameObjects.at(j); // not it
		delete gos;
	}

	this->gameObjects.clear();
	this->gameObjects.shrink_to_fit();

	go_positions.clear();
	go_positions.shrink_to_fit();

	go_orientations.clear();
	go_orientations.shrink_to_fit();

	models.clear();
	models.shrink_to_fit();

	glBindVertexArray(0); // Unbind VAO

	delete m_context;
}

void Viewport3D::InitOpenGl()
{
	if (InitGL) return;

	while (!IsShown()) {};  // Force the Shown
	wxGLCanvas::SetCurrent(*m_context);

	// Init OpenGL
	glLoadIdentity();
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GL_NO_ERROR)
	{
		wxMessageBox(wxString("GLEW Error: ") + wxString(glewGetErrorString(err)), _("OpenGl ERROR"), wxOK | wxICON_EXCLAMATION);
		exit(4001);
	}
	InitGL = true;
}

void Viewport3D::message(wxStyledTextCtrl &output, wxString msg)
{
	output.SetReadOnly(false);

	output.AppendText(msg + "\n");

	output.SetReadOnly(true);
}

void Viewport3D::resized(wxSizeEvent& evt)
{
	Refresh();
}

/** Inits the OpenGL viewport for drawing in 3D. */
void Viewport3D::prepare3DViewport(int topleft_x, int topleft_y, int bottomright_x, int bottomright_y)
{
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.4f, 0.4f, 1.0f); // Black Background
	glClearDepth(1.0f);	// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glDepthFunc(GL_LESS);	// Accept fragment if it closer to the camera than the former one
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	//glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_COLOR_MATERIAL);

	glViewport(topleft_x, topleft_y, bottomright_x - topleft_x, bottomright_y - topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

int Viewport3D::getWidth()
{
	return GetSize().x;
}

int Viewport3D::getHeight()
{
	return GetSize().y;
}

void Viewport3D::render(wxPaintEvent& evt)
{
	if (!IsShown()) return;

	if (checkKeys())
	{
		GetGrandParent()->Close(true);
	}

	camera.setCameraDeltaTime(0.008f);

	m_input.update();

	view = camera.getWorldToViewMatrix();

	projection = glm::perspective(45.0f, float(getWidth()) / float(getHeight()), 0.1f, 1000.0f);

	wxGLCanvas::SetCurrent(*m_context);
	//wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_directionalLight.AmbientIntensity = m_ambientIntensity;
	m_directionalLight.DiffuseIntensity = m_diffuseIntensity;

	//draw some 3D
	prepare3DViewport(0, 0, getWidth(), getHeight());
	//glLoadIdentity();

	m_pEffect.use();

	m_pEffect.SetTextureUnit(0);

	Pipeline p;
	p.Rotate(0.0f, m_scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 15.0f);
	p.SetCamera(camera.getPosition(), camera.getViewDirection(), camera.getUP());
	p.SetPerspectiveProj(m_persProjInfo);

	const Matrix4f& WorldTransformation = p.GetWorldTrans();
	m_pEffect.SetWorldMatrix(WorldTransformation);

	m_pEffect.SetDirectionalLight(m_directionalLight);

	// Use corresponding shader when setting uniforms/drawing objects

	glUniformMatrix4fv(glGetUniformLocation(m_pEffect.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_pEffect.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	
	for (std::vector<GameObject3D*>::iterator iter = this->gameObjects.begin(); iter != this->gameObjects.end(); iter++)
	{
		//float angleX = 0.0f; //90 degrees = 1.575f
		//float angleY = 0.0f;
		//float angleZ = 0.0f;

		glm::mat4 _model;

		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->go = *gameObjects3D[i];
		}

		_model = glm::translate(_model, (*iter)->go.getPosition()); // Translate it down a bit so it's at the center of the scene
		_model = glm::rotate(_model, glm::radians((*iter)->go.getRotation().x * sin(1.575f)), glm::vec3(1.0f, 0.0f, 0.0f));
		_model = glm::rotate(_model, glm::radians((*iter)->go.getRotation().y * sin(1.575f)), glm::vec3(0.0f, 1.0f, 0.0f));
		_model = glm::rotate(_model, glm::radians((*iter)->go.getRotation().z * sin(1.575f)), glm::vec3(0.0f, 0.0f, 1.0f));
		_model = glm::scale(_model, (*iter)->go.getScale());	// It's a bit too big for our scene, so scale it down

		glUniformMatrix4fv(glGetUniformLocation(m_pEffect.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(_model));

		(*iter)->getObjectModel().Draw(shader);
	}

	glm::mat4 _model3;
	_model3 = glm::translate(_model3, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
	_model3 = glm::scale(_model3, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(m_pEffect.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(_model3));

	glBegin(GL_LINES);
	for (float i = -10; i <= 10; i++)
	{
		if (i == 0)
		{
			glColor3f(0.6f, 0.3f, 0.3f);
		}
		else
		{
			glColor3f(0.25f, 0.25f, 0.25f);
		}

		glVertex3f(i, 0, -10);
		glVertex3f(i, 0, 10);

		if (i == 0)
		{
			glColor3f(0.3f, 0.3f, 0.6f);
		}
		else
		{
			glColor3f(0.25f, 0.25f, 0.25f);
		}

		glVertex3f(-10, 0, i);
		glVertex3f(10, 0, i);
	}
	glEnd();

	glm::vec2 mousePos = camera.convertScreenToWorld(glm::vec2(mouseX, mouseY), getWidth(), getHeight(), 2.0f);

	if (rightButtonClicked)
	{
		camera.mouseUpdate(glm::vec2(mousePos.x, -mousePos.y));
	}

	if (hasStarted)
	{
		if (!m_pEffect.Init())
		{
			message(*m_output, "Failed to initialize Directional Light.\n" + m_pEffect.getError());
		}

		if (gameObjects.size() > 0)
		{
			hasGameObjects = true;
		}

		// Cubemap (Skybox)
		std::vector<const GLchar*> faces;

		GLfloat skyboxVertices[] = {
			// Positions          
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};

		// Setup skybox VAO
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);

		faces.push_back("Assets/Skyboxes/SummerSky3/Left.png");
		faces.push_back("Assets/Skyboxes/SummerSky3/Right.png");
		faces.push_back("Assets/Skyboxes/SummerSky3/Top.png");
		faces.push_back("Assets/Skyboxes/SummerSky3/Bottom.png");
		faces.push_back("Assets/Skyboxes/SummerSky3/Front.png");
		faces.push_back("Assets/Skyboxes/SummerSky3/Back.png");

		skyboxTexture = loadCubemap(faces);

		hasStarted = false;
	}

	/*if (gameObjects.size() > 0 && hasGameObjects)
	{
		for (std::vector<GameObject3D::gameObject*>::iterator iter = this->gameObjects3D.begin(); iter != this->gameObjects3D.end(); ++iter)
		{
			go_positions.push_back((*iter)->getPosition());
			go_orientations.push_back(glm::quat((*iter)->getRotation()));
		}

		hasGameObjects = false;
	}*/

	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.getWorldToViewMatrix()));	// Remove any translation component of the view matrix
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default

	glFlush();
	SwapBuffers();

	wxPaintDC(this);
}

void Viewport3D::OnIdle(wxIdleEvent &evnt)
{
	Refresh();
}

void Viewport3D::OnTimer(wxTimerEvent &evnt)
{
	Refresh();
}

void Viewport3D::OnEraseBackground(wxEraseEvent &evnt)
{
	//Do nothing here!
	//This is just to prevent flickering!
}

// some useful events to use
void Viewport3D::OnMouseMoved(wxMouseEvent& event)
{
	const wxPoint pt = event.GetPosition();
	
	mouseX = pt.x;
	mouseY = pt.y;
}

/*void Viewport3D::mouse_click_callback(int b, int s, int mouse_x, int mouse_y)
{
	float x = (2.0f * mouse_x) / getWidth() - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / getHeight();
	float z = 1.0f;
	vec3 ray_nds = vec3(x, y, z);

	vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

	vec4 ray_eye = inverse(projection) * ray_clip;

	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	vec3 ray_wor = vec3((inverse(view) * ray_eye).x, (inverse(view) * ray_eye).y, (inverse(view) * ray_eye).z);
	// don't forget to normalize the vector at some point
	ray_wor = glm::normalize(ray_wor);
}*/

void Viewport3D::OnMouseDown(wxMouseEvent& event)
{
	rightButtonClicked = false;
	leftButtonClicked = true;

	if (event.LeftDown())
	{
		if (!this->HasFocus())
		{
			SetFocus();
		}
	}	

	glm::vec4 viewport = glm::vec4(0, 0, getWidth(), getHeight());
	glm::vec3 wincoord = glm::vec3(mouseX, getHeight() - mouseY - 1, depth);
	glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);
	
	//mouse_click_callback(0, 0, objcoord.x, objcoord.y);

	/*std::string posX = std::to_string(pos.x);
	std::string posY = std::to_string(pos.y);
	std::string posZ = std::to_string(pos.z);

	wxLogMessage("pos: %s, %s, %s.", posX, posY, posZ);*/

	//wxLogMessage("Coordinates in object space: %f, %f, %f", objcoord.x, objcoord.y, objcoord.z);	
}

void Viewport3D::OnMouseWheelMoved(wxMouseEvent& event)
{

}

void Viewport3D::OnMouseReleased(wxMouseEvent& event)
{
	leftButtonClicked = false;
	rightButtonClicked = false;
}

void Viewport3D::OnRightClick(wxMouseEvent& event)
{
	if (!this->HasFocus())
	{
		SetFocus();
	}

	rightButtonClicked = true;
}

void Viewport3D::OnRightReleased(wxMouseEvent& event)
{
	rightButtonClicked = false;
}

void Viewport3D::OnMouseLeftWindow(wxMouseEvent& event)
{

}

void Viewport3D::OnMouseEvent(wxMouseEvent &ev)
{

}

GLboolean Viewport3D::checkKeys()
{
	if (keys[wxKeyCode('W')])
		camera.moveForward();
	if (keys[wxKeyCode('S')])
		camera.moveBackward();
	if (keys[wxKeyCode('A')])
		camera.strafeLeft();
	if (keys[wxKeyCode('D')])
		camera.strafeRight();
	if (keys[wxKeyCode('Q')])
		camera.moveDown();
	if (keys[wxKeyCode('E')])
		camera.moveUp();

	if (keys[wxKeyCode('W')] && keys[WXK_SHIFT])
		camera.moveForwardX2();
	if (keys[wxKeyCode('S')] && keys[WXK_SHIFT])
		camera.moveBackwardX2();
	if (keys[wxKeyCode('A')] && keys[WXK_SHIFT])
		camera.strafeLeftX2();
	if (keys[wxKeyCode('D')] && keys[WXK_SHIFT])
		camera.strafeRightX2();
	if (keys[wxKeyCode('Q')] && keys[WXK_SHIFT])
		camera.moveDownX2();
	if (keys[wxKeyCode('E')] && keys[WXK_SHIFT])
		camera.moveUpX2();

	return false;
}

void Viewport3D::OnKeyDown(wxKeyEvent &evnt)
{
	long key = evnt.GetKeyCode();

	keys[key] = true;

	evnt.Skip();
}

void Viewport3D::OnKeyUp(wxKeyEvent &evnt)
{
	long key = evnt.GetKeyCode();

	keys[key] = false;

	evnt.Skip();
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (left)
// -X (right)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint Viewport3D::loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}


// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint Viewport3D::loadTexture(GLchar const * path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}
