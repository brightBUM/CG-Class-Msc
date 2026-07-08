#version 460 core
out vec4 FragColor;
//in vec3 Color;
in vec2 TexCoord;

uniform sampler2D texSampler_0;
//uniform sampler2D texSampler_1;
uniform float time;
uniform vec3 objectColor ;
float radius =0.5f;
void main()
{
	vec2 centre = vec2(0.5);
	vec4 texValue_0 = texture(texSampler_0,TexCoord);
//	vec4 texValue_1 = texture(texSampler_1,TexCoord);
//	FragColor = vec4(mix(texValue_0,texValue_1,0.0f));
	FragColor = vec4(texValue_0);

//	float dis = distance(TexCoord,centre);
//	if(dis<=radius)
//	{
//		FragColor = vec4(texValue_0.rgb,1.0f);	
//	}
//	else
//	{
//		FragColor = vec4(0.0f);
//
//	}
//	FragColor = vec4(vec3(1.0f,0.0f,0.0f),1.0f);
} 