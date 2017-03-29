#include "GUI.h"



GUI::GUI()
{
	
}


GUI::~GUI()
{

}

GUI::GUIProperties& GUI::GetProperties()
{
	return m_properties;
}

void GUI::Update()
{
	ImGui::Begin("My Options");
	ImGui::ColorEdit3("Clear Colour", &(m_properties.m_clearColour)[0]);
	ImGui::Checkbox("Display Light", &m_properties.m_bDisplayLight);
	ImGui::Checkbox("Use Dynamic Light", &m_properties.m_bUseDynamicLight);
	ImGui::Checkbox("Use Normal Map", &m_properties.m_bUseNormalMap);
	ImGui::Checkbox("Distort Model", &m_properties.m_bDistort);
	ImGui::DragFloat("Specular Power", &m_properties.m_fSpecularPower);
	ImGui::End();
}
