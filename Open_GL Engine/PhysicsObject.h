#pragma once
#include <glm\glm.hpp>
#include "PhysicsShape.h"

using glm::vec2;
using glm::vec3;


enum ShapeType
{
	PLANE	= 0,
	SPHERE	= 1,
	BOX		= 2,
};

class PhysicsObject
{

public:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
	PhysicsObject(const float a_mass);
	PhysicsObject();
	~PhysicsObject();


	void SetPosition(const glm::vec3 a_position) { m_position = a_position; }
	glm::vec3 GetPosition() { return m_position; }

	void SetRotation(const float a_rotation) { m_rotation = a_rotation; }
	float GetRotation() { return m_rotation; }

	void SetVelocity(const glm::vec3 a_velocity) { m_velocity = a_velocity; }
	glm::vec3 GetVelocity() { return m_velocity; }

	void SetMass(const float a_mass) { m_mass = a_mass; }
	float GetMass() { return m_mass; }

	void ApplyForce(const glm::vec3 force);

	void AddAcceleration(const glm::vec3 acceleration);


	void Update(float deltaTime);

	void AddShape(PhysicsShape* shape);
	void RemoveShape();
	PhysicsShape* GetShape() const { return m_shape; }

protected:
	ShapeType		m_shapeID;

private:

	glm::vec3   m_force;
	glm::vec3	m_acceleration;
	glm::vec3	m_position;
	glm::vec3	m_velocity;
	float		m_mass;

	float		m_rotation;

	PhysicsShape*	m_shape;
};

