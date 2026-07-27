#version 460 core
layout (location = 0) in vec3 aPos; //pos attributes
layout (location = 1) in vec2 aTex; //tex attributes
uniform float time;
uniform mat4 model;
out vec2 TexCoord;
void main()
{
    gl_Position = model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTex;
}