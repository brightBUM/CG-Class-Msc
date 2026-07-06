// Common.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include"Common.h"
// TODO: This is an example of a library function
#include <random>
#include<filesystem>
const float Rad2Deg = 57.2958f;
const float Deg2Rad = 3.14159265f / 180.0f;
namespace Debug
{
    unsigned int VBO, VAO;
    const char* assetPath = nullptr;
    Shader* debugShader = nullptr;
    void SetAssetPath(const char* path)
    {
        

        assetPath = path;
    }

    void Init()
    {
        std::string vertexPath = std::string(assetPath) + "Shaders/debug.vert";
        std::string fragmentPath = std::string(assetPath) + "Shaders/debug.frag";

        std::cout << "Vertex Path: "
            << vertexPath << std::endl;
        std::cout << "Current Path: "
            << std::filesystem::current_path() << std::endl;


       /* if (std::filesystem::exists(vertexPath))
        {
            std::cout << "Vertex shader found\n";
        }
        else
        {
            std::cout << "Vertex shader NOT found\n";
        }*/
        debugShader = new Shader(vertexPath.c_str(),fragmentPath.c_str());

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color)
    {
        float vertices[] =
        {
            start.x, start.y, start.z,
            end.x,   end.y,   end.z
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
        
        debugShader->use();

        debugShader->SetVec3("color", color);

        glLineWidth(1.0f);
        glBindVertexArray(VAO);

        glDrawArrays(GL_LINES, 0, 2);
    }
    void DrawArrow(glm::vec3 start, glm::vec3 end, glm::vec3 color)
    {
        DrawLine(start, end, color);

        glm::vec3 dir = glm::normalize(end - start);

        float arrowLength = 0.025f;
        float angle = glm::radians(30.0f);

        glm::vec3 leftWing = end - arrowLength * glm::vec3(dir.x * cos(angle) - dir.y * sin(angle), 
                                                           dir.x * sin(angle) + dir.y * cos(angle), 
                                                           0.0f);

        glm::vec3 rightWing = end - arrowLength * glm::vec3(dir.x * cos(-angle) - dir.y * sin(-angle), 
                                                            dir.x * sin(-angle) + dir.y * cos(-angle), 
                                                            0.0f);
        DrawLine(end, leftWing, color);
        DrawLine(end, rightWing, color);
    }
    void DrawCircle(glm::vec3 centre, float radius, glm::vec3 color)
    {
        const int segments = 32;
        const float angleStep = glm::two_pi<float>() / segments;

        for (int i = 0; i < segments; i++)
        {
            float angle1 = i * angleStep;
            float angle2 = (i + 1) * angleStep;

            glm::vec3 p1 = centre + glm::vec3(radius * cos(angle1), radius * sin(angle1), 0.0f);
            glm::vec3 p2 = centre + glm::vec3(radius * cos(angle2), radius * sin(angle2), 0.0f);

            DrawLine(p1, p2, color);
        }
    }
    void ShutDown()
    {
        delete debugShader;
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

}
namespace
{
    // Created once and reused
    std::random_device rd;
    std::mt19937 rng(rd());
}

namespace Random
{
    int RandomInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    float RandomFloat(float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }
}
void fnCommon()
{
	
}



double deltaTime = 0.0;
double lastFrame = 0.0;

double fps = 0.0;
double fpsTimer = 0.0;
double fpsUpdateInterval = 0.5; // half a second
int frames = 0;

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

namespace Collision
{
    bool Circle_Point(glm::vec3 centre, float radius, glm::vec3 point)
    {
        return glm::distance(centre, point) <= radius;
    }
}

namespace WindowUtils
{
    void DockConsoleAndWindow(GLFWwindow* window, int windowWidth, int windowHeight)
    {
        //console and window docking
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        int screenWidth = mode->width;
        int screenHeight = mode->height;

        //glfw window
        glfwSetWindowPos(window, windowWidth / 2, 50);

        //console
        HWND console = GetConsoleWindow();
        MoveWindow(console, windowWidth / 2, 50 + windowHeight, windowWidth, screenHeight - windowHeight - 125, TRUE);
    }
}

