#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	float ambientScale = 0.1;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = lightColor * diff; 
	vec3 ambient = lightColor * ambientScale;
	vec3 result = objectColor * (ambient + diffuse);
	FragColor = vec4(result, 1.0);
}