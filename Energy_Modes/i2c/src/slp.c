/*
 * slp.c
 *
 *  Created on: Sep 18, 2018
 *      Author: user1
 */

#include "src/slp.h"
#include "src/letimer.h"
#include "src/gpio.h"
#include "em_letimer.h"
#include "em_core.h"
#include "em_cmu.h"

uint32_t sleep_block_counter[5]={0,0,0,0,0};

// Function to block a sleep Mode
void blocksleepmode(int lem)
{
	CORE_ATOMIC_IRQ_DISABLE();
	sleep_block_counter[lem]++;
	CORE_ATOMIC_IRQ_ENABLE();
}



//Function to Unblock Sleep Mode
void unblocksleepmode(int lem)
{
	CORE_ATOMIC_IRQ_DISABLE();
	if (sleep_block_counter[lem]>0)
	{
		sleep_block_counter[lem]--;
	}
	CORE_ATOMIC_IRQ_ENABLE();
}



//Sleep Routine
void slp(void)
{
if (sleep_block_counter[0]>0)
	{return;}
else if (sleep_block_counter[1]>0)
{
	return;}

else if (sleep_block_counter[2]>0)
{
	EMU_EnterEM1();}

else if (sleep_block_counter[3]>0)
{
	EMU_EnterEM2(1);}

else if (sleep_block_counter[4]>0)
{
	EMU_EnterEM3(1);
}

else{
return;
}
}
