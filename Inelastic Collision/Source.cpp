#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Shader.h"
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define HEIGHT 600
#define WIDTH 600
#include"Circle.h"
#include<vector>

float xPos, yPos;
float paddleSpeed = 2.0f;
glm::vec2 centre(0.5f, 0.5f);
float radius = 0.5f;

//fps variables
double deltaTime = 0.0;
double lastFrame = 0.0;

double fps = 0.0;
double fpsTimer = 0.0;
double fpsUpdateInterval = 0.5; // half a second
int frames = 0;


bool wHeld;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void LoadTexture(unsigned int& texture, const char* fileName)
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height, nrChannels;

	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	Log(nrChannels);
	if (data)
	{
		switch (nrChannels)
		{
		case 3: // jpg format
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:// png format
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			Log("invalid image channels")
				break;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		//inc

		if (yPos <= 0.75)
			yPos += 0.1f * paddleSpeed;

		//radius += 0.1f;
	}


	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{

		//dec
		if (yPos >= -0.75f)
			yPos -= 0.1f * paddleSpeed;
		//radius -= 0.1f;

	}
}
void FPSCounter(GLFWwindow* window)
{
	// --- DELTA TIME ---
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// --- FPS COUNTER ---
	frames++;
	fpsTimer += deltaTime;

	if (fpsTimer >= fpsUpdateInterval)   // update every 0.5s
	{
		fps = frames / fpsTimer;         // true FPS
		frames = 0;
		fpsTimer = 0.0;

		std::string title =
			"CG_Class | FPS: " + std::to_string((int)fps) +
			" | delta: " + std::to_string(deltaTime);

		glfwSetWindowTitle(window, title.c_str());
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

	//callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	//glfwSwapInterval(0);
	//glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

#pragma endregion

#pragma region Vertex Buffer

	float vertices[] =
	{
		//pos                //col			   //Tex Coords
		-0.5f, -0.5f, 0.0f,	 1.0f, 0.0f, 0.0f, 0.0f,0.0f, // point -1
		 0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f, 1.0f,0.0f, // point -2
		-0.5f,  0.5f, 0.0f,	 0.0f, 0.0f, 1.0f, 0.0f,1.0f, // point -3

		-0.5f,  0.5f, 0.0f,	 0.0f, 0.0f, 1.0f, 0.0f,1.0f, // point -1
		 0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f, 1.0f,0.0f, // point -2
		 0.5f,  0.5f, 0.0f,	 0.0f, 0.0f, 1.0f, 1.0f,1.0f  // point -3
	};
	//VBO - vertex buffer object
	//VAO - vertex array object
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Binding the buffer - selecting current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//assign vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#pragma endregion

#pragma region Shaders

	//linking attributes
	//1st - para - layout location(in vertex shader)
	//2nd - para - size of the component;
	//5th - para - total size of the vertex/point
	//6th - para - offset within the vertex size
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader defaultShader("Resources/Shaders/default.vert",
		"Resources/Shaders/default.frag");
	Shader circleShader("Resources/Shaders/default.vert",
		"Resources/Shaders/circle.frag");

	defaultShader.use();
	circleShader.use();

	unsigned int texture_0, texture_1;
	LoadTexture(texture_0, "Resources/Textures/water.png");
	//LoadTexture(texture_1, "Resources/Textures/cat_close.png");
	circleShader.SetInt("texSampler_0", 0);
	//defaultShader.SetInt("texSampler_1", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_0);

	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_1);*/
#pragma endregion


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	std::cout << "starting game loop -" << std::endl;
	
	std::vector<Circle> circles;

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			glm::vec3 vel = glm::vec3((i % 2 == 0 ? -0.01f : 0.01f),
									  (i % 2 == 0 ? 0.01f : -0.01f), 0.0f);
			vel *=0.0f;
			circles.push_back(Circle(glm::vec3(-0.5f + 0.27f * i, 0.5f + (-1.0f) * j, 0.0f), vel));
		}
		
	}
	
#pragma region RenderLoop
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic
		defaultShader.use();
		defaultShader.SetFloat("time", glfwGetTime());

		FPSCounter(window);
		//draw circle
		
		//circle-circle collision

		for (int i = 0; i < circles.size() - 1; i++)
		{
			for (int j = i+1; j < circles.size(); j++)
			{
				auto& circle1 = circles[i];
				auto& circle2 = circles[j];
				float distance = glm::distance(circle1.pos, circle2.pos);

				if (distance < (circle1.radius * circle1.scale.x) + (circle2.radius * circle2.scale.x))
				{
					//Log(circle1.vel.x);
					circle1.vel = -circle1.vel;
					//Log(circle1.vel.x);
					circle2.vel = -circle2.vel;
				}
			}
			
		}
		

		glm::mat4 circleMatrix = glm::mat4(1.0f);

		for (auto &i : circles)
		{
			i.Move(deltaTime);
			//circle-window collision
			i.CheckBounds();

			circleShader.use();
			circleMatrix = glm::mat4(1.0f);

			circleMatrix = glm::translate(circleMatrix, i.pos);
			//circleMatrix = glm::scale(circleMatrix, i.scale);
			circleShader.SetMat4("translate", circleMatrix);

			circleShader.SetFloat("centreX", centre.x);
			circleShader.SetFloat("centreY", centre.y);
			circleShader.SetFloat("radius", i.radius);
			circleShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
			circleShader.SetFloat("time", glfwGetTime());
			circleShader.SetVec3("dir", i.vel);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}


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