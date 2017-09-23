#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 viewPos;



void main()
{
    //ambient
	//float ambientStrength = 0.5;
	vec3 ambient = material.ambient * light.ambient;

	//diffuse
	//float diffuseStrength = 0.5;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = diff * material.diffuse * light.diffuse;

	//specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * material.specular;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}