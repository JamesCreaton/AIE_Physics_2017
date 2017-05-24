#include "PhysicsCollision.h"

#include "PhysicsObject.h"
#include "PhysicsSphereShape.h"
#include "PhysicsPlaneShape.h"
#include "PhysicsAABBShape.h"

#include <algorithm>
#include <cmath>

bool PhysicsCollision::CheckCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	// create a function pointer type for our collision functions
	typedef bool(*collisionFnc)(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	// create an array with all of our collision functions
	// these are ordered based on the "shapeType" enum order
	static collisionFnc collisionFunctionArray[] =
	{
		//Plane Plane               //Plane AABB                //Plane Sphere
		CheckPlanePlaneCollision,	CheckPlaneAABBCollision,	CheckPlaneSphereCollision,
		//AABB Plane				//AABB AABB					//AABB Sphere
		CheckAABBPlaneCollision,	CheckAABBAABBCollision,		CheckAABBSphereCollision,
		//Sphere Plane				//Sphere AABB				//Sphere Sphere
		CheckSpherePlaneCollision,	CheckSphereAABBCollision,	CheckSphereSphereCollision

	};
	//'hash' the two shapeIds to create a unique index
	auto index = ((int)obj1->GetShape()->GetType() *
		(int)PhysicsShape::ShapeType::NUM_SHAPES) +
		(int)(obj2->GetShape()->GetType());
	// if we have a collision function for this combination of shapes, call the collision function.
	if (collisionFunctionArray[index] != nullptr)
	{
		return collisionFunctionArray[index](obj1, obj2, collisionInfo);
	}

	// otherwise, return false
	return false;
}
//Plane Collisions
bool PhysicsCollision::CheckPlanePlaneCollision(const PhysicsObject * obj1, const PhysicsObject * obj2, CollisionInfo & collisionInfo)
{
	return false;
}
bool PhysicsCollision::CheckPlaneAABBCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	PhysicsPlaneShape* plane = (PhysicsPlaneShape*)obj1->GetShape();
	PhysicsAABBShape* AABB = (PhysicsAABBShape*)obj2->GetShape();

	glm::vec3 minExtents((glm::vec3(obj2->GetPosition()) - AABB->GetExtents()));
	glm::vec3 maxExtents((glm::vec3(obj2->GetPosition()) + AABB->GetExtents()));

	float minDot = glm::dot(minExtents, glm::vec3(glm::vec3(plane->GetNormal()))) - plane->GetDistance();
	float maxDot = glm::dot(maxExtents, glm::vec3(glm::vec3(plane->GetNormal()))) - plane->GetDistance();

	float dotToUse;
	if (minDot < maxDot)
		dotToUse = minDot;
	else
		dotToUse = maxDot;

	if (dotToUse <= 0) {
		//we have a collision
		collisionInfo.normal = glm::vec3(plane->GetNormal());
		collisionInfo.interceptDistance = -dotToUse;
		collisionInfo.wasCollision = true;
	}
	else
		collisionInfo.wasCollision = false;

	return collisionInfo.wasCollision;
}
bool PhysicsCollision::CheckPlaneSphereCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	bool wasCollision = CheckSpherePlaneCollision(obj2, obj1, collisionInfo);
	if (wasCollision)
	{

		collisionInfo.normal = -collisionInfo.normal;
	}
	return wasCollision;
}

//AABB Collisions
bool PhysicsCollision::CheckAABBPlaneCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	bool wasCollision = CheckPlaneAABBCollision(obj2, obj1, collisionInfo);
	if (wasCollision) {
		collisionInfo.normal = -collisionInfo.normal;
	}
	return wasCollision;
}
bool PhysicsCollision::CheckAABBAABBCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	//NOT IMPLEMENTED
	return false;
}
bool PhysicsCollision::CheckAABBSphereCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	PhysicsAABBShape* AABB = (PhysicsAABBShape*)obj1->GetShape();
	PhysicsSphereShape* sphere = (PhysicsSphereShape*)obj2->GetShape();

	// Min Max Extents
	glm::vec3 aMin = glm::vec3(obj1->GetPosition()) - AABB->GetExtents();
	glm::vec3 aMax = glm::vec3(obj1->GetPosition()) + AABB->GetExtents();

	glm::vec3 bSpherePos = glm::vec3(obj2->GetPosition());
	//Clamp the points
	float x = std::max(aMin.x, std::min(bSpherePos.x, aMax.x));
	float y = std::max(aMin.y, std::min(bSpherePos.y, aMax.y));
	float z = std::max(aMin.z, std::min(bSpherePos.z, aMax.z));

	float distance = glm::length(bSpherePos - glm::vec3(x, y, z));
	if (distance < sphere->GetRadius()) {
		collisionInfo.normal = glm::normalize(glm::vec3(obj2->GetPosition() - obj1->GetPosition()));
		collisionInfo.interceptDistance = sphere->GetRadius() - distance;
		collisionInfo.wasCollision = true;
	}
	else
		collisionInfo.wasCollision = false;

	return collisionInfo.wasCollision;
}

//Sphere Collisions
bool PhysicsCollision::CheckSpherePlaneCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
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
bool PhysicsCollision::CheckSphereAABBCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	bool wasCollision = CheckAABBSphereCollision(obj2, obj1, collisionInfo);
	if (wasCollision)
		collisionInfo.normal = -collisionInfo.normal;

	return wasCollision;
}
bool PhysicsCollision::CheckSphereSphereCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	glm::vec3 distanceBetweenSpheres;
	distanceBetweenSpheres = obj1->GetPosition() - obj2->GetPosition();

	PhysicsSphereShape *sphere1 = (PhysicsSphereShape*)(obj1->GetShape());
	PhysicsSphereShape *sphere2 = (PhysicsSphereShape*)(obj2->GetShape());

	float distance = glm::length(distanceBetweenSpheres);
	distance -= sphere1->GetRadius() + sphere2->GetRadius();

	if (distance <= 0.0f) {
		collisionInfo.normal = -glm::normalize(distanceBetweenSpheres);
		collisionInfo.interceptDistance = -distance;
		collisionInfo.wasCollision = true;
	}
	else {
		//No Collision
		collisionInfo.wasCollision = false;
	}
	return collisionInfo.wasCollision;

}

//Collision Resolution
void PhysicsCollision::ResolveCollision(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	if (collisionInfo.wasCollision) {
		HandleSeperation(obj1, obj2, collisionInfo);
		HandleVelocityChange(obj1, obj2, collisionInfo);
	}
}
void PhysicsCollision::HandleSeperation(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo)
{
	float totalSystemMass = obj1->GetMass() + obj2->GetMass();
	float obj1Offset = collisionInfo.interceptDistance * (obj2->GetMass() / totalSystemMass);
	float obj2Offset = collisionInfo.interceptDistance * (obj1->GetMass() / totalSystemMass);

	glm::vec3 obj1NewPosition = obj1->GetPosition() - (obj1Offset * collisionInfo.normal);
	glm::vec3 obj2NewPosition = obj2->GetPosition() + (obj2Offset * collisionInfo.normal);

	obj1->SetPosition(obj1NewPosition);
	obj2->SetPosition(obj2NewPosition);
}
void PhysicsCollision::HandleVelocityChange(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo)
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

