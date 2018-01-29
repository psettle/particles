/**
* @file main.c
*
* @brief Entry point to the particles program, this program runs an n-body gravity simulation
*
* @author Patrick Settle (https://github.com/psettle)
*/

#include    "system.h"
#include    "bouncy_sphere.h"
#include    "model_loader.h"

int main()
{
    if( system_init() )
    {
        /* texture_cube_start(); */
        bouncy_sphere_start();
        system_run();
    }
    
    return 0;
}