#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
float ambientScale = 0.1;
	FragColor = vec4(objectColor * lightColor * ambientScale, 1.0);
}