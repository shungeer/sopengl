#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D objTex;

void main()
{    
    FragColor = texture(objTex, TexCoords);
}