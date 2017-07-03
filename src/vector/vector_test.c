/**
 * @file vector_test.c
 *
 * @brief Simple tests to validate the vector interface
 */
/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "vector.h"

#include <stdio.h>

/**********************************************************************
                            PROTOTYPES
**********************************************************************/

static void print_vector
    (
        vector_type* vector
    );

static void push_back_test
    (
        void
    );

static void push_front_test
    (
        void
    );

static void pop_back_test
    (
        void
    );

static void pop_front_test
    (
        void
    );

/**********************************************************************
                            FUNCTIONS
**********************************************************************/

void vector_tests_run
    (
        void
    )
{
    push_back_test();
    push_front_test();
    pop_front_test();
    pop_back_test();
}

static void push_back_test
    (
        void
    )
{
    vector_type   * vector;
    uint32_t        i;

    printf( "Push back test start:\n" );

    vector = vector_init( sizeof(uint32_t) );

    for( i = 0; i < 20; ++i )
    {
        vector_push_back( vector, &i );
    }

    print_vector( vector );
}

static void push_front_test
    (
        void
    )
{
    vector_type   * vector;
    uint32_t        i;

    printf( "Push front test start:\n" );

    vector = vector_init( sizeof(uint32_t) );

    for( i = 0; i < 20; ++i )
    {
        vector_push_front( vector, &i );
    }

    print_vector( vector );
}

static void pop_back_test
    (
        void
    )
{
    vector_type   * vector;
    uint32_t        i;
    uint32_t        item;

    printf( "Pop back test start:\n" );

    vector = vector_init( sizeof( uint32_t ) );

    for( i = 0; i < 20; ++i )
    {
        vector_push_back( vector, &i );
    }

    for( i = 0; i < 10; ++i )
    {
        vector_pop_back( vector, &item );
        printf( "Popped: %d\n", item );
    }

    print_vector( vector );
}

static void pop_front_test
    (
        void
    )
{
    vector_type   * vector;
    uint32_t        i;
    uint32_t        item;

    printf( "Pop front test start:\n" );

    vector = vector_init( sizeof( uint32_t ) );

    for( i = 0; i < 20; ++i )
    {
        vector_push_back( vector, &i );
    }

    for( i = 0; i < 10; ++i )
    {
        vector_pop_front( vector, &item );
        printf( "Popped: %d\n", item );
    }

    print_vector( vector );
}

static void print_vector( vector_type* vector )
{
    uint32_t i;
    uint32_t len;
    uint32_t element;

    len = vector_size( vector );

    printf("Start len: %d\n", len);

    for( i = 0; i < len; ++i )
    {
        element = *(uint32_t*)vector_access( vector, i );
        printf("%d: %d\n", i, element);
    }
}
