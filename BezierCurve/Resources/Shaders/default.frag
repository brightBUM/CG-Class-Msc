#version 460 core
out vec4 FragColor;
//in vec3 Color;
uniform float time; // uniform - special keyword to receive data from cpu
uniform vec3 objectColor;
void main()
{
    FragColor = vec4(objectColor,1.0f);
} 