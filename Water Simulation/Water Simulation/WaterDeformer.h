#ifndef WATER_DEFORMER_INCLUDED_H
#define WATER_DEFORMER_INCLUDED_H

#include <GL\glew.h>
#include <glm\glm.hpp>

class WaterDeformer
{
public:
	WaterDeformer(GLuint program)
	{
		uTime		= glGetUniformLocation(program, "time");
		uNumWaves	= glGetUniformLocation(program, "numWaves");
		uAmplitude	= glGetUniformLocation(program, "amplitude");
		uWavelength = glGetUniformLocation(program, "wavelength");
		uSpeed		= glGetUniformLocation(program, "speed");
		uDirection	= glGetUniformLocation(program, "direction");

		for (int i = 0; i < SIZE; ++i)
		{
			m_amplitude[i] = 0.25f / (i + 1);
			m_wavelength[i] = 8 * PI / (i + 1);
			m_speed[i] = 1.0f + 2 * i;
			float v = glm::radians(360.0f / SIZE * i);
			m_direction[i] = glm::vec2(glm::cos(v), glm::sin(v));
		}
	}

	void Update(float dt)
	{
		glUniform1f(uTime, dt);
		glUniform1i(uNumWaves, SIZE);
		glUniform1fv(uAmplitude, SIZE, m_amplitude);
		glUniform1fv(uWavelength, SIZE, m_wavelength);
		glUniform1fv(uSpeed, SIZE, m_speed);
		glUniform2fv(uDirection, SIZE, (float*) m_direction);
	}
	
private:
	const float PI = 3.14159f;
	static const int SIZE = 8;

	int uTime;
	int uNumWaves;
	int uAmplitude;
	int uWavelength;
	int uSpeed;
	int uDirection;

	float m_amplitude[SIZE];
	float m_wavelength[SIZE];
	float m_speed[SIZE];
	glm::vec2 m_direction[SIZE];

	float uniformRandomInRange(float min, float max)
	{		
		float n = (float) rand() / RAND_MAX;
		float v = min + n * (max - min);
		return v;
	}	
};

#endif