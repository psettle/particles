/**
 * @file matrix_math.h
 *
 * @brief Ansi-c openGL math implementation
 */

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "math.h"
#include "string.h"
#include "matrix_math.h"

/**********************************************************************
                              PROTOTYPES
**********************************************************************/

/**********************************************************************
                              FUNCTIONS
**********************************************************************/

void vec3_set
    (
        vec3_type * vec3,
        GLfloat     x,
        GLfloat     y,
        GLfloat     z
    )
{
    vec3->x = x;
    vec3->y = y;
    vec3->z = z;
}

void vec3_cpy
    (
        vec3_type       * vec3,
        vec3_type const * source
    )
{
    vec3_set( vec3, source->x, source->y, source->z );
}

GLfloat vec3_length
    (
        vec3_type * vec3
    )
{
    return sqrt( vec3_dot( vec3, vec3 ) );
}

void vec3_cross
    (
        vec3_type       * out,
        vec3_type const * left,
        vec3_type const * right
    )
{
    out->x = left->y * right->z - left->z * right->y;
    out->y = left->z * right->x - left->x * right->z;
    out->z = left->x * right->y - left->y * right->x;
}

GLfloat vec3_dot
    (
        vec3_type const * left,
        vec3_type const * right
    )
{
    GLfloat sum;

    sum = 0.0f;
    sum += left->x * right->x;
    sum += left->y * right->y;
    sum += left->z * right->z;

    return sum;
}

void vec3_normalize
    (
        vec3_type * vec3
    )
{
    GLfloat length = vec3_length( vec3 );

    vec3->x = vec3->x / length;
    vec3->y = vec3->y / length;
    vec3->z = vec3->z / length;
}

void vec3_subtract
    (
        vec3_type * out,
        vec3_type const * left,
        vec3_type const * right
    )
{
    out->x = left->x - right->x;
    out->y = left->y - right->y;
    out->z = left->z - right->z;
}

void vec3_add
    (
        vec3_type * out,
        vec3_type const * left,
        vec3_type const * right
    )
{
    out->x = left->x + right->x;
    out->y = left->y + right->y;
    out->z = left->z + right->z;
}

void vec3_scale
    (
    vec3_type*        out,
    GLfloat           scaling_factor,
    vec3_type const * in
    )
{
    out->x = in->x * scaling_factor;
    out->y = in->y * scaling_factor;
    out->z = in->z * scaling_factor;
}

void vec4_set
    (
        vec4_type * vec4,
        GLfloat     x,
        GLfloat     y,
        GLfloat     z,
        GLfloat     w
    )
{
    vec4->x = x;
    vec4->y = y;
    vec4->z = z;
    vec4->w = w;
}

GLfloat vec4_dot
    (
        vec4_type const * left,
        vec4_type const * right
    )
{
    GLfloat sum;

    sum = 0.0f;
    sum += left->x * right->x;
    sum += left->y * right->y;
    sum += left->z * right->z;
    sum += left->w * right->w;

    return sum;
}

void mat4_set
    (
        mat4_type * mat4,
        GLfloat x_x, GLfloat y_x, GLfloat z_x, GLfloat w_x,
        GLfloat x_y, GLfloat y_y, GLfloat z_y, GLfloat w_y,
        GLfloat x_z, GLfloat y_z, GLfloat z_z, GLfloat w_z,
        GLfloat x_w, GLfloat y_w, GLfloat z_w, GLfloat w_w
    )
{
    vec4_set( &mat4->x, x_x, x_y, x_z, x_w );
    vec4_set( &mat4->y, y_x, y_y, y_z, y_w );
    vec4_set( &mat4->z, z_x, z_y, z_z, z_w );
    vec4_set( &mat4->w, w_x, w_y, w_z, w_w );
}

void mat4_inverse
    (
        mat4_type*        out,
        mat4_type const * in
    )
{
    mat4_type in_copy;

    /* copy in case in == out */
    memcpy( &in_copy, in, sizeof( mat4_type ) );

    out->x.y = in_copy.y.x;
    out->x.z = in_copy.z.x;
    out->x.w = in_copy.w.x;

    out->y.x = in_copy.x.y;
    out->y.z = in_copy.z.y;
    out->y.w = in_copy.w.y;

    out->z.x = in_copy.x.z;
    out->z.y = in_copy.y.z;
    out->z.w = in_copy.w.z;

    out->w.x = in_copy.x.w;
    out->w.y = in_copy.y.w;
    out->w.z = in_copy.z.w;
}

void mat4_multiply
    (
        mat4_type*        product,
        mat4_type const * left,
        mat4_type const * right
    )
{
    mat4_type right_copy;

    vec4_type row_x;
    vec4_type row_y;
    vec4_type row_z;
    vec4_type row_w;

    /* Make copies incase left == product || right == product */
    memcpy( &right_copy, right, sizeof( mat4_type ) );
    vec4_set( &row_x, left->x.x, left->y.x, left->z.x, left->w.x );
    vec4_set( &row_y, left->x.y, left->y.y, left->z.y, left->w.y );
    vec4_set( &row_z, left->x.z, left->y.z, left->z.z, left->w.z );
    vec4_set( &row_w, left->x.w, left->y.w, left->z.w, left->w.w );

    product->x.x = vec4_dot( &row_x, &right_copy.x );
    product->x.y = vec4_dot( &row_y, &right_copy.x );
    product->x.z = vec4_dot( &row_z, &right_copy.x );
    product->x.w = vec4_dot( &row_w, &right_copy.x );

    product->y.x = vec4_dot( &row_x, &right_copy.y );
    product->y.y = vec4_dot( &row_y, &right_copy.y );
    product->y.z = vec4_dot( &row_z, &right_copy.y );
    product->y.w = vec4_dot( &row_w, &right_copy.y );

    product->z.x = vec4_dot( &row_x, &right_copy.z );
    product->z.y = vec4_dot( &row_y, &right_copy.z );
    product->z.z = vec4_dot( &row_z, &right_copy.z );
    product->z.w = vec4_dot( &row_w, &right_copy.z );

    product->w.x = vec4_dot( &row_x, &right_copy.w );
    product->w.y = vec4_dot( &row_y, &right_copy.w );
    product->w.z = vec4_dot( &row_z, &right_copy.w );
    product->w.w = vec4_dot( &row_w, &right_copy.w );
}

void mat4_look_at
    (
        mat4_type       * view,
        vec3_type const * from,
        vec3_type const * to,
        vec3_type const * up
    )
{
    vec3_type forward;
    vec3_type left;
    vec3_type up_local;

    vec3_subtract( &forward, from , to );
    vec3_normalize( &forward );

    vec3_cross( &left, up, &forward );
    vec3_normalize( &left );

    vec3_cross( &up_local, &forward, &left );

    view->x.x = left.x;
    view->x.y = up_local.x;
    view->x.z = forward.x;
    view->x.w = 0.0f;

    view->y.x = left.y;
    view->y.y = up_local.y;
    view->y.z = forward.y;
    view->y.w = 0.0f;

    view->z.x = left.z;
    view->z.y = up_local.z;
    view->z.z = forward.z;
    view->z.w = 0.0;

    view->w.x = -vec3_dot( &left, from );
    view->w.y = -vec3_dot( &up_local, from );
    view->w.z = -vec3_dot( &forward, from );
    view->w.w = 1.0f;
}

void mat4_perspective
    (
        mat4_type       * perspective,
        GLfloat           field_of_view,  /* FOV in radians */
        GLfloat           aspect_ratio,   /* The ration of width/height for the persective */
        GLfloat           front,          /* Distance from the front of the camera to the first thing that can be rendered (0.1 is normal) */
        GLfloat           back            /* Distance from the front of the camera to the last thing that can be rendered  (10.0 is normal) */
    )
{
    GLdouble f;

    f = atan( field_of_view / 2.0f );

    perspective->x.x = f / aspect_ratio;
    perspective->x.y = 0.0f;
    perspective->x.z = 0.0f;
    perspective->x.w = 0.0f;

    perspective->y.x = 0.0f;
    perspective->y.y = f;
    perspective->y.z = 0.0f;
    perspective->y.w = 0.0f;

    perspective->z.x = 0.0f;
    perspective->z.y = 0.0f;
    perspective->z.z = ( front + back ) / ( front - back );
    perspective->z.w = -1.0f;

    perspective->w.x = 0.0f;
    perspective->w.y = 0.0f;
    perspective->w.z = 2 * front * back / ( front - back );
    perspective->w.w = 0.0f;
}

void mat4_rotate
    (
        mat4_type       * to_rotate,
        vec3_type const * rotation_axis,
        GLfloat           angle_rad
    )
{
    vec3_type axis;
    GLfloat cos_angle;
    GLfloat sin_angle;
    GLfloat one_min_cos;
    GLfloat x_comp;
    GLfloat y_comp;
    GLfloat z_comp;
    mat4_type rotation_matrix;

    axis = *rotation_axis;
    cos_angle = cos( angle_rad );
    sin_angle = sin( angle_rad );
    one_min_cos = 1 - cos_angle;

    vec3_normalize( &axis );
    x_comp = axis.x;
    y_comp = axis.y;
    z_comp = axis.z;

    rotation_matrix.x.x = x_comp * x_comp * one_min_cos + cos_angle;
    rotation_matrix.x.y = x_comp * y_comp * one_min_cos + z_comp * sin_angle;
    rotation_matrix.x.z = x_comp * z_comp * one_min_cos - y_comp * sin_angle;
    rotation_matrix.x.w = 0.0f;

    rotation_matrix.y.x = y_comp * x_comp * one_min_cos - z_comp * sin_angle;
    rotation_matrix.y.y = y_comp * y_comp * one_min_cos + cos_angle;
    rotation_matrix.y.z = y_comp * z_comp * one_min_cos + x_comp * sin_angle;
    rotation_matrix.y.w = 0.0f;

    rotation_matrix.z.x = z_comp * x_comp * one_min_cos + y_comp * sin_angle;
    rotation_matrix.z.y = z_comp * y_comp * one_min_cos - x_comp * sin_angle;
    rotation_matrix.z.z = z_comp * z_comp * one_min_cos + cos_angle;
    rotation_matrix.z.w = 0.0f;

    rotation_matrix.w.x = 0.0f;
    rotation_matrix.w.y = 0.0f;
    rotation_matrix.w.z = 0.0f;
    rotation_matrix.w.w = 1.0f;

    mat4_multiply
        (
            to_rotate,
            &rotation_matrix,
            to_rotate
        );
}

void mat4_translate
    (
        mat4_type       * to_translate,
        vec3_type const * amount
    )
{
    mat4_type translation_matrix;

    translation_matrix.x.x = 1.0f;
    translation_matrix.x.y = 0.0f;
    translation_matrix.x.z = 0.0f;
    translation_matrix.x.w = 0.0f;

    translation_matrix.y.x = 0.0f;
    translation_matrix.y.y = 1.0f;
    translation_matrix.y.z = 0.0f;
    translation_matrix.y.w = 0.0f;

    translation_matrix.z.x = 0.0f;
    translation_matrix.z.y = 0.0f;
    translation_matrix.z.z = 1.0f;
    translation_matrix.z.w = 0.0f;

    translation_matrix.w.x = amount->x;
    translation_matrix.w.y = amount->y;
    translation_matrix.w.z = amount->z;
    translation_matrix.w.w = 1.0f;

    mat4_multiply
        (
        to_translate,
        &translation_matrix,
        to_translate
        );
}