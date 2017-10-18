#pragma once
/**
 * @file system.h
 *
 * @brief Core system driver for the openGL layer
 */
#ifndef SYSTEM_H
#define SYSTEM_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "system_types.h"

/**********************************************************************
                              LITERAL CONSTANTS
**********************************************************************/

#define PRINT_FRAMERATE     ( TRUE )

/**********************************************************************
                              PROTOTYPES
**********************************************************************/

/**
 * @brief Initialize openGL and the system
 */
void register_system_listeners
    (
        system_listener_callbacks_type  * callbacks
    );

/**
 * @brief Sets the active camera for the system
 */
void system_set_camera
    (
        camera_type const * camera
    );

/**
 * @brief Initialize openGL and the system
 *
 * @return TRUE on success, FALSE on failure
 */
boolean system_init
    (
        void
    );

/**
 * @brief Run the application to completion
 */
void system_run
    (
        void
    );

#endif /* SYSTEM_H */
