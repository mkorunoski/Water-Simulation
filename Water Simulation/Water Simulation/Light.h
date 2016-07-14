#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

template <typename T>
std::string Str(const T & t) {
	std::ostringstream os;
	os << t;
	return os.str();
}

class DirectionalLight
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 position;

	GLuint uAmbient, uDiffuse, uSpecular, uPosition;

public:
	DirectionalLight() { }

	DirectionalLight& operator=(const DirectionalLight& light)
	{
		ambient   = light.ambient;
		diffuse   = light.diffuse;
		specular  = light.specular;
		position  = light.position;

		uAmbient  = light.uAmbient;
		uDiffuse  = light.uDiffuse;
		uSpecular = light.uSpecular;
		uPosition = light.uPosition;
		return *this;
	}

	DirectionalLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
		const glm::vec3& position)
	{
		this->ambient  = ambient;
		this->diffuse  = diffuse;
		this->specular = specular;
		this->position = position;
	}

	void SetUniforms(const GLuint& program)
	{
		uAmbient  = glGetUniformLocation(program, "directionalLight.light.ambient");
		uDiffuse  = glGetUniformLocation(program, "directionalLight.light.diffuse");
		uSpecular = glGetUniformLocation(program, "directionalLight.light.specular");
		uPosition = glGetUniformLocation(program, "directionalLight.light.position");
	}

	void Use()
	{
		glUniform3fv(uAmbient, 1, glm::value_ptr(ambient));
		glUniform3fv(uDiffuse, 1, glm::value_ptr(diffuse));
		glUniform3fv(uSpecular, 1, glm::value_ptr(specular));
		glUniform3fv(uPosition, 1, glm::value_ptr(position));
	}

	void InvertY()
	{
		this->position.y = (-1) * this->position.y;
	}

	void Move(GLfloat dx, GLfloat dz)
	{
		position.x = dx;
		position.z = dz;
	}

	~DirectionalLight() { }
};

class PointLight
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;

	GLuint uAmbient, uDiffuse, uSpecular, uPosition, uConstant, uLinear, uQuadratic;
	
public:
	PointLight() { }

	PointLight& operator=(const PointLight& light)
	{
		ambient   = light.ambient;
		diffuse   = light.diffuse;
		specular  = light.specular;
		position  = light.position;
		constant  = light.constant;
		linear    = light.linear;
		quadratic = light.quadratic;

		uAmbient   = light.uAmbient;
		uDiffuse   = light.uDiffuse;
		uSpecular  = light.uSpecular;
		uPosition  = light.uPosition;
		uConstant  = light.uConstant;
		uLinear    = light.uLinear;
		uQuadratic = light.uQuadratic;
		return *this;
	}

	PointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
		const glm::vec3& position, float constant, float linear, float quadratic)
	{
		this->ambient   = ambient;
		this->diffuse   = diffuse;
		this->specular  = specular;
		this->position  = position;
		this->constant  = constant;
		this->linear    = linear;
		this->quadratic = quadratic;
	}

	void SetUniforms(const GLuint& program, int light)
	{
		uAmbient   = glGetUniformLocation(program, ("pointLight[" + Str(light) + "].light.ambient").c_str());
		uDiffuse   = glGetUniformLocation(program, ("pointLight[" + Str(light) + "].light.diffuse").c_str());
		uSpecular  = glGetUniformLocation(program, ("pointLight[" + Str(light) + "].light.specular").c_str());
		uPosition  = glGetUniformLocation(program, ("pointLight[" + Str(light) + "].light.position").c_str());
		uConstant  = glGetUniformLocation(program, ("pointLight[" + Str(light) + "].constant").c_str());
		uLinear	   = glGetUniformLocation(program, ("pointLight[" + Str(light) + "].linear").c_str());
		uQuadratic = glGetUniformLocation(program, ("pointLight[" + Str(light) + "].quadratic").c_str());
	}

	void Use()
	{
		glUniform3fv(uAmbient, 1, glm::value_ptr(ambient));
		glUniform3fv(uDiffuse, 1, glm::value_ptr(diffuse));
		glUniform3fv(uSpecular, 1, glm::value_ptr(specular));
		glUniform3fv(uPosition, 1, glm::value_ptr(position));
		glUniform1f(uConstant, constant);
		glUniform1f(uLinear, linear);
		glUniform1f(uQuadratic, quadratic);
	}

	void InvertY()
	{
		this->position.y = (-1) * this->position.y;
	}

	~PointLight() { }
};

class SpotLight
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 position;
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;

	GLuint uAmbient, uDiffuse, uSpecular, uPosition, uDirection, uCutOff, uOuterCutOff;

public:
	SpotLight() { }

	SpotLight& operator=(const SpotLight& light)
	{
		ambient		= light.ambient;
		diffuse		= light.diffuse;
		specular	= light.specular;
		position	= light.position;
		direction	= light.direction;
		cutOff		= light.cutOff;
		outerCutOff = light.outerCutOff;

		uAmbient	 = light.uAmbient;
		uDiffuse	 = light.uDiffuse;
		uSpecular	 = light.uSpecular;
		uPosition	 = light.uPosition;
		uDirection	 = light.uDirection;
		uCutOff		 = light.uCutOff;
		uOuterCutOff = light.uOuterCutOff;
		return *this;
	}

	SpotLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
		const glm::vec3& position, const glm::vec3& direction, float cutOff, float outerCutOff)
	{
		this->ambient	  = ambient;
		this->diffuse	  = diffuse;
		this->specular	  = specular;
		this->position	  = position;
		this->direction	  = direction;
		this->cutOff	  = cutOff;
		this->outerCutOff = outerCutOff;
	}

	void SetUniforms(const GLuint& program)
	{
		uAmbient	 = glGetUniformLocation(program, "spotLight.light.ambient");
		uDiffuse	 = glGetUniformLocation(program, "spotLight.light.diffuse");
		uSpecular	 = glGetUniformLocation(program, "spotLight.light.specular");
		uPosition    = glGetUniformLocation(program, "spotLight.light.position");
		uDirection   = glGetUniformLocation(program, "spotLight.direction");
		uCutOff		 = glGetUniformLocation(program, "spotLight.cutOff");
		uOuterCutOff = glGetUniformLocation(program, "spotLight.outerCutOff");
	}
	
	void Use()
	{
		glUniform3fv(uAmbient, 1, glm::value_ptr(ambient));
		glUniform3fv(uDiffuse, 1, glm::value_ptr(diffuse));
		glUniform3fv(uSpecular, 1, glm::value_ptr(specular));
		glUniform3fv(uPosition, 1, glm::value_ptr(position));
		glUniform3fv(uDirection, 1, glm::value_ptr(direction));
		glUniform1f(uCutOff, glm::cos(glm::radians(cutOff)));
		glUniform1f(uOuterCutOff, glm::cos(glm::radians(outerCutOff)));
	}

	void SetPosition(const glm::vec3& position) { this->position = position; }

	void SetDirection(const glm::vec3& direction) { this->direction = direction; }

	~SpotLight() { }
};

#endif