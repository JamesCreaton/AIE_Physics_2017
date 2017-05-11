#pragma once

#include "PhysicsShape.h"
#include <glm\glm.hpp>

class PhysicsPlaneShape : public PhysicsShape
{
public:
	PhysicsPlaneShape() = delete;
	PhysicsPlaneShape(float radius)
		: PhysicsShape(PhysicsShape::ShapeType::PLANE),
		m_position(glm::vec3(0, 0, 0)) {}

	void SetPosition(glm::vec3 position) { m_position = position; }
	glm::vec3 GetRadius() const { return m_position; }

private:
	glm::vec3	m_position;
	glm::vec3	m_normal;
	float		m_constant;
};