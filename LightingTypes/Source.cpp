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
#define LogVec3(name,val) std::cout<<name <<" : "<<val.x<<" , "<< val.y << " , "<<val.z << std::endl;
const float Deg2Rad = 3.14159265f / 180.0f;

glm::vec3 lightPositions[6];
float paddleSpeed = 2.0f;
glm::vec2 centre(0.5f, 0.5f);
float radius = 0.5f;
int selectedLight = 1;
float speed = 0.0f;
double worldX, worldY;
double prevX, prevY;
bool wHeld;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void CameraInput(GLFWwindow* window, Camera& camera);
void LightInput(GLFWwindow* window);

void LoadTexture(unsigned int& texture, const char* fileName)
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // to prevent image with wdith!=height from getting corrupted
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	Log("loaded texture : "<<fileName<<" , channels : "<<nrChannels);
	if (data)
	{
		switch (nrChannels)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			break;
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
	worldX = xpos;
	worldY = ypos;

	/*Log(worldX);
	Log(worldY);*/
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_RELEASE))
	//{
	//	//inc

	//	zPos -= 0.1f * paddleSpeed;

	//	//radius += 0.1f;
	//}


	//else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	//{

	//	//dec
	//	zPos += 0.1f * paddleSpeed;
	//	//radius -= 0.1f;

	//}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		speed = speed == 0.5f ? 0.0f : 0.5f;

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
		//pos                //normal		   //Tex Coords
		 0.5f,  0.0f, 0.5f,	 0.0f, 1.0f,0.0f, 1.0f,1.0f,  // point -0
		 0.5f,  0.0f,-0.5f,	 0.0f, 1.0f,0.0f, 1.0f,0.0f,  // point -1
		-0.5f,  0.0f,-0.5f,	 0.0f, 1.0f,0.0f, 0.0f,0.0f,  // point -2
		-0.5f,  0.0f, 0.5f,	 0.0f, 1.0f,0.0f, 0.0f,1.0f,  // point -3
	};

	int indices[] = {
		0,1,3,
		1,2,3
	};

//quad with TBN matrix
	// positions
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


	float quadVertices[] = {
		// positions            // normal         // texcoords  // tangent                          // bitangent
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
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

	//cube - pos,normal,uv
	float cubeVertices3[] = {
		// ===== FRONT FACE (normal: 0, 0, 1) =====
	//    position              normal           uv
	-0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

	// ===== BACK FACE (normal: 0, 0, -1) =====
	-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 1.0f,

	// ===== LEFT FACE (normal: -1, 0, 0) =====
	-0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

	// ===== RIGHT FACE (normal: 1, 0, 0) =====
	 0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,

	 // ===== TOP FACE (normal: 0, 1, 0) =====
	 -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,

	 // ===== BOTTOM FACE (normal: 0, -1, 0) =====
	 -0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,   0.0f, 1.0f
	};


	//VBO - vertex buffer object
	//EBO - element buffer object
	//VAO - vertex array object
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Binding the buffer - selecting current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//assign vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

	Shader defaultShader("Resources/Shaders/default.vert",
		"Resources/Shaders/default.frag");
	/*Shader circleShader("Resources/Shaders/default.vert",
		"Resources/Shaders/circle.frag");*/

		//circleShader.use();

	unsigned int texture_0, texture_1, texture_2;
	//minecraft path
	/*LoadTexture(texture_0, "Resources/Textures/minecraft blocks/bricks.png");
	LoadTexture(texture_1, "Resources/Textures/minecraft blocks/bricks_s.png");
	LoadTexture(texture_2, "Resources/Textures/minecraft blocks/bricks_n.png");*/

	LoadTexture(texture_0, "Resources/Textures/metal_diff.jpg");
	LoadTexture(texture_1, "Resources/Textures/metal_spec.jpg");
	LoadTexture(texture_2, "Resources/Textures/metal_normal.jpg");

	defaultShader.use();
	//circleShader.SetInt("texSampler_0", 0);
	defaultShader.SetInt("texSampler_0", 0);
	defaultShader.SetInt("texSampler_1", 1);
	defaultShader.SetInt("texSampler_2", 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_2);


	//lighting params
	unsigned int lightVAO, lightVBO, lightEBO;

	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);
	glGenBuffers(1, &lightEBO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices3), cubeVertices3, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices2), cubeIndices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	/*Shader lineShader("Resources/Shaders/line.vert",
		"Resources/Shaders/light.frag");

	lineShader.use();*/

	Shader lightShader("Resources/Shaders/light.vert",
		"Resources/Shaders/light.frag");

#pragma endregion

#pragma region Shaders



#pragma endregion

	//enable blending
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	//enable depth 
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
	//glPolygonMode(GL_BACK, GL_LINE);

	std::cout << "Starting Render Loop " << std::endl;

	Camera camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 2;j++)
		{
			int k = (i*2) +j;
			//Log(k);
			//lightPositions[k].x = i * 1.1f;
			lightPositions[k].y = 0.3f;
			//lightPositions[k].z = j * 1.1f;
		}
		
	}
	glm::vec3 lightColors[6] = {
		glm::vec3(1.0f),
		glm::vec3(0.157f, 0.455f, 0.929f),
		 glm::vec3(0.98f, 0.969f, 0.384f),
		 glm::vec3(0.941f, 0.286f, 0.098f),
		 glm::vec3(0.098f, 0.941f, 0.157f),
		 glm::vec3(0.788f, 0.149f, 0.922f)
	};
	SetProjectAssetPath("../Common/Resources/");
	Grid::InitGrid(50);


#pragma region RenderLoop
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//logic

		FPSCounter(window);

		CameraInput(window, camera);
		LightInput(window);

		//LogVec3("camPos", camera.Position);
		//view matrix
		glm::mat4 view = glm::mat4(1.0f);
		

		view = camera.GetViewMatrix();

		//view = glm::rotate(view,(float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//LogVec3("lightPos", lightPos);

		//projection matrix
		glm::mat4 proj = glm::mat4(1.0f);
		//fov,aspect ratio (W/H),near plane , far plane
		proj = glm::perspective(camera.Zoom, (float)WIDTH / HEIGHT, 0.1f, 100.0f);

		Grid::DrawGrid(view,proj);


		glm::mat4 model = glm::mat4(1.0f);

		///draw Cube
		defaultShader.use();

		defaultShader.SetFloat("time", glfwGetTime());
		defaultShader.SetFloat("speed", speed);
		defaultShader.SetInt("material.specularStrength", 64);
		//light params
		for (int i = 0;i < 6;i++)
		{
			defaultShader.SetVec3("lights[" + std::to_string(i) + "].pos", lightPositions[i]);
			defaultShader.SetVec3("lights[" + std::to_string(i) + "].direction", glm::vec3(0.0f, 1.0f, 1.0f));
			defaultShader.SetVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);
			defaultShader.SetFloat("lights[" + std::to_string(i) + "].constant", 1.0f);
			defaultShader.SetFloat("lights[" + std::to_string(i) + "].linear", 0.7f);
			defaultShader.SetFloat("lights[" + std::to_string(i) + "].quadratic", 1.8f);
		}
		defaultShader.SetVec3("camPos", camera.Position);
		defaultShader.SetFloat("material.ambient", 0.2f);
		defaultShader.SetMat4("view", view);
		defaultShader.SetMat4("proj", proj);
		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 2;j++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i * 1.1f, 0.1f, j * 1.1f));
				//model = glm::rotate(model, -90.0f*Deg2Rad,glm::vec3(1.0f,0.0f,0.0f));
				model = glm::scale(model, glm::vec3(0.5f));
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				defaultShader.SetMat4("model", model);

				//defaultShader.SetVec3("objectColor", glm::vec3(0.5f, 1.0f, 0.0f));
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			
		}

		//lighting placeholder - cube
		lightShader.use();
		lightShader.SetMat4("view", view);
		lightShader.SetMat4("proj", proj);
		for (int i = 0;i < 6;i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.1f));
			lightShader.SetMat4("model", model);
			lightShader.SetVec3("objectColor", lightColors[i]);
			glBindVertexArray(lightVAO);
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
	/*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		Log("rightmouse press");
	}*/
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
void LightInput(GLFWwindow* window)
{
	
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		selectedLight = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		selectedLight = 2;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		selectedLight = 3;
	}
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		selectedLight = 4;
	}
	else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		selectedLight = 5;
	}
	else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		selectedLight = 6;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		lightPositions[selectedLight - 1].z += paddleSpeed * deltaTime;
		
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		lightPositions[selectedLight - 1].z -= paddleSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		lightPositions[selectedLight - 1].x += paddleSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		lightPositions[selectedLight - 1].x -= paddleSpeed * deltaTime;
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}