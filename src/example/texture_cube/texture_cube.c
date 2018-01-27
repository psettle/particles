/**
 * @file texture_cube.c
 *
 * @brief Sample app that creates a textures cube and spins it.
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "object.h"
#include "object_group_core.h"
#include "camera.h"
#include "file_api.h"
#include "stdio.h"
#include "model_loader.h"
#include "texture_cube.h"

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

/* Use C built in string concat for easy filename assembly */
#define RESOURCE_DIR( filename ) "src/example/texture_cube/resource/"filename

/**********************************************************************
                               TYPES
**********************************************************************/

/**********************************************************************
                             PROTOTYPES
**********************************************************************/

/**
 * @brief Create and assign a system camera.
 */ 
static void create_camera
    (
    void
    );

/**
 * @brief Initialize the texture cube object group
 */
static void create_object_group
    (
    void
    );

/**
 * @brief Callback for object events, this is where objects can be moved/updated.
 */ 
static void texture_cube_object_cb
    (
    object_event_type const * event_data
    );

/**********************************************************************
                             VARIABLES
**********************************************************************/

static object_group_type* texture_cube_group;
static camera_type camera;

/**********************************************************************
                             FUNCTIONS
**********************************************************************/

void texture_cube_start
    (
    void
    )
{
object_type* cube;
    
create_camera();
create_object_group();

cube = object_create( texture_cube_group );
object_set_visibility( cube, TRUE );

/* Note: no need for teardown function, the render system will automatically free remaining objects. */
}

static void create_object_group
    (
    void
    )
{
vector_type*                      vertex_shader_code;
vector_type*                      fragment_shader_code;
object_group_create_argument_type texture_cube;
    
/* Read shaders into RAM. */
vertex_shader_code = vector_init(sizeof(sint8_t));
fragment_shader_code = vector_init(sizeof(sint8_t));

file_read( RESOURCE_DIR( "vertex_shader.glsl" ), vertex_shader_code );
file_read( RESOURCE_DIR( "fragment_shader.glsl" ), fragment_shader_code );

/* Build shaders from source. */
texture_cube.shader = shader_build
( 
    vector_access( vertex_shader_code, 0, sint8_t ),
    vector_access( fragment_shader_code, 0, sint8_t )
);

/* Free shader code. */
vector_deinit( vertex_shader_code );
vector_deinit( fragment_shader_code );

/* Assign vertex info, shader info. */
texture_cube.model_uniform_name = "model_matrix";       /* Corresponds with uniform mat4 model_matrix in vertex_shader.glsl */
texture_cube.vertices = (vec3_type*)vertices_raw;
texture_cube.vertex_channel = 0;                        /* Corresponds with layout(location = 0) in vertex_shader.glsl */
texture_cube.normals = NULL;
texture_cube.normal_channel = 0;                        /* Doesn't matter, no normals provided. */
texture_cube.uvs = (uv_type*)uvs_raw;
texture_cube.uv_channel = 1;                            /* Corresponds with layout(location = 1) in vertex_shader.glsl */
texture_cube.vertex_count = vertex_count;
texture_cube.object_cb = texture_cube_object_cb;        /* Called each frame and on system events */

texture_cube.texture = texture_init
(
    RESOURCE_DIR( "qbd.png" ),
    GL_TEXTURE0,
    texture_cube.shader,
    "image" /* Corresponds with uniform sampler2D image in fragment_shader.glsl */
);

/* Create the group. */
texture_cube_group = object_group_create( &texture_cube );
}

static void create_camera
    (
    void
    )
{
    vec3_type from;
    vec3_type to;
    vec3_type up;

    camera_init( &camera );
    camera_set_perspective( &camera, 90.0f * M_PI / 180.0f, 1920, 1080, 0.1f, 100.0f );

    vec3_set( &from, 2.0f, 2.0f, 2.0f );
    vec3_set( &to, 0.0f, 0.0f, 0.0f );
    vec3_set( &up, 0.0f, 1.0f, 0.0f );

    camera_set_view( &camera, &from, &to, &up );
}

static void texture_cube_object_cb
    (
    object_event_type const * event_data
    )
{
    vec3_type axis;

    switch( event_data->event_type )
    {
    case OBJECT_EVENT_TYPE_RENDER_START:
        break;
    case OBJECT_EVENT_TYPE_RENDER_OBJECT:
        vec3_set( &axis, 0.5, 0.5, 0.5 );
        object_rotate( event_data->event_data.render_object_data.object, &axis, event_data->event_data.render_object_data.time_since_last_frame );
        break;
    default:
        break;
    }
}