#version 330 core

struct PointLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
	float attenuation;
	int specularStrength;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

in vec3 f_position;
in vec2 f_texCoord;
in vec3 f_normal;

uniform samplerCube skybox;
uniform sampler2D tex;
uniform PointLight pointLight;
uniform Material material;
uniform vec3 eyePos;

out vec4 fragColor;

void computePointLight(out vec4 diffuse, out vec4 specular);

void main()
{	
	vec3 normal = normalize(f_normal);

	vec4 diffuse;
	vec4 specular;
	computePointLight(diffuse, specular);

	// reflection
	vec3 I = normalize(f_position - eyePos);
    vec3 R = reflect(I, normal);
    vec4 reflectionColor = texture(skybox, R);

	fragColor = reflectionColor * (diffuse + specular);
}

void computePointLight(out vec4 diffuse, out vec4 specular)
{
	vec3 lightVector;

	lightVector 	= normalize(pointLight.position - f_position);
	float distance	= length(lightVector);
	float diff 		= max(dot(f_normal, lightVector), 0.1f);
	diff 			= diff * 1.0f / (pointLight.attenuation + pow(distance, 2));
	diffuse 		= material.diffuse * (diff * pointLight.diffuse);

	vec3 viewVector 	= normalize(eyePos - f_position);
	lightVector 		= normalize(pointLight.position - f_position);
	vec3 reflectVector 	= reflect(-lightVector, f_normal);
	float spec 			= pow(max(dot(viewVector, reflectVector), 0.1f), material.shininess);
	specular 			= material.specular * (spec * pointLight.specular * pointLight.specularStrength);
}