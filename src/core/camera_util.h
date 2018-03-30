/**
 * @file camera_util.h
 *
 * @brief Camera utilities for debugging.
 */
#ifndef CAMERA_UTIL_H
#define CAMERA_UTIL_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "system.h"
#include "camera.h"
#include "common_util.h"

/**********************************************************************
                              PROTOTYPES
**********************************************************************/

void camera_util_init
    (
    void
    );

void camera_util_enable_mouse_keyboard_control
    (
    camera_type * camera,
    GLfloat fov,
    GLfloat front,
    GLfloat back
    );

void camera_util_disable_mouse_keyboard_control
    (
    void
    );

#endif /* CAMERA_UTIL_H */
