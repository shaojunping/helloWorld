#pragma once
#ifndef GRASSH
#define GRASSH
#include "Object.h"

class Grass:public Object
{
public:
	Grass(string const & modelPath, const GLchar *vertexShader, const GLchar *fragmentShader);
	~Grass();
	void SetShaderTime(const char *name, float time)
	{
		mShader->setFloat(name, time);
	}
	
protected:
	/*Shader *mShader;
	Model *mModel;
	glm::mat4 model;*/
};
#endif // !GRASSH

