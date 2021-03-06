#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	float ambientScale = 0.1;
	float specularStrength = 0.5;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = lightColor * diff; 
	vec3 ambient = lightColor * ambientScale;
	vec3 result = objectColor * (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
}