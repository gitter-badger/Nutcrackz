#pragma once

#include <string>
#include <GLM/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "model.h"
#include "shaderprogram.h"

class GameObject2D
{
public:
	struct gameObject
	{
		void setName(char *name) { strcpy(m_name, name); }
		void setPath(char *path) { strcpy(m_filepath, path); }
		void setNewLine(char *lineChange) { strcpy(m_newline, lineChange); }

		void setPosition(glm::vec2 value) { m_positionXY = value; }
		void setRotation(glm::vec2 value) { m_rotationXY = value; }
		void setScale(glm::vec2 value) { m_scaleXY = value; }

		void setPosition(glm::vec3 value) { m_positionXYZ = glm::vec3(value.x, value.y, value.z); }
		void setRotation(glm::vec3 value) { m_rotationXYZ = value; }
		void setScale(glm::vec3 value) { m_scaleXYZ = value; }

		char m_filepath[200];
		char m_name[50];

		glm::vec2 m_positionXY;
		glm::vec2 m_rotationXY;
		glm::vec2 m_scaleXY;

		glm::vec3 m_positionXYZ;
		glm::vec3 m_rotationXYZ;
		glm::vec3 m_scaleXYZ;

		char m_newline[2];
	};

	GameObject2D() {}
	GameObject2D(std::string filepath)
	{
		m_filepath = filepath;
	}

	//Setters
	void setName(std::string name) { m_name = name; }

	void setPosition(glm::vec2 value) { m_positionXY = value; }
	void setRotation(glm::vec2 value) { m_rotationXY = value; }
	void setScale(glm::vec2 value) { m_scaleXY = value; }

	void setPosition(glm::vec3 value) { m_positionXYZ = value; }
	void setRotation(glm::vec3 value) { m_rotationXYZ = value; }
	void setScale(glm::vec3 value) { m_scaleXYZ = value; }

	//Getters
	std::string getName() { return m_name; }

	glm::vec2 getPosition() { return m_positionXY; }
	glm::vec2 getRotation() { return m_rotationXY; }
	glm::vec2 getScale() { return m_scaleXY; }

	glm::vec3 getPosition3D() { return m_positionXYZ; }
	glm::vec3 getRotation3D() { return m_rotationXYZ; }
	glm::vec3 getScale3D() { return m_scaleXYZ; }

private:
	std::string m_filepath;
	std::string m_name;

	glm::vec2 m_positionXY;
	glm::vec2 m_rotationXY;
	glm::vec2 m_scaleXY;

	glm::vec3 m_positionXYZ;
	glm::vec3 m_rotationXYZ;
	glm::vec3 m_scaleXYZ;
};

class Viewport3D;

class GameObject3D
{
public:
	struct gameObject
	{
		//Setters
		void setName(char *name) { strcpy(m_name, name); }
		void setPath(char *path) { strcpy(m_filepath, path); }
		void setNewLine(char *lineChange) { strcpy(m_newline, lineChange); }
		void setPosition(glm::vec3 value) { m_positionXYZ = glm::vec3(value.x, value.y, value.z); }
		void setRotation(glm::vec3 value) { m_rotationXYZ = value; }
		void setScale(glm::vec3 value) { m_scaleXYZ = value; }

		//Getters
		char *getName() { return m_name; }
		glm::vec3 getPosition() { return m_positionXYZ; }
		glm::vec3 getRotation() { return m_rotationXYZ; }
		glm::vec3 getScale() { return m_scaleXYZ; }

		char m_filepath[200];
		char m_name[50];

		glm::vec3 m_positionXYZ;
		glm::vec3 m_rotationXYZ;
		glm::vec3 m_scaleXYZ;

		char m_newline[2];
	};

	GameObject3D() {}
	GameObject3D(std::string filepath)
	{
		m_filepath = filepath;
		m_model = new Model(filepath);
	}

	//Setters
	void setName(char *name) { strcpy(m_name, name); }
	void setNameStr(char *name) { m_str_name = name; }
	void setPosition(glm::vec3 value) { glm::vec3 newPos = glm::vec3(value.x, value.y, value.z); m_positionXYZ = newPos; }
	void setPositionGO(glm::vec3 value) { glm::vec3 newPos = glm::vec3(value.x, value.y, value.z); go.setPosition(newPos); }
	void setRotation(glm::vec3 value) { m_rotationXYZ = value; }
	void setScale(glm::vec3 value) { m_scaleXYZ = value; }
	void select(bool result) { isSelected = result; }

	//Getters
	char *getName() { return m_name; }
	std::string getNameStr() { return m_str_name; }
	glm::vec3 getPosition() { return m_positionXYZ; }
	glm::vec3 getRotation() { return m_rotationXYZ; }
	glm::vec3 getScale() { return m_scaleXYZ; }
	Model getObjectModel() { return *m_model; }
	gameObject getGameObject() { return go; }
	bool isThisSelected() { return isSelected; }

	gameObject go;

private:
	std::string m_filepath;
	char m_name[50];
	std::string m_str_name;

	glm::vec3 m_positionXYZ;
	glm::vec3 m_rotationXYZ;
	glm::vec3 m_scaleXYZ;

	Model *m_model;
	bool isSelected = true;
};