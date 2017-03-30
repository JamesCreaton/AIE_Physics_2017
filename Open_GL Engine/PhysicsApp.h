#pragma once
#include "Application.h"
#include "Input.h"
#include "FlyCamera.h"
#include "AIE Classes\Renderer2D.h"
#include "AIE Classes\Font.h"

#include "Rocket.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using aie::Input;

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

	GUI*				m_GUI;
	PhysicsScene*       m_physicsScene;
	aie::Input*			m_input;

	glm::mat4			m_viewMatrix;
	glm::mat4			m_projectionMatrix;
	aie::Renderer2D*	m_2dRenderer;
	Rocket* rocket;
	aie::Font*			m_font;
	
	std::vector<Sphere*> m_exhaustParticles;

	float timer = 0.0f;
	float timeStep = 0.4f;
};

