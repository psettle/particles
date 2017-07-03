/**
 * @file object_group_core.h
 *
 * @brief Core interface for rendering a group of objects
 */
#ifndef OBJECT_CORE_H
#define OBJECT_CORE_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "system_types.h"

/**********************************************************************
                              PROTOTYPES
**********************************************************************/

/**
 * @brief Initialize the object group system
 */
void object_group_init
    (
        void
    );

/**
 * @brief Create a new object group
 */
object_group_type * object_group_create
    (
        sint8_t     const * vertex_shader_filename,
        sint8_t     const * fragment_shader_filename,
        vector_type       * vertices /* Array of vec3_type representing the position of each vertex  */
    );

/**
 * @brief Delete a particular object group
 */
void object_group_delete
    (
        object_group_type * object_group
    );

/**
 * @brief deinit the object group system
 */
void object_group_deinit
    (
        void
    );

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
