#include "Object.h"


Object::Object(string const & modelPath, const GLchar *vertexShader, const GLchar *fragmentShader)
{
	mShader = new Shader(vertexShader, fragmentShader);
	mModel = new Model(modelPath);
	model = glm::mat4();
}

void Object::SetMats(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	mShader->use();
	mShader->setMat4("model", model);
	mShader->setMat4("view", view);
	mShader->setMat4("projection", projection);
}

Object::~Object()
{
	if (mShader != NULL)
	{
		delete mShader;
	}
	if (mModel != NULL)
	{
		delete mModel;
	}
}

void Object::Draw()
{
	mModel->Draw(*mShader);
}