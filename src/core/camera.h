/**
 * @file camera.h
 *
 * @brief Core camera driver for the openGL layer
 */
#ifndef CAMERA_H
#define CAMERA_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "system_types.h"
#include <math.h>

/**********************************************************************
                                CONSTANTS
**********************************************************************/

#define DEFAULT_UP ( &( vec3_type ){ 0.0f, 1.0f, 0.0f } );

#define DEFAULT_FOV   ( M_PI * ( 100.0f / 180.0f ) )
#define DEFAULT_FRONT ( 0.001f )
#define DEFAULT_BACK  ( 10.0f )


/**********************************************************************
                              PROTOTYPES
**********************************************************************/

/**
 * @brief Initializes a camera
 *
 * @note: Camera will be largely useless until camera_set_view
 *        and camera_set_perspective are called
 */
void camera_init
    (
        camera_type * camera
    );

/**
 * @brief Sets a camera as the one to use on a particular shader
 */
void camera_set_active
    (
        camera_type const * camera,
        shader_type const * shader
    );

/**
 * @brief Sets the view direction for a camera (what to see)
 */
void camera_set_view
    (
        camera_type     * camera,
        vec3_type const * from,
        vec3_type const * to,
        vec3_type const * up
    );

/**
 * @brief Sets the perspective for a camera (how to see)
 */
void camera_set_perspective
    (
        camera_type     * camera,
        GLfloat           field_of_view,  /* FOV in radians */
        uint32_t          width,          /* Perspective width in pixels */
        uint32_t          height,         /* Perspective height in pixels */
        GLfloat           front,          /* Distance from the front of the camera to the first thing that can be rendered (0.1 is normal) */
        GLfloat           back            /* Distance from the front of the camera to the last thing that can be rendered  (10.0 is normal) */
    );

/**
 * @brief Gets the current position of a camera
 *       ( Set using camera_set_view )
 */
void camera_get_position
    (
        camera_type const * camera,
        vec3_type         * position
    );

#endif /* SYSTEM_H */
