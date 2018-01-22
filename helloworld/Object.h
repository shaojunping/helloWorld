#pragma once
#ifndef OBJECTH
#define OBJECTH
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "model.h"

class Object
{
public:
	Object(string const & modelPath, const GLchar *vertexShader, const GLchar *fragmentShader);
	virtual ~Object();
	virtual void Draw();
	virtual void SetMats(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
protected:
	Shader *mShader;
	Model *mModel;
	glm::mat4 model;
};
#endif
