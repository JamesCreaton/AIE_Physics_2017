#include "GameObject.h"
#include "PhysicsObject.h"
#include <Gizmos.h>
#include "PhysicsSphereShape.h"

GameObject::GameObject()
	:m_physicsObject(nullptr)
{
}


GameObject::~GameObject()
{
}

void GameObject::DebugPhysicsRender()
{
	if (m_physicsObject != nullptr) {
		glm::vec3 position = m_physicsObject->GetPosition();

		PhysicsShape* pShape = m_physicsObject->GetShape();
		if (pShape == nullptr) return;

		switch (pShape->GetType())
		{
		case PhysicsShape::ShapeType::PLANE:
			break;
		case PhysicsShape::ShapeType::AABB:
			break;
		case PhysicsShape::ShapeType::SPHERE:
		{
			PhysicsSphereShape* pSphere = (PhysicsSphereShape*)pShape;
			aie::Gizmos::addSphere(position, pSphere->GetRadius(), 15, 15, glm::vec4(1, 0, 0, 1));
			break;
		}
		
		}
	}
}

void GameObject::SetPhysicsObject(PhysicsObject* a_physicsObject)
{
	m_physicsObject = a_physicsObject;
}
