#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

out vec2 UV;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;

void main()
{
    gl_Position = projection_view_matrix * model_matrix * vec4(position, 1.0f);
	UV = uv;
}