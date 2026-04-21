#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Shader.h"

#define HEIGHT 600
#define WIDTH 800
#define Log(x) std::cout<<x<<std::endl;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
#pragma region WindowCreation

	//create a window
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "CG_Class", NULL, NULL);
	//window version and profile
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//setting the context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
#pragma endregion

#pragma region Vertex Buffer

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	//VBO - vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//Binding the buffer - selecting current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//assign vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#pragma endregion

#pragma region Shaders

	


#pragma endregion

#pragma region RenderLoop


	std::cout << "starting game loop -" << std::endl;
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic

		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	std::cout << "game loop Terminated" << std::endl;

	glfwTerminate();

	//std::cout << "Hola " << std::endl;
	return 0;
#pragma endregion

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
