#version 420 core
#extension GL_ARB_explicit_uniform_location : enable

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

struct DirectionalLight
{
	Light light;
};
uniform DirectionalLight directionalLight;

struct Maps
{
	sampler2D diffuse;	
	sampler2D normal;
	sampler2D specular;
};
uniform Maps maps;

in VS_OUT
{
	vec4 position;
	vec4 normal;
	vec2 texCoords;
} fs_in;

// Fragment shader uniforms base: 20
layout(location = 20) uniform vec3 eyePosition;

// Texture samplers base: 30
layout(location = 30) uniform samplerCube skyboxTex;

out vec4 fragColor;

void Phong(in Light light, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular)
{
	vec4 normal = normalize(fs_in.normal);
	
	vec4 lightVector, viewVector, reflectVector;
	float diff, spec;

	lightVector = normalize(vec4(light.position, 1.0f) - fs_in.position);
	diff = max(dot(normal, lightVector), 0.0f);

	viewVector    = normalize(vec4(eyePosition, 1.0f) - fs_in.position);
	reflectVector = normalize(reflect(-lightVector, normal));
	spec = pow(max(dot(viewVector, reflectVector), 0.0f), material.shininess);

	ambient  += 	   vec4(light.ambient, 1.0f);
	diffuse  += diff * vec4(light.diffuse, 1.0f);
	specular += spec * vec4(light.specular, 1.0f);	
}

void main()
{	
	vec4 ambient, diffuse, specular;	
	ambient = diffuse = specular = vec4(0.0f);

	Phong(directionalLight.light, ambient, diffuse, specular);		
	
    fragColor = (ambient + diffuse) * texture(maps.diffuse, fs_in.texCoords) + specular;
}