#version 330 core

out vec4 FragColor;

uniform vec4 lightCol;

void main()
{
    FragColor = lightCol;
};