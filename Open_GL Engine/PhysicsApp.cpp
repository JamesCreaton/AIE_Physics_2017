#include "PhysicsApp.h"
#include "GUI.h"
#include <Gizmos.h>
#include "PhysicsScene.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "PhysicsSphereShape.h"
#include "AIE Classes\FlyCamera.h"



PhysicsApp::PhysicsApp()
{
}


PhysicsApp::~PhysicsApp()
{
}

bool PhysicsApp::startup()
{
	aie::Gizmos::create(64000, 64000, 64000, 64000);
	m_font = new aie::Font("./font/consolas.tff", 32);

	//Set up camera
	m_pCamera = new FlyCamera(*m_window);
	m_pCamera->SetLookAt(glm::vec3(650, 0, 0), glm::vec3(0), glm::vec3(0, 1, 0));
	m_pCamera->SetPerspective(glm::pi<float>() * 0.25f,
		(float)getWindowWidth() / getWindowHeight(),
		0.1f, 1000.f);

	m_pCamera->SetRotationSpeed(0.1f);
	m_pCamera->SetMoveSpeed(15.0f);

	//Set up physics scene
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec3(0, -9.8f, 0));
	m_demoGameObject = new GameObject();

	PhysicsObject* demoPhysicsObject = new PhysicsObject(5.0f);
	demoPhysicsObject->AddShape(new PhysicsSphereShape(2.0f));

	m_physicsScene->Add(demoPhysicsObject);
	m_demoGameObject->SetPhysicsObject(demoPhysicsObject);
	m_demoGameObject->GetPhyicsObject()->SetVelocity(glm::vec3(10.0f, 25.0f, 0.0f));

	return true;
}

void PhysicsApp::shutdown()
{
	delete m_physicsScene;
	delete m_demoGameObject;
	delete m_pCamera;
}

void PhysicsApp::update(float deltaTime)
{
	//Clear Gizmos for this frame
	aie::Gizmos::clear();

	//Update Input
	m_input = aie::Input::getInstance();

	//UpdateCamera
	m_pCamera->Update(deltaTime);

	//Update Physics
	m_physicsScene->Update(0.016f);
}

void PhysicsApp::draw()
{
	//Wipe the screen to the background colour
	clearScreen();
	setBackgroundColour(0.25f, 0.25f, 0.25f, 1);

	//Add a point at wolrd origin
	aie::Gizmos::addTransform(mat4(1));

	//Draw a simple grid
	DrawGizmoGrid();

	//Draw the physics objects
	m_demoGameObject->DebugPhysicsRender();

	//Draw the camera
	aie::Gizmos::draw(m_pCamera->GetProjectionView());
}

//Wrapper function for drawing a simple grid using aie::Gizmos
void PhysicsApp::DrawGizmoGrid()
{
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		aie::Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		aie::Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}
}