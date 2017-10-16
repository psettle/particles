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

typedef struct camera_struct
{
    mat4_type projection_matrix;
    mat4_type view_matrix;
    mat4_type projection_view_matrix;
    vec3_type position;
} camera_type;

/**********************************************************************
                          OBJECT GROUP TYPES
**********************************************************************/

/* A uv pair */
typedef struct uv_struct
{
    GLfloat u;
    GLfloat v;
} uv_type;

/* A triangle of vertex indices */
typedef struct vertex_triangle_struct
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
} vertex_triangle_type;

typedef struct bone_struct
{
    uint16_t  bone_id;
    mat4_type bone_transform_matrix;
} bone_type;

typedef struct object_struct
{
    uint16_t      object_id;
    boolean       is_visible;
    vec3_type     position;
    mat4_type     model_matrix;
    vector_type * bones; /* Vector of bone_type, for each bone in the object */
} object_type;

typedef void(*object_frame_cb_type)
    (
    object_type * object
    );

/* Represents a group of objects that use the same vertexes, shaders and textures */
typedef struct object_group_struct
{
    uint32_t          next_id;
    GLuint            vertex_array_object;
    camera_type     * camera;
    shader_type       shader;
    texture_type      texture;
    vector_type     * objects; /* Array of object_type* representing each unique object in the group */
    uint32_t          vertex_count;
    uint32_t          triangle_count;
    vector_type     * buffers_to_delete; /* GLuint Random buffers that must be deleted when the object goes out of scope */
    boolean              is_3d;
    boolean              use_uvs;
    object_frame_cb_type object_frame_cb;
} object_group_type;

typedef struct object_group_create_argument_struct
{
    boolean                      is_2d;
    boolean                      use_uvs;
    boolean                      is_lighted;
    boolean                      use_normals;
    sint8_t              const * texture_filename;
    sint8_t              const * vertex_shader_filename;
    sint8_t              const * fragment_shader_filename;
    uint32_t                     vertex_count;
    vec3_type            const * vertices;
    vec3_type            const * normals;
    uv_type              const * uvs;
    vec4_type            const * colours;
    vector_type                * bone_associations;
    vector_type                * bone_weights;
    uint32_t                     bone_association_size;
    uint32_t                     triangle_count;
    vertex_triangle_type const * triangles;
    vector_type                * default_bones;
    uint32_t                     texture_slot;
    object_frame_cb_type         object_frame_cb;
} object_group_create_argument_type;



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

typedef union system_event_data_union
{
    camera_type * new_camera_data;
} system_event_data_type;

/* Event when a system level event has occured (init, deinit) */
typedef struct system_event_struct
{
    system_event_code_t8    event_type;
    system_event_data_type  event_data;
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
    system_event_callback   system_event_cb;
    frame_event_callback    frame_event_cb;
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
