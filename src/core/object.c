/**
 * @file object.c
 *
 * @brief Implementation of the object interface
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "object.h"
#include "stdlib.h"

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

void object_rotate
    (
        object_type         * object,
        vec3_type const     * axis,
        GLdouble              angle /* rad */
    )
{
    vec3_type origin;
    vec3_type old_position;

    vec3_cpy( &old_position, &( object->position ) );
    vec3_set( &origin, VEC3_NULL );

    /* Move to origin */
    object_set_position( object, &origin );

    /* Apply rotation */
    mat4_rotate( &( object->model_matrix ), axis, angle );

    /* Move back to position */
    object_set_position( object, &old_position );
}

void object_translate
    (
        object_type         * object,
        vec3_type const     * shift
    )
{
    mat4_translate( &( object->model_matrix ), shift );
    vec3_add( &( object->position ), &( object->position ), shift );
}

void object_set_position
    (
    object_type         * object,
    vec3_type const     * position
    )
{
    vec3_type neg_position;

    /* Translate to origin */
    vec3_scale( &neg_position, -1.0f, &( object->position ) );
    object_translate( object, &neg_position ); 

    /* Translate to new position, if the new position isn't origin. */
    if( ( 0 != position->x ) ||
        ( 0 != position->y ) ||
        ( 0 != position->z ) )
    {
        object_translate( object, position );
    }
    
}