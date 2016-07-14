#version 420 core
#extension GL_ARB_explicit_uniform_location : enable

// Input attributes base: 0
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout(std140) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

// Transformation matrices base: 10
layout(location = 10) uniform mat4 model;

out vec3 f_texCoords;

void main()
{
	f_texCoords	= position;
    gl_Position = projection * view * model * vec4(position, 1.0f);        
}
