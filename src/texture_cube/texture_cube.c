/**
 * @file object.c
 *
 * @brief Implementation of the object interface
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "object.h"
#include "object_group_core.h"
#include "camera.h"
#include "file_api.h"
#include "stdio.h"

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

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
 * @brief Callback for object events, this is where objects can be moved/updated.
 */ 
static void texture_cube_object_cb
    (
    object_event_type const * event_data
    );

/**********************************************************************
                             VARIABLES
**********************************************************************/

/**
 * @brief Every vertex on the cube, each set of 3 is a triangle.
 */ 
static GLfloat const vertices_raw[] = {
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,

    0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,

    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,

   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
   
   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,

    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,

    0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,

    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,
};

/**
 * @brief Texture coordinates for each vertex.
 */
static GLfloat const uvs_raw[] = {
    1.0f,  0.0f,
    1.0f,  1.0f,
    0.0f,  0.0f,

    1.0f,  1.0f,
    0.0f,  1.0f,
    0.0f,  0.0f,

    1.0f,  0.0f,
    1.0f,  1.0f,
    0.0f,  0.0f,

    1.0f,  1.0f,
    0.0f,  1.0f,
    0.0f,  0.0f,

    1.0f,  0.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,

    1.0f,  1.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,

    0.0f,  1.0f,
    0.0f,  0.0f,
    0.0f,  0.0f,

    0.0f,  1.0f,
    0.0f,  0.0f,
    0.0f,  1.0f,

    1.0f,  0.0f,
    0.0f,  0.0f,
    1.0f,  0.0f,

    1.0f,  0.0f,
    0.0f,  0.0f,
    0.0f,  0.0f,

    1.0f,  1.0f,
    0.0f,  1.0f,
    0.0f,  1.0f,

    1.0f,  1.0f,
    1.0f,  1.0f,
    0.0f,  1.0f,
};

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
    vec3_type    amount;
    vector_type* vertex_shader_code;
    vector_type* fragment_shader_code;

    object_group_create_argument_type texture_cube;

    create_camera();
     
    vertex_shader_code = vector_init(sizeof(sint8_t));
    fragment_shader_code = vector_init(sizeof(sint8_t));

    file_read( "shaders/vertex_shader_3d_uv_no_light.glsl", vertex_shader_code );
    file_read( "shaders/fragment_shader_3d_uv_no_light.glsl", fragment_shader_code );

    texture_cube.shader = shader_build
    ( 
        vector_access( vertex_shader_code, 0, sint8_t ),
        vector_access( fragment_shader_code, 0, sint8_t )
    );

    vector_deinit( vertex_shader_code );
    vector_deinit( fragment_shader_code );

    texture_cube.model_uniform_name = "model_matrix";
    texture_cube.vertices = (vec3_type*)vertices_raw;
    texture_cube.vertex_channel = 0;
    texture_cube.normals = NULL;
    texture_cube.normal_channel = 0; /* Doesn't matter, no normals provided. */
    texture_cube.uvs = (uv_type*)uvs_raw;
    texture_cube.uv_channel = 1;
    texture_cube.vertex_count = sizeof( vertices_raw ) / sizeof( vec3_type );
    texture_cube.object_cb = texture_cube_object_cb;

    texture_cube.texture = texture_init
    (
        "images/qbd.png",
        GL_TEXTURE0,
        texture_cube.shader,
        "image"
    );

    texture_cube_group = object_group_create( &texture_cube );

    cube = object_create( texture_cube_group );
    object_set_visibility( cube, TRUE );

    vec3_set( &amount, 0.1f, 0.2f, 0.3f );
    object_translate( cube, &amount ); 
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
    camera_set_perspective( &camera, 110.0f * M_PI / 180.0f, 1920, 1080, 0.1f, 10.0f );

    vec3_set( &from, 1.0f, 1.0f, 1.0f );
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