#version 460 core
out vec4 FragColor;
//in vec3 Color;

uniform sampler2D texSampler_0;
//uniform sampler2D texSampler_1;
uniform float time;
uniform vec3 objectColor ;
void main()
{
	FragColor = vec4(objectColor,1.0f);

} 