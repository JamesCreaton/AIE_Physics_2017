#pragma once
#include "Rigidbody.h"

class Rocket : public Rigidbody
{
public:
	Rocket();
	Rocket(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Rocket();

	void makeGizmo();

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

	void SetMass(float mass);

protected:
	float		m_radius;
	glm::vec4	m_colour;
};

