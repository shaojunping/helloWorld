#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 VertexCol;

uniform sampler2D texture_diffuse1;

void main()
{    
    vec4 color = texture(texture_diffuse1, TexCoords);
	if(color.a < 0.5)
	{
		discard;
	}
	FragColor = vec4(color.rgb, 1.0);
}