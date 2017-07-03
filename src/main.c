/**
* @file main.c
*
* @brief Entry point to the particles program, this program runs an n-body gravity simulation
*
* @author Patrick Settle (https://github.com/psettle)
*/

#include    "system.h"

int main()
{
    if ( system_init() )
    {
        system_run();
    }

    return 0;
}