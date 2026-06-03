#version 460 core
layout (location = 0) in vec3 aPos; //pos attributes
layout (location = 1) in vec3 aCol; //col attributes
layout (location = 2) in vec2 aTex; //tex attributes

uniform float time;
out vec3 Color;
out vec2 Texcoord;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    Color = aCol;
    Texcoord = aTex;
}