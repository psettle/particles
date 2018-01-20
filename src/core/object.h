/**
 * @file object.h
 *
 * @brief Interface to a single object
 */
#ifndef OBJECT_H
#define OBJECT_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "system_types.h"

/**********************************************************************
                              PROTOTYPES
**********************************************************************/

/**
 * @brief Create a new instance of an object group.
 *        May be deleted with object_delete, but will automatically
 *        be removed when the object group is deleted or the system shuts down.
 */
object_type * object_create
    (
        object_group_type * object_group
    );

/**
 * @brief Decide if an object should be rendered. Persists indefinetely.
 */
void object_set_visibility
    (
        object_type * object,
        boolean       visible
    );

/**
 * @brief Delete an object, the memory associated with the object will be cleared
 *        and the object will not be rendered.
 */
void object_delete
    (
        object_group_type   * object_group,
        object_type         * object
    );

/**
 * @brief Rotate an instance of an object around an axis
 *        rotation occurs around the model centre of the
 *        instance, not around the universal centre.
 */
void object_rotate
    (
        object_type         * object,
        vec3_type const     * axis,
        GLdouble              angle /* rad */
    );

/**
 * @brief Translate an object by shift relative to
 *        the universal centre.
 */
void object_translate
    (
        object_type         * object,
        vec3_type const     * shift
    );

/**
 * @brief Set the current position of an object relative
 *        to the universal centre.
 */
void object_set_position
    (
        object_type         * object,
        vec3_type const     * position
    );

#endif /* OBJECT_H */
