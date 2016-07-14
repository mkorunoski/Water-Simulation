#ifndef WATER_DEFORMER_INCLUDED_H
#define WATER_DEFORMER_INCLUDED_H

#include <GL\glew.h>
#include <glm\glm.hpp>

#define PI 3.14159f
#define SIZE 6

class WaveGenerator
{
protected:
	float amplitude[SIZE];
	float wavelength[SIZE];
	float speed[SIZE];
	glm::vec2 direction[SIZE];

public:
	WaveGenerator()
	{
		for (int i = 0; i < SIZE; ++i)
		{
			amplitude[i] = 0.25f / (i + 1);
			wavelength[i] = 8 * PI / (i + 1);
			speed[i] = 1.0f + 2 * i;
			float v = glm::radians(360.0f / SIZE * i);
			direction[i] = glm::vec2(glm::cos(v), glm::sin(v));
		}
	}
};

class WaveGeneratorGPU : private WaveGenerator 
{
private:
	int uTime;
	int uNumWaves;
	int uAmplitude;
	int uWavelength;
	int uSpeed;
	int uDirection;

public:
	WaveGeneratorGPU() : WaveGenerator() { }

	WaveGeneratorGPU(GLuint program)
	{
		uTime		= glGetUniformLocation(program, "waterDeformer.time");
		uNumWaves	= glGetUniformLocation(program, "waterDeformer.numWaves");
		uAmplitude	= glGetUniformLocation(program, "waterDeformer.amplitude");
		uWavelength = glGetUniformLocation(program, "waterDeformer.wavelength");
		uSpeed		= glGetUniformLocation(program, "waterDeformer.speed");
		uDirection	= glGetUniformLocation(program, "waterDeformer.direction");
	}

	WaveGeneratorGPU& operator=(const WaveGeneratorGPU& waterDeformer)
	{
		uTime		= waterDeformer.uTime;
		uNumWaves	= waterDeformer.uNumWaves;
		uAmplitude	= waterDeformer.uAmplitude;
		uWavelength	= waterDeformer.uWavelength;
		uSpeed		= waterDeformer.uSpeed;
		uDirection	= waterDeformer.uDirection;

		for (int i = 0; i < SIZE; ++i) {
			amplitude[i]	= waterDeformer.amplitude[i];
			wavelength[i]	= waterDeformer.wavelength[i];
			speed[i]		= waterDeformer.speed[i];
			direction[i]	= waterDeformer.direction[i];
		}
		
		return *this;
	}

	void Update(float dt)
	{
		glUniform1f(uTime, dt);
		glUniform1i(uNumWaves, SIZE);
		glUniform1fv(uAmplitude, SIZE, amplitude);
		glUniform1fv(uWavelength, SIZE, wavelength);
		glUniform1fv(uSpeed, SIZE, speed);
		glUniform2fv(uDirection, SIZE, (float*) direction);
	}
};

class WaveGeneratorCPU : private WaveGenerator
{
private:
	GLfloat time;
public:
	WaveGeneratorCPU()  : WaveGenerator() { }

	void Update(GLfloat dt, std::vector<Vertex>& vertices)
	{
		time = dt;
		for (GLuint i = 0; i < vertices.size(); ++i)
		{
			glm::vec3 deformedPosition = vertices[i].position;
			deformedPosition.y = waveHeight(deformedPosition.x, deformedPosition.z);
			glm::vec3 deformedNormal = waveNormal(deformedPosition.x, deformedPosition.z);

			vertices[i].position = deformedPosition;
			vertices[i].normal   = deformedNormal;
		}
	}

	~WaveGeneratorCPU() { }

private:	
	float wave(int i, float x, float z)
	{
		float frequency = 2 * PI / wavelength[i];
		float phase = speed[i] * frequency;
		float theta = glm::dot(direction[i], glm::vec2(x, z));
		return amplitude[i] * sin(theta * frequency + time * phase);
	}

	float waveHeight(float x, float z)
	{
		float height = 0.0f;
		for (int i = 0; i < SIZE; ++i)
			height += wave(i, x, z);
		return height;
	}

	float dWavedx(int i, float x, float z)
	{
		float frequency = 2 * PI / wavelength[i];
		float phase = speed[i] * frequency;
		float theta = glm::dot(direction[i], glm::vec2(x, z));
		float A = amplitude[i] * direction[i].x * frequency;
		return A * cos(theta * frequency + time * phase);
	}

	float dWavedz(int i, float x, float z)
	{
		float frequency = 2 * PI / wavelength[i];
		float phase = speed[i] * frequency;
		float theta = glm::dot(direction[i], glm::vec2(x, z));
		float A = amplitude[i] * direction[i].y * frequency;
		return A * cos(theta * frequency + time * phase);
	}

	glm::vec3 waveNormal(float x, float z)
	{
		float dx = 0.0f;
		float dz = 0.0f;
		for (int i = 0; i < SIZE; ++i)
		{
			dx += dWavedx(i, x, z);
			dz += dWavedz(i, x, z);
		}
		glm::vec3 n = glm::vec3(dx, 1.0f, dz);
		return normalize(n);
	}
};

#endif