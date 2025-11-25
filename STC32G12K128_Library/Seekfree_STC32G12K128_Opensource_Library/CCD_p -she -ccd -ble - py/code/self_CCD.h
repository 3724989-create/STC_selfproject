#ifndef _SELF_CCD_H_
#define _SELF_CCD_H_

#include "bsp.h"

extern uint8 y1_boundary[128];
extern uint8 y2_boundary[128];
extern uint8 y3_boundary[128];

void  CCD_init();   
void ccd_process_data(void);
void CCD_process(void);


#endif // DEBUG