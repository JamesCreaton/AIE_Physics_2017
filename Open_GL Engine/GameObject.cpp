#include "GameObject.h"
#include "PhysicsObject.h"
#include <Gizmos.h>
#include "PhysicsSphereShape.h"
#include "PhysicsPlaneShape.h"
#include "PhysicsAABBShape.h"


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
		{
			PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)pShape;
			glm::vec3 right(pPlane->GetNormal());
			glm::vec3 forward;
			glm::vec3 up;

			glm::vec3 worldup(0, 1, 0);
			glm::vec3 worldForward(0, 0, 1);

			if (glm::dot(right, worldup) < 0.99f)
			{
				forward = glm::cross(right, worldup);
				up = glm::cross(forward, right);
			}
			else
			{
				up = glm::vec3(-1, 0, 0);
				forward = worldForward;
			}

			glm::mat4 rotation(1);
			rotation[0] = glm::vec4(right, 0);
			rotation[1] = glm::vec4(up, 0);
			rotation[2] = glm::vec4(forward, 0);
			rotation[3] = glm::vec4(pPlane->GetDistance(), 0, 0, 1);

			glm::vec3 offset;
			glm::vec3 normal = pPlane->GetNormal();

			aie::Gizmos::addAABBFilled((position), glm::vec3(0.3f, 20, 20), glm::vec4(0, 0, 1, 1), &rotation);

			/*PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)pShape;

			glm::vec3 centerPoint = pPlane->GetNormal() * pPlane->GetDistance();
			glm::vec3 planeNormal = pPlane->GetNormal();
			float planeScale = 25.0f;
			float normalVecScale = 25.0f;
			glm::vec4 planeColour(0, 1, 0, 1);

			glm::vec3 tangent, biTangent;
			OrthogonalBasis(planeNormal, tangent, biTangent);

			const glm::vec3 v1(centerPoint - (tangent * planeScale) - (biTangent * planeScale));
			const glm::vec3 v2(centerPoint + (tangent * planeScale) - (biTangent * planeScale));
			const glm::vec3 v3(centerPoint + (tangent * planeScale) + (biTangent * planeScale));
			const glm::vec3 v4(centerPoint - (tangent * planeScale) + (biTangent * planeScale));

			// Draw wireframe plane quadrilateral:
			aie::Gizmos::addLine(v1, v2, planeColour);
			aie::Gizmos::addLine(v2, v3, planeColour);
			aie::Gizmos::addLine(v3, v4, planeColour);
			aie::Gizmos::addLine(v4, v1, planeColour);

			const glm::vec3 pvn(
				(centerPoint[0] + planeNormal[0] * normalVecScale),
				(centerPoint[1] + planeNormal[1] * normalVecScale),
				(centerPoint[2] + planeNormal[2] * normalVecScale)
			);

			aie::Gizmos::addLine(centerPoint, pvn, planeColour);
			break;*/
		}
		case PhysicsShape::ShapeType::AABB:
		{
			PhysicsAABBShape* pAABB = (PhysicsAABBShape*)pShape;

			aie::Gizmos::addAABBFilled(position, pAABB->GetExtents(), glm::vec4(1, 0, 0, 1));
			break;
		}
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

void GameObject::OrthogonalBasis(glm::vec3 a_planeNormal, glm::vec3& a_tangent, glm::vec3& a_biTangent)
{
	glm::vec3 tangent;

	glm::vec3 c1 = glm::cross(a_planeNormal, glm::vec3(0.0, 0.0, 1.0));
	glm::vec3 c2 = glm::cross(a_planeNormal, glm::vec3(0.0, 1.0, 0.0));

	if (glm::length(c1) > glm::length(c2))
	{
		tangent = c1;
	}
	else
	{
		tangent = c2;
	}

	a_biTangent = glm::cross(tangent, a_planeNormal);
	a_tangent = tangent;
}