#version 460 core
layout (location = 0) in vec3 aPos; //pos attributes
layout (location = 1) in vec3 aNormal; //normal attributes
layout (location = 2) in vec2 aTex; //tex attributes
layout (location = 3) in vec3 aTangent; 
layout (location = 4) in vec3 aBitTanget
; 
uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 Color;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;
void main()
{
    gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = normalize(mat3(model) * aTangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));
    
    TBN = mat3(T, B, N);    

    TexCoord = aTex;

//    Color = aCol;
}