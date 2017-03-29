#pragma once

#define GLM_SWIZZLE
#include <glm\glm.hpp>
#include <gl_core_4_4.h>

#include "Shader.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class ParticleEmitter
{
public:

	struct Particle {
		vec3 position;
		vec3 velocity;
		vec4 colour;
		float size;
		float lifetime;
		float lifespan;
	};

	struct ParticleVertex {
		vec4 position;
		vec4 colour;
	};


	ParticleEmitter();
	virtual ~ParticleEmitter();

	void Initalise(unsigned int a_maxParticles, unsigned int a_emitRate,
		float a_lifetimeMin, float a_lifetimeMax,
		float a_velocityMin, float a_velocityMax,
		float a_startSize, float a_endSize,
		const vec4& a_startColour, const vec4& a_endColour);

	void Emit();
	void Update(float a_deltaTime, const mat4& a_cameraTransform);
	void Draw();


protected:
	
	Particle*			m_pParticles;
	unsigned int		m_uiFirstDead;
	unsigned int		m_uiMaxParticles;

	unsigned int		m_vao, m_vbo, m_ibo;
	ParticleVertex*		m_pVertexData;

	vec3				m_v3Position;

	float               m_fEmitTimer;
	float               m_fEmitRate;
	
	float               m_fLifespanMin;
	float				m_fLifespanMax;

	float				m_fVelocityMin;
	float				m_fVelocityMax;

	float				m_fStartSize;
	float               m_fEndSize;

	vec4				m_v4StartColour;
	vec4				m_v4EndColour;

	float               m_fGravity;

};

