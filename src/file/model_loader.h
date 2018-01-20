/**
 * @file model_loader.h
 *
 * @brief Load files with vertexs, normals and uvs into
 *        C components.
 */
#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "vector.h"

/**********************************************************************
                                TYPES
**********************************************************************/

typedef uint8_t model_file_format_t8; enum
{
    MODEL_FILE_FORMAT_AUTO, /* Guess file format using common file extensions */
    MODEL_FILE_FORMAT_STL,  /* STereoLithography */

    MODEL_FILE_FORMAT_COUNT
};

/**********************************************************************
                              PROTOTYPES
**********************************************************************/

/**
 * @brief Loads a model file into an array of vertices
 *
 * @return
 *        TRUE if the file exists and had a valid format.
 *        FALSE if the file did not exist or the format was invalid.
 */
boolean model_load
    (
        model_file_format_t8 file_format,
        sint8_t const *      file_name,
        vector_type*         vertices       /* out: vertex_type */
    );

#endif /* MODEL_LOADER_H */
