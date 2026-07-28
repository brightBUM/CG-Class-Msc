#version 460 core
out vec4 FragColor;
in vec2 TexCoord;
uniform float time; // uniform - special keyword to receive data from cpu
uniform vec3 objectColor;
uniform sampler2D texSampler_0;
void main()
{
    vec4 texValue_0 = texture(texSampler_0,TexCoord);
    FragColor = vec4(texValue_0.rgb,1.0f);
} 