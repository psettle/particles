/**
 * @file matrix_math.h
 *
 * @brief Ansi-c openGL math interface
 */
#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

/**********************************************************************
							GENERAL INCLUDES
**********************************************************************/

#include "opengl_includes.h"

/**********************************************************************
							LITERAL CONSTANTS
**********************************************************************/

/*
 * Used to simplify 0 set:
 * e.g. mat4_set( &mat4, MAT4_NULL );
 */
#define VEC3_NULL 0.0, 0.0, 0.0
#define VEC4_NULL VEC3_NULL, 0.0
#define MAT4_NULL VEC4_NULL, VEC4_NULL, VEC4_NULL, VEC4_NULL


/**********************************************************************
								TYPES
**********************************************************************/

typedef struct vec3_struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} vec3_type;

typedef struct vec4_struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4_type;

/* Note: Columns then Rows */
typedef struct mat4_struct
{
    vec4_type x;
    vec4_type y;
    vec4_type z;
    vec4_type w;
} mat4_type;

/**********************************************************************
								MACROS
**********************************************************************/

/**********************************************************************
							  PROTOTYPES
**********************************************************************/

/**
 * @brief Set a vec3
 */
void vec3_set
    (
        vec3_type * vec3,
        GLfloat     x,
        GLfloat     y,
        GLfloat     z
    );

/**
 * @brief Set a vec4
 */
void vec4_set
    (
        vec4_type * vec4,
        GLfloat     x,
        GLfloat     y,
        GLfloat     z,
        GLfloat     w
    );

/**
 * @brief Set a mat4
 */
void mat4_set
    (
        mat4_type * mat4,
        GLfloat x_x, GLfloat y_x, GLfloat z_x, GLfloat w_x,
        GLfloat x_y, GLfloat y_y, GLfloat z_y, GLfloat w_y,
        GLfloat x_z, GLfloat y_z, GLfloat z_z, GLfloat w_z,
        GLfloat x_w, GLfloat y_w, GLfloat z_w, GLfloat w_w
    );

#endif /* MATRIX_MATH_H */
