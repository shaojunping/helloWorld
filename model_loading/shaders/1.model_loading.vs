#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float t;

void main()
{
    TexCoords = aTexCoords;    
	vec4 posWorld = model * vec4(aPos, 1.0);
	
	float windTime = t * 2.0;
	clamp(windTime, 0, 1);
	posWorld.x += cos(windTime + 0.2) * sin(posWorld.z + windTime) * 0.1;
	posWorld.z += sin(windTime + 0.3) * cos(posWorld.z + windTime) * 0.1;
    gl_Position = projection * view * posWorld;
}