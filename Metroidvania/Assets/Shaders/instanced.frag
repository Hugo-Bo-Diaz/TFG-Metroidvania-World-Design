#version 330 core

in vec2 vTexCoords;

out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
   FragColor = vec4(vTexCoords.xy,0.0f,1.0f);
}