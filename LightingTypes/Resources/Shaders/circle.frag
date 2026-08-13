#version 460 core
out vec4 FragColor;
in vec3 Color;
in vec2 TexCoord;

uniform sampler2D texSampler_0;
uniform float time;
uniform float radius;
uniform float centreX;
uniform float centreY;
uniform vec3 objectColor;
uniform vec3 dir;
void main()
{
	vec2 uv = TexCoord;
//	uv.x += time*0.25f;
	uv.x-=dir.x*time*2.0f;
	uv.y-=dir.y*time*2.0f;
	vec4 texValue_0 = texture(texSampler_0,uv);
	float dis = distance(TexCoord,vec2(centreX,centreY));
//	FragColor = vec4(texValue_0);

	if(dis<=radius)
	{
		FragColor = vec4(texValue_0.rgb,1.0f);	
	}
	else
	{
		FragColor = vec4(0.0f);

	}

	
} 