#pragma once
#include <glm\glm.hpp>

class PhysicsObject;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void DebugPhysicsRender();

	void SetPhysicsObject(PhysicsObject* a_physicsObject);
	void OrthogonalBasis(glm::vec3 a_planeNormal, glm::vec3 & a_tangent, glm::vec3 & a_biTangent);
	PhysicsObject* GetPhysicsObject() { return m_physicsObject; }


private:
	PhysicsObject*	m_physicsObject;
};

