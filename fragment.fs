#version 330 core

in vec3 Color;
in vec2 TexCoord;
in float Normal;

out vec4 outColor;

uniform sampler2D ourTexture;
uniform float ambientStrength;

void main()
{
    // outColor = vec4(Color, 1.0);
    vec2 convTexCoord = TexCoord;
    convTexCoord.x = TexCoord.x / 240;
    convTexCoord.y = 1 - (TexCoord.y / 340);
    outColor = texture(ourTexture, convTexCoord) * vec4(Color * ambientStrength, 1.0);
}