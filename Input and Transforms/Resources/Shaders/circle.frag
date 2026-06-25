#version 460 core
out vec4 FragColor;
in vec3 Color;
in vec2 TexCoord;

uniform sampler2D texSampler_0;
uniform float time;
uniform float radius;
uniform float centreX;
uniform float centreY;
void main()
{
	vec4 texValue_0 = texture(texSampler_0,TexCoord);
	float dis = distance(TexCoord,vec2(centreX,centreY));
	if(dis<=radius)
	{
		FragColor = vec4(Color,1.0f);
	}
	else
	{
		FragColor = vec4(0.0f);

	}

	
} 