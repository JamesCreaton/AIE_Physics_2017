#pragma once

#include "PhysicsShape.h"
#include <glm\glm.hpp>

class PhysicsPlaneShape : public PhysicsShape
{
public:
	PhysicsPlaneShape() = delete;
	PhysicsPlaneShape(glm::vec3 normal, float distance)
		: PhysicsShape(PhysicsShape::ShapeType::PLANE),
		m_normal(normal),
		m_distanceToOrigin(distance) {}

	glm::vec3 GetNormal() { return m_normal; }
	float GetDistance() { return m_distanceToOrigin; }

private:
	glm::vec3	m_normal;
	float		m_distanceToOrigin;
};