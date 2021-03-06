#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../helloworld/shader.h"
#include "../helloworld/Texture.h"
#include "../helloworld/myCamera.h"
#include "../helloworld/model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>


// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

//time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

const unsigned int NUM_TEXS = 4;
const unsigned int NUM_RAIN_FRAMES = 10;
bool enableRain = true;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "t4m", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
	glDepthFunc(GL_LESS);

	//build and compile our shader program
	Shader shader("..//terrain//shaders//t4m.vs",
		"..//terrain//shaders//t4m.fs");
	shader.use();
	
	for (int i = 0; i < NUM_TEXS; i++)
	{
		stringstream ss;
		ss << i;
		string num = ss.str();
		shader.setInt("diffuses[" + num + "].sample", i);
		shader.setInt("normals["+ num + "].sample", i + 4);
	}
	
	shader.setInt("control.sample", 8);
	shader.setInt("reflection.sample", 9);
	shader.setInt("rain.sample", 10);
	shader.setInt("skybox", 11);

	//Model ourModel("..//terrain//mesh//teapotYup.FBX");
	Model ourModel("..//terrain//mesh//planeYup.FBX");
	Mesh ourMesh = ourModel.meshes[0];
	vector<Vertex> ourVertex = ourMesh.vertices;
	Texture2D diffuse1 = Texture2D("..//terrain//textures//db0011.tga", Repeat);
	Texture2D diffuse2 = Texture2D("..//terrain//textures//db0012.tga", Repeat);
	Texture2D diffuse3 = Texture2D("..//terrain//textures//db0009.tga", Repeat);
	Texture2D diffuse4 = Texture2D("..//terrain//textures//db0015.tga", Repeat);
	
	Texture2D normal1 = Texture2D("..//terrain//textures//db0011_Bump.tga", Repeat);
	Texture2D normal2 = Texture2D("..//terrain//textures//db0012_Bump.tga", Repeat);
	Texture2D normal3 = Texture2D("..//terrain//textures//db0009_Bump.tga", Repeat);
	Texture2D normal4 = Texture2D("..//terrain//textures//db0015_Bump.tga", Repeat);

	Texture2D control = Texture2D("..//terrain//textures//control.png", Clamp);
	Texture2D reflection = Texture2D("..//terrain//textures//reflection.png", Repeat);

	Texture2D rainFrames[NUM_RAIN_FRAMES];
	for (unsigned int i = 0; i < NUM_RAIN_FRAMES; i++)
	{
		stringstream ss;
		ss << i+1;
		string num = "..//terrain//textures//rain" + ss.str() + ".png";
		const char* texturePath = const_cast<char*>(num.c_str());
		rainFrames[i] = Texture2D(texturePath, Repeat);
	}

	unsigned int m_vao = ourMesh.VAO;
	vector<unsigned int> m_indice = ourMesh.indices;

	Shader skyboxShader("..//advanced_opengl//shaders//6.1.skybox.vs",
		"..//advanced_opengl//shaders//6.1.skybox.fs");
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVao, skyboxVbo;
	glGenVertexArrays(1, &skyboxVao);
	glGenBuffers(1, &skyboxVbo);
	glBindVertexArray(skyboxVao);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		
	std::vector<std::string> faces
	{
		"..//advanced_opengl//skybox//right.jpg",
		"..//advanced_opengl//skybox//left.jpg",
		"..//advanced_opengl//skybox//top.jpg",
		"..//advanced_opengl//skybox//bottom.jpg",
		"..//advanced_opengl//skybox//back.jpg",
		"..//advanced_opengl//skybox//front.jpg"
	};
	Cubemap skybox(faces);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_COLOR);
	unsigned int frameCount = 0;
	unsigned int curFrame = 0;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetCameraMatrix();
		//view = glm::rotate(view, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

		glm::mat4 model = glm::mat4();
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

		shader.setMat4("model", model);

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		shader.setVec4("diffuses[0].tex_st", glm::vec4(10.0f, 10.0f, 0.0f, 0.0f));
		shader.setVec4("diffuses[1].tex_st", glm::vec4(10.0f, 10.0f, 0.0f, 0.0f));
		shader.setVec4("diffuses[2].tex_st", glm::vec4(10.0f, 10.0f, 0.0f, 0.0f));
		shader.setVec4("diffuses[3].tex_st", glm::vec4(10.0f, 10.0f, 0.0f, 0.0f));
		shader.setVec4("normals[0].tex_st", glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
		shader.setVec4("normals[1].tex_st", glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
		shader.setVec4("normals[2].tex_st", glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
		shader.setVec4("normals[3].tex_st", glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
		shader.setVec4("control.tex_st", glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
		shader.setVec4("reflection.tex_st", glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
		shader.setVec4("rain.tex_st", glm::vec4(40.0f, 40.0f, 0.0f, 0.0f));

		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightDir", glm::vec3(0.2f, 0.8f, 1.0f));
		shader.setFloat("ambientScale", 0.2f);
		shader.setFloat("specularScale", 1.5f);
		shader.setVec3("lightCol", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setFloat("shininess", 64.0f);

		//draw mesh
		glBindVertexArray(m_vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse1.m_ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, diffuse2.m_ID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, diffuse3.m_ID);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, diffuse4.m_ID);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, normal1.m_ID);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, normal2.m_ID);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, normal3.m_ID);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, normal4.m_ID);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, control.m_ID);
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, reflection.m_ID);
		if (curFrame < NUM_RAIN_FRAMES)
		{
			frameCount++;
			if (frameCount % 4 == 0)
				curFrame++;
		}
		else
		{
			frameCount = 0;
			curFrame = 0;
		}
		//shader.setInt("rainTex", rainFrames[frameCount].m_ID);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, rainFrames[frameCount].m_ID);
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.m_ID);
		
		if (enableRain)
		{
			shader.setVec3("wetCol", glm::vec3(1.0f, 0.6f, 0.1f));
			shader.setFloat("reflectionFactor", 1.0f);
			shader.setFloat("rainFactor", 1.0);
		}
		else
		{
			shader.setVec3("wetCol", glm::vec3(1.0f, 1.0f, 1.0f));
			shader.setFloat("reflectionFactor", 0.0f);
			shader.setFloat("rainFactor", 0.0);
		}
		glDrawElements(GL_TRIANGLES, m_indice.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		skyboxShader.setMat4("projection", projection);
		view = glm::mat4(glm::mat3(camera.GetCameraMatrix()));
		skyboxShader.setMat4("view", view);

		// skybox cube
		glBindVertexArray(skyboxVao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.m_ID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
		//always good practice to set everything back to defaults
		//glActiveTexture(GL_TEXTURE0);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessInput(CameraDirection::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessInput(CameraDirection::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessInput(CameraDirection::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessInput(CameraDirection::RIGHT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		if (enableRain)
		{
			enableRain = false;
		}
		else
		{
			enableRain = true;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseInput(xoffset, yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



