/**
 * @file object_group_core.c
 *
 * @brief Implementation of the core object group driver
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "object_group_core.h"
#include "object.h"
#include "texture.h"
#include "camera.h"
#include "common_util.h"
#include <stdlib.h>

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

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
static camera_type*             active_camera;

/**********************************************************************
                             PROTOTYPES
**********************************************************************/
/**
 * @brief Perform openGL specific init
 *
 * @return TRUE on success, FALSE on failure.
 */
static boolean openGL_init
    (
        object_group_type * object_group,
        object_group_create_argument_type const * params
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
 * @brief Object group system event callback
 */
static void object_group_system_cb
    (
        object_group_type       * object_group,
        system_event_type const * event_data
    );

/**********************************************************************
                             FUNCIONS
**********************************************************************/

void uv_set
    (
        uv_type   * uv,
        GLfloat     u,
        GLfloat     v
    )
{
    uv->u = u;
    uv->v = v;
}

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
        object_group_create_argument_type const * params
    )
{
    object_group_type * object_group;
    boolean             status;

    object_group = calloc( 1, sizeof( object_group_type ) );

    object_group->vertex_count          = params->vertex_count;
    object_group->object_cb             = params->object_cb;
    object_group->camera                = active_camera;
    object_group->model_uniform_name    = params->model_uniform_name;

    /* Init the array of object positions */
    object_group->objects = vector_init( sizeof( object_type* ) );

    /* Init the array of buffers to delete */
    object_group->buffers_to_delete = vector_init( sizeof( GLuint ) );

    /* Add the object to the list of active groups */
    vector_push_back( active_object_groups.active_object_groups, &object_group );

    /* Copy the shader to use */
    object_group->shader = params->shader;

    /* Copy texture to use */
    object_group->texture = params->texture;

    /* Do openGL specific init */
    status = openGL_init
        (
            object_group,
            params
        );

    if( !status )
    {
        return NULL;
    }

    return object_group;
}

void object_group_delete
    (
        object_group_type * object_group
    )
{
    uint32_t i;
    uint32_t len;

    /* Free openGL resources */
    glDeleteVertexArrays( 1, &object_group->vertex_array_object );
    glDeleteBuffers( vector_size( object_group->buffers_to_delete ), vector_access( object_group->buffers_to_delete, 0, GLuint ) );

    len = vector_size( object_group->objects );
    for( i = 0; i < len; ++i )
    {
        object_delete( object_group, *vector_access( object_group->objects, i, object_type* ) );
    }

    /* Free system resources */
    vector_remove( active_object_groups.active_object_groups, &object_group );
    vector_deinit( object_group->objects );
    vector_deinit( object_group->buffers_to_delete );
    texture_free( object_group->texture );
    shader_free( object_group->shader );
    free( object_group );
}

void object_group_deinit
    (
        void
    )
{
    uint32_t i;
    uint32_t len;

    len = vector_size( active_object_groups.active_object_groups );
    for( i = 0; i < len; ++i )
    {
        object_group_delete( *vector_access( active_object_groups.active_object_groups, i, object_group_type* ) );
    }

    vector_deinit( active_object_groups.active_object_groups );
}

static boolean openGL_init
    (
        object_group_type * object_group,
        object_group_create_argument_type const * params
    )
{
    GLuint              vertex_buffer_object;
    GLuint              uv_buffer_object;
    GLuint              normal_buffer_object;

    /* Allocate data buffers. */ 
    glGenVertexArrays( 1, &object_group->vertex_array_object );
    glGenBuffers( 1, &vertex_buffer_object );
    glGenBuffers( 1, &uv_buffer_object );
    glGenBuffers( 1, &normal_buffer_object );

    /* Push data buffers onto the group, so they can be cleared later. */
    vector_push_back( object_group->buffers_to_delete, &vertex_buffer_object );
    vector_push_back( object_group->buffers_to_delete, &uv_buffer_object );
    vector_push_back( object_group->buffers_to_delete, &normal_buffer_object );

    /* Set the vertex array active. */
    glBindVertexArray( object_group->vertex_array_object );

    /* Assign channel data. */

    if( NULL != params->vertices )
    {
        glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer_object );
        glBufferData( GL_ARRAY_BUFFER, params->vertex_count * sizeof( vec3_type ), params->vertices, GL_STATIC_DRAW );
        glVertexAttribPointer( params->vertex_channel, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( params->vertex_channel );
    }
    else
    {
        return FALSE;
    }

    if( params->normals )
    {
        glBindBuffer( GL_ARRAY_BUFFER, normal_buffer_object );
        glBufferData( GL_ARRAY_BUFFER, params->vertex_count * sizeof( vec3_type ), params->normals, GL_STATIC_DRAW );
        glVertexAttribPointer( params->normal_channel, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( params->normal_channel );
    }

    if( params->uvs )
    {
        glBindBuffer( GL_ARRAY_BUFFER, uv_buffer_object );
        glBufferData( GL_ARRAY_BUFFER, params->vertex_count * sizeof( uv_type ), params->uvs, GL_STATIC_DRAW );
        glVertexAttribPointer( params->uv_channel, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( params->uv_channel );
    }

    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    return TRUE;
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
        object_group = *vector_access( active_object_groups.active_object_groups, i, object_group_type* );
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

    if( SYSTEM_EVENT_NEW_CAMERA == event_data->event_type )
    {
        active_camera = event_data->event_data.new_camera_data;
    }

    len = vector_size( active_object_groups.active_object_groups );

    for( i = 0; i < len; ++i )
    {
        object_group = *vector_access( active_object_groups.active_object_groups, i, object_group_type* );
        object_group_system_cb( object_group, event_data );
    }
}

static void object_group_frame_cb
    (
        object_group_type       * object_group,
        frame_event_type  const * event_data
    )
{
    uint16_t i;
    uint16_t len;
    object_event_type object_event;
    
    shader_use( object_group->shader );

    if ( NULL != object_group->texture )
    {
        texture_use( object_group->texture );
    }
    
    camera_set_active( object_group->camera, object_group->shader );
    
    len = vector_size( object_group->objects );

    glBindVertexArray( object_group->vertex_array_object );

    if( NULL != object_group->object_cb )
    {
        object_event.event_type = OBJECT_EVENT_TYPE_RENDER_START;
        object_event.event_data.render_start_data.shader = object_group->shader;
        object_group->object_cb( &object_event );

        object_event.event_type = OBJECT_EVENT_TYPE_RENDER_OBJECT;
        object_event.event_data.render_object_data.time_since_last_frame = event_data->timesince_last_frame;
    }

    for( i = 0; i < len; ++i )
    {
        object_type* object;

        object = *vector_access( object_group->objects, i, object_type* );

        if( NULL != object_group->object_cb )
        {
            object_event.event_data.render_object_data.object = object;
            object_group->object_cb( &object_event );
        }

        if( object->is_visible )
        {
            if( object_group->model_uniform_name )
            {
                shader_set_uniform_mat4( object->shader, object_group->model_uniform_name, &object->model_matrix );
            }
            glDrawArrays( GL_TRIANGLES, 0, object_group->vertex_count );
        }
    }

    glBindVertexArray( 0 );

    texture_clear();
    shader_clear();
}

static void object_group_system_cb
    (
        object_group_type       * object_group,
        system_event_type const * event_data
    )
{
    switch( event_data->event_type )
    {
    case SYSTEM_EVENT_NEW_CAMERA:
        object_group->camera = event_data->event_data.new_camera_data;
        break;
    default:
        break;
    }
}