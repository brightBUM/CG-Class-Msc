#version 460 core
out vec4 FragColor;
//in vec3 Color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

//uniform sampler2D texSampler_2;
uniform float time;
uniform vec3 objectColor ;
uniform vec3 lightPos ;
uniform vec3 lightColor ;
uniform vec3 camPos ;

struct Material
{
	vec3 ambient;
	int specularStrength;
	sampler2D diffuse;
	sampler2D specular;
};

uniform Material material;
void main()
{
	vec4 diffuseMap = texture(material.diffuse,TexCoord);
	vec4 specularMap = texture(material.specular,TexCoord);
	//vec4 normalMap = texture(texSampler_2,TexCoord);
	vec3 A = Normal;
//	A = normalize(A * 2.0 - 1.0);  // color space to direction space
//	A = A*0.5f+0.5f; // direction space to color space 

	vec3 lightDir = normalize(lightPos-FragPos);

	//diffuse
	float dotValue = dot(A,lightDir);
	float diffuseValue = max(dotValue,0.0f);

	//specular
	vec3 viewDir = normalize(camPos-FragPos);
	vec3 refLightDir = reflect(-lightDir,A);
	float specDotValue = dot(refLightDir,viewDir);
	float specularValue = max(specDotValue,0.0f);
	specularValue = pow(specularValue,material.specularStrength);
		
	//phong lighting
	FragColor = vec4((diffuseMap.rgb*(diffuseValue+material.ambient)+
					 (specularMap.rgb*specularValue)*lightColor),1.0f);
	
//	FragColor = vec4(A,1.0f);

} 