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

	/*Grass grass("..//model_loading//plane.obj", "..//model_loading//shaders//1.model_loading.vs",
		"..//model_loading//shaders//1.model_loading.fs");*/
	Grass grass("..//model_loading//grass//ms224_5.fbx", "..//model_loading//shaders//1.model_loading.vs",
		"..//model_loading//shaders//1.model_loading.fs");
	/*Grass grass("..//model_loading//planeTixingNotFlip.obj", "..//advanced_opengl//shaders//1.1.depth_testing.vs",
		"..//advanced_opengl//shaders//1.1.coordinateTesting.fs"); */
	/*Grass grass("..//model_loading//planeTixingNotFlip.obj", "..//advanced_opengl//shaders//1.1.depth_testing.vs",
		"..//advanced_opengl//shaders//1.1.coordinateTesting.fs");*/
	/*Grass grass("..//model_loading//planeFlipYz.obj", "..//advanced_opengl//shaders//1.1.depth_testing.vs",
		"..//advanced_opengl//shaders//1.1.depth_testing.fs");*/
	// build and compile our shader program
	//Shader shader("..//model_loading//shaders//1.model_loading.vs",
	//	"..//model_loading//shaders//1.model_loading.fs");

	////Model ourModel("..//model_loading//nanosuit//nanosuit.obj");
	////Model ourModel("..//model_loading//grass//ms224_5.fbx");
	////Model ourModel("..//terrain//mesh//planeYup.fbx");
	//Model ourModel("..//model_loading//plane.obj");

	glPolygonMode(GL_FRONT_AND_BACK, GL_COLOR);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		grass.Draw();
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