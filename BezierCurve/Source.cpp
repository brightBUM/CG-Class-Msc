#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include<vector>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"Common.h"
#define HEIGHT 1200
#define WIDTH 1200
#define Log(x) std::cout<<x<<std::endl;

//global variables
float worldX, worldY;

std::vector<glm::vec3> points;
std::vector<glm::vec3> lerpPoints;

bool clicked;
float t = 0.0f;
int segments = 20;


glm::vec3* selectedPoint = nullptr;
glm::vec3 Lerp(glm::vec3 A, glm::vec3 B, float t)
{
	return A + ((B - A) * t);
}

glm::vec3 BezierPoint(const std::vector<glm::vec3>& points, float t)
{
	if (points.size() == 1)
		return points[0];

	std::vector<glm::vec3> lerpPoints;

	for (size_t i = 0; i < points.size() - 1; i++)
	{
		lerpPoints.push_back(Lerp(points[i], points[i + 1], t));
	}

	return BezierPoint(lerpPoints, t);

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glm::vec3 newPoint(worldX, worldY, 0.0f);

		auto it = std::lower_bound(points.begin(),points.end(),newPoint,
			[](const glm::vec3& a, const glm::vec3& b)
			{
				return a.x < b.x;
			});

		points.insert(it, newPoint);

		lerpPoints.clear();

		for (int i = 0;i <= segments;i++)
		{
			t = i / static_cast<float>(segments);
			auto point = BezierPoint(points, t);

			lerpPoints.push_back(point);

		}
		
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		selectedPoint = nullptr;
	}
	
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Log("cursor callback");
	//screen space to ndc
	worldX = xpos / (double)WIDTH * 2.0f - 1.0f;
	worldY = 1.0f - ypos / (double)HEIGHT * 2.0f;

	/*Log(worldX);
	Log(worldY);*/
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && (action == GLFW_PRESS ))
	{
		//inc
		if (t < 1)
			t += 0.1f;
		Log(t);
		Log(lerpPoints[0].x << ", " << lerpPoints[0].y);

		//radius += 0.1f;
	}


	else if (key == GLFW_KEY_S && (action == GLFW_PRESS))
	{

		//dec
		if (t > 0)
			t -= 0.1f;
		Log(t);
		Log(lerpPoints[0].x << ", " << lerpPoints[0].y);
		

		//radius -= 0.1f;

	}
}
void LoadTexture(unsigned int& texture, const char* fileName)
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // to prevent image with wdith!=height from getting corrupted
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	Log("nr channels : " << nrChannels);
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
		Log("Loaded : " << fileName);
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

	//mouse input
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);

	stbi_set_flip_vertically_on_load(true);

	WindowUtils::DockConsoleAndWindow(window, WIDTH, HEIGHT);

	//glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	//glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	float vertices[] =
	{
		//pos                //Tex Coords
		-0.5f, -0.5f, 0.0f,	 0.0f,0.0f, // point -1
		 0.5f, -0.5f, 0.0f,	 1.0f,0.0f, // point -2
		-0.5f,  0.5f, 0.0f,	 0.0f,1.0f, // point -3

		-0.5f,  0.5f, 0.0f,	 0.0f,1.0f, // point -1
		 0.5f, -0.5f, 0.0f,	 1.0f,0.0f, // point -2
		 0.5f,  0.5f, 0.0f,	 1.0f,1.0f  // point -3
	};
	std::cout << "starting game loop - bezier curve " << std::endl;

	//1st vertex
	points.push_back(glm::vec3(-0.5f, 0.0f, 0.0f));
	//2nd vertex
	points.push_back(glm::vec3(0.5f,0.0f,0.0f));


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

	for (int i = 0;i <= segments;i++)
	{
		t = i / static_cast<float>(segments);
		auto point = BezierPoint(points, t);

		lerpPoints.push_back(point);

	}
	
	glGenBuffers(1, &lerpVBO);
	glGenVertexArrays(1, &lerpVAO);
	glBindVertexArray(lerpVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lerpVBO);
	glBufferData(GL_ARRAY_BUFFER, lerpPoints.size() * sizeof(glm::vec3), lerpPoints.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	unsigned int objectVBO, objectVAO;
	glGenBuffers(1, &objectVBO);
	glGenVertexArrays(1, &objectVAO);
	glBindVertexArray(objectVAO);

	glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader objectShader("Resources/Shaders/default.vert",
						"Resources/Shaders/circle.frag");
	objectShader.use();

	unsigned int texture_0, texture_1;
	LoadTexture(texture_0, "Resources/Textures/cat_open.png");
	LoadTexture(texture_1, "Resources/Textures/cat_close.png");
	
	objectShader.SetInt("texSampler_0", 0);
	objectShader.SetInt("texSampler_1", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float k = 0.0f;
	glm::vec3 movingPoint;
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

		FPSCounter(window);

		//logic
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT == GLFW_PRESS))
		{
			if (selectedPoint == nullptr)
			{
				for (int i = 0;i < points.size();i++)
				{
					if (glm::distance(points[i], glm::vec3(worldX, worldY, 0.0f)) <= 0.1)
					{
						selectedPoint = nullptr;
						selectedPoint = &points[i];
					}
				}
			}
			else
			{
				*selectedPoint = glm::vec3(worldX, worldY, 0.0f);
			}
			
		}
		
		//Log("selected Point : " << selectedPoint);
		for (int i = 0;i <= segments;i ++)
		{
			t = i/static_cast<float>(segments);
			auto point =  BezierPoint(points, t);

			lerpPoints[i] = point;

		}

		glm::mat4 model = glm::mat4(1.0f);
		//lines
		defaultShader.use();
		defaultShader.SetMat4("model", model);
		defaultShader.SetFloat("time", (float)glfwGetTime());
		glLineWidth(5.0f);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,points.size() * sizeof(glm::vec3),points.data(),GL_STATIC_DRAW);

		/*defaultShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
		glDrawArrays(GL_LINE_STRIP, 0, points.size());*/
		
		//bezier control points
		glPointSize(15.0f);
		defaultShader.SetVec3("objectColor", glm::vec3(1.0f));
		glDrawArrays(GL_POINTS, 0, points.size());

		//lerpPoints
		defaultShader.use();

		glPointSize(5.0f);
		glBindVertexArray(lerpVAO);

		glBindBuffer(GL_ARRAY_BUFFER, lerpVBO);
		glBufferData(GL_ARRAY_BUFFER, lerpPoints.size() * sizeof(glm::vec3), lerpPoints.data(), GL_STATIC_DRAW);

		defaultShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
		glLineWidth(5.0f);
		glDrawArrays(GL_LINE_STRIP, 0, lerpPoints.size());

		defaultShader.SetVec3("objectColor", glm::vec3(0.0f));
		glDrawArrays(GL_POINTS, 0, lerpPoints.size());
		

		//object
		k += 0.5f * deltaTime;
		movingPoint = BezierPoint(points, k);
		if (k >= 1)
			k = 0;

		model = glm::mat4(1.0f);
		model = glm::translate(model, movingPoint);
		model = glm::scale(model, glm::vec3(0.1f));

		objectShader.use();
		glBindVertexArray(objectVAO);

		glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_1);

		objectShader.SetVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
		objectShader.SetMat4("model", model);
		float value = (sin(15.0f*glfwGetTime()) > 0.0f) ? 1.0f : 0.0f;
		objectShader.SetFloat("mixValue", value);
		glDrawArrays(GL_TRIANGLES, 0,6);

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
