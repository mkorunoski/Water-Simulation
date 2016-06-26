#include "CubemapTexure.h"
#include "stb_image.h"

CubemapTexure::CubemapTexure(const std::string& baseName, const std::string& format)
{
	std::string sides[] = { "right", "left", "top", "bottom", "back", "front" };
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	for (int i = 0; i < 6; ++i)
	{
		int width, height, numComponents;
		unsigned char* data = stbi_load((baseName + sides[i] + "." + format).c_str(), &width, &height, &numComponents, 4);

		if (data == NULL)
			std::cerr << "Unable to load texture: " << baseName + sides[i] << std::endl;
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubemapTexure::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

void CubemapTexure::Unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubemapTexure::~CubemapTexure()
{
	glDeleteTextures(1, &m_texture);
}
