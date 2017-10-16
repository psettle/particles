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
 * @brief Set the value of a vertex triangle
 */
void uv_set
    (
        uv_type   * uv,
        GLfloat     u,
        GLfloat     v
    );

/**
 * @brief Set the value of a vertex triangle
 */
void vertex_triangle_set
    (
        vertex_triangle_type    * vertex_triangle,
        uint32_t                  a,
        uint32_t                  b,
        uint32_t                  c
    );

/**
 * @brief Initialize the object group system
 */
void object_group_init
    (
        void
    );

/**
 * @brief Create a new object group
 *
 * @param params
 *      Information about the object group to create.
 *
 * @param[out] Status of creating the object group
 *
 * @return A pointer to the object group container.
 */
object_group_type * object_group_create
    (
    object_group_create_argument_type const * params
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

#endif /* OBJECT_CORE_H */
