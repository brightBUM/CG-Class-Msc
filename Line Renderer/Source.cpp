#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include<vector>

#define HEIGHT 600
#define WIDTH 800
#define Log(x) std::cout<<x<<std::endl;

//global variables
float worldX, worldY;
int Width = 800;
int Height = 600;
std::vector<float> points;
bool clicked;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		clicked = true;
		points.push_back(worldX);
		points.push_back(worldY);
		points.push_back(0.0f);

		Log(worldX);
		Log(worldY);

	}
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Log("cursor callback");
	//screen space to ndc
	worldX = xpos / (double)Width * 2.0f - 1.0f;
	worldY = 1.0f - ypos / (double)Height * 2.0f;

	if (clicked)
	{
		
		clicked = false;
	}
	/*Log(worldX);
	Log(worldY);*/
}

int main()
{
	//create a window
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(Width, Height, "CG_Class", NULL, NULL);

	//setting the context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//mouse input
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);


	//glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "starting game loop - basis setup " << std::endl;
	
	
	//1st vertex
	for (int i = 0; i < 3; i++)
	{
		points.push_back(0.0f);
	}
	//2nd vertex
	points.push_back(0.0f);
	points.push_back(-0.5f);
	points.push_back(0.0f);

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic
		glPointSize(8.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < points.size(); i += 3)
		{
			glVertex3f(points[i], points[i + 1], points[i + 2]);
		}
		glEnd();

		glLineWidth(5.0f);
		glBegin(GL_LINE_STRIP);
		
		for (int i = 0; i < points.size(); i+=3)
		{
			glVertex3f(points[i], points[i + 1], points[i + 2]);
		}

		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	std::cout << "game loop Terminated" << std::endl;

	glfwTerminate();

	//std::cout << "Hola " << std::endl;
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Width = width;
	Height = height;
	glViewport(0, 0, width, height);

}
