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
uniform TextureStruct rain;
uniform samplerCube skybox;
uniform vec3 viewPos;
uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec4 wetCol;
uniform float shininess;
uniform float ambientScale;
uniform float specularScale;
uniform float reflectionFactor;
uniform float rainFactor;

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
	vec4 reflection = CalColor(reflection, TexCoords);
	vec4 colors[NUM_TEXS];
	colors[0] = col[0] * controlCol.r;
	colors[1] = col[1] * controlCol.g;
	colors[2] = col[2] * controlCol.b;
	colors[3] = col[3] * controlCol.a;
	//FragColor = fixed4(colors[0] + colors[1] + colors[2] + colors[3], 1.0);
	vec4 colorSum = colors[0] + colors[1] + colors[2] + colors[3];

	vec3 albedo = vec3(0.0);
	vec3 normalVecs[NUM_TEXS];
	for(int i = 0; i < NUM_TEXS; i++)
	{
		vec4 normalFromMap = CalColor(normals[i], TexCoords);
		normalVecs[i] = getNormalFromMap(normalFromMap);
	}
	vec3 normalSum = normalVecs[0] * controlCol.r + normalVecs[1] * controlCol.g + normalVecs[2] * controlCol.b + normalVecs[3] * controlCol.a;
	normalSum = normalize(mix(normalSum, vec3(0.0, 0.0, 1.0), reflectionFactor * reflection.r));
	
	float nDotL = dot(normalSum, normalize(lightDir)) * 0.5 + 0.5;
	albedo +=  colorSum.rgb * nDotL;
	albedo *= lightCol;
	albedo = mix(albedo, albedo * wetCol.rgb, reflection.r);
	
	vec3 ambient = colorSum.rgb * ambientScale;
	vec3 viewDir = normalize(viewPos - FragPos);
	//FragColor = vec4(ambient + albedo, 1.0);
	
	vec3 half = normalize(normalSum + viewDir);
	float aSum = colors[0].a * controlCol.r + colors[1].a * controlCol.g + colors[2].a * controlCol.b + colors[3].a * controlCol.a;
	vec3 specular = lightCol * pow(max(0.0, dot(half, normalSum)), shininess) * aSum * specularScale;
	//FragColor = vec4(specularSum, 1.0f);

	vec4 rainMap = CalColor(rain, TexCoords);
	vec3 rainNormal = getNormalFromMap(rainMap);
	normalSum = mix(normalSum, rainNormal, rainFactor);
	normalSum = normalize(normalSum);
	vec3 reflectDir = reflect(viewDir, rainNormal);
	vec3 emission = mix(vec3(0,0,0), texture(skybox, reflectDir).rgb, reflection.r);
	//FragColor = vec4(emission * 1.4, 1.0);
	FragColor = vec4(albedo + ambient + specular + emission, 1.0);
}

vec4 CalColor(TextureStruct texture1, vec2 TexCoords)
{
	vec4 tex_st = texture1.tex_st;
	vec2 ownUv = TexCoords * tex_st.xy + tex_st.zw;
	vec4 color = texture(texture1.sample, ownUv);
	return color;
}

