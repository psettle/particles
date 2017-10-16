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

static void create_camera
	(
	void
	);

static void texture_cube_object_frame_cb
	(
	object_type* cube
	);

/**********************************************************************
                             VARIABLES
**********************************************************************/

GLfloat vertices_raw[] = {
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
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
	0, 1, 4,
	1, 5, 4,
	2, 3, 7,
	2, 7, 6,
	0, 3, 4,
	4, 7, 3,
	1, 2, 6,
	5, 1, 6
};

object_group_create_argument_type texture_cube =
{
    FALSE,
    TRUE,
    FALSE,
    FALSE,
    "images/test.jpg",
    "shaders/vertex_shader_3d_uv_no_light.glsl",
    "shaders/fragment_shader_3d_uv_no_light.glsl",
    8,
    (vec3_type*)vertices_raw,
    NULL,
    (uv_type*)uvs_raw,
    NULL,
    NULL,
    NULL,
    0,
    12,
    (vertex_triangle_type*)triangles_raw,
    NULL,
    GL_TEXTURE0,
	texture_cube_object_frame_cb
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

	create_camera();
	
	texture_cube_group = object_group_create( &texture_cube );

	cube = object_create( texture_cube_group );
	object_set_visibility( cube, TRUE );
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

	vec3_set( &from, 2.0f, 2.0f, -1.0f );
	vec3_set( &to, 0.0f, 0.0f, 0.0f );
	vec3_set( &up, 0.0f, 1.0f, 0.0f );

	camera_set_view( &camera, &from, &to, &up );
}

static void texture_cube_object_frame_cb
	(
	object_type* cube
	)
{
	printf("Frame\n");
}