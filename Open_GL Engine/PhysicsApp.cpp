#include "PhysicsApp.h"

#include "GUI.h"
#include "AIE Classes\FlyCamera.h"
#include <Gizmos.h>

#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "GameObject.h"

#include "PhysicsScene.h"
#include "PhysicsSphereShape.h"
#include "PhysicsPlaneShape.h"
#include "PhysicsAABBShape.h"
#include "PhysicsCollision.h"


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
	m_pCamera->SetLookAt(glm::vec3(250, 0, 0), glm::vec3(0), glm::vec3(0, 1, 0));
	m_pCamera->SetPerspective(glm::pi<float>() * 0.25f,
		(float)getWindowWidth() / getWindowHeight(),
		0.1f, 1000.f);

	m_pCamera->SetRotationSpeed(0.1f);
	m_pCamera->SetMoveSpeed(15.0f);

	//Set up physics scene
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec3(0, -9.8f, 0));

	//Create a Plane
	m_gameObjects.push_front(new GameObject());
	PhysicsObject* physicsPlane = new PhysicsObject(std::numeric_limits<float>().max());
	physicsPlane->AddShape(new PhysicsPlaneShape(glm::vec3(0, 1, 0), 0.0f));
	physicsPlane->SetIsStatic(true);
	m_gameObjects.front()->SetPhysicsObject(physicsPlane);

	//Create an AABB
	m_gameObjects.push_front(new GameObject());
	PhysicsObject* physicsAABB = new PhysicsObject(3.0f);
	physicsAABB->AddShape(new PhysicsAABBShape(glm::vec3(3)));
	m_gameObjects.front()->SetPhysicsObject(physicsAABB);
	m_gameObjects.front()->GetPhysicsObject()->SetPosition(glm::vec3(30, 25, 0));

	//Create an AABB
	m_gameObjects.push_front(new GameObject());
	PhysicsObject* physicsAABB2 = new PhysicsObject(3.0f);
	physicsAABB2->AddShape(new PhysicsAABBShape(glm::vec3(13)));
	m_gameObjects.front()->SetPhysicsObject(physicsAABB2);
	m_gameObjects.front()->GetPhysicsObject()->SetPosition(glm::vec3(0, 30, 0));

	//Create a sphere
	m_gameObjects.push_front(new GameObject());
	PhysicsObject* physicsSphere = new PhysicsObject(2.0f);
	physicsSphere->AddShape(new PhysicsSphereShape(2.0f));
	m_gameObjects.front()->SetPhysicsObject(physicsSphere);
	m_gameObjects.front()->GetPhysicsObject()->SetPosition(glm::vec3(0, 215, 0));

	m_physicsScene->Add(physicsPlane);
	m_physicsScene->Add(physicsAABB);
	m_physicsScene->Add(physicsAABB2);
	m_physicsScene->Add(physicsSphere);

	return true;
}

void PhysicsApp::shutdown()
{
	delete m_physicsScene;
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
	m_physicsScene->Update(deltaTime);
}

void PhysicsApp::draw()
{
	//Wipe the screen to the background colour
	clearScreen();
	setBackgroundColour(0.25f, 0.25f, 0.25f, 1);

	//Add a point at wolrd origin
	aie::Gizmos::addTransform(mat4(1));

	//Draw the physics objects
	for (auto object : m_gameObjects) {
		object->DebugPhysicsRender();
	}

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