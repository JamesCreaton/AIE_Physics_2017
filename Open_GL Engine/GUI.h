#pragma once
#include <map>
#include "imgui_glfw3.h"

#include <glm\glm.hpp>

class GUI
{
public:
	GUI();
	~GUI();

	struct GUIProperties {
		glm::vec4			m_clearColour;
		bool				m_bDisplayLight;
		bool				m_bUseNormalMap;
		bool				m_bDistort;
		bool				m_bUseDynamicLight;
		float				m_fSpecularPower;
	};

	GUIProperties& GetProperties();

	void Update();

protected:
	std::map<std::string, bool>			m_boolValues;
	std::map<std::string, float>		m_floatValues;

	GUIProperties						m_properties;

};

