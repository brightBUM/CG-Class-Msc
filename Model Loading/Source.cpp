#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"camera.h"
#include"common.h"
#include"Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#define HEIGHT 800
#define WIDTH 600

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

	Model bagModel("Resources/Models/backpack/backpack.obj");
	Shader defaultShader("Resources/Shaders/default.vert",
		"Resources/Shaders/default.frag");

	defaultShader.use();

#pragma endregion

#pragma region RenderLoop
	
	Camera camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(camera.Zoom, (float)WIDTH / HEIGHT, 0.1f, 100.0f);

		defaultShader.use();
		defaultShader.SetFloat("time", (float)glfwGetTime());
		defaultShader.SetMat4("model", model);
		defaultShader.SetMat4("view", view);
		defaultShader.SetMat4("proj", proj);


		bagModel.Draw(defaultShader);

		//Log(glfwGetTime());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	std::cout << "game loop Terminated" << std::endl;

	glfwTerminate();

	return 0;
#pragma endregion

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}