#version 330 core

in vec3 colorOut;
in vec2 UV;
out vec4 color;

uniform sampler2D image1;
uniform sampler2D specularMap;

void main()
{
    color = texture(image1, UV);
}