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
#include "file_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

#define VERTEX_ATTRIB_POINTER   0
#define UV_ATTRIB_POINTER       1
#define NORMAL_ATTRIB_POINTER   2
#define BONE_ATTRIB_POINTER     3

#define TEXTURE_UNIFORM_NAME    "image"

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
static camera_type*                active_camera;

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

void vertex_triangle_set
    (
        vertex_triangle_type    * vertex_triangle,
        uint32_t                  a,
        uint32_t                  b,
        uint32_t                  c
    )
{
    vertex_triangle->a = a;
    vertex_triangle->b = b;
    vertex_triangle->c = c;
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
    vector_type*        vertex_shader_code;
    vector_type*        fragment_shader_code;

    object_group = calloc( 1, sizeof( object_group_type ) );

    object_group->is_3d                 = !params->is_2d;
    object_group->use_uvs               = params->use_uvs;
    object_group->vertex_count          = params->vertex_count;
    object_group->triangle_count        = params->triangle_count;
    object_group->object_cb             = params->object_cb;
    object_group->camera                = active_camera;

    /* Init the array of object positions */
    object_group->objects = vector_init( sizeof( object_type* ) );

    /* Init the array of buffers to delete */
    object_group->buffers_to_delete = vector_init( sizeof( GLuint ) );

    /* Add the object to the list of active groups */
    vector_push_back( active_object_groups.active_object_groups, &object_group );

    /* Build the shader to use */
    vertex_shader_code = vector_init(sizeof(sint8_t));
    fragment_shader_code = vector_init(sizeof(sint8_t));

    status = file_read( params->vertex_shader_filename, vertex_shader_code );
    if( !status )
    {
        return NULL;
    }
    status = file_read( params->fragment_shader_filename, fragment_shader_code );
    if( !status )
    {
        return NULL;
    }

    status = shader_build( &object_group->shader, vector_access( vertex_shader_code, 0, sint8_t ), vector_access( fragment_shader_code, 0, sint8_t )  );
    if( !status )
    {
        return NULL;
    }

    vector_deinit( vertex_shader_code );
    vector_deinit( fragment_shader_code );

    /* Build texture if required */
    if( params->use_uvs )
    {
        status = texture_init
        (
            &object_group->texture,
            params->texture_filename,
            params->texture_slot,
            &object_group->shader,
            TEXTURE_UNIFORM_NAME
        );
        if( !status )
        {
            return NULL;
        }
    }

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
    texture_free( &object_group->texture );
    shader_free( &object_group->shader );
    free( object_group );
}

void object_group_deinit
    (
        void
    )
{
    while( vector_size( active_object_groups.active_object_groups ) > 0 )
    {
        object_group_delete( *vector_access(active_object_groups.active_object_groups, 0, object_group_type* ) );
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
    GLuint              element_buffer_object;
    GLuint              uv_buffer_object;
    GLuint              normal_buffer_object;
    GLuint              bone_buffer_object;

    /* Assign the vertexes to the object group */
    glGenVertexArrays( 1, &object_group->vertex_array_object );
    glGenBuffers( 1, &vertex_buffer_object );
    glGenBuffers( 1, &element_buffer_object );
    glGenBuffers( 1, &uv_buffer_object );
    glGenBuffers( 1, &normal_buffer_object );
    glGenBuffers( 1, &bone_buffer_object );
    vector_push_back( object_group->buffers_to_delete, &vertex_buffer_object );
    vector_push_back( object_group->buffers_to_delete, &element_buffer_object );
    vector_push_back( object_group->buffers_to_delete, &uv_buffer_object );
    vector_push_back( object_group->buffers_to_delete, &normal_buffer_object );
    vector_push_back( object_group->buffers_to_delete, &bone_buffer_object );

    glBindVertexArray( object_group->vertex_array_object );

    glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer_object );
    glBufferData( GL_ARRAY_BUFFER, params->vertex_count * sizeof( vec3_type ), params->vertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, element_buffer_object );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, params->triangle_count * sizeof( vertex_triangle_type ), params->triangles, GL_STATIC_DRAW );

    glVertexAttribPointer( VERTEX_ATTRIB_POINTER, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( VERTEX_ATTRIB_POINTER );

    if( params->use_uvs )
    {
        glBindBuffer( GL_ARRAY_BUFFER, uv_buffer_object );
        glBufferData( GL_ARRAY_BUFFER, params->vertex_count * sizeof( uv_type ), params->uvs, GL_STATIC_DRAW );
        glVertexAttribPointer( UV_ATTRIB_POINTER, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( UV_ATTRIB_POINTER );
    }

    if( params->use_normals )
    {
        glBindBuffer( GL_ARRAY_BUFFER, normal_buffer_object );
        glBufferData( GL_ARRAY_BUFFER, params->vertex_count * sizeof( vec3_type ), params->normals, GL_STATIC_DRAW );
        glVertexAttribPointer( NORMAL_ATTRIB_POINTER, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( NORMAL_ATTRIB_POINTER );
    }

    if( params->bone_association_size > 0 )
    {
        glBindBuffer( GL_ARRAY_BUFFER, bone_buffer_object );
        glBufferData( GL_ARRAY_BUFFER, params->vertex_count * params->bone_association_size * sizeof( GLfloat ), vector_access( params->bone_associations, 0, uint8_t ), GL_STATIC_DRAW );
        glVertexAttribPointer( BONE_ATTRIB_POINTER, params->bone_association_size, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( BONE_ATTRIB_POINTER );
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
    
    shader_use( &( object_group->shader ) );

    if ( object_group->use_uvs )
    {
        texture_use( &( object_group->texture ) );
    }
    
    camera_set_active( object_group->camera, &( object_group->shader ) );
    
    len = vector_size( object_group->objects );

    glBindVertexArray( object_group->vertex_array_object );

    if( NULL != object_group->object_cb )
    {
        object_event.event_type = OBJECT_EVENT_TYPE_RENDER_START;
        object_event.event_data.render_start_data.shader = &( object_group->shader );
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
            if(object_group->is_3d )
            {
                shader_set_uniform_mat4( object->shader, "model_matrix", &object->model_matrix );
            }
            
            glDrawElements( GL_TRIANGLES, object_group->triangle_count * sizeof( vertex_triangle_type ) / sizeof( GLuint ), GL_UNSIGNED_INT, 0 );
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