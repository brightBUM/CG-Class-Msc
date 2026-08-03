#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"Common.h"
#define WIDTH 1920
#define HEIGHT 1080
#define Log(x) std::cout<<x<<std::endl;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void LoadTexture(unsigned int& texture, const char* fileName)
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height, nrChannels;

	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	Log("nr channels : "<<nrChannels);
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
		Log("Loaded texture : " << fileName);
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

	//WindowUtils::DockConsoleAndWindow(window, WIDTH, HEIGHT);

#pragma endregion

#pragma region Vertex Buffer

	float vertices[] =
	{
		//pos                //col			   //Tex Coords
		-1.0f, -1.0f, 0.0f,	 1.0f, 0.0f, 0.0f, 0.0f,0.0f, // point -1
		 1.0f, -1.0f, 0.0f,	 0.0f, 1.0f, 0.0f, 1.0f,0.0f, // point -2
		-1.0f,  1.0f, 0.0f,	 0.0f, 0.0f, 1.0f, 0.0f,1.0f, // point -3

		-1.0f,  1.0f, 0.0f,	 0.0f, 0.0f, 1.0f, 0.0f,1.0f, // point -1
		 1.0f, -1.0f, 0.0f,	 0.0f, 1.0f, 0.0f, 1.0f,0.0f, // point -2
		 1.0f,  1.0f, 0.0f,	 0.0f, 0.0f, 1.0f, 1.0f,1.0f  // point -3
	};
	//VBO - vertex buffer object
	//VAO - vertex array object
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#pragma endregion

#pragma region Shaders

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader defaultShader("Resources/Shaders/default.vert",
		"Resources/Shaders/default.frag");

	defaultShader.use();

	unsigned int textures[8];
	LoadTexture(textures[0], "Resources/Textures/background.png");
	LoadTexture(textures[1], "Resources/Textures/city4plan.png");
	LoadTexture(textures[2], "Resources/Textures/city3plan.png");
	LoadTexture(textures[3], "Resources/Textures/smog2.png");
	LoadTexture(textures[4], "Resources/Textures/city2plan.png");
	LoadTexture(textures[5], "Resources/Textures/smog1.png");
	//LoadTexture(textures[6], "Resources/Textures/light.png");
	LoadTexture(textures[6], "Resources/Textures/city1plan.png");
	defaultShader.SetInt("texSampler_0", 0);
	//defaultShader.SetInt("texSampler_1", 1);

	
	


#pragma endregion

#pragma region RenderLoop

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	float xInput = 0.0f;
	float scrollSpeed = 0.0f;

	std::cout << "starting game loop -" << std::endl;
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		//logic
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			xInput -= 0.001f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			xInput += 0.001f;
			
		}
		/*if ((glfwGetKey(window, GLFW_KEY_A) | glfwGetKey(window, GLFW_KEY_D)) == GLFW_RELEASE)
		{
			xInput = 0.0f;
		}*/

		defaultShader.use();
		defaultShader.SetFloat("time", glfwGetTime());
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		defaultShader.SetFloat("scrollSpeed", 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		for (int i = 1;i < 7;i++)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			scrollSpeed = i*0.5f;
			defaultShader.SetFloat("scrollSpeed", -xInput*scrollSpeed);
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