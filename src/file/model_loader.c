/**
 * @file file_api.c
 *
 * @brief Implementation of an interface to the filesystem
 */

/**********************************************************************
                                INCLUDES
**********************************************************************/

#include "file_api.h"
#include "model_loader.h"
#include "string.h"
#include "common_util.h"
#include <string.h>
#include <stdlib.h>
#include "system_types.h"

#define STL_MIN_LEN ( 19 )

/**********************************************************************
                                    TYPES
**********************************************************************/

typedef boolean (*model_load_cb)
    (
        sint8_t const *           file_name,
        model_load_data_out_type* model_load_data_out
    );

typedef struct model_load_associations_struct
{
    sint8_t const * file_extension;
    model_load_cb   loader_function;
} model_load_associations_type;

typedef struct ascii_token_struct
{
    uint32_t current_token_index;
    uint32_t number_of_tokens;
    vector_type* token_string;  /* sint8_t array of tokens will null seperators */
    vector_type* token_indices; /* uint32_t the start index of each token */
} ascii_token_type;

typedef uint8_t stl_ascii_state_t8; enum
{
STL_ASCII_STATE_UNKNOWN,
STL_ASCII_STATE_HEADER,
STL_ASCII_STATE_FACET_HEADER,
STL_ASCII_STATE_VERTICES_HEADER,
STL_ASCII_STATE_VERTEX,
STL_ASCII_STATE_VERTICES_FOOTER,
STL_ASCII_STATE_FACET_FOOTER,
STL_ASCII_STATE_FOOTER
};

/**********************************************************************
                                PROTOTYPES
**********************************************************************/

static boolean model_load_format_auto
    (
        sint8_t const *           file_name,
        model_load_data_out_type* model_load_data_out
    );

static boolean model_load_format_stl
    (
        sint8_t const *           file_name,
        model_load_data_out_type* model_load_data_out
    );

static boolean model_load_format_stl_ascii
    (
        vector_type*              file_contents, /* in: sint8_t */
        model_load_data_out_type* model_load_data_out
    );

static boolean model_load_format_stl_binary
    (
        vector_type*              file_contents, /* in: sint8_t */
        model_load_data_out_type* model_load_data_out
    );

static void ascii_token_parse_tokens
    (    
        ascii_token_type*   tokens,     /* out */
        vector_type*        string      /* in  */
    );

static boolean is_whitespace
    (
        sint8_t character
    );

static ascii_token_type* ascii_token_init
    (
        void
    );

static void ascii_token_deinit
    (
        ascii_token_type* tokens
    );

static sint8_t const * ascii_token_get_token
    (
        ascii_token_type* tokens
    );

static stl_ascii_state_t8 stl_ascii_identify_next_line
    (
        sint8_t const * token
    );

/**********************************************************************
                                MEMORY CONSTANTS
**********************************************************************/

static model_load_associations_type const file_extensions[] =
{
    { "STL", model_load_format_stl }
};

/**********************************************************************
                                FUNCTIONS
**********************************************************************/

boolean model_load
    (
        model_file_format_t8      file_format,
        sint8_t const *           file_name,
        model_load_data_out_type* model_load_data_out
    )
{
    switch( file_format )
    {
    case MODEL_FILE_FORMAT_AUTO:
        return model_load_format_auto( file_name, model_load_data_out );
    case MODEL_FILE_FORMAT_STL:
        return model_load_format_stl( file_name, model_load_data_out );
    default:
        DEBUG_LINE();
        return FALSE;
    }
}

static boolean model_load_format_auto
    (
        sint8_t const *           file_name,
        model_load_data_out_type* model_load_data_out
    )
{
    sint8_t const * file_extension;
    uint8_t i;
    uint8_t len;
    uint8_t pos;

    /* Get the file extension. */

    len = strlen( file_name );
    file_extension = NULL;

    for( i = len; i > 0; --i )
    {
        pos = i - 1;

        /* If current entry is . and there is another character after the dot (the extension) grab it */
        if( ( file_name[pos] == '.' ) &&
            ( pos + 1 < len ) )
        {
            file_extension = &( file_name[pos + 1] );
            break;
        }
    }

    if( NULL == file_extension )
    {
        /* No extension on file, fail. */
        DEBUG_LINE();
        return FALSE;
    }

    /* Find the correct callback for the extension */
    len = strlen( file_extension );
    for( i = 0; i < array_count( file_extensions ); ++i )
    {
        if( len != strlen( file_extensions[i].file_extension ) )
        {
            continue;
        }

        if( 0 == memcmp( file_extension, file_extensions[i].file_extension, len ) )
        {
            return file_extensions[i].loader_function( file_name, model_load_data_out );
        }
    }

    /* Unknown extension, fail. */
    DEBUG_LINE();
    return FALSE;
}

static boolean model_load_format_stl
    (
        sint8_t const *           file_name,
        model_load_data_out_type* model_load_data_out
    )
{
    vector_type*    file_contents;
    uint32_t        len;
    boolean         ret;

    file_contents = vector_init( sizeof( sint8_t ) );
    if( !file_read( file_name, file_contents ) )
    {
        vector_deinit( file_contents );
        DEBUG_LINE();
        return FALSE;
    }

    len = vector_size( file_contents );
    if( len < STL_MIN_LEN )
    {
        vector_deinit( file_contents );
        DEBUG_LINE();
        return FALSE;
    }

    if( 0 == memcmp( vector_access(  file_contents, 0, sint8_t ), "solid", 5 ) )
    {
        ret = model_load_format_stl_ascii( file_contents, model_load_data_out );
    }
    else
    {
        ret = model_load_format_stl_binary( file_contents, model_load_data_out );
    }

    vector_deinit( file_contents );
    return ret;
}

static boolean model_load_format_stl_ascii
    (
        vector_type*              file_contents, /* in: sint8_t */
        model_load_data_out_type* model_load_data_out
    )
{
    stl_ascii_state_t8  stl_ascii_state;
    ascii_token_type*   tokens;
    sint8_t const *     token;

    sint8_t const *     x;
    sint8_t const *     y;
    sint8_t const *     z;
    
    vector_type*        vertices;
    vector_type*        normals;

    vec3_type           active_normal;
    vec3_type           active_vertex;

    /* Init output vectors */
    memset( model_load_data_out, 0, sizeof( model_load_data_out_type ) );
    vertices = vector_init( sizeof( vec3_type ) );
    normals  = vector_init( sizeof( vec3_type ) );

    /* Init token obj */
    tokens = ascii_token_init();

    /* Tokenize the file contents */
    ascii_token_parse_tokens( tokens, file_contents );

    /* Init token loop */
    token = ascii_token_get_token( tokens );
    stl_ascii_state = STL_ASCII_STATE_UNKNOWN;

    /* Iterate over tokens until they are all used. */
    while( NULL != token )
    {
        switch( stl_ascii_state )
        {
            case STL_ASCII_STATE_UNKNOWN:
                /* Figure out what type of row is next: */
                stl_ascii_state = stl_ascii_identify_next_line( token );
                break;
            case STL_ASCII_STATE_HEADER:
                /* one token for object name, we don't care */
                stl_ascii_state = STL_ASCII_STATE_UNKNOWN;
                break;
            case STL_ASCII_STATE_FACET_HEADER:
                /* first token is "normal" next 3 are x, y, z of the normal */
                x = ascii_token_get_token( tokens );
                y = ascii_token_get_token( tokens );
                z = ascii_token_get_token( tokens );

                vec3_set( &active_normal, atof( x ), atof( y ), atof( z ) );

                stl_ascii_state = STL_ASCII_STATE_UNKNOWN;
                break;
            case STL_ASCII_STATE_VERTICES_HEADER:
                /* one token, "loop", we don't care */
                stl_ascii_state = STL_ASCII_STATE_UNKNOWN;
                break;
            case STL_ASCII_STATE_VERTEX:
                /* first token is x, followed by y and z */
                x = token;
                y = ascii_token_get_token( tokens );
                z = ascii_token_get_token( tokens );

                vec3_set( &active_vertex, atof( x ), atof( y ), atof( z ) );

                vector_push_back( vertices, &active_vertex );
                vector_push_back( normals, &active_normal );

                stl_ascii_state = STL_ASCII_STATE_UNKNOWN;
                break;
            case STL_ASCII_STATE_VERTICES_FOOTER:
                /* no tokens we care about */
                stl_ascii_state = STL_ASCII_STATE_UNKNOWN;
                break;
            case STL_ASCII_STATE_FACET_FOOTER:
                /* no tokens we care about */
                stl_ascii_state = STL_ASCII_STATE_UNKNOWN;
                break;
            case STL_ASCII_STATE_FOOTER:
                /* one token for object name, we don't care */
                stl_ascii_state = STL_ASCII_STATE_UNKNOWN;
                break;
        }

        token = ascii_token_get_token( tokens );
    }

    /* Deinit token resources */
    ascii_token_deinit( tokens );

    /* Assert vertices == normals */
    if( vector_size( vertices ) != vector_size( normals ) )
    {
        ASSERT_ALWAYS();
    }

    /* Assign or clean up output */
    if( vector_size( vertices ) > 0 )
    {
        model_load_data_out->vertices = vertices;
        model_load_data_out->normals = normals;
    }
    else
    {
        vector_deinit( vertices );
        vector_deinit( normals );
    }

    return TRUE;
}

static boolean model_load_format_stl_binary
    (
        vector_type*              file_contents, /* in: sint8_t */
        model_load_data_out_type* model_load_data_out
    )
{
    /* not implemented */
    DEBUG_LINE();
    return FALSE;
}

static void ascii_token_parse_tokens
    (    
        ascii_token_type*   tokens,     /* out */
        vector_type*        string      /* in  */
    )
{
    boolean         in_whitespace;
    uint32_t        len;
    uint32_t        i;
    uint32_t        token_index;
    sint8_t const * c_str;
    sint8_t         current_char;

    in_whitespace = TRUE;
    c_str = vector_access( string, 0, sint8_t );
    len = vector_size( string );
    token_index = 0;

    for( i = 0; i < len; ++i )
    {
        current_char = c_str[i];

        if( is_whitespace( current_char ) )
        {
            /* Continue skipping whitespace */
            if( in_whitespace )
            {
                continue;
            }
            /* Found the end of a token, replace current char with a null terminator */
            else
            {
                current_char = '\0';
                in_whitespace = TRUE;
            }
        }
        else
        {
            /* Found start of new token, add the start index to the set */
            if( in_whitespace )
            {
                in_whitespace = FALSE;
                vector_push_back( tokens->token_indices, &token_index );
            }  
        }

        /* add the current character to the string and increment the token index */
        vector_push_back( tokens->token_string, &current_char );
        token_index++;
    }

    tokens->number_of_tokens = vector_size( tokens->token_indices );
}

static boolean is_whitespace
    (
        sint8_t character
    )
{
    switch( character )
    {
    case '\n':
    case '\r':
    case '\t':
    case ' ':
    case '\0':
        return TRUE;
    default:
        return FALSE;
    }
}

static ascii_token_type* ascii_token_init
    (
        void
    )
{
    ascii_token_type* tokens    = calloc( sizeof( ascii_token_type ), 1 );
    tokens->token_indices       = vector_init( sizeof( uint32_t ) );
    tokens->token_string        = vector_init( sizeof( sint8_t ) );
    return tokens;
}

static void ascii_token_deinit
    (
        ascii_token_type* tokens
    )
{
    vector_deinit( tokens->token_indices );
    vector_deinit( tokens->token_string );
    free( tokens );
}

static sint8_t const * ascii_token_get_token
    (
        ascii_token_type* tokens
    )
{
    sint8_t const * token;
    uint32_t token_index;

    if( tokens->current_token_index >= tokens->number_of_tokens )
    {
        return NULL;
    }

    token_index = *vector_access( tokens->token_indices, tokens->current_token_index, uint32_t);
    token = vector_access( tokens->token_string, token_index, sint8_t );

    tokens->current_token_index++;

    return token;
}

static stl_ascii_state_t8 stl_ascii_identify_next_line
    (
        sint8_t const * token
    )
{
    if( !strcmp( token, "solid") )
    {
        return STL_ASCII_STATE_HEADER;
    }
    if( !strcmp( token, "build" ) )
    {
        return STL_ASCII_STATE_HEADER;
    }
    if( !strcmp( token, "facet" ) )
    {
        return STL_ASCII_STATE_FACET_HEADER;
    }
    if( !strcmp( token, "outer" ) )
    {
        return STL_ASCII_STATE_VERTICES_HEADER;
    }
    if( !strcmp( token, "vertex" ) )
    {
        return STL_ASCII_STATE_VERTEX;
    }
    if( !strcmp( token, "endloop") )
    {
        /* There are no other tokens for endloop, so continue with unknown */
        return STL_ASCII_STATE_UNKNOWN;
    }
    if( !strcmp( token, "endfacet" ) )
    {
        /* There are no other tokens for facet, so continue with unknown */
        return STL_ASCII_STATE_UNKNOWN;
    }
    if( !strcmp( token, "endsolid" ) )
    {
        return STL_ASCII_STATE_FOOTER;
    }

    /* Bad token, hopefully the next one looks correct */
    DEBUG_LINE();
    return STL_ASCII_STATE_UNKNOWN;
}