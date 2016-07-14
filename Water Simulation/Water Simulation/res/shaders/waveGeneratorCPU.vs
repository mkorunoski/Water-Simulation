#version 420 core
#extension GL_ARB_explicit_uniform_location : enable

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

out VS_OUT
{
	vec4 position;
	vec4 normal;
	vec2 texCoords;
} vs_out;

void main()
{	
	vs_out.position  = model * vec4(position, 1.0f);
	vs_out.normal 	 = inverseTranspose * vec4(normal, 0.0f);
	vs_out.texCoords = texCoords;

    gl_Position = projection * view * model * vec4(position, 1.0f);        
}