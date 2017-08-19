/**
 * @file file_api.c
 *
 * @brief Implementation of an interface to the filesystem
 */

/**********************************************************************
                                INCLUDES
**********************************************************************/

#include "file_api.h"
#include "common_util.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************************
                                CONSTANTS
**********************************************************************/

#define READ_BUFFER_SIZE 10000

/**********************************************************************
                                FUNCTIONS
**********************************************************************/

boolean file_exists
    (
        sint8_t const * filename
    )
{
    return ( access(filename, F_OK) != -1 );
} /* file_exists */

boolean file_read
    (
        sint8_t         const *    filename,
        vector_type           *    contents /* [out] sint8_t */
    )
{
    FILE*       fp;
    uint8_t     read_buffer[READ_BUFFER_SIZE];
    uint32_t    read_count;
    boolean     status;

    /* If the file doesn't exist, return false */
    status = file_exists( filename );
    CHECK_STATUS( status );

    fp = fopen( filename, "r" );

    if( NULL == fp )
    {
        return FALSE;
    }

    vector_empty( contents );

    while( TRUE )
    {
        read_count = fread( read_buffer, 1, sizeof( read_buffer ), fp );
        vector_push_back_many( contents, read_buffer, read_count );

        if( 0 == read_count )
        {
            break;
        }
    }

    vector_push_back( contents, "\0" );

    fclose( fp );
    return TRUE;
} /* file_read */

boolean file_write
    (
        sint8_t         const *     filename,
        vector_type     const *     contents /* [in] sint8_t */
    )
{
    FILE*       fp;

    fp = fopen( filename, "w" );

    if( NULL == fp )
    {
        return FALSE;
    }

    fprintf( fp, "%s", vector_access( contents, 0 ) );

    fclose( fp );

    return TRUE;
}