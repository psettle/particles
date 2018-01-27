/**
 * @file texture_cube.h
 *
 * @brief Test app that creates a textured cube and moves it
 *        around a bit.
 */
#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "system_types.h"

/**********************************************************************
                             VARIABLES
**********************************************************************/

/* These are from texture_cube_model.c, keeps clutter out of texture_cube.c */
extern GLfloat const vertices_raw[];
extern GLfloat const uvs_raw[];
extern GLuint const vertex_count;

/**********************************************************************
                              PROTOTYPES
**********************************************************************/

/**
 * @brief Starts the texture cube app.
 * 
 * Should be called after system init.
 */
void texture_cube_start
    (
    void
    );

#endif /* TEXTURE_CUBE_H */
