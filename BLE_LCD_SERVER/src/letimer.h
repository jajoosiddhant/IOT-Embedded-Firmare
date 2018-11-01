//***********************************************************************************
// Include files
//***********************************************************************************
#include "sleep.h"
#include "em_emu.h"

#include <stdint.h>
#include <stdbool.h>

//***********************************************************************************
// defined files
//***********************************************************************************

#define event1 			0x04
//#define event2 1<<1
#define lem 			sleepEM4  			// Lowest Block Energy Sleep Mode
//#define ontime 0.175  			// LED on time
#define tp 				3.0		  			//Total Time Period
#define letimer_event 	0x01

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************

void letimer_init(void);
void LETIMER0_IRQHandler(void);
void prescale(void);


