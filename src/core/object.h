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

object_type * object_create
    (
        object_group_type * object_group,
        boolean * status
    );

void object_set_visibility
    (
        object_type * object,
        boolean       visible
    );

void object_delete
    (
        object_group_type   * object_group,
        object_type         * object
    );

#endif /* OBJECT_H */
