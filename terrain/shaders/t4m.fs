#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

// texture sampler
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;
uniform sampler2D diffuse4;
uniform sampler2D normal1;
uniform sampler2D normal2;
uniform sampler2D normal3;
uniform sampler2D normal4;
uniform sampler2D control;
uniform sampler2D reflection;
//uniform sampler2D texture2;

void main()
{
	vec4 control = texture(control, TexCoords);
	vec3 color1 = texture(diffuse1, TexCoords).rgb * control.r;
	vec3 color2 = texture(diffuse2, TexCoords).rgb * control.g;
	vec3 color3 = texture(diffuse3, TexCoords).rgb * control.b;
	vec3 color4 = texture(diffuse4, TexCoords).rgb * control.a;
	vec3 albedo = color1 + color2 + color3 + color4;
	FragColor = vec4(color1, 1.0);
}