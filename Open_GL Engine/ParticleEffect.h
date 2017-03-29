#pragma once
#include "Particle.h"
#include <gl_core_4_4.h>
#include "FlyCamera.h"
#include <random>
#include "GeometryHelper.h"
#include "ParticleEmitter.h"

class ParticleEffect
{
public:

	// A vertex for the particle
	struct Vertex
	{
		Vertex()
			: m_position(0)
			, m_diffuse(0)
			, m_tex0(0)
		{}

		glm::vec3   m_position;      // Vertex position
		glm::vec4   m_diffuse;  // Diffuse color
		glm::vec2   m_tex0;     // Texture coordinate
	};

	typedef std::vector<Vertex> VertexBuffer;

	ParticleEffect(unsigned int numParticles = 0);
	virtual ~ParticleEffect();

	void SetCamera(FlyCamera* pCamera);
	void SetParticleEmitter(ParticleEmitter* pEmitter);

	// Test method to randomize the particles in an interesting way
	void RandomizeParticles();
	void EmitParticles();

	virtual void Update(float fDeltaTime);
	virtual void Render();

	bool LoadTexture(const std::string& fileName);

	// Resize the particle buffer with numParticles
	void Resize(unsigned int numParticles);

protected:
	void RandomizeParticle(Particle& particle);
	void EmitParticle(Particle& particle);
public:
	// Build the vertex buffer from the particle buffer
	void BuildVertexBuffer();
private:

	FlyCamera*          m_pCamera;
	ParticleEmitter*    m_pParticleEmitter;

	ParticleBuffer      m_particles;
	VertexBuffer        m_vertexBuffer;
	glm::mat4x4         m_localToWorldMatrix;
	GLuint              m_textureID;

	// Apply this force to every particle in the effect
	glm::vec3           m_force;
};

