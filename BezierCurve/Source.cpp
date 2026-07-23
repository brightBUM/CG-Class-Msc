#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include<vector>
#include"glm/glm.hpp"
#include"Shader.h"
#define HEIGHT 600
#define WIDTH 800
#define Log(x) std::cout<<x<<std::endl;

//global variables
float worldX, worldY;
int Width = 800;
int Height = 600;

std::vector<glm::vec3> points;
std::vector<glm::vec3> lerpPoints;

bool clicked;
float t = 0.0f;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	//{
	//	points.push_back(glm::vec3(worldX,worldY,0.0f));

	//	/*Log(worldX);
	//	Log(worldY);*/
	//	Log("xPos : " << worldX << " , yPos : " << worldY);
	//	//Log("points : " << points.size()/2);
	//}
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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && (action == GLFW_PRESS ))
	{
		//inc

		t -= 0.1f ;
		Log(t);
		Log(lerpPoints[0].x << ", " << lerpPoints[0].y);

		//radius += 0.1f;
	}


	else if (key == GLFW_KEY_S && (action == GLFW_PRESS))
	{

		//dec
		t += 0.1f;
		Log(t);
		Log(lerpPoints[0].x << ", " << lerpPoints[0].y);

		//radius -= 0.1f;

	}
}
glm::vec3 Lerp(glm::vec3 A, glm::vec3 B, float t)
{
	return A + ((B - A) * t);
}

int main()
{
	//create a window
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(Width, Height, "CG_Class", NULL, NULL);
	//window version and profile
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//setting the context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//mouse input
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);

	//glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "starting game loop - bezier curve " << std::endl;

	//1st vertex
	points.push_back(glm::vec3(0.0f, -0.5f, 0.0f));
	//2nd vertex
	points.push_back(glm::vec3(0.2f, 0.5f,0.0f));


	unsigned int VBO, VAO,lerpVAO,lerpVBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Binding the buffer - selecting current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//assign vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	Shader defaultShader("Resources/Shaders/default.vert",
		"Resources/Shaders/default.frag");

	auto lerpPoint = Lerp(points[0], points[1], t);
	lerpPoints.push_back(lerpPoint);

	glGenBuffers(1, &lerpVBO);
	glGenVertexArrays(1, &lerpVAO);
	glBindVertexArray(lerpVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lerpVBO);
	glBufferData(GL_ARRAY_BUFFER, lerpPoints.size() * sizeof(glm::vec3), lerpPoints.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Log(lerpPoints[0].x<<", "<< lerpPoints[0].y);
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic
		lerpPoints[0] = Lerp(points[0], points[1], t);

		//lines
		defaultShader.use();
		defaultShader.SetFloat("time", (float)glfwGetTime());
		glLineWidth(5.0f);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER,
			points.size() * sizeof(glm::vec3),
			points.data(),
			GL_STATIC_DRAW);

		defaultShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
		glDrawArrays(GL_LINE_STRIP, 0, points.size());
		
		//points
		glPointSize(6.0f);
		defaultShader.SetVec3("objectColor", glm::vec3(0.0f));
		glDrawArrays(GL_POINTS, 0, points.size());

		//lerpPoints
		glPointSize(15.0f);
		glBindVertexArray(lerpVAO);

		defaultShader.SetVec3("objectColor", glm::vec3(1.0f));
		glDrawArrays(GL_POINTS, 0, lerpPoints.size());


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
