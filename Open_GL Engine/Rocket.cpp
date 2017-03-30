#include "Rocket.h"
#include <Gizmos.h>



Rocket::Rocket(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_radius = radius;
	m_colour = colour;
}


Rocket::~Rocket()
{
}

void Rocket::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 30, m_colour);
}

void Rocket::SetMass(float mass)
{
	m_mass = mass;
}

