#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include "../helloworld/stb_image.h"
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <vector>

enum TextureType
{
	Repeat,
	Clamp
};

//enum Texture2DWrapMode
//{
//	Repeat,
//	MirroredRepeat,
//	ClampToEdge,
//	ClampToBorder
//	
//};
//enum Texture2DFilterMode
//{
//	Nearest,
//	Linear,
//	NearestMipmapNearest, //use the nearest mipmap to match the pixel size and nearest neighbor interplation
//	LinearMipmapNearest, //the last one represent mipmap mode, nearest mipmap sample using linear interpolation
//	NearMipmapLinear, // linear interpolate between two mipmap, sample via nearest neighbor
//	LinearMipmapLinear //linear interpolate between mipmaps, linear sample
//};

class Texture2D
{
public:
	  Texture2D(char const *path, TextureType type);
	  Texture2D();
	  unsigned int m_ID;
	  

private:
	unsigned int m_width;
	unsigned int m_height;
	GLenum m_format;

};

class Cubemap
{
public:
	Cubemap();
	Cubemap(std::vector<std::string> faces);
	unsigned int m_ID;
//private:

};
#endif