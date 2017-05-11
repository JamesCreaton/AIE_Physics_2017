#pragma once

class PhysicsObject;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void DebugPhysicsRender();

	void SetPhysicsObject(PhysicsObject* a_physicsObject);
	PhysicsObject* GetPhyicsObject() { return m_physicsObject; }


private:
	PhysicsObject*	m_physicsObject;
};

