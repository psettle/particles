/**
 * @file texture.h
 *
 * @brief Interface for creating and using open gl textures
 */
#ifndef TEXTURE_H
#define TEXTURE_H

/**********************************************************************
                                INCLUDES
**********************************************************************/

#include "common_types.h"
#include "opengl_includes.h"
#include "shader.h"

/**********************************************************************
                                 TYPES
**********************************************************************/

/* Data to describe one texture */
typedef struct texture_struct
{
    GLuint          texture_id;
    GLuint          slot;
    shader_type   * shader;
    sint8_t const * uniform_name;
} texture_type;

/**********************************************************************
                                PROTOTYPES
**********************************************************************/

/**
 * @brief Initializes a new texture object
 */
boolean texture_init
    (
        texture_type  * texture,
        sint8_t const * image_filename,
        GLuint          slot,
        shader_type   * shader,
        sint8_t const * uniform_name
    );

/**
 * @brief Sets a texture as active
 */
void texture_use
    (
        texture_type  * texture
    );

/**
 * @brief Clears the active texture
 */
void texture_clear
    (
        void
    );

/**
 * @brief Deletes the resources associated with a texture
 */
void texture_free
    (
        texture_type  * texture
    );

#endif /* TEXTURE_H */
