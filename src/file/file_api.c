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
        file_contents_type    *    p_file_contents
    )
{
    FILE*       fp;
    uint8_t     read_buffer[READ_BUFFER_SIZE];
    uint8_t*    temp_buffer;
    uint32_t    read_count;
    boolean     status;

    /* If the file doesn't exist, return false */
    status = file_exists( filename );
    CHECK_STATUS( status );

    /* Clear the return struct */
    memset( p_file_contents, 0, sizeof(file_contents_type) );

    fp = fopen( filename, "r" );

    if( NULL == fp )
    {
        return FALSE;
    }

    while( TRUE )
    {
        read_count = fread( read_buffer, 1, sizeof( read_buffer ), fp );
        if( 0 == read_count )
        {
            break;
        }

        temp_buffer = malloc( p_file_contents->length + read_count );
        memcpy( temp_buffer, p_file_contents->p_file_contents, p_file_contents->length );
        memcpy( temp_buffer + p_file_contents->length, read_buffer, read_count);

        if( NULL != p_file_contents->p_file_contents )
        {
            free( p_file_contents->p_file_contents );
        }

        p_file_contents->p_file_contents = temp_buffer;
        p_file_contents->length += read_count;
    }

    fclose( fp );
    return TRUE;
} /* file_read */

boolean file_free
    (
        file_contents_type const * p_file_contents
    )
{
    if( ( p_file_contents->length > 0 ) && ( NULL != p_file_contents->p_file_contents ) )
    {
        free( p_file_contents->p_file_contents );
    }
    return TRUE;
} /* file_free */