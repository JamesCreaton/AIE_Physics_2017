#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture(std::string a_uiTexture) :
	imgageWidth(0),
	imageHeight(0),
	imageFormat(0),
	data(0)
{
	data = stbi_load(a_uiTexture.c_str(), &imgageWidth, &imageHeight, &imageFormat, STBI_default);


	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
}

Texture::~Texture()
{
	
}
