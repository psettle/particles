/**
 * @file matrix_math.h
 *
 * @brief Ansi-c openGL math implementation
 */

/**********************************************************************
							GENERAL INCLUDES
**********************************************************************/

#include "matrix_math.h"

/**********************************************************************
							  PROTOTYPES
**********************************************************************/

/**********************************************************************
							  FUNCTIONS
**********************************************************************/

void vec3_set
    (
        vec3_type * vec3,
        GLfloat     x,
        GLfloat     y,
        GLfloat     z
    )
{
    vec3->x = x;
    vec3->y = y;
    vec3->z = z;
}

void vec4_set
    (
        vec4_type * vec4,
        GLfloat     x,
        GLfloat     y,
        GLfloat     z,
        GLfloat     w
    )
{
    vec4->x = x;
    vec4->y = y;
    vec4->z = z;
    vec4->w = w;
}

void mat4_set
    (
        mat4_type * mat4,
        GLfloat x_x, GLfloat y_x, GLfloat z_x, GLfloat w_x,
        GLfloat x_y, GLfloat y_y, GLfloat z_y, GLfloat w_y,
        GLfloat x_z, GLfloat y_z, GLfloat z_z, GLfloat w_z,
        GLfloat x_w, GLfloat y_w, GLfloat z_w, GLfloat w_w
    )
{
    vec4_set( &mat4->x, x_x, x_y, x_z, x_w );
    vec4_set( &mat4->y, y_x, y_y, y_z, y_w );
    vec4_set( &mat4->z, z_x, z_y, z_z, z_w );
    vec4_set( &mat4->w, w_x, w_y, w_z, w_w );
}