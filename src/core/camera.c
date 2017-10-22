/**
 * @file camera.c
 *
 * @brief Core camera driver for the openGL layer
 */
/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "camera.h"
#include "system.h"

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

#define VIEW_PROJECTION_UNIFORM "projection_view_matrix"

/**********************************************************************
                               FUNCTIONS
**********************************************************************/

void camera_init
    (
        camera_type * camera
    )
{
    system_set_camera( camera );
}

void camera_set_active
    (
        camera_type const * camera,
        shader_type const * shader
    )
{
    shader_set_uniform_mat4( shader, VIEW_PROJECTION_UNIFORM, &( camera->projection_view_matrix ) );
}

void camera_set_view
    (
        camera_type     * camera,
        vec3_type const * from,
        vec3_type const * to,
        vec3_type const * up
    )
{
    mat4_look_at( &( camera->view_matrix ), from, to, up );

    /* Recalculate the position and projection view matrix */
    mat4_multiply( &( camera->projection_view_matrix ), &( camera->projection_matrix ), &( camera->view_matrix ) );
    camera->position = *from;
}

void camera_set_perspective
    (
        camera_type     * camera,
        GLfloat           field_of_view,  /* FOV in radians */
        uint32_t          width,          /* Perspective width in pixels */
        uint32_t          height,         /* Perspective height in pixels */
        GLfloat           front,          /* Distance from the front of the camera to the first thing that can be rendered (0.1 is normal) */
        GLfloat           back            /* Distance from the front of the camera to the last thing that can be rendered  (10.0 is normal) */
    )
{
    GLfloat aspect_ratio = ( GLfloat )((double_t)width / (double_t)height);
    mat4_perspective( &camera->projection_matrix, field_of_view, aspect_ratio, front, back );
     /* Recalculate the projection view matrix */
    mat4_multiply( &camera->projection_view_matrix, &camera->projection_matrix, &camera->view_matrix);
}

void camera_get_position
    (
        camera_type const * camera,
        vec3_type         * position
    )
{
    *position = camera->position;
}