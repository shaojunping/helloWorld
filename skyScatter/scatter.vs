#version 300 es
uniform highp mat4 ViewProjVS;
uniform highp vec4 CameraPosVS;
uniform highp mat4 World;
uniform highp vec4 cBeginLenUV;
in highp vec4 POSITION;
in highp vec2 TEXCOORD0;
out highp vec3 xlv_TEXCOORD0;
out highp vec4 xlv_TEXCOORD1;
void main ()
{
highp vec4 vPos_1;
highp vec4 tmpvar_2;
highp vec3 tmpvar_3;
highp vec4 tmpvar_4;
vPos_1.w = POSITION.w;
highp float tmpvar_5;
tmpvar_5 = ((1.570796 * cBeginLenUV.y) + ((1.570796 * cBeginLenUV.z) * POSITION.y));
vPos_1.y = cos(tmpvar_5);
vPos_1.xz = (POSITION.xz * sin(tmpvar_5));
highp vec4 tmpvar_6;
tmpvar_6.w = 1.0;
tmpvar_6.xyz = (vPos_1 * World).xyz;
tmpvar_2 = (tmpvar_6 * ViewProjVS);
tmpvar_2.z = (0.999995 * tmpvar_2.w);
tmpvar_3.xy = TEXCOORD0;
tmpvar_3.z = (cBeginLenUV.y + (TEXCOORD0.y * cBeginLenUV.z));
tmpvar_4.xyz = (tmpvar_6.xyz - CameraPosVS.xyz);
gl_Position.xyw = tmpvar_2.xyw;
xlv_TEXCOORD0 = tmpvar_3;
xlv_TEXCOORD1 = tmpvar_4;
gl_Position.z = ((tmpvar_2.z * 2.0) - tmpvar_2.w);
}
