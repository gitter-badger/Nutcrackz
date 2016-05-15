#pragma once

#include "shaderprogram.h"
#include "model.h"
#include "audioengine.h"

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/stc/stc.h>
#include <SDL/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

// Include Bullet
#include <Bullet/btBulletDynamicsCommon.h>

#include <string>

#include "gameobject.h"
#include "inputmanager.h"
#include "lightingprogram.h"
#include "3dpicking.h"
#include "simplecolor.h"
#include "util.h"
#include "camera3d.h"
#include "time.h"

class Viewport3D : public wxGLCanvas
{
	wxGLContext* m_context;

public:
	Viewport3D(wxPanel* parent, int* args, long style = 0);
	virtual ~Viewport3D();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	void render(wxPaintEvent& evt);
	void prepare3DViewport(int topleft_x, int topleft_y, int bottomright_x, int bottomright_y);
	GLboolean checkKeys();

	void setOutput(wxStyledTextCtrl &output) { m_output = &output; }
	bool hasOutput() { if (m_output != nullptr) { return true; }  return false; }
	void message(wxStyledTextCtrl &output, wxString msg);

	//Setters
	void setRotateX(float speed) { rotation.x += speed; }
	void setRotateY(float speed) { rotation.y += speed; }
	void setPositionXYZ(glm::vec3 newPos) { position = newPos; }
	void setRotationXYZ(glm::vec3 newRot) { rotation = newRot; }
	void setScaleXYZ(glm::vec3 newScale) { scale = newScale; }
	void setAmbientIntensity(float value) { m_ambientIntensity = value; }
	void setDiffuseIntensity(float value) { m_diffuseIntensity = value; }
	void setDeltaTime(float value) { deltaTime = value; }
	void resizeGameObjects(int value) { gameObjects.resize(value); }
	void addGameObject(GameObject3D *go) { gameObjects.push_back(go); hasGameObjects = true; }

	//Getters
	bool get3DModelSelected() { return model3DSelected; }
	wxString getPositionX() { return std::to_string(position.x); }
	wxString getPositionY() { return std::to_string(position.y); }
	wxString getPositionZ() { return std::to_string(position.z); }
	glm::vec3 positions() { return position; }
	float getAmbientIntensity() { return m_ambientIntensity; }
	float getDiffuseIntensity() { return m_diffuseIntensity; }
	std::vector<GameObject3D*> getMyGameObjects() { return gameObjects; }

	// events
	void OnIdle(wxIdleEvent &evnt);
	void OnEraseBackground(wxEraseEvent &evnt);
	void OnTimer(wxTimerEvent &evnt);
	void OnMouseMoved(wxMouseEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseWheelMoved(wxMouseEvent& event);
	void OnMouseReleased(wxMouseEvent& event);
	void OnRightClick(wxMouseEvent& event);
	void OnRightReleased(wxMouseEvent& event);
	void OnMouseLeftWindow(wxMouseEvent& event);
	void OnMouseEvent(wxMouseEvent &ev);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);
	
private:
	void InitOpenGl();
	
	GLuint loadTexture(GLchar const * path);
	GLuint loadCubemap(std::vector<const GLchar*> faces);

	bool InitGL;
	bool model3DSelected = false;
	bool keys[500];
	bool rightButtonClicked = false;
	bool leftButtonClicked = false;
	bool isMouseMoving = false;
	bool hasStarted = false;
	bool hasGameObjects = false;
	int mouseX;
	int mouseY;

	float m_scale;

	float m_ambientIntensity;
	float m_diffuseIntensity;

	GLuint cubeVAO, cubeVBO;
	GLuint skyboxVAO, skyboxVBO;
	GLuint skyboxTexture;

	btDiscreteDynamicsWorld* dynamicsWorld;
	btSequentialImpulseConstraintSolver* solver;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btCollisionShape* boxCollisionShape;
	
	std::vector<btRigidBody*> rigidbodies;

	LightingProgram m_pEffect;
	DirectionalLight m_directionalLight;

	PersProjInfo m_persProjInfo;

	ShaderProgram shader;

	Model model;
	Model model2;

	std::vector<Model> models;
	AudioEngine aEngine;

	std::vector<GameObject3D*> gameObjects;
	std::vector<glm::vec3> go_positions;
	std::vector<glm::quat> go_orientations;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 view;
	glm::mat4 projection;

	wxStyledTextCtrl *m_output;
	
	GLfloat lastFrame = 0.0f;
	
	InputManager m_input;
	Camera3D camera;

	Picking *picker;

	ShaderProgram skyboxShader;

	long last = 0;
	float deltaTime = 0.0;

	float time;

	Time m_time;

	DECLARE_EVENT_TABLE()
};