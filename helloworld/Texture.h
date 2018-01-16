#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include "../helloworld/stb_image.h"
#include <glad/glad.h>

#include <string>
//#include <fstream>
//#include <sstream>
#include <iostream>
//
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

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
	  unsigned int m_ID;
	  

private:
	unsigned int m_width;
	unsigned int m_height;
	GLenum m_format;

};

#endif