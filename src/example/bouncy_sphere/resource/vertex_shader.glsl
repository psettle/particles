#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;

out vec3 normal_out;
out vec3 pos_out;

void main()
{
    mat4 MVP = projection_view_matrix * model_matrix;

    gl_Position = MVP * vec4(position, 1.0f);

    normal_out = mat3( transpose( inverse( model_matrix ) ) ) * normal;  
    pos_out = vec3( gl_Position );
}