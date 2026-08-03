#version 460 core
out vec4 FragColor;
in vec3 Color;
in vec2 TexCoord;
uniform float scrollSpeed;
uniform sampler2D texSampler_0;
//uniform sampler2D texSampler_1;
uniform float time; // uniform - special keyword to receive data from cpu
void main()
{
	vec2 uv = TexCoord;
	uv.x -= scrollSpeed;
	vec4 texValue_0 = texture(texSampler_0,uv);
//	vec4 texValue_1 = texture(texSampler_1,TexCoord);
	
	FragColor = texValue_0;
} 