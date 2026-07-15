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
uniform int specularStrength;
uniform vec3 objectColor ;
uniform vec3 lightPos ;
uniform vec3 camPos ;
void main()
{
	vec4 texValue_0 = texture(texSampler_0,TexCoord);
	vec3 A = Normal;
	vec3 lightDir = normalize(lightPos-FragPos);

	//diffuse
	float dotValue = dot(A,lightDir);
	float diffuse = max(dotValue,0.0f);

	//specular
	vec3 viewDir = normalize(camPos-FragPos);
	vec3 refLightDir = reflect(lightDir,A);
	float specDotValue = dot(-refLightDir,viewDir);
	float specular = max(specDotValue,0.0f);
	specular = pow(specular,specularStrength);

	//phong lighting
	FragColor = vec4(texValue_0*(diffuse+ambient+specular));

	
//	vec4 texValue_1 = texture(texSampler_1,TexCoord);
//	FragColor = vec4(mix(texValue_0,texValue_1,0.0f));

//	FragColor = vec4(vec3(1.0f,0.0f,0.0f),1.0f);
} 