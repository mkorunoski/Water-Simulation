#version 420 core
#extension GL_ARB_explicit_uniform_location : enable
#define PI 3.14159f
#define SIZE 8

// Input attributes base: 0
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;

layout(std140) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

// Transformation matrices base: 10
layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 inverseTranspose;

struct WaterDeformer
{
    float time;
    int numWaves;
    float amplitude[SIZE];
    float wavelength[SIZE];
    float speed[SIZE];
    vec2 direction[SIZE];
};
uniform WaterDeformer waterDeformer;

float wave(int i, float x, float z);
float waveHeight(float x, float z);
float dWavedx(int i, float x, float z);
float dWavedz(int i, float x, float z);
vec3 waveNormal(float x, float z);

out VS_OUT
{
	vec4 position;
	vec4 normal;
	vec2 texCoords;
} vs_out;

void main()
{	
	vec3 deformedPosition = position;
    deformedPosition.y = waveHeight(deformedPosition.x, deformedPosition.z);    
    vec3 deformedNormal = waveNormal(deformedPosition.x, deformedPosition.z);

	vs_out.position  = model * vec4(deformedPosition, 1.0f);
	vs_out.normal 	 = inverseTranspose * vec4(deformedNormal, 0.0f);
	vs_out.texCoords = texCoords;

    gl_Position = projection * view * model * vec4(deformedPosition, 1.0f);        
}




float wave(int i, float x, float z)
{
    float frequency = 2 * PI / waterDeformer.wavelength[i];
    float phase = waterDeformer.speed[i] * frequency;
    float theta = dot(waterDeformer.direction[i], vec2(x, z));
    return waterDeformer.amplitude[i] * sin(theta * frequency + waterDeformer.time * phase);
}

float waveHeight(float x, float z)
{
    float height = 0.0f;
    for (int i = 0; i < waterDeformer.numWaves; ++i)
        height += wave(i, x, z);        
    return height;
}

float dWavedx(int i, float x, float z)
{
    float frequency = 2 * PI / waterDeformer.wavelength[i];
    float phase = waterDeformer.speed[i] * frequency;
    float theta = dot(waterDeformer.direction[i], vec2(x, z));
    float A = waterDeformer.amplitude[i] * waterDeformer.direction[i].x * frequency;
    return A * cos(theta * frequency + waterDeformer.time * phase);
}

float dWavedz(int i, float x, float z)
{
    float frequency = 2 * PI / waterDeformer.wavelength[i];
    float phase = waterDeformer.speed[i] * frequency;
    float theta = dot(waterDeformer.direction[i], vec2(x, z));
    float A = waterDeformer.amplitude[i] * waterDeformer.direction[i].y * frequency;
    return A * cos(theta * frequency + waterDeformer.time * phase);
}

vec3 waveNormal(float x, float z)
{
    float dx = 0.0f;
    float dz = 0.0f;
    for (int i = 0; i < waterDeformer.numWaves; ++i)
    {
        dx += dWavedx(i, x, z);
        dz += dWavedz(i, x, z);
    }
    vec3 n = vec3(dx, 1.0f, dz);
    return normalize(n);
}