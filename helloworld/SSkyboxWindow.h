#pragma once
#ifndef SSKYBOXWINDOWH
#define SSKYBOXWINDOWH
#include "sWidget.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Camera;
class SSkyboxWindow :public SWidget
{
public:
	SSkyboxWindow(const unsigned int mW, const unsigned int mH, const char *title);
	~SSkyboxWindow();
	virtual void OnMouseMove(double x, double y);
	virtual void OnMousePress(MouseType type);
	virtual void OnMouseRelease(MouseType type);
	virtual void processInput(float deltaTime);
	virtual void ProcessMouseScroll(float yoffset);
	virtual bool IsValid();
	virtual void Exec();
protected:
	double mMouseX;
	double mMouseY;
	bool mIsFirstMove;
	bool mIsMousePressed[4];
	Camera * mCamera;
	GLFWwindow * mWindow;

	bool mIsClosed;
};
#endif // !SSKYBOXWINDOWH

