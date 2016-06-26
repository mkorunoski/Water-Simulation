#version 420 core

const vec2 SIZE    = vec2(2.0, 0.0);
const ivec3 OFFSET = ivec3(-1, 0, 1);

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

vec4 bumpFromHeight();

void main()
{	
	vec4 bump = bumpFromHeight();

	vec3 displacedPosition = vec3(position.x, 15.0f * bump.a, position.z);
	f_position 	= mat3(model) * displacedPosition;
	gl_Position = projection * view * model * vec4(displacedPosition, 1.0);
	f_texCoord  = texCoord;
	f_normal    = mat3(inverseTranspose) * bump.xyz;
}

vec4 bumpFromHeight()
{
	vec4 s = texture(heightmap, texCoord);
	float s11 = s.x;
    float s01 = textureOffset(heightmap, texCoord, OFFSET.xy).x;
    float s21 = textureOffset(heightmap, texCoord, OFFSET.zy).x;
    float s10 = textureOffset(heightmap, texCoord, OFFSET.yx).x;
    float s12 = textureOffset(heightmap, texCoord, OFFSET.yz).x;
    vec3 va = normalize(vec3(SIZE.x, s21 - s01, SIZE.y));
    vec3 vb = normalize(vec3(SIZE.y, s12 - s10, -SIZE.x));
    return vec4( cross(va, vb), s11 );
}