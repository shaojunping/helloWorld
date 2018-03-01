#version 300 es
uniform highp vec4 EnvInfo;
uniform highp vec4 SunDirection;
uniform highp vec4 FogColor;
uniform highp vec4 ScreenColor;
uniform highp vec4 FogColor2;
uniform highp vec4 FogColor3;
uniform highp vec4 cFogParam;
uniform highp float cGlobalUBegin;
uniform highp float cGlobalURepeat;
uniform highp float cGlobalVBegin;
uniform highp float cGlobalVLen;
uniform highp float cGlobalHDR;
uniform highp float cLocalHDR;
uniform highp sampler2D sSkyLayer;
in highp vec3 xlv_TEXCOORD0;
in highp vec4 xlv_TEXCOORD1;
out highp vec4 SV_Target;
void main ()
{
  mediump float VoL_1;
  mediump vec3 fogColor_2;
  highp vec2 baseTC1_3;
  highp vec4 Color_4;
  baseTC1_3.x = ((xlv_TEXCOORD0.x * cGlobalURepeat) + cGlobalUBegin);
  baseTC1_3.y = ((xlv_TEXCOORD0.y * cGlobalVLen) + cGlobalVBegin);
  highp vec3 tmpvar_5;
  tmpvar_5 = normalize(-(xlv_TEXCOORD1.xyz)); //position
  highp vec4 tmpvar_6;
  tmpvar_6.x = cGlobalHDR;            //2.5
  tmpvar_6.y = clamp (cGlobalHDR, 0.0, 1.0);
  tmpvar_6.z = cLocalHDR;             //0
  tmpvar_6.w = clamp (cLocalHDR, 0.0, 1.0);
  highp vec4 tmpvar_7;
  tmpvar_7 = texture (sSkyLayer, baseTC1_3);   //uv 采样
  highp vec4 tmpvar_8;
  tmpvar_8.xyz = (tmpvar_7.xyz * tmpvar_7.xyz);  
  tmpvar_8.w = tmpvar_7.w;
  Color_4 = (tmpvar_8 * (tmpvar_6.xxxy - (tmpvar_6.xxxy * cFogParam.z)));
  highp float tmpvar_9;
  tmpvar_9 = clamp ((xlv_TEXCOORD0.z - cFogParam.x), 0.0, 1.0); 
  highp float tmpvar_10;
  tmpvar_10 = clamp (((tmpvar_9 * tmpvar_9) * cFogParam.y), 0.0, 1.0);
  highp vec3 tmpvar_11;
  tmpvar_11 = ((FogColor2.xyz * clamp (
    ((tmpvar_5.y * 5.0) + 1.0)
  , 0.0, 1.0)) + FogColor.xyz);
  fogColor_2 = tmpvar_11;
  highp float tmpvar_12;
  tmpvar_12 = clamp (dot (-(tmpvar_5), SunDirection.xyz), 0.0, 1.0);
  VoL_1 = tmpvar_12;
  fogColor_2 = (fogColor_2 + (FogColor3 * (VoL_1 * VoL_1)).xyz);
  Color_4.xyz = ((Color_4.xyz * (1.0 - tmpvar_10)) + ((
    (Color_4.xyz * (1.0 - tmpvar_10))
   + fogColor_2) * tmpvar_10));
  Color_4.xyz = (Color_4.xyz * EnvInfo.z);
  Color_4.xyz = ((Color_4.xyz * (1.0 - ScreenColor.w)) + (ScreenColor.xyz * ScreenColor.w));
  highp vec3 tmpvar_13;
  tmpvar_13.x = FogColor.w;
  tmpvar_13.y = FogColor2.w;
  tmpvar_13.z = FogColor3.w;
  Color_4.xyz = (Color_4.xyz * tmpvar_13);
  Color_4.xyz = (Color_4.xyz / ((Color_4.xyz * 0.9661836) + 0.180676));
  Color_4.xyz = (Color_4.xyz * Color_4.w);
  SV_Target = Color_4;
}

