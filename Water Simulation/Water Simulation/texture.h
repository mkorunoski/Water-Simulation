#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
#include <SOIL/SOIL.h>

class Texture
{
private:
	GLuint texture;

public:
	Texture() { }

	Texture& operator=(const Texture& texture)
	{
		this->texture = texture.texture;
		return *this;
	}

	Texture(const std::string& textureLocation)
	{
		int textureWidth, textureHeight;
		unsigned char* image = SOIL_load_image(textureLocation.c_str(), &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		SOIL_free_image_data(image);
	}

	void Use(const GLuint& program, const std::string& name, GLuint i)
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture);		
	}

	void Unuse()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~Texture()
	{
		// glDeleteTextures(1, &texture);
	}
};

#endif