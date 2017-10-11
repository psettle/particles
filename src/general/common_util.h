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

#define CHECK_STATUS( status )\
do\
{\
    if( !( status ) )\
    {\
        if( CHECK_STATUS_DEBUG )\
        {\
        const char* err = "Fail on line " " of file "  " \n";\
        printf( "%s", err );\
        }\
        return FALSE;\
    }\
}\
while( FALSE )

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


#endif /* COMMON_UTIL_H */
