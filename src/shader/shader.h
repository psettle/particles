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
        sint8_t const   * vertex_shader_filename,
        sint8_t const   * fragment_shader_filename
    );

/**
 * @brief Set a shader as active for rendering
 */
void shader_use
    (
        shader_type     * p_shader
    );

/**
 * @brief Clear the resources associated with a shader
 */
void shader_free
    (
        shader_type     * p_shader
    );

#endif /* SHADER_H */
