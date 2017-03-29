#pragma once
#include <glm\glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::mat4;

struct GUIProperties {
	glm::vec4			m_clearColour;
	bool				m_bDisplayLight;
	bool				m_bUseNormalMap;
	bool				m_bDistort;
	bool				m_bUseDynamicLight;
	float				m_fSpecularPower;
};