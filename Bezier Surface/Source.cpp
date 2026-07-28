#include <iostream>
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include<vector>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"Common.h"
#include"Camera.h"
#define HEIGHT 1200
#define WIDTH 1200
#define Log(x) std::cout<<x<<std::endl;

//global variables
float worldX, worldY;
double prevX, prevY;

std::vector<glm::vec3> points;
std::vector<glm::vec3> lerpPoints;

float t = 0.0f;
int segments = 20;

glm::vec3* selectedPoint = nullptr;
glm::vec3 Lerp(glm::vec3 A, glm::vec3 B, float t)
{
	return A + ((B - A) * t);
}

//DecastleJau's method for higher order bezier curve
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
		/*glm::vec3 newPoint(worldX, worldY, 0.0f);

		auto it = std::lower_bound(points.begin(), points.end(), newPoint,
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

		}*/

	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		selectedPoint = nullptr;
	}

}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	worldX = xpos ;
	worldY = ypos ;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && (action == GLFW_PRESS))
	{
		//inc
		//radius += 0.1f;
	}


	else if (key == GLFW_KEY_S && (action == GLFW_PRESS))
	{
		//dec
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
void CameraInput(GLFWwindow* window, Camera& camera)
{
	
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

glm::vec3 getMouseRay(float mouseX, float mouseY,
	const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {

	// 1. Convert screen position to NDC (-1.0 to 1.0)
	float x = (2.0f * mouseX) / WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / HEIGHT;

	// 2. Clip space coordinates
	glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);

	// 3. Eye/View space coordinates
	glm::vec4 ray_eye = glm::inverse(projectionMatrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

	// 4. World space coordinates
	glm::vec3 ray_wor = glm::vec3(glm::inverse(viewMatrix) * ray_eye);

	// 5. Normalize direction vector
	return glm::normalize(ray_wor);
}
bool RayPlaneIntersection(glm::vec3 rayOrigin, glm::vec3 rayDir,glm::vec3 planePoint, glm::vec3 planeNormal,
	float& t, glm::vec3& hitPoint)
{
	float denom = glm::dot(planeNormal, rayDir);

	if (std::abs(denom) > 1e-6f) {
		t = glm::dot(planePoint - rayOrigin, planeNormal) / denom;
		if (t >= 0.0f) {
			hitPoint = rayOrigin + t * rayDir;
			return true;
		}
	}
	return false;
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
	//glfwSwapInterval(0);
	//glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	//glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	std::cout << "starting game loop - bezier curve " << std::endl;

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

	//1st vertex
	points.push_back(glm::vec3(-0.5f, 0.0f, 0.0f));
	//2nd vertex
	points.push_back(glm::vec3(0.0f, 0.0f, 0.5f));
	//3rd vertex
	points.push_back(glm::vec3(0.5f, 0.0f, 0.0f));


	unsigned int VBO, VAO, lerpVAO, lerpVBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Binding the buffer - selecting current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//assign vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader defaultShader("Resources/Shaders/default.vert",
						 "Resources/Shaders/default.frag");

	Shader curveShader("Resources/Shaders/curve.vert",
					   "Resources/Shaders/curve.frag");

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


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float k = 0.0f;

	unsigned int cubeVBO, cubeVAO, cubeEBO;
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeEBO);
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	//Binding the buffer - selecting current buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	//assign vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices2), cubeVertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices2), cubeIndices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	unsigned int texture_0;
	LoadTexture(texture_0, "Resources/Textures/yellow_ball.png");

	defaultShader.use();
	defaultShader.SetInt("texSampler_0", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_0);

	Camera camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.373, 0.294, 0.545, 1.0f);

		FPSCounter(window);

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		
		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(camera.Zoom, (float)WIDTH / HEIGHT, 0.1f, 100.0f);

		CameraInput(window, camera);

		//logic
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			auto mouseRay = getMouseRay(worldX, worldY, proj, view);
			float t;
			glm::vec3 hitPoint;
			if (selectedPoint == nullptr)
			{
				if (RayPlaneIntersection(camera.Position, mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f), t, hitPoint))
				{
					for (int i = 0;i < points.size();i++)
					{
						if (glm::distance(points[i], hitPoint) <= 0.2f)
						{
							selectedPoint = &points[i];
							*selectedPoint = hitPoint;

							/*Log("hit point select " << " : "
								<< hitPoint.x << ", "
								<< hitPoint.y << ", "
								<< hitPoint.z);*/
						}

					}
				}
				
			}
			else
			{
				RayPlaneIntersection(camera.Position, mouseRay, *selectedPoint,
					glm::vec3(0.0f, 1.0f, 0.0f), t, hitPoint);

				hitPoint.y = 0.0f;
				*selectedPoint = hitPoint;
				/*Log("hit point drag " << " : "
					<< hitPoint.x << ", "
					<< hitPoint.y << ", "
					<< hitPoint.z);*/
			}

		}

		//Log("selected Point : " << selectedPoint);
		for (int i = 0;i <= segments;i++)
		{
			t = i / static_cast<float>(segments);
			auto point = BezierPoint(points, t);

			lerpPoints[i] = point;

		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		
		defaultShader.use();
		defaultShader.SetMat4("model", model);
		defaultShader.SetMat4("view", view);
		defaultShader.SetMat4("proj", proj);
		defaultShader.SetFloat("time", (float)glfwGetTime());
		defaultShader.SetVec3("objectColor", glm::vec3(1.0f,0.5f,0.0f));
		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//lines
		model = glm::mat4(1.0f);
		curveShader.use();
		curveShader.SetMat4("model", model);
		curveShader.SetMat4("view", view);
		curveShader.SetMat4("proj", proj);
		curveShader.SetFloat("time", (float)glfwGetTime());
		glLineWidth(2.0f);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);
		
		defaultShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
		glDrawArrays(GL_LINE_STRIP, 0, points.size());

		//bezier control points
		glPointSize(15.0f);
		curveShader.SetVec3("objectColor", glm::vec3(1.0f));
		glDrawArrays(GL_POINTS, 0, points.size());

		//lerpPoints
		curveShader.use();

		glPointSize(5.0f);
		glBindVertexArray(lerpVAO);

		glBindBuffer(GL_ARRAY_BUFFER, lerpVBO);
		glBufferData(GL_ARRAY_BUFFER, lerpPoints.size() * sizeof(glm::vec3), lerpPoints.data(), GL_STATIC_DRAW);
		
		curveShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
		glLineWidth(5.0f);
		glDrawArrays(GL_LINE_STRIP, 0, lerpPoints.size());

		curveShader.SetVec3("objectColor", glm::vec3(0.0f));
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
	glViewport(0, 0, width, height);

}
