#version 330 core

layout (location=0) in vec3 position;
// layout (location=1) in vec3 color;
layout (location=2) in vec2 texCoord;
layout (location=3) in float normal;

out vec3 Color;
out vec2 TexCoord;
out float Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(position, 1.0);
    Color = vec3(1.0, 1.0, 1.0);
    TexCoord = texCoord;
    Normal = normal;
    FragPos = vec3(model * vec4(position, 1.0));
}