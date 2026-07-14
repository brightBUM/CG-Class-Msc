#version 460 core
layout (location = 0) in vec3 aPos; //pos attributes
//layout (location = 1) in vec3 aCol; //col attributes
uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main()
{
    gl_Position = model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
//    Color = aCol;
}