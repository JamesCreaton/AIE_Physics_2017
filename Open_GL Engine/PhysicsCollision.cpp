#include "PhysicsCollision.h"

#include "PhysicsObject.h"
#include "PhysicsSphereShape.h"
#include "PhysicsPlaneShape.h"

bool PhysicsCollision::CheckCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	// create a function pointer type for our collision functions
	typedef bool(*collisionFnc)(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	// create an array with all of our collision functions
	// these are ordered based on the "shapeType" enum order
	static collisionFnc collisionFunctionArray[] =
	{
		nullptr,nullptr,CheckPlaneSphereCollision
		, nullptr, nullptr, nullptr,
		CheckSpherePlaneCollision, nullptr, nullptr

	};
	//'hash' the two shapeIds to create a unique index
	auto index = ((int)obj1->GetShape()->GetType() *
		(int)PhysicsShape::ShapeType::NUM_SHAPES) +
		(int)(obj2->GetShape()->GetType());
	// uf we have a collision function for this combination of shapes, call the collision function.
	if (collisionFunctionArray[index] != nullptr)
	{
		return collisionFunctionArray[index](obj1, obj2, collisionInfo);
	}

	// otherwise, return false
	return false;
}

bool PhysicsCollision::CheckSpherePlaneCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	PhysicsSphereShape* pSphere = (PhysicsSphereShape*)obj1->GetShape();
	PhysicsPlaneShape* pPlane = (PhysicsPlaneShape*)obj2->GetShape();


	float distanceFromPlane(glm::dot(obj1->GetPosition(), pPlane->GetNormal()));
	distanceFromPlane -= pPlane->GetDistance();

	if (distanceFromPlane < pSphere->GetRadius())
	{
		collisionInfo.normal = -pPlane->GetNormal();
		collisionInfo.interceptDistance = pSphere->GetRadius() - distanceFromPlane;
		collisionInfo.wasCollision = true;

	}
	else
	{
		collisionInfo.wasCollision = false;
	}

	return collisionInfo.wasCollision;
}

bool PhysicsCollision::CheckPlaneSphereCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	bool wasCollision = CheckSpherePlaneCollision(obj2, obj1, collisionInfo);
	if (wasCollision)
	{

		collisionInfo.normal = -collisionInfo.normal;
	}
	return wasCollision;
}

void PhysicsCollision::ResolveCollision(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	if (collisionInfo.wasCollision) {
		HandleSeperation(obj1, obj2, collisionInfo);
		HandleVelocityChange(obj1, obj2, collisionInfo);
	}
}

void PhysicsCollision::HandleSeperation(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	float totalSystemMass = obj1->GetMass() + obj2->GetMass();
	float obj1Offset = collisionInfo.interceptDistance * (obj2->GetMass() / totalSystemMass);
	float obj2Offset = collisionInfo.interceptDistance * (obj1->GetMass() / totalSystemMass);

	glm::vec3 obj1NewPosition = obj1->GetPosition() - (obj1Offset * collisionInfo.normal);
	glm::vec3 obj2NewPosition = obj2->GetPosition() + (obj2Offset * collisionInfo.normal);

	obj1->SetPosition(obj1NewPosition);
	obj2->SetPosition(obj2NewPosition);
}

void PhysicsCollision::HandleVelocityChange(PhysicsObject * obj1, PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	float e = 0.7f;
	glm::vec3 relativeVelocity = obj2->GetVelocity() - obj1->GetVelocity();

	float jTop = -(1 + e) * glm::dot(relativeVelocity, collisionInfo.normal);
	float jBottom = glm::dot(collisionInfo.normal, collisionInfo.normal) * (1.0f / obj1->GetMass() + 1.0f / obj2->GetMass());

	float j = jTop / jBottom;

	glm::vec3 obj1NewVelocity = obj1->GetVelocity() - (j / obj1->GetMass() * collisionInfo.normal);
	glm::vec3 obj2NewVelocity = obj2->GetVelocity() + (j / obj2->GetMass() * collisionInfo.normal);

	obj1->SetVelocity(obj1NewVelocity);
	obj2->SetVelocity(obj2NewVelocity);
}

