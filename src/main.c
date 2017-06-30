/**
* @file main.c
*
* @brief Entry point to the particles program, this program runs an n-body gravity simulation
*
* @author Patrick Settle (https://github.com/psettle)
*/

#include    "stdio.h"
#include    "matrix_math.h"

int main()
{
    vec3_type vec3;
    mat4_type mat4;

    GLFWwindow*         pWindow;
    GLubyte const *     version;


    vec3_set( &vec3, VEC3_NULL );
    mat4_set( &mat4, MAT4_NULL );

    while(1);

    return 0;

	if (!glfwInit()) {
		return GLFW_FALSE;
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	pWindow = glfwCreateWindow(600, 600, "Window", NULL, NULL);

	glfwMakeContextCurrent(pWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		return GLFW_FALSE;
	}

    version = glGetString(GL_VERSION);

	printf("Version: %s\n", version);

	return 0;
}