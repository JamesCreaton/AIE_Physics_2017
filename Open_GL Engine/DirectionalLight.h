#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight() = delete;
	DirectionalLight(glm::vec3 colour);

	void SetupLight(int shaderID, int lightID) override;

	void SetDirection(glm::vec3 direction) { m_direction = direction; }
	glm::vec3 GetDirection() const { return m_direction; }

	void SetColour(glm::vec4 colour) { m_colour = colour; }
	glm::vec4 GetColour() const { return m_colour; }

private:
	glm::vec3 m_direction;
	glm::vec3 m_position;
	glm::vec4 m_colour;
};
