
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define HEIGHT 800
#define WIDTH 600

#define Log(x) std::cout << x << std::endl;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


void LoadTexture(unsigned int& texture, const char* fileName)
{
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;

    unsigned char* data =
        stbi_load(fileName, &width, &height, &nrChannels, 0);

    Log(nrChannels);

    if (data)
    {
        switch (nrChannels)
        {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;

        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;

        default:
            Log("invalid image channels");
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

    glfwInit();

    GLFWwindow* window;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Quad Shader", NULL, NULL);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);



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

    unsigned int VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader defaultShader(
        "Resources/Shaders/default.vert",
        "Resources/Shaders/default.frag");

    defaultShader.use();

    unsigned int texture_0, texture_1;

    LoadTexture(texture_0,
        "Resources/Textures/wlp.png");

    LoadTexture(texture_1,
        "Resources/Textures/wlp.png");

    defaultShader.SetInt("texSampler_0", 0);
    defaultShader.SetInt("texSampler_1", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_1);



    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.5f, 0.4f, 1.0f);

        defaultShader.use();

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    glfwTerminate();


    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
