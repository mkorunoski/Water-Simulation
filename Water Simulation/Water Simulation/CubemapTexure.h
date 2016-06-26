#ifndef CUBEMAP_TEXTURE_H
#define CUBEMAP_TEXTURE_H

#include "GL\glew.h"
#include <iostream>
#include <string>

class CubemapTexure
{
public:
	CubemapTexure(const std::string& baseName, const std::string& format);

	void Bind();
	void Unbind();

	virtual ~CubemapTexure();
private:
	GLuint m_texture;
};

#endif

