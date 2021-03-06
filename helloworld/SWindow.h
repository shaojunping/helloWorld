#pragma once
#ifndef SWINDOWH
#define SWINDOWH
#include "sWidget.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Camera;
class SWindow :public SWidget
{
public:
	SWindow(const unsigned int mW, const unsigned int mH, const char *title);
	~SWindow();
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
#endif // !SWINDOWH

