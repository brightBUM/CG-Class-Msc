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
		points.push_back(worldX);
		points.push_back(worldY);

		/*Log(worldX);
		Log(worldY);*/
		Log("xPos : " << worldX << " , yPos : " << worldY);
		//Log("points : " << points.size()/2);
	}
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Log("cursor callback");
	//screen space to ndc
	worldX = xpos / (double)Width * 2.0f - 1.0f;
	worldY = 1.0f - ypos / (double)Height * 2.0f;

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
	for (int i = 0; i < 2; i++)
	{
		points.push_back(0.0f);
	}
	//2nd vertex
	points.push_back(0.0f);
	points.push_back(-0.5f);

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic

		glLineWidth(5.0f);
		glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < points.size(); i+=2)
		{
			glVertex3f(points[i], points[i + 1], 0.0f);
		}
		glEnd();

		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 0.5f, 0.0f);
		for (int i = 0; i < points.size(); i += 2)
		{
			glVertex3f(points[i], points[i + 1], 0.0f);
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
