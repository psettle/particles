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

#define MAT4_IDENTITY 1.0f, 0.0f, 0.0f, 0.0f,\
                      0.0f, 1.0f, 0.0f, 0.0f,\
                      0.0f, 0.0f, 1.0f, 0.0f,\
                      0.0f, 0.0f, 0.0f, 1.0f

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
 * @brief Get the length of a vector
 */
GLfloat vec3_length
    (
        vec3_type * vec3
    );

/**
 * @brief out = left X right
 */
void vec3_cross
    (
        vec3_type       * out,
        vec3_type const * left,
        vec3_type const * right
    );

/**
 * @brief returns left . right
 */
GLfloat vec3_dot
    (
        vec3_type const * left,
        vec3_type const * right
    );

/**
 * @brief normalize a vec3
 */
void vec3_normalize
    (
        vec3_type * vec3
    );

/**
 * @brief out = left - right
 */
void vec3_subtract
    (
        vec3_type * out,
        vec3_type const * left,
        vec3_type const * right
    );

/**
 * @brief out = left + right
 */
void vec3_add
    (
        vec3_type * out,
        vec3_type const * left,
        vec3_type const * right
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
 * @brief returns left . right
 */
GLfloat vec4_dot
    (
        vec4_type const * left,
        vec4_type const * right
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

/**
 * @brief product = left * right
 */
void mat4_multiply
    (
        mat4_type * product,
        mat4_type * left,
        mat4_type * right
    );

/**
 * @brief Sets view to a matrix tranform that looks from to
 */
void mat4_look_at
    (
        mat4_type       * view,
        vec3_type const * from,
        vec3_type const * to,
        vec3_type const * up
    );

/**
 * @brief Sets the perspective matrix to use the provided field of view, aspect ratio, front and back
 */
void mat4_perspective
    (
        mat4_type       * perspective,
        GLfloat           field_of_view,  /* FOV in radians */
        GLfloat           aspect_ratio,    /* The ration of width/height for the persective */
        GLfloat           front,          /* Distance from the front of the camera to the first thing that can be rendered (0.1 is normal) */
        GLfloat           back            /* Distance from the front of the camera to the last thing that can be rendered  (10.0 is normal) */
    );

void mat4_rotate
    (
        mat4_type       * to_rotate,
        vec3_type const * rotation_axis,
        GLfloat           angle_rad
    );

void mat4_translate
    (
        mat4_type       * to_translate,
        vec3_type       * amount
    );

#endif /* MATRIX_MATH_H */
