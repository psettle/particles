/**
 * @file common_util.h
 *
 * @brief Common utility functions used by many units
 */
#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

/**********************************************************************
                                MACROS
**********************************************************************/

#define CHECK_STATUS( status )\
do\
{\
    if( !status )\
    {\
        return FALSE;\
    }\
}\
while( FALSE )




#endif /* COMMON_UTIL_H */
