#pragma once
#include "PhysicsObject.h"
#include "PhysicsAABBShape.h"

#include <algorithm>
#include <limits>

namespace Collision
{
	static bool CheckAABBCollision(PhysicsObject& a, PhysicsObject& b)
	{
		PhysicsAABBShape* aAABB = (PhysicsAABBShape*)a.GetShape();
		PhysicsAABBShape* bAABB = (PhysicsAABBShape*)b.GetShape();

		glm::vec3 aMin = a.GetPosition() - aAABB->GetExtents();
		glm::vec3 bMin = b.GetPosition() - bAABB->GetExtents();

		glm::vec3 aMax = a.GetPosition() + aAABB->GetExtents();
		glm::vec3 bMax = b.GetPosition() + bAABB->GetExtents();


		return  (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
			(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
			(aMin.z <= bMax.z && aMax.z >= bMin.z);

		delete aAABB;
		delete bAABB;
	}

	float SweptAABB(PhysicsObject& a, PhysicsObject& b, float& normalx, float& normaly)
	{
		PhysicsAABBShape* aAABB = (PhysicsAABBShape*)a.GetShape();
		PhysicsAABBShape* bAABB = (PhysicsAABBShape*)b.GetShape();

		float xInvEntry, yInvEntry;
		float xInvExit, yInvExit;

		// find the distance between the objects on the near and far sides for both x and y
		if (a.GetVelocity().x > 0.0f)
		{
			xInvEntry = b.GetPosition().x - (a.GetPosition().x + aAABB->GetExtents().x);
			xInvExit = (b.GetPosition().x + bAABB->GetExtents().x) - a.GetPosition().x;
		}
		else
		{
			xInvEntry = (b.GetPosition().x + bAABB->GetExtents().x) - a.GetPosition().x;
			xInvExit = b.GetPosition().x - (a.GetPosition().x + aAABB->GetExtents().x);
		}

		if (a.GetVelocity().y > 0.0f)
		{
			yInvEntry = b.GetPosition().y - (a.GetPosition().y + aAABB->GetExtents().y);
			yInvExit = (b.GetPosition().y + bAABB->GetExtents().y) - a.GetPosition().y;
		}
		else
		{
			yInvEntry = (b.GetPosition().y + bAABB->GetExtents().y) - a.GetPosition().y;
			yInvExit = b.GetPosition().y - (a.GetPosition().y + aAABB->GetExtents().y);
		}

		// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
		float xEntry, yEntry;
		float xExit, yExit;

		if (a.GetVelocity().x == 0.0f)
		{
			xEntry = -std::numeric_limits<float>::infinity();
			xExit = std::numeric_limits<float>::infinity();
		}
		else
		{
			xEntry = xInvEntry / a.GetVelocity().x;
			xExit = xInvExit / a.GetVelocity().x;
		}

		if (a.GetVelocity().y == 0.0f)
		{
			yEntry = -std::numeric_limits<float>::infinity();
			yExit = std::numeric_limits<float>::infinity();
		}
		else
		{
			yEntry = yInvEntry / a.GetVelocity().y;
			yExit = yInvExit / a.GetVelocity().y;
		}

		// find the earliest/latest times of collision
		float entryTime = std::max(xEntry, yEntry);
		float exitTime = std::min(xExit, yExit);

		// if there was no collision
		if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f)
		{
			normalx = 0.0f;
			normaly = 0.0f;
			return 1.0f;
		}
		else // if there was a collision
		{
			// calculate normal of collided surface
			if (xEntry > yEntry)
			{
				if (xInvEntry < 0.0f)
				{
					normalx = 1.0f;
					normaly = 0.0f;
				}
				else
				{
					normalx = -1.0f;
					normaly = 0.0f;
				}
			}
			else
			{
				if (yInvEntry < 0.0f)
				{
					normalx = 0.0f;
					normaly = 1.0f;
				}
				else
				{
					normalx = 0.0f;
					normaly = -1.0f;
				}
			}

			// return the time of collision
			return entryTime;
		}
		delete aAABB;
		delete bAABB;
	}

	static bool AABB(PhysicsObject& a, PhysicsObject& b, float& moveX, float& moveY)
	{
		PhysicsAABBShape* aAABB = (PhysicsAABBShape*)a.GetShape();
		PhysicsAABBShape* bAABB = (PhysicsAABBShape*)b.GetShape();

		moveX = moveY = 0.0f;

		float l = b.GetPosition().x - (a.GetPosition().x + aAABB->GetExtents().x);
		float r = (b.GetPosition().x + bAABB->GetExtents().x) - a.GetPosition().x;
		float t = b.GetPosition().y - (a.GetPosition().y + aAABB->GetExtents().y);
		float bf = (b.GetPosition().y + bAABB->GetExtents().y) - a.GetPosition().y;

		// check that there was a collision
		if (l > 0 || r < 0 || t > 0 || bf < 0)
			return false;

		// find the offset of both sides
		moveX = abs(l) < r ? l : r;
		moveY = abs(t) < bf ? t : bf;

		// only use whichever offset is the smallest
		if (abs(moveX) < abs(moveY))
			moveY = 0.0f;
		else
			moveX = 0.0f;

		return true;

		delete aAABB;
		delete bAABB;
	}
}