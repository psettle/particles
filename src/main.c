/**
* @file main.c
*
* @brief Entry point to the particles program, this program runs an n-body gravity simulation
*
* @author Patrick Settle (https://github.com/psettle)
*/

#include    "system.h"
#include    "texture_cube.h"

int main()
{
    if ( system_init() )
    {
        texture_cube_start();
        system_run();
    }

    return 0;
}