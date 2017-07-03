/**
 * @file system.c
 *
 * @brief Implementation of the system singleton
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "system.h"
#include <stdlib.h>
#include <string.h>

/**********************************************************************
                           LITERAL CONSTANTS
**********************************************************************/

#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   600
#define FULLSCREEN      FALSE
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

/**********************************************************************
                             VARIABLES
**********************************************************************/

/* The singleton system instance */
static system_type system_instance;

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

boolean system_init
    (
        void
    )
{
    memset( &system_instance, 0, sizeof( system_type ) );

    system_instance.system_event_listeners    = vector_init( sizeof( system_event_callback ) );
    system_instance.frame_event_listeners     = vector_init( sizeof( frame_event_callback ) );

    return openGL_system_init();
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
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
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

    return TRUE;
}

static void system_deinit
    (
        void
    )
{
    uint32_t                len;
    uint32_t                i;
    system_event_callback   cb;
    system_event_type       event_data;

    /* Send the deinit events */
    memset( &event_data, 0, sizeof( system_event_type ) );
    event_data.event_type = SYSTEM_EVENT_DEINIT_START;
    len = vector_size( system_instance.system_event_listeners );
    for( i = 0; i < len; ++i )
    {
        cb = *( system_event_callback * )vector_access( system_instance.system_event_listeners, i );
        cb( &event_data );
    }

    /* Free the system memory */
    vector_deinit( system_instance.system_event_listeners );
    vector_deinit( system_instance.frame_event_listeners );
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
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

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