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
 *		TRUE if the file exists
 *		FALSE if the file does not exist
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
 *		TRUE on success
 *		FALSE on failure
 */
boolean file_read
    (
        sint8_t         const *    filename,
        file_contents_type    *    p_file_contents /* [out] */
    );

/**
 * @brief Clear the resources associated with a read file @see file_read
 *
 * @return
 *      TRUE on success
 *      FALSE on failure
 */
boolean file_free
    (
        file_contents_type const * p_file_contents /* [in] */
    );

#endif /* FILE_API_H */
