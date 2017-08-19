/**
 * @file vector.c
 *
 * @brief Generic vector implementation
 */
/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "vector.h"
#include "common_util.h"
#include <string.h>
#include <stdlib.h>

/**********************************************************************
                            LITERAL CONSTANTS
**********************************************************************/

#define VECTOR_DEFAULT_SIZE     10
#define VECTOR_GROWTH_FACTOR    2

/**********************************************************************
                            PROTOTYPES
**********************************************************************/

/**
 * @brief Reallocates the internal buffer to new_size
 */
static void resize
    (
        vector_type   * vector,
        uint32_t        new_size
    );

/**
 * @brief Shifts the elements of the array, either deleting
 * them from the from of opening up new slots at the front
 */
static void shift_items
    (
        vector_type   * vector,
        uint32_t        amount_of_shift,
        boolean         direction /* TRUE for forward */
    );

/**********************************************************************
                             FUNCTIONS
**********************************************************************/

vector_type * vector_init
    (
        uint16_t  item_size
    )
{
    vector_type * vector;

    vector = (vector_type *)calloc( 1, sizeof( vector_type ) );

    vector->item_size = item_size;

    resize( vector, VECTOR_DEFAULT_SIZE );

    return vector;
}

void vector_deinit
    (
        vector_type     * vector
    )
{
    if( NULL != vector->list_items )
    {
        free( vector->list_items );
    }

    free( vector );
}

uint32_t vector_size
    (
        vector_type const * vector
    )
{
    return vector->item_count;
}

void vector_push_back
    (
        vector_type     * vector,
        void      const * item
    )
{
    void * new_item_buffer;

    if( vector->item_count >= vector->list_max )
    {
        resize( vector, vector->list_max * VECTOR_GROWTH_FACTOR );
    }

    new_item_buffer = vector_access( vector, vector->item_count );
    memcpy( new_item_buffer, item, vector->item_size );
    vector->item_count += 1;
}

void vector_push_front
    (
        vector_type     * vector,
        void      const * item
    )
{
    void * new_item_slot;

    shift_items( vector, 1, TRUE );
    new_item_slot = vector_access( vector, 0 );
    memcpy( new_item_slot, item, vector->item_size );
}

void vector_push_back_many
    (
        vector_type     * vector,
        void      const * items,
        uint32_t          count
    )
{
    uint8_t     * items_local;
    uint32_t      i;

    items_local = (uint8_t *)items;

    for( i = 0; i < count; ++i )
    {
        vector_push_back( vector, items_local + vector->item_size * i );
    }
}

void vector_pop_front
    (
        vector_type     * vector,
        void            * item
    )
{
    void * old_item_slot;

    old_item_slot = vector_access( vector, 0 );
    memcpy( item, old_item_slot, vector->item_size );

    shift_items( vector, 1, FALSE );
}

void vector_pop_back
    (
        vector_type     * vector,
        void            * item
    )
{
    void * old_item;

    old_item = vector_access( vector, vector->item_count - 1);

    memcpy( item, old_item, vector->item_size );

    vector->item_count -= 1;

    /* Shrink the vector if the item count is getting small enough */
    if( ( vector->item_count <= vector->list_max / VECTOR_GROWTH_FACTOR ) &&
        ( vector->item_count > VECTOR_DEFAULT_SIZE ) )
    {
        resize( vector, vector->list_max / VECTOR_GROWTH_FACTOR );
    }
}

void * vector_access
    (
        vector_type const * vector,
        uint32_t            index
    )
{
    return (void *)&vector->list_items[index * vector->item_size];
}

static void resize
    (
        vector_type   * vector,
        uint32_t        new_size
    )
{
    uint32_t    old_elements_to_copy_count;
    void      * new_array;

    if ( 0 == new_size )
    {
        new_array = NULL;
    }
    else
    {
        new_array = calloc( new_size, vector->item_size );
    }

    old_elements_to_copy_count = MIN( vector->item_count, new_size );
    memcpy( new_array, vector->list_items, vector->item_size * old_elements_to_copy_count );

    if( NULL != vector->list_items )
    {
        free( vector->list_items );
    }

    vector->list_max = new_size;
    vector->item_count = old_elements_to_copy_count;
    vector->list_items = new_array;
}

static void shift_items
    (
        vector_type   * vector,
        uint32_t        amount_of_shift,
        boolean         direction /* TRUE for forward */
    )
{
    uint8_t * new_list_items;
    uint32_t new_size;
    uint32_t memory_offset;

    /* Calculate the minimum new size */
    if( direction )
    {
        new_size = vector->item_count + amount_of_shift;
    }
    else if( amount_of_shift < vector->item_count )
    {
        new_size = vector->item_count - amount_of_shift;
    }
    else
    {
        /* The request is to shift the entire array off the front, empty and return */
        new_size = VECTOR_DEFAULT_SIZE;
        vector->item_count = 0;
        resize( vector, new_size );
        return;
    }

    vector->list_max = new_size;

    /* Copy the elements over and re-assign the array */
    new_list_items = calloc( vector->list_max, vector->item_size );
    memory_offset =  amount_of_shift * vector->item_size;

    if( direction )
    {
        memcpy( new_list_items + memory_offset, vector->list_items, vector->item_size * vector->item_count );
    }
    else
    {
        memcpy( new_list_items, vector->list_items + memory_offset, ( vector->item_count - amount_of_shift ) * vector->item_size );
    }

    vector->item_count = new_size;

    free( vector->list_items );
    vector->list_items = new_list_items;
}

void vector_empty
    (
        vector_type     * vector
    )
{
    vector->item_count = 0;
    resize( vector, VECTOR_DEFAULT_SIZE );
}

void vector_remove
    (
        vector_type     * vector,
        void      const * target
    )
{
    uint32_t len;
    uint32_t i;

    len = vector_size( vector );
    for( i = 0; i < len; ++i )
    {
        uint8_t * item;

        item = (uint8_t *)vector_access( vector, i );

        if( 0 == memcmp( item, target, vector->item_size ) )
        {
            len -= 1;
            vector->item_count -= 1;
            memcpy( item, item + vector->item_size, vector->item_size * ( len - i ) );
        }
    }
}