#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"camera.h"
#include"common.h"
#include"Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#define HEIGHT 1200
#define WIDTH 1200

double worldX, worldY;
double prevX, prevY;
float xPos, zPos = 0.0f;
float paddleSpeed = 2.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	worldX = xpos;
	worldY = ypos;
}
void CameraInput(GLFWwindow* window, Camera& camera)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		//Log("rightmouse press");
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}


	double mouseDeltaX = worldX - prevX;
	double mouseDeltaY = prevY - worldY;
	prevX = worldX;
	prevY = worldY;

	camera.ProcessMouseMovement(mouseDeltaX, mouseDeltaY);

	//mouse scroll
}
void LightInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		zPos += paddleSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		zPos -= paddleSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		xPos += paddleSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		xPos -= paddleSpeed * deltaTime;
	}
}
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
	glfwSetCursorPosCallback(window, cursor_position_callback);


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
	glEnable(GL_DEPTH_TEST);


	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);


		CameraInput(window, camera);
		LightInput(window);
		FPSCounter(window);
		//logic
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(camera.Zoom, (float)WIDTH / HEIGHT, 0.1f, 100.0f);


		glm::vec3 lightPos = glm::vec3(xPos, 0.0f, zPos);

		defaultShader.use();
		defaultShader.SetFloat("time", (float)glfwGetTime());
		defaultShader.SetMat4("model", model);
		defaultShader.SetMat4("view", view);
		defaultShader.SetMat4("proj", proj);
		defaultShader.SetFloat("material.ambient", 0.2f);
		defaultShader.SetFloat("material.shininess", 128.0f);
		defaultShader.SetVec3("lightPos",lightPos);
		defaultShader.SetVec3("lightColor", glm::vec3(1.0f));
		defaultShader.SetVec3("camPos", camera.Position);

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