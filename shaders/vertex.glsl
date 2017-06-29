#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colorData;
layout(location = 2) in vec2 dataUV;
layout(location = 3) in vec3 normalData;

out vec2 UV;
out vec3 colorOut;
out vec3 normals;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0f);
	UV = dataUV;
	colorOut = colorData;
	normals = vec3(mat3(transpose(inverse(model))) * normalData);
	fragPos = vec3(model * vec4(position, 1.0f));
}