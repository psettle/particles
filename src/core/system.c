/**
 * @file system.c
 *
 * @brief Implementation of the system singleton
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "system.h"
#include "object_group_core.h"
#include "object.h"
#include "camera.h"
#include <stdlib.h>
#include <string.h>

/**********************************************************************
                           LITERAL CONSTANTS
**********************************************************************/
#define FULLSCREEN      FALSE

#if FULLSCREEN
#define WINDOW_WIDTH    1920
#define WINDOW_HEIGHT   1080
#else
#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   600
#endif

#define WINDOW_NAME     "Particles"

/**********************************************************************
                             PROTOTYPES
**********************************************************************/

/**
 * @brief Init openGL libraries
 */
static boolean openGL_system_init
    (
        void
    );

/**
 * @brief deinitialize the system before exit
 */
static void system_deinit
    (
        void
    );

/**
 * @brief Run a single frame of the applciation
 */
static void frame
    (
        void
    );

/**
 * @brief Sends a system event
 */
static void send_system_event
    (
        system_event_type const * event_data
    );

/**********************************************************************
                             VARIABLES
**********************************************************************/

/* The singleton system instance */
static system_type system_instance;
static camera_type system_camera_instance;

/**********************************************************************
                             FUNCIONS
**********************************************************************/

void register_system_listeners
    (
        system_listener_callbacks_type  * callbacks
    )
{
    system_event_type event_data;

    if( NULL != callbacks->system_event_cb )
    {
        vector_push_back( system_instance.system_event_listeners, &( callbacks->system_event_cb ) );

        /* Send the init event to the listener */
        memset( &event_data, 0, sizeof( system_event_type ) );
        event_data.event_type = SYSTEM_EVENT_INIT_START;
        callbacks->system_event_cb( &event_data );
    }

    if( NULL != callbacks->frame_event_cb )
    {
        vector_push_back( system_instance.frame_event_listeners, &( callbacks->frame_event_cb ) );
    }
}

void system_set_camera
    (
        camera_type const * camera
    )
{
    system_event_type event_data;

    system_instance.system_camera = ( camera_type * )camera;
    /* Send the new camera event */
    memset( &event_data, 0, sizeof( system_event_type ) );
    event_data.event_type = SYSTEM_EVENT_NEW_CAMERA;
    event_data.event_data.new_camera_data = ( camera_type * )camera;

    send_system_event( &event_data );
}

    GLfloat vertices_raw[] = {
         0.5f, 0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
    };

    GLfloat uvs_raw[] = {
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
    };

    GLuint triangles_raw[] = {
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
    };

boolean system_init
    (
        void
    )
{
    vector_type* vertices;
    vector_type* triangles;
    vector_type* uvs;
    vec3_type from;
    vec3_type to;
    vec3_type up;
    object_group_type* object_group;
    object_type* object;

    memset( &system_instance, 0, sizeof( system_type ) );

    system_instance.system_event_listeners    = vector_init( sizeof( system_event_callback ) );
    system_instance.frame_event_listeners     = vector_init( sizeof( frame_event_callback ) );

    object_group_init();

    vertices = vector_init( sizeof( vec3_type ) );
    vector_push_back_many( vertices, vertices_raw, 8 );
    triangles = vector_init( sizeof( vertex_triangle_type ) );
    vector_push_back_many( triangles, triangles_raw, 4 );
    uvs = vector_init( sizeof( uv_type ) );
    vector_push_back_many( uvs, uvs_raw, 8 );

    openGL_system_init();

    object_group = object_group_create("images/test.jpg", vertices, triangles, uvs );

    vec3_set( &from, 1.0f, 0.0f, 1.0f );
    vec3_set( &to, 0.0f, 0.0f, 0.0f );
    vec3_set( &up, 0.0f, 1.0f, 0.0f );
    camera_init( &system_camera_instance );
    camera_set_view
        (
            &system_camera_instance,
            &from,
            &to,
            &up
        );

    camera_set_perspective
        (
            &system_camera_instance,
            DEFAULT_FOV,
            1920,
            1080,
            DEFAULT_FRONT,
            DEFAULT_BACK
        );

    object = object_create( object_group );
    object_set_visibility( object, TRUE );

    return TRUE;
}

void system_run
    (
        void
    )
{
    while( TRUE )
    {
        if( system_instance.should_close_window )
        {
            system_deinit();
            return;
        }

        frame();
    }
}

static boolean openGL_system_init
    (
        void
    )
{
    if( !glfwInit() )
    {
        return FALSE;
    }

    glfwWindowHint( GLFW_SAMPLES, 16 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#if FULLSCREEN
    system_instance.glfw_window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, glfwGetPrimaryMonitor(), NULL );
#else
    system_instance.glfw_window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL );
#endif

    glfwMakeContextCurrent( system_instance.glfw_window );

    glewExperimental = GL_TRUE;
    if( glewInit() != GLEW_OK )
    {
        return FALSE;
    }

    glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSwapInterval( 0 );

    return TRUE;
}

static void system_deinit
    (
        void
    )
{
    system_event_type       event_data;

    /* Send the deinit events */
    memset( &event_data, 0, sizeof( system_event_type ) );
    event_data.event_type = SYSTEM_EVENT_DEINIT_START;

    send_system_event( &event_data );

    object_group_deinit();

    /* Free the system memory */
    vector_deinit( system_instance.system_event_listeners );
    vector_deinit( system_instance.frame_event_listeners );

    glfwTerminate();
}

static void frame
    (
        void
    )
{
    uint32_t                len;
    uint32_t                i;
    frame_event_callback    cb;
    frame_event_type        event_data;

    /* Check for close signal */
    if ( ( glfwGetKey( system_instance.glfw_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) ||
         ( glfwWindowShouldClose( system_instance.glfw_window ) != 0 ) )
    {
        system_instance.should_close_window = TRUE;
        return;
    }

    /* Clear the last frame */
    glfwPollEvents();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    /* Send the frame events out */
    memset( &event_data, 0, sizeof( frame_event_type ) );
    event_data.timestamp = glfwGetTime();
    len = vector_size( system_instance.frame_event_listeners );
    for( i = 0; i < len; ++i )
    {
        cb = *( frame_event_callback * )vector_access( system_instance.frame_event_listeners, i );
        cb( &event_data );
    }

    /* Swap the buffer (finishing the frame) */
    glfwSwapBuffers( system_instance.glfw_window );
}

static void send_system_event
    (
        system_event_type const * event_data
    )
{
    uint32_t                len;
    uint32_t                i;
    system_event_callback   cb;

    len = vector_size( system_instance.system_event_listeners );
    for( i = 0; i < len; ++i )
    {
        cb = *( system_event_callback * )vector_access( system_instance.system_event_listeners, i );
        cb( event_data );
    }
}