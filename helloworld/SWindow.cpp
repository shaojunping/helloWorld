#include "SWindow.h"
#include "../helloworld/myCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "grass.h"

#include <iostream>

SWindow *gWindow;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (NULL == gWindow) {
		return;
	}
	gWindow->OnMouseMove(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (NULL == window)
	{
		return;
	}
	MouseType btnType = MouseType::Unknown;
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		btnType = MouseType::Left;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		btnType = MouseType::Right;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		btnType = MouseType::Middle;
	}
	if (btnType != MouseType::Unknown)
	{
		if (action == GLFW_PRESS)
		{
			gWindow->OnMousePress(btnType);
		}
		else if (action == GLFW_RELEASE)
		{
			gWindow->OnMouseRelease(btnType);
		}
	}
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gWindow->ProcessMouseScroll(yoffset);
}

void SWindow::ProcessMouseScroll(float yoffset)
{
	mCamera->ProcessMouseScroll(yoffset);
}

SWindow::SWindow(const unsigned int mW, const unsigned int mH, const char *title):SWidget(mW, mH), mMouseX(0), mMouseY(0), mIsFirstMove(true), mIsClosed(false), mCamera(NULL)
{
	mWindow = glfwCreateWindow(mW, mH, title, NULL, NULL);
	if (mWindow == NULL)
	{
		return;
	}
	gWindow = this;
	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetScrollCallback(mWindow, scroll_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
	mCamera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
}

SWindow::~SWindow()
{
	if (mCamera != NULL)
	{
		delete mCamera;
	}
}

void SWindow::OnMouseMove(double xpos, double ypos)
{
	if (mIsFirstMove)
	{
		mMouseX = xpos;
		mMouseY = ypos;
		mIsFirstMove = false;
		return;
	}
	float xOffset = xpos - mMouseX;
	float yOffset = ypos - mMouseY;
	mMouseX = xpos;
	mMouseY = ypos;
	if (mIsMousePressed[Middle])
	{
		mCamera->Move(xOffset, yOffset);
	}
	if (mIsMousePressed[Right])
	{
		mCamera->ProcessMouseInput(xOffset, yOffset);
	}
}

void SWindow::OnMousePress(MouseType type)
{
	mIsMousePressed[type] = true;
}

void SWindow::OnMouseRelease(MouseType type)
{
	mIsMousePressed[type] = false;
}

bool SWindow::IsValid()
{
	return mWindow != NULL;
}

void SWindow::Exec()
{
	//time
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	glEnable(GL_DEPTH_TEST);

	Grass grass("..//model_loading//grass//ms224_5.fbx", "..//model_loading//shaders//1.model_loading.vs",
		"..//model_loading//shaders//1.model_loading.fs");

	glPolygonMode(GL_FRONT_AND_BACK, GL_COLOR);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glm::vec2 translations[20];
	srand(glfwGetTime());
	GLuint grassNum = 20;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[grassNum];
	for (GLuint i = 0; i < grassNum; i++)
	{
		glm::mat4 grassModel = glm::mat4();
		GLuint x = (rand() % 100) * 0.4;
		GLuint y = (rand() % 100) * 0.4;
		grassModel = glm::translate(grassModel, glm::vec3(x, 0, y));
		modelMatrices[i] = grassModel;
		//translations[i] = trans;
	}
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * grassNum, &modelMatrices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int grassVAO = grass.GetMode()->meshes[0].VAO;
	glBindVertexArray(grassVAO);
	/*glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));*/
	// set attribute pointers for matrix (4 times vec4)
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(mWindow))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(deltaTime);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(mCamera->Zoom), (float)mW / (float)mH, 0.1f, 100.0f);
		glm::mat4 view = mCamera->GetCameraMatrix();

		//render the loaded model
		glm::mat4 model = glm::mat4();
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		grass.SetMats(model, view, projection);
		grass.SetShaderTime("t", glfwGetTime());
		glBindVertexArray(grass.GetMode()->meshes[0].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, grass.GetMode()->meshes[0].indices.size(), GL_UNSIGNED_INT, 0, grassNum);
		glBindVertexArray(0);
		//grass.DrawInstanced(grassNum);
		/*shader.setMat4("model", model);
		shader.setFloat("t", glfwGetTime());
		ourModel.Draw(shader);*/

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

void SWindow::processInput(float deltaTime)
{
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow, true);

	if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
		mCamera->ProcessInput(CameraDirection::FORWARD, deltaTime);
	if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
		mCamera->ProcessInput(CameraDirection::BACKWARD, deltaTime);
	if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
		mCamera->ProcessInput(CameraDirection::LEFT, deltaTime);
	if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
		mCamera->ProcessInput(CameraDirection::RIGHT, deltaTime);
}