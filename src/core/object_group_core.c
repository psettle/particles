/**
 * @file object_group_core.c
 *
 * @brief Implementation of the core object group driver
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "object_group_core.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/**********************************************************************
                               TYPES
**********************************************************************/

typedef struct active_object_group_struct
{
    vector_type     * active_object_groups; /* List of object_group_type* for all active objects */
} active_object_group_type;

/**********************************************************************
                             VARIABLES
**********************************************************************/

static active_object_group_type active_object_groups;

/**********************************************************************
                             PROTOTYPES
**********************************************************************/
/**
 * @brief Perform openGL specific init
 */
static void openGL_init
    (
        object_group_type * object_group,
        vector_type       * vertices /* Array of vec3_type representing the position of each vertex  */
    );

/**
 * @brief Object group frame event callback
 */
static void object_group_global_frame_cb
    (
        frame_event_type const * event_data
    );

/**
 *@brief Object group system event callback
 */
static void object_group_global_system_cb
    (
        system_event_type const * event_data
    );

/**
 * @brief Object group frame event callback
 */
static void object_group_frame_cb
    (
        object_group_type       * object_group,
        frame_event_type  const * event_data
    );

/**
 *@brief Object group system event callback
 */
static void object_group_system_cb
    (
        object_group_type       * object_group,
        system_event_type const * event_data
    );

/**********************************************************************
                             FUNCIONS
**********************************************************************/

void object_group_init
    (
        void
    )
{
    system_listener_callbacks_type callbacks;

    active_object_groups.active_object_groups = vector_init( sizeof ( object_group_type * ) );

    callbacks.frame_event_cb    = object_group_global_frame_cb;
    callbacks.system_event_cb   = object_group_global_system_cb;

    register_system_listeners( &callbacks );
}

object_group_type * object_group_create
    (
        sint8_t     const * vertex_shader_filename,
        sint8_t     const * fragment_shader_filename,
        vector_type       * vertices /* Array of vec3_type representing the position of each vertex  */
    )
{
    object_group_type * object_group;

    object_group = calloc( 1, sizeof( object_group_type ) );

    /* Init the array of object positions */
    object_group->positions = vector_init( sizeof( vec3_type ) );

    /* Add the object to the list of active groups */
    vector_push_back( active_object_groups.active_object_groups, &object_group );

    /* Build the shader to use */
    shader_build
        (
            &object_group->shader,
            vertex_shader_filename,
            fragment_shader_filename
        );

    object_group->vertex_count = vector_size( vertices );

    /* Do openGL specific init */
    openGL_init
        (
            object_group,
            vertices
        );

    return object_group;
}

void object_group_delete
    (
        object_group_type * object_group
    )
{
    glDeleteVertexArrays( 1, &object_group->vertex_array_object );
    vector_deinit( object_group->positions );
    shader_free( &object_group->shader );
    free( object_group );
}

void object_group_deinit
    (
        void
    )
{
    vector_deinit( active_object_groups.active_object_groups );
}

static void openGL_init
    (
        object_group_type * object_group,
        vector_type       * vertices /* Array of vec3_type representing the position of each vertex  */
    )
{
    GLuint              vertex_buffer_object;

    /* Assign the vertexes to the object group */
    glGenVertexArrays( 1, &object_group->vertex_array_object );
    glGenBuffers( 1, &vertex_buffer_object );

    printf("Array O: %d\n", object_group->vertex_array_object);

    glBindVertexArray( object_group->vertex_array_object );

    glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer_object );

    /* Load the data into the buffer, and tell openGL what type of data it was */
    glBufferData
    (
        GL_ARRAY_BUFFER,
        object_group->vertex_count * sizeof( vec3_type ),
        vector_access( vertices, 0 ),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer
    (
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        NULL
    );

    glEnableVertexAttribArray( 0 );

    glBindVertexArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    /* glDeleteBuffers( 1, &vertex_buffer_object ); */
}

static void object_group_global_frame_cb
    (
        frame_event_type const * event_data
    )
{
    /* Distribute the event onto each active object */

    uint32_t              i;
    uint32_t              len;
    object_group_type   * object_group;

    len = vector_size( active_object_groups.active_object_groups );

    for( i = 0; i < len; ++i )
    {
        object_group = *(object_group_type **)vector_access( active_object_groups.active_object_groups, i );
        object_group_frame_cb( object_group, event_data );
    }
}

static void object_group_global_system_cb
    (
        system_event_type const * event_data
    )
{
    /* Distribute the event onto each active object */

    uint32_t              i;
    uint32_t              len;
    object_group_type   * object_group;

    len = vector_size( active_object_groups.active_object_groups );

    for( i = 0; i < len; ++i )
    {
        object_group = *(object_group_type **)vector_access( active_object_groups.active_object_groups, i );
        object_group_system_cb( object_group, event_data );
    }
}

static void object_group_frame_cb
    (
        object_group_type       * object_group,
        frame_event_type  const * event_data
    )
{
    shader_use( &object_group->shader );

    glBindVertexArray( object_group->vertex_array_object );

    glDrawArrays( GL_TRIANGLES, 0, object_group->vertex_count );

    glBindVertexArray( 0 );

    shader_clear();
}

static void object_group_system_cb
    (
        object_group_type       * object_group,
        system_event_type const * event_data
    )
{
}
