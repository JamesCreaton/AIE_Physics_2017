#include "DirectionalLight.h"
#include <string>
#include <gl_core_4_4.h>

DirectionalLight::DirectionalLight(glm::vec3 colour)
	:Light(LightType::Direction, colour)
	, m_direction(1, 0, 0)
	, m_position(0, 1, 0)
	, m_colour(colour.r, colour.g, colour.b, 1)
{
}

void DirectionalLight::SetupLight(int shaderID, int a_lightID)
{
	std::string lightID = std::string("light[") + std::to_string(a_lightID) + std::string("]");

	std::string uniformName = lightID + ".type";
	int loc = glGetUniformLocation(shaderID, uniformName.c_str());
	glUniform1i(loc, (int)m_type);

	uniformName = lightID + ".position";
	loc = glGetUniformLocation(shaderID, uniformName.c_str());
	glUniform3f(loc, m_position.x, m_position.y, m_position.z);

	uniformName = lightID + ".direction";
	loc = glGetUniformLocation(shaderID, uniformName.c_str());
	glUniform3f(loc, m_direction.x, m_direction.y, m_direction.z);

	uniformName = lightID + ".colour";
	loc = glGetUniformLocation(shaderID, uniformName.c_str());
	glUniform3f(loc, m_colour.r, m_colour.g, m_colour.b);
}
