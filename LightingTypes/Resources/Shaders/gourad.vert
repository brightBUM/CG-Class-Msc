#version 460 core
layout (location = 0) in vec3 aPos; //pos attributes
layout (location = 1) in vec3 aNormal; //normal attributes
layout (location = 2) in vec2 aTex; //tex attributes
uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 objectColor ;
uniform vec3 lightPos ;
uniform vec3 lightColor ;
uniform vec3 camPos ;

out vec3 Color;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

struct Material
{
	float ambient;
	int specularStrength;
};

uniform Material material;

out vec4 outputColor;
void main()
{
    gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal= aNormal; //model's local space normal vector
//    Normal = normalize(mat3(model) * aNormal); //normal vector to world space
//    Color = aCol;
    TexCoord = aTex;

    //vec4 texValue_0 = texture(texSampler_0,TexCoord);
	//vec4 specularMap = texture(texSampler_1,TexCoord);
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
	specular = pow(specular,material.specularStrength);
		
	//phong lighting
	outputColor = vec4((diffuse+material.ambient+specular)*lightColor,1.0f);
}