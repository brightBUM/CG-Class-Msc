#version 460 core
layout (location = 0) in vec3 aPos; //pos attributes
layout (location = 1) in vec3 aCol; //col attributes
uniform float time;
out vec3 Color;
void main()
{
    gl_Position = vec4(aPos.x*sin(time), aPos.y, aPos.z, 1.0);
    Color = aCol;
}