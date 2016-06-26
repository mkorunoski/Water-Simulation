#version 420 core

const float PI = 3.14159f;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec3 f_position;
out vec2 f_texCoord;
out vec3 f_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseTranspose;
layout (binding = 0) uniform sampler2D heightmap;

uniform float time;
uniform int numWaves;
uniform float amplitude[8];
uniform float wavelength[8];
uniform float speed[8];
uniform vec2 direction[8];

float wave(int i, float x, float z);
float waveHeight(float x, float z);
float dWavedx(int i, float x, float z);
float dWavedz(int i, float x, float z);
vec3 waveNormal(float x, float z);

void main()
{
    float offset = 4.0f;
	vec3 deformedPosition = position;
    deformedPosition.y = offset +
        1.5f * texture(heightmap, texCoord).z +
        waveHeight(deformedPosition.x, deformedPosition.z);    
    vec3 deformedNormal = waveNormal(deformedPosition.x, deformedPosition.z);
    
	f_position 	= mat3(model) * deformedPosition;
	gl_Position = projection * view * model * vec4(deformedPosition, 1.0);
	f_texCoord  = texCoord;
	f_normal    = mat3(inverseTranspose) * deformedNormal;
}

float wave(int i, float x, float z)
{
    float frequency = 2 * PI / wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, z));
    return amplitude[i] * sin(theta * frequency + time * phase);
}

float waveHeight(float x, float z)
{
    float height = 0.0f;
    for (int i = 0; i < numWaves; ++i)
        height += wave(i, x, z);        
    return height;
}

float dWavedx(int i, float x, float z)
{
    float frequency = 2 * PI / wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, z));
    float A = amplitude[i] * direction[i].x * frequency;
    return A * cos(theta * frequency + time * phase);
}

float dWavedz(int i, float x, float z)
{
    float frequency = 2 * PI / wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, z));
    float A = amplitude[i] * direction[i].y * frequency;
    return A * cos(theta * frequency + time * phase);
}

vec3 waveNormal(float x, float z)
{
    float dx = 0.0f;
    float dz = 0.0f;
    for (int i = 0; i < numWaves; ++i)
    {
        dx += dWavedx(i, x, z);
        dz += dWavedz(i, x, z);
    }
    vec3 n = vec3(dx, 1.0f, dz);
    return normalize(n);
}