/**
* @file main.c
*
* @brief Entry point to the particles program, this program runs an n-body gravity simulation
*
* @author Patrick Settle (https://github.com/psettle)
*/

#include    "stdio.h"
#include    "opengl_includes.h"

int main()
{
    GLFWwindow*         pWindow;
    GLubyte const *     version;

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