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
        sint8_t const * shader_code,
        shader_type_t   shader_type,
        GLuint*         shader_handle   /* [out] Handle to the shader */
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
        GLuint  vertex_shader_handle,
        GLuint  fragment_shader_handle,
        GLuint* program_handle
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

shader_type* shader_build
    (
        sint8_t const * vertex_shader_code,
        sint8_t const * fragment_shader_code
    )
{
    GLuint       vertex_shader_handle;
    GLuint       fragment_shader_handle;
    boolean      status;
    shader_type* shader;

    shader = calloc( 1, sizeof( shader_type ) );

    /* Compile vertex shader */
    status = shader_compile( vertex_shader_code, SHADER_TYPE_VERTEX, &vertex_shader_handle );

    if( !status )
    {
        return NULL;
    }

    /* Compile fragment shader */
    status = shader_compile( fragment_shader_code, SHADER_TYPE_FRAGMENT, &fragment_shader_handle );

    if( !status )
    {
        return NULL;
    }

    /* Link the program */
    status = shader_link( vertex_shader_handle, fragment_shader_handle, &shader->program_id );

    if( !status )
    {
        return NULL;
    }

    return shader;
}

void shader_use
    (
        shader_type* shader
    )
{
    glUseProgram( shader->program_id );
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
        shader_type* shader
    )
{
    glDeleteProgram( shader->program_id );
    free( shader );
}

void shader_set_uniform_mat4
    (
        shader_type const * shader,
        sint8_t     const * uniform_name,
        mat4_type   const * mat4
    )
{
    GLuint uniform_location;

    uniform_location = glGetUniformLocation( shader->program_id, uniform_name );
    glUniformMatrix4fv( uniform_location, 1, GL_FALSE, &mat4->x.x );
}

void shader_set_uniform_uint32
    (
        shader_type const * shader,
        sint8_t     const * uniform_name,
        uint32_t            uint32
    )
{
    GLuint uniform_location;

    uniform_location = glGetUniformLocation( shader->program_id, uniform_name );
    glUniform1i( uniform_location, uint32 );
}

static boolean shader_compile
    (
        sint8_t const * shader_code,
        shader_type_t   shader_type,
        GLuint        * shader_handle /* [out] Handle to the shader */
    )
{
    GLchar const * const *      gl_file_contents;
    GLint                       compile_status;

    /* Prepare shader arguments */
    *shader_handle      = glCreateShader( shader_type );
    gl_file_contents    = ( GLchar const * const * )&shader_code;

    /* Attach the source of the shader */
    glShaderSource( *shader_handle, 1, gl_file_contents, NULL );

    /* Compile the shader */
    glCompileShader( *shader_handle );

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