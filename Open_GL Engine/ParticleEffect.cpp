#include "ParticleEffect.h"


ParticleEffect::ParticleEffect(unsigned int numParticles /* = 0 */)
	: m_pCamera(NULL)
	, m_pParticleEmitter(NULL)
	, m_localToWorldMatrix(1)
	, m_textureID(0)
	, m_force(0, -9.81f, 0)
{
	Resize(numParticles);
}

ParticleEffect::~ParticleEffect()
{
	if (m_textureID != 0)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}
}

void ParticleEffect::SetCamera(FlyCamera* pCamera)
{
	m_pCamera = pCamera;
}

void ParticleEffect::SetParticleEmitter(ParticleEmitter* pEmitter)
{
	m_pParticleEmitter = pEmitter;
}

void ParticleEffect::Update(float fDeltaTime)
{
}

void ParticleEffect::Render()
{
}

bool ParticleEffect::LoadTexture(const std::string& fileName)
{
	if (m_textureID != 0) {
		glDeleteTextures(1, &m_textureID);
	}

	//Need to change over implementation from SOIL to current texture loading method

	//m_textureID = GeometryHelper::LoadOBJFromDisk(fileName);

	return (m_textureID != 0);
}

void ParticleEffect::Resize(unsigned int numParticles)
{
}

