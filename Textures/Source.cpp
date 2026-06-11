#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#define HEIGHT 800
#define WIDTH 600
#define Log(x) std::cout<<x<<std::endl;

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

	defaultShader.use();

	unsigned int texture_0,texture_1;
	LoadTexture(texture_0, "Resources/Textures/cat_open.png");
	LoadTexture(texture_1, "Resources/Textures/cat_close.png");
	defaultShader.SetInt("texSampler_0", 0);
	defaultShader.SetInt("texSampler_1", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_1);
#pragma endregion

#pragma region RenderLoop

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	std::cout << "starting game loop -" << std::endl;
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic
		defaultShader.use();
		defaultShader.SetFloat("time", glfwGetTime());


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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