#version 460 core
out vec4 FragColor;
in vec3 Color;
in vec2 TexCoord;

//uniform sampler2D texSampler_0;
//uniform sampler2D texSampler_1;
uniform float time;
void main()
{
//	vec4 texValue_0 = texture(texSampler_0,TexCoord);
//	vec4 texValue_1 = texture(texSampler_1,TexCoord);
//	FragColor = vec4(mix(texValue_0,texValue_1,0.0f));
	FragColor = vec4(1.0f);
} 