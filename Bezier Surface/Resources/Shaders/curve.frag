#version 460 core
out vec4 FragColor;

uniform float time;

uniform vec3 objectColor;
void main()
{
	 FragColor = vec4(objectColor,1.0f);
} 