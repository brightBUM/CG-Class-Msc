
#version 460 core

out vec4 FragColor;

in vec3 Color;
in vec2 Texcoord;

uniform float time;

uniform sampler2D texSampler1;
uniform sampler2D texSampler2;

uniform float mouseVal;

void main()
{
    // comment this to see only image
    FragColor = vec4(Color.x, Color.y, Color.z, 1.f);

    // comment this to see the color only
    FragColor = texture(texSampler1, Texcoord);
}
