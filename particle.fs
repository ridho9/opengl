#version 330 core

out vec4 outColor;

uniform vec3 particleColor;  
uniform float alpha;

void main()
{
    outColor = vec4(particleColor, alpha);
    outColor.a = alpha;
}