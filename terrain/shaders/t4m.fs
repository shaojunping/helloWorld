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
uniform TextureStruct control;
uniform TextureStruct normals[NUM_TEXS];
uniform TextureStruct reflection;
uniform TextureStruct rain;
uniform TextureStruct snowDiffuse;
uniform TextureStruct snowNormal;
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
uniform float snowFactor;

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
	vec4 colorSum = colors[0] + colors[1] + colors[2] + colors[3];
	vec4 snow = CalColor(snowDiffuse, TexCoords);
	//FragColor = snow;
// return;
	vec3 normalVecs[NUM_TEXS];
	for(int i = 0; i < NUM_TEXS; i++)
	{
		vec4 normalFromMap = CalColor(normals[i], TexCoords);
		normalVecs[i] = getNormalFromMap(normalFromMap);
	}
	vec3 normalSum = normalize(normalVecs[0] * controlCol.r + normalVecs[1] * controlCol.g + normalVecs[2] * controlCol.b + normalVecs[3] * controlCol.a);

	vec4 reflection = CalColor(reflection, TexCoords);
	normalSum = normalize(mix(normalSum, vec3(0.0, 1.0, 0.0), reflectionFactor * reflection.r));
	
	vec4 snowMap = CalColor(snowNormal, TexCoords);
	vec3 snowNormal = getNormalFromMap(snowMap);
	normalSum = mix(normalSum, snowNormal, snowFactor);

	float nDotL = max(0, dot(normalSum, normalize(lightDir))) * 0.5 + 0.5;
	nDotL = mix(nDotL, 1.0, snowFactor);
	colorSum.rgb = mix(colorSum.rgb, colorSum.rgb * wetCol.rgb,  rainFactor * reflection.r);
	//FragColor = colorSum;
	colorSum.rgb = mix(colorSum.rgb, snow.rgb, snowFactor);
	//FragColor = colorSum;
	vec3 albedo =  colorSum.rgb * nDotL;
	albedo *= lightCol;
	
	vec3 ambient = colorSum.rgb * ambientScale;
	vec3 viewDir = normalize(viewPos - WorldPos);
	// FragColor = vec4(ambient + albedo, 1.0);
	// FragColor = vec4(albedo, 1.0);
	vec3 half = normalize(normalize(lightDir) + viewDir);
	float aSum = colorSum.a;
	float shininessFactor = shininess * mix(1.0, 0.2, reflection.r);
	//vec3 specular = lightCol * pow(max(0.0, dot(half, normalSum)), shininess) * aSum * specularScale;
	vec3 specular = lightCol * pow(max(0.0, dot(half, normalSum)), shininessFactor) * aSum;
	// FragColor = vec4(specular, 1.0f);
	
	vec4 rainMap = CalColor(rain, TexCoords);
	vec3 rainNormal = getNormalFromMap(rainMap);
	vec3 rainSum = mix(normalSum, normalize(rainNormal + normalSum), rainFactor);

	vec3 R = reflect(-viewDir, normalSum);
	vec3 rainR = reflect(-viewDir, rainSum);
	vec3 reflectionCol = mix(texture(skybox, R).rgb * 0.1, texture(skybox, rainR).rgb * 0.2, rainFactor);// * clamp(1.0 - clamp(dot(half, normalSum), 0.0, 1.0), 0.0, 1.0);

	float alpha = mix(1.0, 0.7, reflection.r);
	FragColor = vec4(albedo + specular + reflectionCol, alpha);
}

vec4 CalColor(TextureStruct texture1, vec2 TexCoords)
{
	vec4 tex_st = texture1.tex_st;
	vec2 ownUv = TexCoords * tex_st.xy + tex_st.zw;
	vec4 color = texture(texture1.sample, ownUv);
	return color;
}

