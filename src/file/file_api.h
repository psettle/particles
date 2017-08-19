/**
 * @file file_api.h
 *
 * @brief Simple interface to the file system, to read in resource files
 */
#ifndef FILE_API_H
#define FILE_API_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "common_types.h"
#include "vector.h"

/**********************************************************************
                                TYPES
**********************************************************************/

/* Describes the contents of a file */
typedef struct file_contents_struct
{
    uint8_t*    p_file_contents;
    uint32_t    length;
} file_contents_type;

/**********************************************************************
                              PROTOTYPES
**********************************************************************/

/**
 * @brief Checks if a file exists using a full file path
 *
 * @return
 *        TRUE if the file exists
 *        FALSE if the file does not exist
 */
boolean file_exists
    (
        sint8_t const * filename
    );

/**
 * @brief Open a file and return the contents
 *
 * @note file_contents must be cleared with @see file_free
 *
 * @return
 *        TRUE on success
 *        FALSE on failure
 */
boolean file_read
    (
        sint8_t         const *    filename,
        vector_type           *    contents /* [out] sint8_t */
    );

/**
 * @brief Writes contents to a file
 *
 * @return
 *        TRUE on success
 *        FALSE on failure
 */
boolean file_write
    (
        sint8_t         const *     filename,
        vector_type     const *     contents /* [in] sint8_t */
    );

#endif /* FILE_API_H */
