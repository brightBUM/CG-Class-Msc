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
std::vector<glm::vec3> sampleRowBezierPoints;
std::vector<glm::vec3> rowBezierPoints;
std::vector<glm::vec3> columnBezierPoints;

float t = 0.0f;
int segments = 10;

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
bool RayPlaneIntersection(glm::vec3 rayOrigin, glm::vec3 rayDir,glm::vec3 planePoint, 
	glm::vec3 planeNormal,float& t, glm::vec3& hitPoint)
{
	float denom = glm::dot(planeNormal, rayDir);

	if (std::abs(denom) > 1e-6f) 
	{
		t = glm::dot(planePoint - rayOrigin, planeNormal) / denom;
		if (t >= 0.0f) 
		{
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
	
	Log("starting game loop - bezier curve ");

	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			float x = i * 0.5f;
			float z = j * 0.5f;
			points.push_back(glm::vec3(x, 0.0f, z));
		}
	}

	Log("points size : "<<points.size());

	unsigned int VBO, VAO, columnVAO, columnVBO,rowVAO,rowVBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Binding the buffer - selecting current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//assign vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader curveShader("Resources/Shaders/curve.vert",
					   "Resources/Shaders/curve.frag");

	for (int i = 0;i < points.size();i += 3)
	{
		std::vector<glm::vec3> rowPoints;
		for (int j = i;j < i + 3;j++)
		{
			rowPoints.push_back(points[j]);
		}
		float u;
		for (int k = 0;k <= segments;k++)  
		{
			u = k / static_cast<float>(segments);

			auto point = BezierPoint(rowPoints, u);

			sampleRowBezierPoints.push_back(point);
		}

	}

	for (int i = 0;i < segments + 1;i++)
	{
		std::vector<glm::vec3> columnPoints;
		for (int j = i;j < sampleRowBezierPoints.size();j+=segments+1)
		{
			auto point = sampleRowBezierPoints[j];
			columnPoints.push_back(point);
		}
		float v;
		for (int k = 0;k <= segments;k++)
		{
			v = k / static_cast<float>(segments);

			auto point = BezierPoint(columnPoints, v);
			//Log(" columnBezierPoints : " << point.x << " , " << point.y << " , " << point.z);

			columnBezierPoints.push_back(point);
		}
	}

	for (int i = 0;i < segments + 1;i++)
	{
		for (int j = i ;j < columnBezierPoints.size();j+=segments+1)
		{
			rowBezierPoints.push_back(columnBezierPoints[j]);
		}
	}

	Log("sample row bezierPoints size : " << sampleRowBezierPoints.size());
	Log("column bezierPoints size : " << columnBezierPoints.size());
	Log("row bezierPoints size : " << rowBezierPoints.size());

	glGenBuffers(1, &columnVBO);
	glGenVertexArrays(1, &columnVAO);
	glBindVertexArray(columnVAO);
	glBindBuffer(GL_ARRAY_BUFFER, columnVBO);
	glBufferData(GL_ARRAY_BUFFER, columnBezierPoints.size() * sizeof(glm::vec3), columnBezierPoints.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &rowVBO);
	glGenVertexArrays(1, &rowVAO);
	glBindVertexArray(rowVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rowVBO);
	glBufferData(GL_ARRAY_BUFFER, columnBezierPoints.size() * sizeof(glm::vec3), columnBezierPoints.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
				for (int i = 0;i < points.size();i++)
				{
					RayPlaneIntersection(camera.Position, mouseRay, points[i],
						glm::vec3(0.0f, 1.0f, 0.0f), t, hitPoint);

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
			else
			{
				if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				{
					RayPlaneIntersection(camera.Position, mouseRay, *selectedPoint,
						glm::vec3(0.0f, 0.0f, 1.0f), t, hitPoint);
				}
				else
				{
					RayPlaneIntersection(camera.Position, mouseRay, *selectedPoint,
						glm::vec3(0.0f, 1.0f, 0.0f), t, hitPoint);
				}

				//hitPoint.y = 0.0f;
				*selectedPoint = hitPoint;
				
			}

		}

		//update sample rows
		for (int row = 0; row < 3; row++)
		{
			std::vector<glm::vec3> rowPoints;

			for (int col = 0; col < 3; col++)
			{
				rowPoints.push_back(points[row * 3 + col]);
			}

			for (int sample = 0; sample <= segments; sample++)
			{
				float u = sample / static_cast<float>(segments);

				glm::vec3 point = BezierPoint(rowPoints, u);

				int index = row * (segments + 1) + sample;

				sampleRowBezierPoints[index] = point;
			}
		}
		//update column 
		for (int column = 0; column <= segments; column++)
		{
			std::vector<glm::vec3> columnPoints;

			for (int row = 0; row < 3; row++)
			{
				int index = row * (segments + 1) + column;

				columnPoints.push_back(sampleRowBezierPoints[index]);
			}

			for (int sample = 0; sample <= segments; sample++)
			{
				float v = sample / static_cast<float>(segments);

				glm::vec3 point = BezierPoint(columnPoints, v);

				int index = column * (segments + 1) + sample;

				columnBezierPoints[index] = point;
			}
		}
		//update main rows
		rowBezierPoints.clear();

		for (int row = 0; row <= segments; row++)
		{
			for (int column = row; column < columnBezierPoints.size(); column += segments + 1)
			{
				rowBezierPoints.push_back(columnBezierPoints[column]);
			}
		}

		glm::mat4 model = glm::mat4(1.0f);

		//lines
		model = glm::mat4(1.0f);
		curveShader.use();
		curveShader.SetMat4("model", model);
		curveShader.SetMat4("view", view);
		curveShader.SetMat4("proj", proj);
		curveShader.SetFloat("time", (float)glfwGetTime());
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);
		

		glLineWidth(2.0f);
		glPointSize(15.0f);
		//control points
		for (int i = 0;i < points.size();i+=3)
		{
			/*curveShader.SetVec3("objectColor", glm::vec3(0.0f));
			glDrawArrays(GL_LINE_STRIP, i, 3);*/

			curveShader.SetVec3("objectColor", glm::vec3(1.0f));
			glDrawArrays(GL_POINTS, i, 3);
			
		}
		
		

		//Bezier Points
		curveShader.use();
		glPointSize(5.0f);
		glLineWidth(5.0f);

		glBindVertexArray(columnVAO);

		glBindBuffer(GL_ARRAY_BUFFER, columnVBO);
		glBufferData(GL_ARRAY_BUFFER, columnBezierPoints.size() * sizeof(glm::vec3), columnBezierPoints.data(), GL_STATIC_DRAW);
		
		/*for (int i = 0;i < rowBezierPoints.size() ;i += segments + 1)
		{
			curveShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
			glDrawArrays(GL_LINE_STRIP, i, 11);

			curveShader.SetVec3("objectColor", glm::vec3(0.0f));
			glDrawArrays(GL_POINTS, i, 11);
		}*/
		 
		curveShader.SetVec3("objectColor", glm::vec3(0.0f));
		glDrawArrays(GL_POINTS, 0, columnBezierPoints.size());
		

		//columns bezier curves
		for (int i = 0;i < columnBezierPoints.size();i += segments + 1)
		{
			curveShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
			glDrawArrays(GL_LINE_STRIP, i, 11);
		}

		//row bezier curves

		glBindVertexArray(rowVAO);

		glBindBuffer(GL_ARRAY_BUFFER, rowVBO);
		glBufferData(GL_ARRAY_BUFFER, rowBezierPoints.size() * sizeof(glm::vec3), rowBezierPoints.data(), GL_STATIC_DRAW);

		for (int i = 0;i < rowBezierPoints.size();i += segments + 1)
		{
			curveShader.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.0f));
			glDrawArrays(GL_LINE_STRIP, i, 11);
		}

		

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
