/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#define GLEW_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "game.h"
#include "resource_manager.h"


// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLuint SCREEN_WIDTH = 800;
// The height of the screen
const GLuint SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif
	
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	/*glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
/*
	glewExperimental = GL_TRUE;
	glewInit();*/
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.


	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize game
	Breakout.Init();

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Start Game within Menu State
	Breakout.State = GAME_ACTIVE;

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//deltaTime = 0.001f;
		// Manage user input
		Breakout.ProcessInput(deltaTime);

		// Update Game state
		Breakout.Update(deltaTime);

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.Render();

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Breakout.Keys[key] = GL_FALSE;
	}
}