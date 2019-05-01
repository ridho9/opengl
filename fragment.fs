#version 330 core

in vec3 Color;
in vec2 TexCoord;
in float Normal;
in vec3 FragPos;

out vec4 outColor;

uniform sampler2D ourTexture;
uniform float ambientStrength;
uniform vec3 lightPos;  

void main()
{
    vec3 normalVec;
    if(Normal < 1) {
        normalVec = vec3(1.0, 0, 0);
    } else if (Normal < 3) {
        normalVec = vec3(-1.0, 0, 0);
    } else if (Normal < 5) {
        normalVec = vec3(0.0, 1.0, 0);
    } else if (Normal < 7) {
        normalVec = vec3(0.0, -1.0, 0);
    } else if (Normal < 9) {
        normalVec = vec3(0.0, 0.0, 1.0);
    } else {
        normalVec = vec3(0.0, 0.0, -1.0);
    }
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(normalVec, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.9, 0.9, 0.9);

    // outColor = vec4(Color, 1.0);
    vec2 convTexCoord = TexCoord;
    convTexCoord.x = TexCoord.x / 240;
    convTexCoord.y = 1 - (TexCoord.y / 340);
    outColor = texture(ourTexture, convTexCoord) * vec4(Color * ambientStrength, 1.0);
}