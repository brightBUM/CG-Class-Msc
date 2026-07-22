#version 460 core
out vec4 FragColor;
//in vec3 Color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 outputColor;

//uniform sampler2D texSampler_0;
//uniform sampler2D texSampler_1;
//uniform sampler2D texSampler_2;
uniform float time;
uniform vec3 objectColor ;
uniform vec3 lightPos ;
uniform vec3 lightColor ;
uniform vec3 camPos ;

struct Material
{
	float ambient;
	int specularStrength;
};

uniform Material material;
void main()
{
	
  FragColor = outputColor;

} 