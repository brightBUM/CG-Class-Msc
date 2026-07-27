#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texSampler_0;
uniform sampler2D texSampler_1;
uniform float time;
uniform float mixValue;
float radius =0.5f;

uniform vec3 objectColor;
uniform vec3 dir;
void main()
{
	vec2 uv = TexCoord;
////	uv.x += time*0.25f;
//	uv.x-=dir.x*time*2.0f;
//	uv.y-=dir.y*time*2.0f;
	vec4 texValue_0 = texture(texSampler_0,uv);
	vec4 texValue_1 = texture(texSampler_1,uv);
	FragColor = mix(texValue_0,texValue_1,mixValue);

	float dis = distance(TexCoord,vec2(0.5f,0.5f));
//	if(dis<=radius)
//	{
//		FragColor = vec4(texValue_0.rgb,1.0f);	
//	}
//	else
//	{
//		FragColor = vec4(0.0f);
//
//	}

	
} 