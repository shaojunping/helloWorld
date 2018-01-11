#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

struct TextureStruct 
{
	sampler2D sample;
	
	vec4 tex_st;
};

vec4 CalColor(TextureStruct texture, vec2 texCoords);
#define NUM_TEXS 4

uniform TextureStruct diffuses[NUM_TEXS];
uniform TextureStruct normals[NUM_TEXS];
uniform TextureStruct control;
uniform TextureStruct reflection;


void main()
{
	vec4 col[NUM_TEXS];
	for(int i = 0; i < NUM_TEXS; i++)
	{
		col[i] = CalColor(diffuses[i], TexCoords);
	}
	vec4 controlCol = CalColor(control, TexCoords);
	vec3 colors[NUM_TEXS];
	colors[0] = col[0].rgb * controlCol.r;
	colors[1] = col[1].rgb * controlCol.g;
	colors[2] = col[2].rgb * controlCol.b;
	colors[3] = col[3].rgb * controlCol.a;
	vec3 albedo = colors[0] + colors[1] + colors[2] + colors[3];
	FragColor = vec4(albedo, 1.0);
}

vec4 CalColor(TextureStruct texture1, vec2 TexCoords)
{
	vec4 tex_st = texture1.tex_st;
	vec2 ownUv = TexCoords * tex_st.xy + tex_st.zw;
	vec4 color = texture(texture1.sample, ownUv);
	return color;
}