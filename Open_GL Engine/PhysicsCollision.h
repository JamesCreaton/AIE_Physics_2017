#pragma once
#include <glm\glm.hpp>

class PhysicsObject;
class CollisionInfo;

using glm::vec3;

namespace PhysicsCollision
{
	struct CollisionInfo {
		bool			wasCollision;
		glm::vec3		normal;
		float			interceptDistance;
	};

	bool CheckCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	bool CheckSpherePlaneCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);
	bool CheckPlaneSphereCollision(const PhysicsObject* obj1, const PhysicsObject* obj2, CollisionInfo& collisionInfo);

	void ResolveCollision(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
	void HandleSeperation(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
	void HandleVelocityChange(PhysicsObject* obj1, PhysicsObject* obj2, CollisionInfo& collisionInfo);
};



