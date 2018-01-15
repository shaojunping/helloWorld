#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec3 FragPos;

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
uniform vec3 viewPos;
uniform vec3 lightDir;
uniform float ambientScale;
uniform vec3 lightCol;
uniform float shininess;

// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap(vec4 normalData)
{
    vec3 tangentNormal = normalData.xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
void main()
{
	vec4 col[NUM_TEXS];
	for(int i = 0; i < NUM_TEXS; i++)
	{
		col[i] = CalColor(diffuses[i], TexCoords);
	}
	vec4 controlCol = CalColor(control, TexCoords);
	vec4 colors[NUM_TEXS];
	colors[0] = col[0] * controlCol.r;
	colors[1] = col[1] * controlCol.g;
	colors[2] = col[2] * controlCol.b;
	colors[3] = col[3] * controlCol.a;
	//FragColor = fixed4(colors[0] + colors[1] + colors[2] + colors[3], 1.0);
	//vec3 ambient = albedo * 
	
	vec3 albedo = vec3(0.0);
	vec3 normalVecs[NUM_TEXS];
	for(int i = 0; i < NUM_TEXS; i++)
	{
		vec4 normalFromMap[NUM_TEXS]; 
		normalFromMap[i] = CalColor(normals[i], TexCoords);
		normalVecs[i] = getNormalFromMap(normalFromMap[i]);
		float nDotL = dot(normalVecs[i], normalize(lightDir)) * 0.5 + 0.5;
		albedo +=  colors[i].rgb * nDotL;
	}
	albedo *= lightCol;
	//FragColor = vec4(albedo, 1.0);
	vec3 color = vec3(0.0);
	for(int i = 0; i < NUM_TEXS; i++)
	{
		color += colors[i].rgb;
	}
	vec3 ambient = color * ambientScale;
	vec3 viewDir = normalize(viewPos - FragPos);
	//FragColor = vec4(ambient + albedo, 1.0);
	
	vec3 specularSum = vec3(0.0);
	for(int i = 0; i < NUM_TEXS; i++)
	{
		vec3 half = normalize(normalVecs[i] + viewDir);
		vec3 specular = lightCol * pow(max(0.0, dot(half, normalVecs[i])), shininess) * colors[i].a;
		specularSum += specular;
	}
	//FragColor = vec4(specularSum, 1.0f);
	FragColor = vec4(albedo + ambient + specularSum, 1.0);
}

vec4 CalColor(TextureStruct texture1, vec2 TexCoords)
{
	vec4 tex_st = texture1.tex_st;
	vec2 ownUv = TexCoords * tex_st.xy + tex_st.zw;
	vec4 color = texture(texture1.sample, ownUv);
	return color;
}

