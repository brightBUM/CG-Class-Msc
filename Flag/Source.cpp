#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"

#define HEIGHT 600
#define WIDTH 800
#define Log(x) std::cout<<x<<std::endl;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void DrawQuad(float xpos, float ypos, float w, float h, float r, float g, float b);

const float Deg2Rad = 3.14159265f / 180.0f;
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

	std::cout << "starting game loop - basis setup " << std::endl;

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//logic
		
		glPolygonMode(GL_FRONT, GL_FILL);

		DrawQuad(-0.8f, 0.8f, 1.6f, 0.5f, 0.89f, 0.49f, 0.07f);
		DrawQuad(-0.8f, 0.3f, 1.6f, 0.5f, 1.0f, 1.0f, 1.0f);
		DrawQuad(-0.8f, -0.2f, 1.6f, 0.5f, 0.071f, 0.89f, 0.227f);
		

		glPolygonMode(GL_FRONT, GL_LINE);

		glLineWidth(5.0f);
		float radius = 0.25f;
		glBegin(GL_TRIANGLE_FAN);

		glColor3f(0.0f, 0.0f, 1.0f);

		float originY = 0.05f;
		glVertex3f(0.0f, originY, 0.0f); // 1- origin

		for (int i = 0; i <= 360; i += 15)
		{
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(radius * cos(i * Deg2Rad),
					   originY +radius * sin(i * Deg2Rad),
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
void DrawQuad(float xpos, float ypos,  float w, float h ,float r, float g , float b)
{
	glColor3f(r,g,b);

	glBegin(GL_QUADS);
	glVertex3f(xpos, ypos, 0.0f);
	glVertex3f(xpos, ypos - h, 0.0f);
	glVertex3f(xpos + w, ypos - h, 0.0f);
	glVertex3f(xpos + w, ypos, 0.0f);
	glEnd();

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
