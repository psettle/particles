/**
 * @file texture.c
 *
 * @brief Interface for creating and using open gl textures
 */
/**********************************************************************
                                INCLUDES
**********************************************************************/

#include "texture.h"
#include "common_util.h"
#include <stdio.h>

/**********************************************************************
                                FUNCTION
**********************************************************************/

boolean texture_init
    (
        texture_type  * texture,
        sint8_t const * image_filename,
        GLuint          slot,
        shader_type   * shader,
        sint8_t const * uniform_name
    )
{
    GLint       width;
    GLint       height;
    uint8_t   * image;

    texture->slot = slot;
	texture->shader = shader;
	texture->uniform_name = uniform_name;

	/* use soil to load the image data */
	image = SOIL_load_image( image_filename, &width, &height, 0, SOIL_LOAD_RGBA);
    if( !image )
    {
        DEBUG_LINE();
        return FALSE;
    }

	/* generate and bind a texture */
	glGenTextures( 1, &texture->texture_id );
	glBindTexture(GL_TEXTURE_2D, texture->texture_id );

	/* tell open gl how to scale and alias the texture */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* bind the image data to the texture */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	/* generate a MIPMAP for the texture */
	glGenerateMipmap(GL_TEXTURE_2D);

	/* safely delete the image data */
	SOIL_free_image_data(image);

	/* clear the bound texture */
    glBindTexture(GL_TEXTURE_2D, 0);

    return TRUE;
}

void texture_use
    (
        texture_type  * texture
    )
{
    glActiveTexture( texture->slot );
	glBindTexture( GL_TEXTURE_2D,  texture->texture_id );
	shader_set_uniform_uint32( texture->shader, texture->uniform_name, texture->slot - GL_TEXTURE0 );
}

void texture_clear
    (
        void
    )
{
    glBindTexture( GL_TEXTURE_2D, 0 );
    glActiveTexture( 0 );
}

void texture_free
    (
        texture_type  * texture
    )
{
    glDeleteTextures( 1, &texture->texture_id );
}