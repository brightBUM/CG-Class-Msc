#version 460 core
out vec4 FragColor;
in vec3 Color;
uniform float time; // uniform - special keyword to receive data from cpu

void main()
{
//      FragColor = vec4(Color, 1.0f);
//    FragColor = vec4(Color.x,Color.y,Color.z, 1.0f);
    FragColor = vec4(Color*sin(time), 1.0f);
//

//    FragColor = vec4(Color.xy,Color.z,1.0f);  //swizzling
//    FragColor = vec4(Color.rgb,1.0f);
} 