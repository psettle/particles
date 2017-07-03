/**
 * @file system_types.h
 *
 * @brief Type defs for the core openGL functions
 */
#ifndef SYSTEM_TYPES_H
#define SYSTEM_TYPES_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "common_types.h"
#include "opengl_includes.h"
#include "vector.h"

/**********************************************************************
                              CAMERA TYPES
**********************************************************************/

typedef struct camera_struct
{
    uint8_t placeholder;
} camera_type;

/**********************************************************************
                           FRAME EVENT TYPES
**********************************************************************/

/* Event passed when a new frame is starting */
typedef struct frame_event_struct
{
    GLdouble timestamp;
} frame_event_type;

typedef void ( * frame_event_callback )
    (
        frame_event_type const * event_data
    );

/**********************************************************************
                            SYSTEM EVENT TYPES
**********************************************************************/

typedef uint8_t system_event_code_t8; enum
{
    SYSTEM_EVENT_INIT_START = 0x00,
    SYSTEM_EVENT_DEINIT_START,
    SYSTEM_EVENT_NEW_CAMERA
};

/* Event when a system level event has occured (init, deinit) */
typedef struct system_event_struct
{
    system_event_code_t8 event_type;
} system_event_type;

typedef void ( * system_event_callback )
    (
        system_event_type const * event_data
    );

/**********************************************************************
                             SYSTEM TYPES
**********************************************************************/

/* Argument a listener can use to register with the system, NULL callbacks will be ignored */
typedef struct system_listener_callbacks_struct
{
    system_event_callback system_event_cb;
    frame_event_callback frame_event_cb;
} system_listener_callbacks_type;

/* System controller data */
typedef struct system_struct
{
    GLFWwindow              * glfw_window;
    camera_type             * system_camera;
    vector_type             * system_event_listeners;
    vector_type             * frame_event_listeners;
    boolean                   should_close_window;
} system_type;

#endif /* SYSTEM_TYPES_H */
