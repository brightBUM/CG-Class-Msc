#version 460 core
out vec4 FragColor;
in vec3 Color;
in vec2 Texcoord;
uniform float time; // uniform - special keyword to receive data from cpu
uniform sampler2D texSampler;
void main()
{
      vec4 texValue = texture(texSampler,Texcoord);
      FragColor = vec4(texValue);
} 