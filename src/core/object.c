/**
 * @file object.c
 *
 * @brief Implementation of the object interface
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "object.h"

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

/**********************************************************************
                               TYPES
**********************************************************************/

/**********************************************************************
                             VARIABLES
**********************************************************************/

/**********************************************************************
                             PROTOTYPES
**********************************************************************/

/**********************************************************************
                             FUNCTIONS
**********************************************************************/

object_type * object_create
    (
        object_group_type * object_group
    )
{
    object_type * object;
    object = calloc( 1, sizeof( object_type ) );

    object->object_id = object_group->next_id++;
    vec3_set( &object->position, VEC3_NULL );
    mat4_set( &object->model_matrix, MAT4_IDENTITY );
    object->bones = vector_init( sizeof( bone_type ) );

    vector_push_back( object_group->objects, &object );

    return object;
}

void object_set_visibility
    (
        object_type * object,
        boolean       visible
    )
{
    object->is_visible = visible;
}

void object_delete
    (
        object_group_type   * object_group,
        object_type         * object
    )
{
    vector_deinit( object->bones );
    vector_remove( object_group->objects, &object );
    free( object );
}