#version 460 core
out vec4 FragColor;
//in vec3 Color;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D texSampler_0;
uniform sampler2D texSampler_1;
uniform sampler2D texSampler_2;
uniform float time;
uniform vec3 objectColor ;

uniform vec3 camPos ;

struct Light
{
	vec3 pos ;
	vec3 direction;
	vec3 color ;
	float constant;
	float linear;
	float quadratic;
};
struct Material
{
	float ambient;
	int specularStrength;
};

uniform Material material;
uniform Light lights[6];


vec3 GetAmbient(vec3 diffuse)
{
	return diffuse*material.ambient;
}
vec3 GetDiffuse(vec3 diffuse,vec3 normal,vec3 lightDir)
{
	//diffuse calculation
	float dotValue = dot(normal,lightDir);
	float diffuseValue = max(dotValue,0.0f);
	
	return diffuse*diffuseValue;
}
vec3 GetSpecular(vec3 specular,vec3 normal,vec3 lightDir,vec3 FragPos,vec3 camPos)
{
	//specular calculation
	vec3 viewDir = normalize(camPos-FragPos);
	vec3 refLightDir = reflect(-lightDir,normal);
	float specDotValue = dot(refLightDir,viewDir);
	specDotValue = max(specDotValue,0.0f);
	float specularValue = pow(specDotValue,material.specularStrength);
	
	return specular*specularValue;
}

vec3 CalculatePointLight(Light light,vec3 Normal,vec3 FragPos,vec3 camPos,vec3 diffuseMap,vec3 specularMap)
{
	vec3 lightDir = normalize(light.pos-FragPos);

	//Point Light 
	float distance    = length(light.pos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));    

	//phong lighting
	vec3 ambient = GetAmbient(diffuseMap.rgb);
	vec3 diffuse = GetDiffuse(diffuseMap.rgb,Normal,lightDir);
	vec3 specular = GetSpecular(specularMap.rgb,Normal,lightDir,FragPos,camPos);
	
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;
	
	vec3 point_output = vec3((ambient+diffuse+length(specular))*light.color);
	return point_output;
}


void main()
{
	vec4 diffuseMap = texture(texSampler_0,TexCoord);
	vec4 specularMap = texture(texSampler_1,TexCoord);
	vec3 normalMap = texture(texSampler_2,TexCoord).rgb;
	normalMap = normalize(normalMap * 2.0 - 1.0);  // color space to direction space
//	A = A*0.5f+0.5f; // direction space to color space 

////	Directional light
//	vec3 lightDir = normalize(light.direction);
	vec3 outputColor = vec3(0.0f);
	
	for(int i=0;i<6;i++)
	{
		vec3 pointLightOutput = CalculatePointLight(lights[i],Normal,FragPos,
		camPos,diffuseMap.rgb,specularMap.rgb);

		outputColor += pointLightOutput;

	}

	FragColor = vec4(outputColor,1.0f);


} 