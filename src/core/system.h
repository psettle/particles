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
