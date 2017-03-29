#pragma once
#include <string>

#include <gl_core_4_4.h>

class Texture
{
public:
	Texture(std::string a_Diffuse);
	~Texture();

	unsigned int* GetTexture() { return &m_uiTexture; }
	


private:
	int imgageWidth, imageHeight, imageFormat;

	unsigned char* data;

	unsigned int   m_uiTexture;
};

