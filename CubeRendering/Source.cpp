#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Shader.h"
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define HEIGHT 1200
#define WIDTH 1200
#include<vector>
#include"Camera.h"
#include"common.h"

float xPos, zPos = 2.0f;
float paddleSpeed = 2.0f;
glm::vec2 centre(0.5f, 0.5f);
float radius = 0.5f;


double worldX, worldY;
double prevX, prevY;
bool wHeld;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void CameraInput(GLFWwindow* window, Camera& camera);
void LoadTexture(unsigned int& texture, const char* fileName)
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // to prevent image with wdith!=height from getting corrupted
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
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Log("cursor callback");
	//screen space to ndc
	worldX = xpos ;
	worldY = ypos ;

	/*Log(worldX);
	Log(worldY);*/
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_RELEASE))
	{
		//inc

		zPos -= 0.1f * paddleSpeed;

		//radius += 0.1f;
	}


	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{

		//dec
		zPos += 0.1f * paddleSpeed;
		//radius -= 0.1f;

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
	glfwSetCursorPosCallback(window, cursor_position_callback);
	
	WindowUtils::DockConsoleAndWindow(window, WIDTH, HEIGHT);

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
		 0.5f,  0.5f, 0.0f,	 0.0f, 0.0f, 1.0f, 1.0f,1.0f,  // point -0
		 0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f, 1.0f,0.0f,  // point -1
		-0.5f, -0.5f, 0.0f,	 1.0f, 0.0f, 0.0f, 0.0f,0.0f,  // point -2
		-0.5f,  0.5f, 0.0f,	 0.0f, 0.0f, 1.0f, 0.0f,1.0f,  // point -3
	};

	int indices[] = {
		0,1,3,
		1,2,3
	};

	//cube - pos , col
	float cubeVertices[] = {
		// position           // color
		-0.5f,-0.5f,-0.5f,    1.0f,0.0f,0.0f,   // 0
		 0.5f,-0.5f,-0.5f,    0.0f,1.0f,0.0f,   // 1
		 0.5f, 0.5f,-0.5f,    0.0f,0.0f,1.0f,   // 2
		-0.5f, 0.5f,-0.5f,    1.0f,1.0f,0.0f,   // 3

		-0.5f,-0.5f, 0.5f,    1.0f,0.0f,1.0f,   // 4
		 0.5f,-0.5f, 0.5f,    0.0f,1.0f,1.0f,   // 5
		 0.5f, 0.5f, 0.5f,    1.0f,1.0f,1.0f,   // 6
		-0.5f, 0.5f, 0.5f,    0.3f,0.3f,0.3f    // 7
	};
	unsigned int cubeIndices[] = {
		// Front face
		0, 1, 2,
		2, 3, 0,

		// Right face
		1, 5, 6,
		6, 2, 1,

		// Back face
		5, 4, 7,
		7, 6, 5,

		// Left face
		4, 0, 3,
		3, 7, 4,

		// Top face
		3, 2, 6,
		6, 7, 3,

		// Bottom face
		4, 5, 1,
		1, 0, 4
	};


	//cube - pos,uv
	float cubeVertices2[] = {
		// ===== FRONT FACE =====
		//    position           uv
		-0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,     0.0f, 1.0f,

		// ===== BACK FACE =====
		-0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,     1.0f, 1.0f,

		// ===== LEFT FACE =====
		-0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,     0.0f, 1.0f,

		// ===== RIGHT FACE =====
		 0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,     0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,     1.0f, 1.0f,

		 // ===== TOP FACE =====
		 -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,     1.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,     0.0f, 0.0f,

		 // ===== BOTTOM FACE =====
		 -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,     0.0f, 1.0f
	};

	unsigned int cubeIndices2[] = {
		// FRONT
		0, 1, 2,  2, 3, 0,

		// BACK
		4, 5, 6,  6, 7, 4,

		// LEFT
		8, 9, 10, 10, 11, 8,

		// RIGHT
		12, 13, 14, 14, 15, 12,

		// TOP
		16, 17, 18, 18, 19, 16,

		// BOTTOM
		20, 21, 22, 22, 23, 20
	};

	//VBO - vertex buffer object
	//EBO - element buffer object
	//VAO - vertex array object
	unsigned int VBO, VAO,EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Binding the buffer - selecting current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//assign vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices2), cubeVertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices2), cubeIndices2, GL_STATIC_DRAW);

#pragma endregion

#pragma region Shaders

	//linking attributes
	//1st - para - layout location(in vertex shader)
	//2nd - para - size of the component;
	//5th - para - total size of the vertex/point
	//6th - para - offset within the vertex size
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	Shader defaultShader("Resources/Shaders/default.vert",
		"Resources/Shaders/default.frag");
	/*Shader circleShader("Resources/Shaders/default.vert",
		"Resources/Shaders/circle.frag");*/

	//circleShader.use();

	unsigned int texture_0, texture_1;
	LoadTexture(texture_0, "Resources/Textures/ball.png");
	//LoadTexture(texture_1, "Resources/Textures/cat_close.png");


	defaultShader.use();
	//circleShader.SetInt("texSampler_0", 0);
	defaultShader.SetInt("texSampler_0", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_0);

	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_1);*/
#pragma endregion

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable depth 
	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	std::cout << "starting game loop -" << std::endl;

	Camera camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

	SetProjectAssetPath("../Common/Resources/");
	//Debug::Init();
	Grid::InitGrid(50);
	
#pragma region RenderLoop
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic
		defaultShader.use();
		defaultShader.SetFloat("time", glfwGetTime());

		FPSCounter(window);
		Grid::DrawGrid();
		
		CameraInput(window, camera);


		//view matrix
		glm::mat4 view = glm::mat4(1.0f);

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 camPos = glm::vec3(0.0f, 0.0f, zPos);

		view = camera.GetViewMatrix();
		
		//view = glm::rotate(view,(float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

		//projection matrix
		glm::mat4 proj = glm::mat4(1.0f);
		//fov,aspect ratio (W/H),near plane , far plane
		proj = glm::perspective(camera.Zoom, (float)WIDTH / HEIGHT, 0.1f, 100.0f);

		glm::mat4 model = glm::mat4(1.0f);

		for (int i = 0; i < 5; i++)
		{
			///draw Cube
			defaultShader.use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.5f+(1.0f*i), 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5));
			//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
			defaultShader.SetMat4("model", model);
			defaultShader.SetMat4("view", view);
			defaultShader.SetMat4("proj", proj);

			//defaultShader.SetVec3("objectColor", glm::vec3(0.5f, 1.0f, 0.0f));
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
void CameraInput(GLFWwindow* window, Camera& camera)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		Log("rightmouse press");
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}