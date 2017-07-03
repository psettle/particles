/**
 * @file shader.c
 *
 * @brief Implementation of interface for creating and using open gl shaders
 */

/**********************************************************************
                                INCLUDES
**********************************************************************/

#include "shader.h"
#include "file_api.h"
#include "opengl_includes.h"
#include "common_util.h"

#include <stdlib.h>
#include <stdio.h>

/**********************************************************************
                                  TYPES
**********************************************************************/

typedef GLenum shader_type_t; enum
{
    SHADER_TYPE_FRAGMENT    = GL_FRAGMENT_SHADER,
    SHADER_TYPE_VERTEX      = GL_VERTEX_SHADER
};

/**********************************************************************
                                PROTOTYPES
**********************************************************************/

/**
 * @brief Compiles one .glsl shader
 *
 * @return
 *      TRUE if successful
 *      FALSE on error
 *          (file does not exist or compile error)
 */
static boolean shader_compile
    (
        sint8_t const * shader_filename,
        shader_type_t   shader_type,
        GLuint        * shader_handle   /* [out] Handle to the shader */
    );

/**
 * @brief Links a fragment and vertex shader
 *
 * @return
 *      TRUE if successful
 *      FALSE on error
 *          (file does not exist or compile error)
 */
static boolean shader_link
    (
        GLuint          vertex_shader_handle,
        GLuint          fragment_shader_handle,
        GLuint        * program_handle
    );

/**
 * @brief Retrieves and prints the shader compile log
 */
static void print_shader_error_log
    (
        GLuint shader_handle
    );

/**
 * @brief Retrieves and prints the program compile log
 */
static void print_program_error_log
    (
        GLuint program_handle
    );

/**********************************************************************
                                FUNCTIONS
**********************************************************************/

boolean shader_build
    (
        shader_type     * p_shader,
        sint8_t const   * vertex_shader_filename,
        sint8_t const   * fragment_shader_filename
    )
{
    GLuint      vertex_shader_handle;
    GLuint      fragment_shader_handle;
    boolean     status;

    /* Compile vertex shader */
    status = shader_compile( vertex_shader_filename, SHADER_TYPE_VERTEX, &vertex_shader_handle );

    CHECK_STATUS( status );

    /* Compile fragment shader */
    status = shader_compile( fragment_shader_filename, SHADER_TYPE_FRAGMENT, &fragment_shader_handle );

    CHECK_STATUS( status );

    /* Link the program */
    status = shader_link( vertex_shader_handle, fragment_shader_handle, &p_shader->program_id );

    CHECK_STATUS( status );

    return TRUE;
}

void shader_use
    (
        shader_type     * p_shader
    )
{
    glUseProgram( p_shader->program_id );
}


void shader_clear
    (
        void
    )
{
    glUseProgram( 0 );
}

void shader_free
    (
        shader_type     * p_shader
    )
{
    glDeleteProgram( p_shader->program_id );
}

static boolean shader_compile
    (
        sint8_t const * shader_filename,
        shader_type_t   shader_type,
        GLuint        * shader_handle /* [out] Handle to the shader */
    )
{
    file_contents_type          file_contents;
    boolean                     status;
    GLchar const * const *      gl_file_contents;
    GLint *                     gl_file_length;
    GLint                       compile_status;

    /* Read the shader contents */
    status = file_read( shader_filename, &file_contents );

    CHECK_STATUS( status );

    /* Prepare shader arguments */
    *shader_handle      = glCreateShader( shader_type );
    gl_file_contents    = ( GLchar const * const * )&file_contents.p_file_contents;
    gl_file_length      = ( GLint * )&file_contents.length;

    /* Attach the source of the shader */
    glShaderSource( *shader_handle, 1, gl_file_contents, gl_file_length );

    /* Compile the shader */
    glCompileShader( *shader_handle );

    /* Clear the shader contents resource */
    status = file_free( &file_contents );

    CHECK_STATUS( status );

    /* Check the result of the compile */
    glGetShaderiv( *shader_handle, GL_COMPILE_STATUS, &compile_status );

#if ( SHADER_PRINT_ERROR_LOG )
    if( compile_status )
    {
        return TRUE;
    }

    print_shader_error_log( *shader_handle );
    return FALSE;
#elif
    return compile_status;
#endif
}

static boolean shader_link
    (
        GLuint          vertex_shader_handle,
        GLuint          fragment_shader_handle,
        GLuint        * program_handle
    )
{
    GLint                   compile_status;

    /* Make the program */
    *program_handle = glCreateProgram();

    /* Attach the shaders to the program */
    glAttachShader( *program_handle, vertex_shader_handle );
    glAttachShader( *program_handle, fragment_shader_handle );

    /* Link the program */
    glLinkProgram( *program_handle );

    /* Detach then delete the shader resources */
    glDetachShader( *program_handle, vertex_shader_handle );
    glDetachShader( *program_handle, fragment_shader_handle );

    glDeleteShader( vertex_shader_handle );
    glDeleteShader( fragment_shader_handle );

    /* Check the result of the link */
    glGetProgramiv( *program_handle, GL_COMPILE_STATUS, &compile_status );

#if ( SHADER_PRINT_ERROR_LOG )
    if( compile_status )
    {
        return TRUE;
    }

    print_program_error_log( *program_handle );
    return FALSE;
#elif
    return compile_status;
#endif
}

static void print_shader_error_log
    (
        GLuint shader_handle
    )
{
    GLint               error_log_length;
    GLchar            * error_log;

    glGetShaderiv( shader_handle, GL_INFO_LOG_LENGTH, &error_log_length );

    if( error_log_length > 1 )
    {
        error_log = malloc( error_log_length * sizeof( GLchar ) );

        glGetShaderInfoLog( shader_handle, error_log_length, NULL, error_log );
        printf( "%s", error_log );

        free( error_log );
    }
}

/**
 * @brief Retrieves and prints the program compile log
 */
static void print_program_error_log
    (
        GLuint program_handle
    )
{
    GLint               error_log_length;
    GLchar            * error_log;

    glGetProgramiv( program_handle, GL_INFO_LOG_LENGTH, &error_log_length );

    if( error_log_length > 1 )
    {
        error_log = malloc( error_log_length * sizeof( GLchar ) );

        glGetProgramInfoLog( program_handle, error_log_length, NULL, error_log );
        printf( "%s", error_log );

        free( error_log );
    }
}