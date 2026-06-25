#include <iostream>
#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define HEIGHT 600
#define WIDTH 800

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void DrawQuad(float xpos, float ypos, float w, float h, float r, float g, float b);

const float Deg2Rad = 3.14159265f / 180.0f;

int main()
{
    glfwInit();

    GLFWwindow* window =
        glfwCreateWindow(WIDTH, HEIGHT, "Japan Flag", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // White flag background
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        DrawQuad(-0.8f, 0.5f, 1.6f, 1.0f,
            1.0f, 1.0f, 1.0f);

        // Red circle
        float radius = 0.25f;

        glBegin(GL_TRIANGLE_FAN);

        glColor3f(1.0f, 0.0f, 0.0f);

        // Center point
        glVertex3f(0.0f, 0.0f, 0.0f);

        for (int i = 0; i <= 360; i++)
        {
            float angle = i * Deg2Rad;

            float x = radius * cos(angle);
            float y = radius * sin(angle);

            glVertex3f(x, y, 0.0f);
        }

        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void DrawQuad(float xpos, float ypos,
    float w, float h,
    float r, float g, float b)
{
    glColor3f(r, g, b);

    glBegin(GL_QUADS);

    glVertex3f(xpos, ypos, 0.0f);
    glVertex3f(xpos, ypos - h, 0.0f);
    glVertex3f(xpos + w, ypos - h, 0.0f);
    glVertex3f(xpos + w, ypos, 0.0f);

    glEnd();
}

void framebuffer_size_callback(GLFWwindow* window,
    int width,
    int height)
{
    glViewport(0, 0, width, height);
}