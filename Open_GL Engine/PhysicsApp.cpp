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
	aie::Gizmos::create(64000, 64000, 64000, 64000);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.tff", 32);

	//Set up physics scene
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(vec2(0, -1.1f));
	m_physicsScene->setTimeStep(0.01f);

	rocket = new Rocket(vec2(0, -30), vec2(0, 1), 1.0f, 3, vec4(0, 1, 1, 1));
	m_physicsScene->addActor(rocket);

	return true;
}

void PhysicsApp::shutdown()
{

}

void PhysicsApp::update(float deltaTime)
{   
	timer += deltaTime;

	//Update Input
	m_input = aie::Input::getInstance();

	//Clear Gizmos for this frame
	aie::Gizmos::clear();

	//Update Physics
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	if (rocket->getMass() > 0.0f) {
		if (m_input->isKeyDown(GLFW_KEY_SPACE)) {
			rocket->applyForce(vec2(0, 0.1f));

			if (timer >= timeStep) {
				rocket->SetMass(rocket->getMass() - 0.7f);
				m_exhaustParticles.push_back(new Sphere(rocket->getPosition(),vec2(-rocket->getVelocity()), 0.2f, 2.0f, vec4(1, 1, 1, 1)));
				rocket->applyForceToActor(m_exhaustParticles[0],
					vec2(0, -rocket->getVelocity().y));

				timer = 0.0f;
			}
		}
	}


	auto gizmoIterator = m_exhaustParticles.begin();
	for (; gizmoIterator != m_exhaustParticles.end(); gizmoIterator++)
	{
		(*gizmoIterator)->makeGizmo();
	}

}

void PhysicsApp::draw()
{
	//Wipe the screen to the background colour
	clearScreen();
	setBackgroundColour(vec4(0.25f, 0.25f, 0.25f, 1));

	//m_2dRenderer->setCameraPos(rocket->getPosition().x, rocket->getPosition().y);

	m_2dRenderer->begin();
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(
		-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	m_2dRenderer->drawText(m_font, "test", 0, 0);

	m_2dRenderer->end();
}

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