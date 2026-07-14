#include"Shader.h"
#include"GLFW/glfw3.h"
#include<string>
#include<Windows.h>

namespace Random
{
    int RandomInt(int min, int max);
    float RandomFloat(float min, float max);
}

extern double deltaTime;
extern double lastFrame;
extern double fps;
extern double fpsTimer;
extern double fpsUpdateInterval; // half a second
extern int frames;

void FPSCounter(GLFWwindow* window);
void SetProjectAssetPath(const char* path);

namespace Debug
{
    void Init();
    void ShutDown();
    //void BeginFrame();
    void DrawLine(glm::vec3, glm::vec3, glm::vec3);
    void DrawArrow(glm::vec3, glm::vec3, glm::vec3);
    void DrawCircle(glm::vec3 centre, float radius, glm::vec3 color);
    //void EndFrame();
}
namespace Grid
{
    void InitGrid(int gridSize);
    void DrawGrid();

}
namespace Collision
{
    bool Circle_Point(glm::vec3, float, glm::vec3);
}

namespace WindowUtils
{
    void DockConsoleAndWindow(GLFWwindow* window,int windowWidth,int windowHeight);
}