/**
 * @file shader.h
 *
 * @brief Interface for creating and using open gl shaders
 */
#ifndef SHADER_H
#define SHADER_H

/**********************************************************************
                                INCLUDES
**********************************************************************/

#include "common_types.h"
#include "opengl_includes.h"
#include "matrix_math.h"

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

#define SHADER_PRINT_ERROR_LOG  TRUE

/**********************************************************************
                                 TYPES
**********************************************************************/

/* Data to describe one shader */
typedef struct shader_struct
{
    GLuint program_id;
} shader_type;

/**********************************************************************
                                PROTOTYPES
**********************************************************************/

/**
 * @brief Build a shader from a vertex and shader .glsl file
 *
 * @return
 *      TRUE on success
 *      FALSE if an error occurs
 */
boolean shader_build
    (
        shader_type     * p_shader,
        sint8_t const   * vertex_shader_code,
        sint8_t const   * fragment_shader_code
    );

/**
 * @brief Set a shader as active for rendering
 */
void shader_use
    (
        shader_type     * p_shader
    );

/**
 * @brief Clear the active shader
 */
void shader_clear
    (
        void
    );

/**
 * @brief Clear the resources associated with a shader
 */
void shader_free
    (
        shader_type     * p_shader
    );

/**
 * @brief Set the value of an mat4 uniform
 */
void shader_set_uniform_mat4
    (
        shader_type const * p_shader,
        sint8_t     const * uniform_name,
        mat4_type   const * mat4
    );

/**
 * @brief Set the value of an int uniform
 */
void shader_set_uniform_uint32
    (
        shader_type const * p_shader,
        sint8_t     const * uniform_name,
        uint32_t            uint32
    );

#endif /* SHADER_H */
