#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const float a_mass)
{
	m_mass = a_mass;
	m_position = glm::vec3(0, 0, 0);
	m_acceleration = glm::vec3(0, 0, 0);
	m_force = glm::vec3(0, 0, 0);
	m_shape = nullptr;
}

PhysicsObject::PhysicsObject()
{

}

PhysicsObject::~PhysicsObject()
{
	
}

void PhysicsObject::AddAcceleration(const glm::vec3 acceleration)
{
	m_acceleration = acceleration;
}

void PhysicsObject::Update(float deltaTime)
{
	m_acceleration += m_force / m_mass;
	m_velocity = m_velocity + m_acceleration + deltaTime;
	m_position = m_position + m_velocity * deltaTime;

	m_force = glm::vec3(0.0f, 0.0f, 0.0f);
	m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

void PhysicsObject::AddShape(PhysicsShape * shape)
{
	m_shape = shape;
}

void PhysicsObject::RemoveShape()
{
	delete m_shape;
	m_shape = nullptr;
}
