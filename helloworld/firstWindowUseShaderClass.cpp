#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// build and compile our shader program
	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions         // colors
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int VBO, VAO;
	//VAO保存了我们的顶点属性配置，我们要用哪个VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); //用一个唯一的id创建一个顶点缓冲对象
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	//将创建的缓冲绑定到GL_ARRAY_BUFFER对象上，这是一个buffer类型，glBindBUffer允许我们一次绑定到好几个不同种类的buffer上面
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	
	//将定义好的数据绑定到buffer的内存上，第一个参数是buffer类型，我们上面设为Array。
	//第二个参数是传递到buffer的数据的大小，单位是比特
	//第三个参数是要拷贝的实际数据
	//第四个参数告诉我们怎么显卡去处理这些数据。这些数据很少变化GL_STATIC_DRAW、可能变化许多GL_DYNAMIC_DRAW或每次渲染都会变化，
	//GL_STREAM_DRAW来决定是否放到显存
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//到此为止，已将将顶点数据存到显存中，通过顶点缓冲对象VBO来管理
	
	//第一个参数指定我们要配置顶点的哪个属性 这里设置location。因为我们在顶点着色器配置 了
	//layout (location = 0) in vec3 aPos;
	//第二个参数指定了顶点属性的大小（有几个值），顶点属性是vec3，所以是由3个值构成的
	//第三个参数指定了数据的类型是GL_FLOAT
	//第四个参数制定了数据是否要被归一化 如果不在-1到1或0到1之间，要被映射到这个范围内
	//第五个参数指定了连续的顶点属性集之间的步长 由于下一个数据position集合距离上一个是3个float的大小，
	//由于我们知道这些array tightly  packed，所以可以设置为0,让opengl来决定（这个仅仅在tightly packed的时候可以用）
	//第六个参数指定了buffer中位置数据起始的offset，由于位置数据是从开头开始的，所以设置为0
	//当调用这个参数的时候，每个顶点属性从VBO管理的内存中取数据，而从哪个VBO？当前绑定到GL_ARRAY_BuFFER的vbo上
	//将buffer绑定到属性上
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	//我们用这句代码将顶点属性 location 作为他的参数，默认顶点属性是disable的
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		ourShader.use();
		ourShader.setVec3("offset", 0.5, 0, 0);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}