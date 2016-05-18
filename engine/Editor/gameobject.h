#pragma once

#include <string>
#include <GLM/glm.hpp>

#include "model.h"
#include "shaderprogram.h"

class GameObject2D
{
public:
	GameObject2D() {}

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
		void setName(char *name) { strcpy(m_name, name); }
		void setPath(char *path) { strcpy(m_filepath, path); }
		void setPosition(glm::vec3 value) { m_positionXYZ = glm::vec3(value.x, value.y, value.z); }
		void setRotation(glm::vec3 value) { m_rotationXYZ = value; }
		void setScale(glm::vec3 value) { m_scaleXYZ = value; }

		char m_filepath[500];
		char m_name[50];

		glm::vec3 m_positionXYZ;
		glm::vec3 m_rotationXYZ;
		glm::vec3 m_scaleXYZ;
	};

	GameObject3D() {}
	GameObject3D(std::string filepath)
	{
		m_filepath = filepath;
		m_model = new Model(filepath);
	}

	//Setters
	//void setFilePath(std::string path) { m_filepath = path; m_model = new Model(path); }
	void setName(std::string name) { m_name = name; }
	void setPosition(glm::vec3 value) { go.m_positionXYZ = glm::vec3(value.x, value.y, value.z); }
	void setRotation(glm::vec3 value) { go.m_rotationXYZ = value; }
	void setScale(glm::vec3 value) { go.m_scaleXYZ = value; }

	//Getters
	std::string getFilepath() { return go.m_filepath; }
	std::string getName() { return go.m_name; }
	glm::vec3 getPosition() { return go.m_positionXYZ; }
	glm::vec3 getRotation() { return go.m_rotationXYZ; }
	glm::vec3 getScale() { return go.m_scaleXYZ; }
	Model getObjectModel() { return *m_model; }

private:
	gameObject go;

	std::string m_filepath;
	std::string m_name;

	glm::vec3 m_positionXYZ;
	glm::vec3 m_rotationXYZ;
	glm::vec3 m_scaleXYZ;

	Model *m_model;
};

/*class GameObject3D
{
public:
	GameObject3D() {}
	GameObject3D(std::string filepath)
	{
		//go3D.m_filepath = filepath;
		go3D.m_model = new Model(filepath);
	}
		
	//Getters
	std::string getFilepath() { return go3D.m_filepath; }
	std::string getName() { return go3D.m_name; }
	glm::vec3 getPosition() { return go3D.m_positionXYZ; }
	glm::vec3 getRotation() { return go3D.m_rotationXYZ; }
	glm::vec3 getScale() { return go3D.m_scaleXYZ; }
	Model getObjectModel() { return *go3D.m_model; }

private:
	gameObject3D go3D;
	std::string m_filepath;
	std::string m_name;

	glm::vec3 m_positionXYZ;
	glm::vec3 m_rotationXYZ;
	glm::vec3 m_scaleXYZ;

	Model *m_model;
};*/