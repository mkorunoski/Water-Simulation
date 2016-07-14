#version 420 core
#extension GL_ARB_explicit_uniform_location : enable

in vec3 f_texCoords;

// Texture samplers base: 30
layout(location = 30) uniform samplerCube skyboxTex;

out vec4 fragColor;

void main()
{
	fragColor = texture(skyboxTex, f_texCoords);	
}