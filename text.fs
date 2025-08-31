#version 330 core

in vec2 TexCoords;


uniform sampler2D text;

uniform vec3 textColor;

out vec4 color;

void main()
{
    color = vec4(textColor , 1.0) * vec4(1.0 , 1.0 , 1.0 , texture(text , TexCoords).r);
}