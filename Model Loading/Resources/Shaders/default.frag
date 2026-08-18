#version 460 core
out vec4 FragColor;
//in vec3 Color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D texSampler_0;
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
    sampler2D diffuse0;
    sampler2D specular0;
    float shininess;
};

uniform Material material;
void main()
{
	vec4 diffuseMap = texture(material.diffuse0,TexCoord);
	vec4 specularMap = texture(material.specular0,TexCoord);
	//vec4 normalMap = texture(texSampler_2,TexCoord);
	vec3 A = Normal;
//	A = normalize(A * 2.0 - 1.0);  // color space to direction space
//	A = A*0.5f+0.5f; // direction space to color space 

	vec3 lightDir = normalize(lightPos-FragPos);

	//diffuse
	float dotValue = dot(A,lightDir);
	float diffuse = max(dotValue,0.0f);

	//specular
	vec3 viewDir = normalize(camPos-FragPos);
	vec3 refLightDir = reflect(-lightDir,A);
	float specDotValue = dot(refLightDir,viewDir);
	float specular = max(specDotValue,0.0f);
	specular = pow(specular,material.shininess);
		
	//phong lighting
	FragColor = vec4(diffuseMap.rgb*(diffuse+material.ambient)+(specularMap.rgb*specular),1.0f);
	
//	FragColor = vec4(A,1.0f);

} 