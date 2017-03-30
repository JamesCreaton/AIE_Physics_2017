#include "Rigidbody.h"


Rigidbody::Rigidbody()
{
}

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position,
	glm::vec2 velocity, float rotation, float mass)
{
}


Rigidbody::~Rigidbody()
{
}

bool Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	//Apply Gravity
	m_mass * -9.81;
	
	

	m_position += m_velocity;
	return false;
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 force)
{
	vec2 acceleration = vec2(force.x / m_mass, force.y / m_mass);

	m_velocity.x += acceleration.x *

}

void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	applyForce(force);
	actor2->applyForce(-force);
}
