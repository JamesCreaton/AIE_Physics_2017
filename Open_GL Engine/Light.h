#pragma once
#include <glm\glm.hpp>

class Light
{
public:

	enum LightType
	{
		Direction,
		Point,
		Spot,
		NumberOfLights
	};

	Light() = delete;
	Light(LightType type, glm::vec3 colour = glm::vec3(1, 1, 1)) : m_type(type), m_colour(colour) {}

	virtual void SetupLight(int shaderID, int lightID) =0;
	void SetColour(glm::vec3 colour){m_colour = colour;}

protected:
	LightType		m_type;
	glm::vec3		m_colour;

};