#version 330 core

in vec2 TexCoords;

uniform vec3 spriteColor;
uniform sampler2D spriteTexture;

out vec4 color;

void main()
{
    vec4 textureColor = texture(spriteTexture , TexCoords);
    color = vec4(spriteColor , 1.0) * textureColor;
}