
#include "bsp.h"



void main()
{

    
    all_init();
    
    
    while(1)
    {
        scheduler_run();
    }
}
