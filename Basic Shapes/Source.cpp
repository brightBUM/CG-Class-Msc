#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"

#define HEIGHT 600
#define WIDTH 800
#define Log(x) std::cout<<x<<std::endl;

void DrawTriangle()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_TRIANGLES);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	glEnd();
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void DrawPoints()
{
	glPointSize(10.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	glEnd();
}
void DrawLines()
{
	glLineWidth(10.0f);
	glColor3f(1.0f, 0.5f, 0.0f);
	//glBegin(GL_LINES);
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.0f, -0.5f, 0.0f);

	glEnd();
}
void DrawTriangleStrip()
{
	glBegin(GL_TRIANGLE_STRIP);

	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);   //-1

	glVertex3f(-0.5f, 0.5f, 0.0f);  //-2
	glVertex3f(-0.5f, -0.5f, 0.0f); //-3

	glColor3f(0.0f, 1.0f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.0f);  //-4

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

	std::cout << "game loop begins" << std::endl;


	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic

		/*DrawTriangle();

		DrawPoints();*/

		//DrawLines();

		DrawTriangleStrip();



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







