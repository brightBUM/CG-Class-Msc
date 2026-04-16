#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include<math.h>
#define HEIGHT 600
#define WIDTH 800
#define Log(x) std::cout<<x<<std::endl;

const float Rad2Deg = 57.2958f;
const float Deg2Rad = 3.14159265f / 180.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void BasicTriangleFan()
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f); // 1 - origin
	glVertex3f(-0.5f, 0.5f, 0.0f); // 2
	glVertex3f(0.0f, -0.5f, 0.0f);  // 3
	glVertex3f(0.5f, -0.5f, 0.0f);  // 4

	glEnd();
}

int main()
{
	//create a window
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "CG_Class", NULL, NULL);

	//setting the context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "starting game loop - circle renderer " << std::endl;
	glPolygonMode(GL_FRONT, GL_LINE);

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic
		float radius = 0.5f;
		glBegin(GL_TRIANGLE_FAN);
		
		glColor3f(1.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); // 1- origin
		
		for (int i = 0; i <= 360; i += 10)
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(radius * cos(i * Deg2Rad), 
					   radius * sin(i * Deg2Rad), 
					   0.0f); // 2
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
	glViewport(0, 0, width, height);
}
