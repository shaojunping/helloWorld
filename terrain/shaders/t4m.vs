#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 WorldPos;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float t;

void main()
{
    TexCoords = aTexCoords;    
	vec4 posWorld = model * vec4(aPos, 1.0);
	Normal = mat3(model) * aNormal;  
	
	WorldPos = posWorld.xyz;
	gl_Position = projection * view * posWorld;
	FragPos = gl_Position.xyz;
}