#include "ParticleEmitter.h"



ParticleEmitter::ParticleEmitter()
	: m_pParticles(nullptr),
	m_uiFirstDead(0),
	m_uiMaxParticles(0),
	m_v3Position(0, 0, 0),
	m_vao(0), m_vbo(0), m_ibo(0),
	m_pVertexData(nullptr)
{
	m_fGravity = 0.5f;
}


ParticleEmitter::~ParticleEmitter()
{
	delete[] m_pParticles;
	delete[] m_pVertexData;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void ParticleEmitter::Initalise(unsigned int a_maxParticles, unsigned int a_emitRate,
	float a_lifetimeMin, float a_lifetimeMax,
	float a_velocityMin, float a_velocityMax,
	float a_startSize, float a_endSize,
	const vec4 & a_startColour, const vec4 & a_endColour)
{

	//set up emit timers
	m_fEmitTimer = 0;
	m_fEmitRate = 1.0f / a_emitRate;

	//store all variables passed in
	m_v4StartColour = a_startColour;
	m_v4EndColour = a_endColour;
	m_fStartSize = a_startSize;
	m_fEndSize = a_endSize;
	m_fVelocityMin = a_velocityMin;
	m_fVelocityMax = a_velocityMax;
	m_fLifespanMin = a_lifetimeMin;
	m_fLifespanMax = a_lifetimeMax;
	m_uiMaxParticles = a_maxParticles;

	//create particle array
	m_pParticles = new Particle[m_uiMaxParticles];
	m_uiFirstDead = 0;

	//Create the array of vertices for the particles
	//4 vertices per particle for a quad.
	//will be filled dring update
	m_pVertexData = new ParticleVertex[m_uiMaxParticles * 4];

	//Create the index buffer data for the particles
	//6 indices per quad of 2 triangles
	//fill it now as it never changes
	unsigned int* indexData = new unsigned int[m_uiMaxParticles * 6];
	for (unsigned int i = 0; i < m_uiMaxParticles; ++i) {
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	//create opengl buffers
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_uiMaxParticles * 4 * sizeof(ParticleVertex), m_pVertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiMaxParticles * 6 * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position;
	glEnableVertexAttribArray(1); //colour

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;

}

void ParticleEmitter::Emit()
{
	//only emit if there is a dead particle to use
	if (m_uiFirstDead >= m_uiMaxParticles)
		return;

	//resurrect the first dead particle
	Particle& particle = m_pParticles[m_uiFirstDead++];

	//assign its starting position
	particle.position = m_v3Position;

	//randomise its lifespan
	particle.lifetime = 0;
	particle.lifespan = (rand() / (float)RAND_MAX) * (m_fLifespanMax - m_fLifespanMin) + m_fLifespanMin;

	//set starting size and colour
	particle.colour = m_v4StartColour;
	particle.colour = m_v4EndColour;

	//randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * (m_fVelocityMax - m_fVelocityMin) + m_fVelocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;
}

void ParticleEmitter::Update(float a_deltaTime, const mat4& a_cameraTransform)
{
	// spawn particles 
	m_fEmitTimer += a_deltaTime;
	while (m_fEmitTimer > m_fEmitRate) {
		Emit();
		m_fEmitTimer -= m_fEmitRate;
	}

	unsigned int quad = 0;

	// update particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < m_uiFirstDead; i++) {
		Particle* particle = &m_pParticles[i];

		particle->lifetime += a_deltaTime;
		if (particle->lifetime >= particle->lifespan) {

			// swap last alive with this one
			*particle = m_pParticles[m_uiFirstDead - 1];
			m_uiFirstDead--;
		}
		else {

			// move particle
			particle->velocity.y -= m_fGravity;

			particle->position += particle->velocity * a_deltaTime;

			// size particle
			particle->size = glm::mix(m_fStartSize, m_fEndSize, particle->lifetime / particle->lifespan);

			// colour particle
			particle->colour = glm::mix(m_v4StartColour, m_v4EndColour, particle->lifetime / particle->lifespan);

			// make a quad the correct size and colour 
			float halfSize = particle->size * 0.5f;

			m_pVertexData[quad * 4 + 0].position = vec4(halfSize, halfSize, 0, 1);
			m_pVertexData[quad * 4 + 0].colour = particle->colour;
			m_pVertexData[quad * 4 + 1].position = vec4(-halfSize, halfSize, 0, 1);
			m_pVertexData[quad * 4 + 1].colour = particle->colour;
			m_pVertexData[quad * 4 + 2].position = vec4(-halfSize, -halfSize, 0, 1);
			m_pVertexData[quad * 4 + 2].colour = particle->colour;
			m_pVertexData[quad * 4 + 3].position = vec4(halfSize, -halfSize, 0, 1);
			m_pVertexData[quad * 4 + 3].colour = particle->colour;

			// create billboard transform 
			vec3 zAxis = glm::normalize(vec3(a_cameraTransform[3].xyz - particle->position));
			vec3 xAxis = glm::cross(vec3(a_cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(vec4(xAxis, 0), vec4(yAxis, 0), vec4(zAxis, 0), vec4(0, 0, 0, 1));

			m_pVertexData[quad * 4 + 0].position = billboard * m_pVertexData[quad * 4 + 0].position + vec4(particle->position, 0);
			m_pVertexData[quad * 4 + 1].position = billboard * m_pVertexData[quad * 4 + 1].position + vec4(particle->position, 0);
			m_pVertexData[quad * 4 + 2].position = billboard * m_pVertexData[quad * 4 + 2].position + vec4(particle->position, 0);
			m_pVertexData[quad * 4 + 3].position = billboard * m_pVertexData[quad * 4 + 3].position + vec4(particle->position, 0);

			++quad;
		}
	}
}

void ParticleEmitter::Draw()
{
	// sync the particle vertex buffer
	// based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_uiFirstDead * 4 * sizeof(ParticleVertex), m_pVertexData);

	// draw particles
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_uiFirstDead * 6, GL_UNSIGNED_INT, 0);
}