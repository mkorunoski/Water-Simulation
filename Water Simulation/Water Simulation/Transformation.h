#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class Transformation
{
private:
	glm::mat4 scale;
	glm::mat4 rotate;
	glm::mat4 translate;

	glm::mat4 modelMatrix;
	glm::mat4 inverseTranspose;

	void UpdateModelMatrix()
	{
		modelMatrix = translate * rotate * scale;
		glm::inverseTranspose(modelMatrix);
	}

public:
	Transformation()
	{
		scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
		rotate = glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
		UpdateModelMatrix();
	}

	Transformation& operator=(const Transformation& transformation)
	{
		scale = transformation.scale;
		rotate = transformation.rotate;
		translate = transformation.translate;

		modelMatrix = transformation.modelMatrix;
		return *this;
	}

	void Scale(const glm::vec3& v)
	{
		scale = glm::scale(v);
		UpdateModelMatrix();
	}

	void Rotate(GLfloat a, const glm::vec3& v)
	{
		rotate = glm::rotate(a, v);
		UpdateModelMatrix();
	}

	void Translate(const glm::vec3& v)
	{
		translate = glm::translate(v);
		UpdateModelMatrix();
	}

	glm::mat4& GetModel() { return modelMatrix; }
	glm::mat4& GetInverseTranspose() { return inverseTranspose; }

	~Transformation() { }
};

#endif