#include "PhysicsApp.h"
#include "GUI.h"
#include <Gizmos.h>
#include "PhysicsScene.h"
#include "Sphere.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>


PhysicsApp::PhysicsApp()
{
}


PhysicsApp::~PhysicsApp()
{
}

bool PhysicsApp::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(vec2(0, -10));
	m_physicsScene->setTimeStep(0.01f);

	
	ball = new Sphere(vec2(0, 0), vec2(10, 30), 3.0f, 1, vec4(1, 0, 0, 1));
	m_physicsScene->addActor(ball);

	return true;
}

void PhysicsApp::shutdown()
{
}

void PhysicsApp::update(float deltaTime)
{
	//Update Input
	m_input = aie::Input::getInstance();

	//Clear Gizmos for this frame
	aie::Gizmos::clear();

	//Update Physics
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();
}

void PhysicsApp::draw()
{
	//Wipe the screen to the background colour
	clearScreen();

	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
}
