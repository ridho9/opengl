#version 330 core
in vec3 Color;
in vec2 TexCoord;

out vec4 outColor;

void main()
{
    outColor = vec4(Color, 1.0);
}