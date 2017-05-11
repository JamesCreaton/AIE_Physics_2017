#pragma once
#include "PhysicsObject.h"
#include <set>
#include <vector>

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void Add(PhysicsObject* physicsObject);
	void Remove(PhysicsObject* objectToRemove);

	void Update(float dt);

	void SetGravity(const glm::vec3 gravity) { m_gravity = gravity; }
	glm::vec3 GetGravity() const { return m_gravity; }

protected:
	std::set<PhysicsObject*> m_physicsObjects;
	glm::vec3	m_gravity;
};


