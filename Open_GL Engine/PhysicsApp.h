#pragma once
#include "Application.h"
#include "Input.h"

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

protected:

	GUI*				m_GUI;
	PhysicsScene*       m_physicsScene;
	Sphere*				ball;
	aie::Input*		m_input;
};

