#version 460 core
layout (location = 0) in vec3 aPos; //attributes
layout (location = 1) in vec3 aCol; //attributes

out vec3 Color;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    Color = aCol;
}