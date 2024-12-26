#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 vTexCoords;

uniform mat4 modelMatrix;
uniform mat4 ViewProjection;

void main()
{
	vTexCoords = aUV;
	gl_Position = ViewProjection * modelMatrix * vec4(aPos, 1.0);
}