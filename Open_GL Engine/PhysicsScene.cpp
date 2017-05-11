#include "PhysicsScene.h"
#include <algorithm>


PhysicsScene::PhysicsScene()
	:m_gravity(glm::vec3(0,0,0))
{
}


PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::Add(PhysicsObject * physicsObject)
{
	m_physicsObjects.insert(physicsObject);
}

void PhysicsScene::Remove(PhysicsObject * objectToRemove)
{
	m_physicsObjects.erase(objectToRemove);
}

void PhysicsScene::Update(float dt)
{
	for (PhysicsObject* object : m_physicsObjects) {
		object->AddAcceleration(m_gravity);
		object->Update(dt);
	}
}