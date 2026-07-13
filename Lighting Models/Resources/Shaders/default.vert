#version 460 core
layout (location = 0) in vec3 aPos; //pos attributes
layout (location = 1) in vec3 aNormal; //normal attributes
layout (location = 2) in vec2 aTex; //tex attributes
uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 Color;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
void main()
{
    gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal= aNormal;
//    Color = aCol;
    TexCoord = aTex;
}