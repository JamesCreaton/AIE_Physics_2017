#pragma once
#include "Application.h"
#include <list>

#include "Input.h"
#include "AIE Classes\Font.h"


using glm::vec2;
using glm::vec3;
using glm::vec4;
using aie::Input;

class FlyCamera;
class PhysicsScene;
class Sphere;
class GUI;
class GameObject;

class PhysicsApp : public Application
{
public:
	PhysicsApp();
	~PhysicsApp();

	bool startup();
	void shutdown();

	void update(float deltaTime);
	void draw();

	void DrawGizmoGrid();

protected:
	FlyCamera*			m_pCamera;
	GUI*				m_GUI;
	PhysicsScene*       m_physicsScene;

	std::list<GameObject*> m_gameObjects;

	aie::Input*			m_input;
	aie::Font*			m_font;

	glm::mat4			m_viewMatrix;
	glm::mat4			m_projectionMatrix;
};

