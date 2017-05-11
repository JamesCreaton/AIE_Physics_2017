#pragma once
#include "Application.h"
#include "Input.h"
#include "AIE Classes\Renderer2D.h"
#include "AIE Classes\Font.h"
#include "GameObject.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using aie::Input;

class FlyCamera;
class PhysicsScene;
class Sphere;
class GUI;

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

	GameObject*			m_SphereObject;
	GameObject*			m_planeObject;
	GameObject*			m_aabbObject;

	aie::Input*			m_input;

	glm::mat4			m_viewMatrix;
	glm::mat4			m_projectionMatrix;

	aie::Font*			m_font;
};

