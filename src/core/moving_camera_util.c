/**
 * @file moving_camera_util.c
 *
 * @brief Utility that allows a camera to be controlled by keyboard and mouse.
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "camera_util.h"
#include "string.h"

/**********************************************************************
                             LITERAL CONSTANTS
**********************************************************************/

#define MOUSE_SPEED     ( 2.0f )
#define MOVEMENT_SPEED  ( 30.0f )

/**********************************************************************
                             TYPES
**********************************************************************/

typedef struct moving_camera_control_struct
{
    GLdouble horizontal_angle;
    GLdouble vertical_angle;
    GLfloat  fov;
    GLfloat  front;
    GLfloat  back;
} moving_camera_control_type;

/**********************************************************************
                             VARIABLES
**********************************************************************/

static camera_type*                active_camera;
static moving_camera_control_type  control;

/**********************************************************************
                             PROTOTYPES
**********************************************************************/

static void frame_cb
    (
    frame_event_type const * event_data
    );

static void system_cb
    (
    system_event_type const * event_data
    );

/**********************************************************************
                               FUNCTIONS
**********************************************************************/

void camera_util_init
    (
    void
    )
{
    system_listener_callbacks_type callbacks;


    callbacks.frame_event_cb    = frame_cb;
    callbacks.system_event_cb   = system_cb;
    register_system_listeners( &callbacks );

    active_camera = NULL;
    memset( &control, 0, sizeof( control ) );
    control.horizontal_angle = M_PI;  
}

void camera_util_enable_mouse_keyboard_control
    (
    camera_type * camera,
    GLfloat fov,
    GLfloat front,
    GLfloat back
    )
{
    GLint width;
    GLint height;

    active_camera = camera;
    control.fov   = fov;
    control.front = front;
    control.back  = back;

    /* Set cursor to centre of screen. */
    glfwGetWindowSize( system_get_window(), &width, &height);
    glfwSetInputMode( system_get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
    glfwSetCursorPos( system_get_window(), width / 2.0, height / 2.0);
}

void camera_util_disable_mouse_keyboard_control
    (
    void
    )
{
    active_camera = NULL;
}

static void frame_cb
    (
    frame_event_type const * event_data
    )
{
    GLdouble  mouse_x;
    GLdouble  mouse_y;
    GLdouble  dt;
    GLint     width;
    GLint     height;
    vec3_type position;
    vec3_type direction;
    vec3_type right;
    vec3_type up;
    vec3_type to;
    
    /* Nothing to do if camera isn't set */
    if( NULL == active_camera )
    {
        return;
    }

    dt = event_data->timesince_last_frame;

    /* Get current cursor position */
    glfwGetCursorPos( system_get_window(), &mouse_x, &mouse_y);

    /* Set cursor back to centre of screen. */
    glfwGetWindowSize( system_get_window(), &width, &height);
    glfwSetCursorPos( system_get_window(), width / 2.0, height / 2.0);

    /* Modify viewing angle */
    control.horizontal_angle += MOUSE_SPEED * dt * (float)( ( (GLdouble)width / 2.0 ) - mouse_x );
    control.vertical_angle += MOUSE_SPEED * dt * (float)( ( (GLdouble)height / 2.0 ) - mouse_y );

    /* Calculate new vectors from angle */
    vec3_set
        (
        &direction,
        cos( control.vertical_angle ) * sin( control.horizontal_angle ),
        sin( control.vertical_angle ),
        cos( control.vertical_angle ) * cos( control.horizontal_angle )
        );

    vec3_set
        (
        &right,
        sin( control.horizontal_angle - M_PI / 2.0f ),
        0,
        cos( control.horizontal_angle - M_PI / 2.0f )
		);

    vec3_cross( &up, &right, &direction );

    /* Calculate new camera position */
    vec3_scale( &direction, dt * MOVEMENT_SPEED, &direction );
    vec3_scale( &right, dt * MOVEMENT_SPEED, &right );
    camera_get_position( active_camera, &position );

    if( GLFW_PRESS == glfwGetKey( system_get_window(), GLFW_KEY_W ) )
    {
        vec3_add( &position, &position, &direction );
    }

    if( GLFW_PRESS == glfwGetKey( system_get_window(), GLFW_KEY_A ) )
    {
        vec3_subtract( &position, &position, &right );
    }

    if( GLFW_PRESS == glfwGetKey( system_get_window(), GLFW_KEY_S ) )
    {
        vec3_subtract( &position, &position, &direction );
    }

    if( GLFW_PRESS == glfwGetKey( system_get_window(), GLFW_KEY_D ) )
    {
        vec3_add( &position, &position, &right );
    }
    
    camera_set_perspective( active_camera, control.fov, width, height, control.front, control.back );

    vec3_add( &to, &position, &direction );
    camera_set_view( active_camera, &position, &to, &up );
}

static void system_cb
    (
    system_event_type const * event_data
    )
{
    /* Nothing to do if camera isn't set */
    if( NULL == active_camera )
    {
        return;
    }

    if( SYSTEM_EVENT_NEW_CAMERA == event_data->event_type )
    {
        active_camera = event_data->event_data.new_camera_data;
    }
}