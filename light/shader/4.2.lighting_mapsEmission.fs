#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	sampler2D emission;
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
	vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;

	//diffuse
	//float diffuseStrength = 0.5;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = diff * texture(material.diffuse, TexCoords).rgb * light.diffuse;

	//specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * texture(material.specular, TexCoords).rgb;

	//emission
	vec3 emission = texture(material.emission, TexCoords).rgb;

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
}