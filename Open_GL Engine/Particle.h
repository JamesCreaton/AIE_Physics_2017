#pragma once
#include <glm\glm.hpp>
#include <vector>

struct Particle 
{
	Particle()
		: m_position(0)
		, m_velocity(0)
		, m_colour(0)
		, m_fRotate(0)
		, m_fAge(0)
		, m_fLifeTime(0)
	{}

	glm::vec3	m_position;	// Center point of particle
	glm::vec3	m_velocity;	// Current particle velocity
	glm::vec3	m_colour;	// Particle color

	float       m_fRotate;	// Rotate the particle the center
	float       m_fSize;    // Size of the particle
	float       m_fAge;
	float       m_fLifeTime;
};

typedef std::vector<Particle> ParticleBuffer;