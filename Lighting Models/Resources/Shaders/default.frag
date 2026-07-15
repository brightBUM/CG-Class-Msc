#version 460 core
out vec4 FragColor;
//in vec3 Color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
uniform float ambient;
uniform sampler2D texSampler_0;
//uniform sampler2D texSampler_1;
uniform float time;
uniform vec3 objectColor ;
uniform vec3 lightPos ;
void main()
{
	vec4 texValue_0 = texture(texSampler_0,TexCoord);
	vec3 A = Normal;
	vec3 B = normalize(lightPos-FragPos);
	//diffuse+ambient
	float dotValue = dot(A,B);
	float diffuse = max(dotValue,0.0f);
	FragColor = vec4(texValue_0*(diffuse+ambient));

	
//	vec4 texValue_1 = texture(texSampler_1,TexCoord);
//	FragColor = vec4(mix(texValue_0,texValue_1,0.0f));

//	FragColor = vec4(vec3(1.0f,0.0f,0.0f),1.0f);
} 