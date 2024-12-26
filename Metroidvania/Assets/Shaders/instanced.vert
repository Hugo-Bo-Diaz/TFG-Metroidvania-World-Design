#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 UV;
layout (location = 2) in mat4 modelMatrix;

out vec2 vTexCoords;

uniform mat4 ViewProjection;

void main()
{
	vTexCoords = UV;
	gl_Position = ViewProjection * modelMatrix * vec4(aPos, 1.0);
}