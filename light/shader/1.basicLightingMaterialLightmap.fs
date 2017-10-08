#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
uniform Material material;
uniform vec3 viewPos;
uniform Light light;

void main()
{
	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//vec3 specular =  light.specular * (vec3(1.0) - vec3(texture(material.diffuse, TexCoords))) * spec;
	vec3 specular =  light.specular * (vec3(texture(material.specular, TexCoords))) * spec;
	
	vec3 result = specular;
	FragColor = vec4(result, 1.0);
}