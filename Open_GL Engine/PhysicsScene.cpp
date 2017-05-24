#include "PhysicsScene.h"
#include <algorithm>
#include "PhysicsCollision.h"

PhysicsScene::PhysicsScene()
	:m_gravity(glm::vec3(0, 0, 0))
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

		if (!object->GetIsStatic()) {
			object->AddAcceleration(m_gravity);
			object->Update(dt);
		}
	}

	PhysicsCollision::CollisionInfo collisionInfo;
	for (auto objectiterator1 = m_physicsObjects.begin(); objectiterator1 != m_physicsObjects.end(); objectiterator1++) 
	{
		for (auto objectiterator2 = std::next(objectiterator1); objectiterator2 != m_physicsObjects.end(); objectiterator2++) 
		{
			bool wasCollision = PhysicsCollision::CheckCollision(*objectiterator1, *objectiterator2, collisionInfo);
			if (wasCollision) 
			{
				//Handle the collision
				PhysicsCollision::ResolveCollision(*objectiterator1, *objectiterator2, collisionInfo);
			}
		}
	}
}