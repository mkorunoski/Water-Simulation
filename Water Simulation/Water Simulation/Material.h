#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{	
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;

public:
	Material() { }

	Material& operator=(const Material& material)
	{
		ambient   = material.ambient;
		diffuse   = material.diffuse;
		specular  = material.specular;
		shininess = material.shininess;
		return *this;
	}

	Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, GLfloat shininess)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}

	void Use(const GLuint& program)
	{
		glUniform3fv(glGetUniformLocation(program, "material.ambient"), 1, glm::value_ptr(ambient));
		glUniform3fv(glGetUniformLocation(program, "material.diffuse"), 1, glm::value_ptr(diffuse));
		glUniform3fv(glGetUniformLocation(program, "material.specular"), 1, glm::value_ptr(specular));
		glUniform1f(glGetUniformLocation(program, "material.shininess"), shininess);
	}

	~Material() { }
};

#endif