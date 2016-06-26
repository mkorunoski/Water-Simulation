#ifndef LIGHT_INCLUDED_H
#define LIGHT_INCLUDED_H

#include "glm\glm.hpp"
#include "GL\glew.h"

class Material
{
public:
	Material(
		const glm::vec4& ambient,
		const glm::vec4& diffuse,
		const glm::vec4& specular,
		float shininess)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}

	void Bind(GLuint program)
	{
		int uAmbient = glGetUniformLocation(program, "material.ambient");
		int uDiffuse = glGetUniformLocation(program, "material.diffuse");
		int uSpecular = glGetUniformLocation(program, "material.specular");
		int uShininess = glGetUniformLocation(program, "material.shininess");

		glUniform4fv(uAmbient, 1, &ambient[0]);
		glUniform4fv(uDiffuse, 1, &diffuse[0]);
		glUniform4fv(uSpecular, 1, &specular[0]);
		glUniform1f(uShininess, shininess);
	}

	virtual ~Material() { }
private:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;
};

class PointLight
{
public:
	PointLight(
		const glm::vec4& ambient,
		const glm::vec4& diffuse,
		const glm::vec4& specular,
		const glm::vec3& position,
		float attenuation,
		int specularStrength
		)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->position = position;
		this->attenuation = attenuation;
		this->specularStrength = specularStrength;
	}

	void Bind(GLuint program)
	{
		int uAmbient = glGetUniformLocation(program, "pointLight.ambient");
		int uDiffuse = glGetUniformLocation(program, "pointLight.diffuse");
		int uSpecular = glGetUniformLocation(program, "pointLight.specular");
		int uPosition = glGetUniformLocation(program, "pointLight.position");		
		int uAttenuation = glGetUniformLocation(program, "pointLight.attenuation");
		int uSpecularStrength = glGetUniformLocation(program, "pointLight.specularStrength");

		glUniform4fv(uAmbient, 1, &ambient[0]);
		glUniform4fv(uDiffuse, 1, &diffuse[0]);
		glUniform4fv(uSpecular, 1, &specular[0]);
		glUniform3fv(uPosition, 1, &position[0]);
		glUniform1f(uAttenuation, attenuation);
		glUniform1i(uSpecularStrength, specularStrength);
	}

	virtual ~PointLight() {}
private:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec3 position;
	float attenuation;
	int specularStrength;
};

#endif