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
#include "string.h"

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
    object->shader = object_group->shader;

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
    vec3_type zero;

    vec3_set( &zero, VEC3_NULL );

    /* Only shift is the amount of shift is non-zero */
    if( 0 != memcmp( &zero, shift, sizeof( vec3_type ) ) )
    {   
        mat4_translate( &( object->model_matrix ), shift );
        vec3_add( &( object->position ), &( object->position ), shift );
    }
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
    object_translate( object, position );
    
}