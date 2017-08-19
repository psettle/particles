/**
* @file main.c
*
* @brief Entry point to the Object Compiler program, this program runs an n-body gravity simulation
*
* @author Patrick Settle (https://github.com/psettle)
*/
#include "common_types.h"
#include "file_api.h"
#include <stdio.h>
#include <string.h>

int main
    (
        sint32_t    argc,
        sint8_t  ** argv
    )
{
    vector_type * file_contents;
    vector_type * output;
    uint32_t      i;
    uint32_t      len;
    sint8_t       buffer[100];

    if( argc < 2 )
    {
        printf("Please provide a text file to parse.\n");
        return 1;
    }

    if( argc < 3 )
    {
        printf("Please provide an output file.\n");
        return 1;
    }

    file_contents = vector_init( sizeof( sint8_t ) );
    output        = vector_init( sizeof( sint8_t) );

    if( !file_read( argv[1], file_contents ) )
    {
        printf("File did not exist.\n");
        vector_deinit( file_contents );
        vector_deinit( output );
        return 1;
    }

    len = vector_size( file_contents );
    for( i = 0; i < len; ++i )
    {
        sprintf( buffer, "0x%02X, ", *( sint8_t * )vector_access( file_contents, i ) );

        vector_push_back_many( output, buffer, strlen( buffer ) );
    }

    file_write( argv[2], output );


    vector_deinit( file_contents );
    vector_deinit( output );

    return 0;
}