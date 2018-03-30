/**
 * @file texture_cube_model.c
 *
 * @brief Definition of the texture cube model
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "texture_cube.h"

/**********************************************************************
                             VARIABLES
**********************************************************************/

/**
 * @brief Every vertex on the cube, each set of 3 is a triangle.
 */ 
GLfloat const vertices_raw[] = {
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,

    0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,

    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,

   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
   
   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,

    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,

    0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,

    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,
};

/**
 * @brief Texture coordinates for each vertex.
 */
GLfloat const uvs_raw[] = {
    1.0f,  0.0f,
    1.0f,  1.0f,
    0.0f,  0.0f,

    1.0f,  1.0f,
    0.0f,  1.0f,
    0.0f,  0.0f,

    1.0f,  0.0f,
    1.0f,  1.0f,
    0.0f,  0.0f,

    1.0f,  1.0f,
    0.0f,  1.0f,
    0.0f,  0.0f,

    1.0f,  0.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,

    1.0f,  1.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,

    0.0f,  1.0f,
    0.0f,  0.0f,
    0.0f,  0.0f,

    0.0f,  1.0f,
    0.0f,  0.0f,
    0.0f,  1.0f,

    1.0f,  0.0f,
    0.0f,  0.0f,
    1.0f,  0.0f,

    1.0f,  0.0f,
    0.0f,  0.0f,
    0.0f,  0.0f,

    1.0f,  1.0f,
    0.0f,  1.0f,
    0.0f,  1.0f,

    1.0f,  1.0f,
    1.0f,  1.0f,
    0.0f,  1.0f,
};

GLuint const vertex_count = sizeof( vertices_raw ) / sizeof( vec3_type );