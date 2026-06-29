#include"GLFW/glfw3.h"
#include<string>


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

