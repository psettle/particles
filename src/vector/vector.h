/**
 * @file vector.h
 *
 * @brief Generic vector interface
 */
#ifndef VECTOR_H
#define VECTOR_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "common_types.h"

/**********************************************************************
                                TYPES
**********************************************************************/

/* Vector type, should only be accessed with interface functiosn below */
typedef struct vector_struct
{
    uint8_t       * list_items;
    uint32_t        item_count;
    uint32_t        list_max;
    uint16_t        item_size;
} vector_type;

/**********************************************************************
                             PROTOTYPES
**********************************************************************/

/**
 * @brief Allocates and returns a new vector
 */
vector_type * vector_init
    (
        uint16_t  item_size
    );

/**
 * @brief Deletes an existing vector
 */
void vector_deinit
    (
        vector_type     * vector
    );

/**
 * @brief Gets the size of a vector
 */
uint32_t vector_size
    (
        vector_type const * vector
    );

/**
 * @brief Appends an element to the end of a vector
 */
void vector_push_back
    (
        vector_type     * vector,
        void      const * item
    );

/**
 * @brief Appends an element to the front of a vector
 */
void vector_push_front
    (
        vector_type     * vector,
        void      const * item
    );

/**
 * @brief Pops an element off the front of a vector
 */
void vector_pop_front
    (
        vector_type     * vector,
        void            * item /* [out] The popped element */
    );

/**
 * @brief Pops an element off the back of a vector
 */
void vector_pop_back
    (
        vector_type     * vector,
        void            * item /* [out] The popped element */
    );

/**
 * @brief Accessed an element from the vector
 *
 * @return A pointer to the internal memory that holds the element
 * (Any vector function may reallocate this memory, so the pointer should be fetched every use)
 */
void * vector_access
    (
        vector_type     * vector,
        uint32_t          index
    );

#endif /* VECTOR_H */
