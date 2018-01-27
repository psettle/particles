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
#include "shader.h"
#include "texture.h"
#include "vector.h"
#include "matrix_math.h"

/**********************************************************************
                              CAMERA TYPES
**********************************************************************/

/**
 * @brief
 */
typedef struct camera_struct
{
    mat4_type projection_matrix;
    mat4_type view_matrix;
    mat4_type projection_view_matrix;
    vec3_type position;
} camera_type;


/**********************************************************************
                           FRAME EVENT TYPES
**********************************************************************/

/**
 * @brief
 */
typedef struct frame_event_struct
{
    GLdouble timestamp;
    GLdouble timesince_last_frame;
} frame_event_type;


/**
 * @brief
 */
typedef void ( * frame_event_callback )
    (
        frame_event_type const * event_data
    );


/**********************************************************************
                          OBJECT GROUP TYPES
**********************************************************************/

/**
 * @brief
 */
typedef struct uv_struct
{
    GLfloat u;
    GLfloat v;
} uv_type;

typedef struct vertex_struct
{
    vec3_type vertex;
    vec3_type normal;
    uv_type   uv;
} vertex_type;

/**
 * @brief
 */
typedef struct bone_struct
{
    uint16_t  bone_id;
    mat4_type bone_transform_matrix;
} bone_type;

/**
 * @brief
 */
typedef struct object_struct
{
    uint16_t      object_id;
    boolean       is_visible;
    vec3_type     position;
    mat4_type     model_matrix;
    shader_type*  shader;
    vector_type*  bones; /* Vector of bone_type, for each bone in the object */
} object_type;

/**
 * @brief
 */
typedef uint8_t object_event_type_t8; enum
{
    OBJECT_EVENT_TYPE_RENDER_START,     /* Rendering of an object type has started, the shader is bound and uniforms can be passed in. */
    OBJECT_EVENT_TYPE_RENDER_OBJECT,    /* Rendering of a particular object is about to start. Changes to that objects position, rotation or scale will appear in the next frame. */

    OBJECT_EVENT_TYPE_RENDER_COUNT
};

/**
 * @brief
 */
typedef struct object_event_type_render_start_data_struct
{
    shader_type const *  shader;
} object_event_type_render_start_data_type;

/**
 * @brief
 */
typedef struct object_event_type_render_object_data_struct
{
    object_type*         object;
    GLdouble             time_since_last_frame;
} object_event_type_render_object_data_type;

/**
 * @brief
 */
typedef union object_event_data_union
{
    object_event_type_render_start_data_type  render_start_data;    /* Data for OBJECT_EVENT_TYPE_RENDER_START */
    object_event_type_render_object_data_type render_object_data;   /* Data for OBJECT_EVENT_TYPE_RENDER_OBJECT */
} object_event_data_type;

/**
 * @brief
 */
typedef struct object_event_struct
{
    object_event_type_t8     event_type;
    object_event_data_type   event_data;
} object_event_type;

/**
 * @brief
 */
typedef void (*object_cb_type)
    (
    object_event_type const * event_data
    );

/**
 * @brief
 */
typedef struct object_group_struct
{
    uint32_t        next_id;
    GLuint          vertex_array_object;
    camera_type   * camera;
    shader_type   * shader;
    sint8_t const * model_uniform_name;
    texture_type  * texture;
    vector_type   * objects; /* Array of object_type* representing each unique object in the group */
    uint32_t        vertex_count;
    vector_type   * buffers_to_delete; /* GLuint Random buffers that must be deleted when the object goes out of scope */
    object_cb_type  object_cb;
} object_group_type;

/**
 * @brief Arguments for creating a new dynamic object class.
 */
typedef struct object_group_create_argument_struct
{
    shader_type*    shader;       /* A shader object (@see shader.h), will be automatically deleted when the object group goes out of scope. */  
    sint8_t const * model_uniform_name; /* (Optional) A model uniform name to automatically set objects model matrix each frame. */
    vec3_type*      vertices;     /* A pointer to all vertices for the model, count is vertex_count  */
    uint8_t         vertex_channel;
    vec3_type*      normals;
    uint8_t         normal_channel;
    uv_type*        uvs;
    uint8_t         uv_channel;
    uint32_t        vertex_count;
    texture_type*   texture;      /* A texture object (@see texture.h), will be automatically deleted when the object group goes out of scope.  */
    object_cb_type  object_cb;    /* Will be called on every frame for each instance of this object type. @see object_event_type_t8 */
} object_group_create_argument_type;


/**********************************************************************
                            SYSTEM EVENT TYPES
**********************************************************************/

/**
 * @brief
 */
typedef uint8_t system_event_code_t8; enum
{
    SYSTEM_EVENT_INIT_START = 0x00,
    SYSTEM_EVENT_DEINIT_START,
    SYSTEM_EVENT_NEW_CAMERA
};

/**
 * @brief
 */
typedef union system_event_data_union
{
    camera_type * new_camera_data;
} system_event_data_type;

/**
 * @brief
 */
typedef struct system_event_struct
{
    system_event_code_t8    event_type;
    system_event_data_type  event_data;
} system_event_type;

/**
 * @brief
 */
typedef void ( * system_event_callback )
    (
        system_event_type const * event_data
    );

/**********************************************************************
                             SYSTEM TYPES
**********************************************************************/

/**
 * @brief
 */
typedef struct system_listener_callbacks_struct
{
    system_event_callback   system_event_cb;
    frame_event_callback    frame_event_cb;
} system_listener_callbacks_type;

/**
 * @brief
 */
typedef struct system_struct
{
    GLFWwindow              * glfw_window;
    camera_type             * system_camera;
    vector_type             * system_event_listeners;
    vector_type             * frame_event_listeners;
    boolean                   should_close_window;
} system_type;

#endif /* SYSTEM_TYPES_H */
