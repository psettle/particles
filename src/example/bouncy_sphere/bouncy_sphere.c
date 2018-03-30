/**
 * @file bouncy_sphere.c
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
#include "bouncy_sphere.h"
#include "model_loader.h"
#include "camera_util.h"
#include "string.h"

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

/* Use C built in string concat for easy filename assembly */
#define RESOURCE_DIR( filename ) "src/example/bouncy_sphere/resource/"filename

/**********************************************************************
                               TYPES
**********************************************************************/

typedef struct directional_light_config_struct {
	vec3_type position;
	vec3_type ambient;
	vec3_type diffuse;
	vec3_type specular;
    vec3_type attenuation;
} directional_light_config_type;

typedef struct material_config_struct {
	vec3_type ambient;
	vec3_type diffuse;
	vec3_type specular;
	GLfloat shininess;
} material_config_type;

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
static void object_cb
    (
    object_event_type const * event_data
    );

/**
 * @brief Pass frame data into the uniform.
 */ 
static void bouncy_sphere_pass_uniforms
    (
        void
    );

/**
 * @brief Apply gravity to the sphere
 */ 
static void bouncy_sphere_apply_gravity
    (
        object_type* object,
        GLfloat      dt
    );

/**********************************************************************
                             VARIABLES
**********************************************************************/

static directional_light_config_type light_config =
{
    { 100.0f, 0.0f, 0.0f },   /* position */
    { 0.1f, 0.3f, 0.1f },   /* ambient */
    { 0.5f, 0.8f, 0.5f },   /* diffuse */
    { 0.3f, 0.6f, 0.3f },   /* specular */
    { 0.0f, 0.0f, 0.00005f }    /* attenuation */
};

static material_config_type material_config =
{
    { 1.0f, 1.0f, 1.0f },   /* ambient */
    { 1.0f, 1.0f, 1.0f },   /* diffuse */
    { 1.0f, 1.0f, 1.0f },   /* specular */
    32.0f
};

static object_group_type* bouncy_sphere_group;
static camera_type camera;

/**********************************************************************
                             FUNCTIONS
**********************************************************************/

void bouncy_sphere_start
    (
    void
    )
{
    object_type* cube;
    vec3_type pos;
        
    create_camera();
    create_object_group();

    cube = object_create( bouncy_sphere_group );

    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    object_set_position( cube, &pos );
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
    object_group_create_argument_type bouncy_sphere;
    model_load_data_out_type          model;
        
    memset( &bouncy_sphere, 0, sizeof( bouncy_sphere ) );

    /* Read shaders into RAM. */
    vertex_shader_code = vector_init( sizeof( sint8_t ) );
    fragment_shader_code = vector_init( sizeof( sint8_t ) );

    file_read( RESOURCE_DIR( "vertex_shader.glsl" ), vertex_shader_code );
    file_read( RESOURCE_DIR( "fragment_shader.glsl" ), fragment_shader_code );

    /* Build shaders from source. */
    bouncy_sphere.shader = shader_build
    ( 
        vector_access( vertex_shader_code, 0, sint8_t ),
        vector_access( fragment_shader_code, 0, sint8_t )
    );

    /* Free shader code. */
    vector_deinit( vertex_shader_code );
    vector_deinit( fragment_shader_code );

    /* Load the model */
    model_load( MODEL_FILE_FORMAT_AUTO, RESOURCE_DIR( "sphere.STL" ), &model );

    /* Assign vertex info, shader info. */
    bouncy_sphere.model_uniform_name = "model_matrix";       /* Corresponds with uniform mat4 model_matrix in vertex_shader.glsl */
    bouncy_sphere.vertices = (vec3_type*)vector_access( model.vertices, 0, vec3_type );
    bouncy_sphere.vertex_channel = 0;                        /* Corresponds with layout(location = 0) in vertex_shader.glsl */
    bouncy_sphere.normals = (vec3_type*)vector_access( model.normals, 0, vec3_type );
    bouncy_sphere.normal_channel = 1;                        /* Doesn't matter, no normals provided. */
    bouncy_sphere.uvs = NULL;
    bouncy_sphere.uv_channel = 0;                            /* Doesn't matter, no uvs provided. */
    bouncy_sphere.vertex_count = vector_size( model.vertices );
    bouncy_sphere.object_cb = object_cb;                     /* Called each frame and on system events */

    /* Create the group. */
    bouncy_sphere_group = object_group_create( &bouncy_sphere );

    /* Free the model. */
    model_load_free_data( &model );
}

static void create_camera
    (
    void
    )
{
    vec3_type from;
    vec3_type to;
    vec3_type up;
    GLfloat   fov;

    fov = 90.0f * M_PI / 180.0f;

    camera_init( &camera );
    camera_set_perspective( &camera, fov, 1920, 1080, 0.1f, 100.0f );

    vec3_set( &from, 100.0f, 100.0f, 100.0f );
    vec3_set( &to, 0.0f, 0.0f, 0.0f );
    vec3_set( &up, 0.0f, 1.0f, 0.0f );

    camera_set_view( &camera, &from, &to, &up );

    camera_util_init();
    camera_util_enable_mouse_keyboard_control( &camera, fov, 0.1f, 1000.0f );
}

static void object_cb
    (
    object_event_type const * event_data
    )
{
    switch( event_data->event_type )
    {
    case OBJECT_EVENT_TYPE_RENDER_START:
        bouncy_sphere_pass_uniforms();
        break;
    case OBJECT_EVENT_TYPE_RENDER_OBJECT:
        bouncy_sphere_apply_gravity
        (
            event_data->event_data.render_object_data.object,
            event_data->event_data.render_object_data.time_since_last_frame
        );
        break;
    default:
        break;
    }
}

static void bouncy_sphere_pass_uniforms
    (
        void
    )
{
    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "camera_position",
            &camera.position
        );

    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "light_config.position",
            &light_config.position
        );
    
    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "light_config.diffuse",
            &light_config.diffuse
        );

    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "light_config.specular",
            &light_config.specular
        );

    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "light_config.ambient",
            &light_config.ambient
        );

    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "light_config.attenuation",
            &light_config.attenuation
        );

    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "material_config.ambient",
            &material_config.ambient
        );

    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "material_config.specular",
            &material_config.specular
        );

    shader_set_uniform_vec3
        (
            bouncy_sphere_group->shader,
            "material_config.diffuse",
            &material_config.diffuse
        );

    shader_set_uniform_float
        (
            bouncy_sphere_group->shader,
            "material_config.shininess",
            material_config.shininess
        );
}

static void bouncy_sphere_apply_gravity
    (
        object_type* object,
        GLfloat      dt
    )
{
    static vec3_type speed = { 0 };
    vec3_type pos;
    vec3_type dv = { 0 };

    object_get_position( object, &pos );

    /* If the object passed ground level, bounce it. */
    if( pos.y < -15.0f && speed.y < 0.0f )
    {
        vec3_scale( &speed, -0.9f, &speed );
    }

    /* Apply acceleration to speed */
    dv.y = -dt / 10.0f;
    vec3_add( &speed, &speed, &dv );

    if( GLFW_PRESS == glfwGetKey( system_get_window(), GLFW_KEY_SPACE ) )
    {
        speed.y += dt;
    }

    /* Apply speed to position */
    object_translate( object, &speed );
}