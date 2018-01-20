/**
 * @file common_util.h
 *
 * @brief Common utility functions used by many units
 */
#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

/**********************************************************************
                            GENERAL INCLUDES
**********************************************************************/

#include "stdio.h"

/**********************************************************************
                                MACROS
**********************************************************************/

#define CHECK_STATUS_DEBUG      TRUE

#if CHECK_STATUS_DEBUG
    #define DEBUG_LINE() printf("Fail on line %d of %s\n", __LINE__, __FILE__ )
#else
    #define DEBUG_LING()
#endif


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define array_count( a )  ( sizeof( a ) / sizeof( ( a )[0] ) )


#endif /* COMMON_UTIL_H */
