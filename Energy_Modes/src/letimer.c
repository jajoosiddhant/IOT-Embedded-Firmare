//***********************************************************************************
// Include files
//***********************************************************************************

#include "src/letimer.h"
#include "src/gpio.h"
#include "em_letimer.h"
#include "em_core.h"
#include "em_cmu.h"


//#include <stdint.h>
//#include <stdbool.h>


//***********************************************************************************
// defined files
//***********************************************************************************



//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************

void LETIMER0_IRQHandler(void)
{
	int flags;

	//Disable all interrupts
	CORE_ATOMIC_IRQ_DISABLE();

	//Get Pending Interrupts
	flags = LETIMER_IntGet(LETIMER0);

	//Check interrupt due to COMP1
	if (flags & LETIMER_IF_COMP1)
	{
		//LED off
		GPIO_PinOutClear(LED0_port, LED0_pin);

		//Clear all Interrupts due to COMP1
		LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP1);
	}

	//Check interrupt due to Underflow
	if (flags & LETIMER_IF_UF)

	{
		//LED on
		GPIO_PinOutSet(LED0_port, LED0_pin);

		//Check interrupt due to Underflow
		LETIMER_IntClear(LETIMER0,LETIMER_IFC_UF);
	}

	//Enable all interrupts
	CORE_ATOMIC_IRQ_ENABLE();

	//unblocksleepmode(lem);

}


void letimer_init(void)
{
	//restrict to the specified energy mode
	blocksleepmode(lem);

	const LETIMER_Init_TypeDef regs =
	{
			 .enable         = false,                   /* Start counting when init completed. */

			  .debugRun       = false,                  /* Counter shall not keep running during debug halt. */

			  .comp0Top       = true,                   /* Load COMP0 register into CNT when counter underflows. COMP0 is used as TOP */

			  .bufTop         = false,                  /* Don't load COMP1 into COMP0 when REP0 reaches 0. */

			  .out0Pol        = 0,                      /* Idle value for output 0. */

			  .out1Pol        = 0,                      /* Idle value for output 1. */

			  .ufoa0          = letimerUFOANone,         /* PWM output on output 0 */

			  .ufoa1          = letimerUFOANone,       /* PWM output on output 1*/

			  .repMode        = letimerRepeatFree       /* Count until stopped */
	};

	// Initialize Letimer0
	LETIMER_Init(LETIMER0, &regs);

	//Enable LETIMER0 COMP1 and Underflow Interrupt
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF | LETIMER_IEN_COMP1);

	//Enable NVIC interrupt
	NVIC_EnableIRQ(LETIMER0_IRQn);

	//Changing the frequency depending on the Energy MOde
	prescale();

	//Interrupt Flag Clear Register
	LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP1|LETIMER_IFC_UF);

	//Enable The peripheral LETIMER0
	LETIMER_Enable(LETIMER0, true);

	// Start the timer
	LETIMER0->CMD=LETIMER_CMD_START;

}



void prescale(void)
{
	if ( lem == sleepEM0 || lem == sleepEM1 || lem == sleepEM2 || lem==sleepEM3)
		{
			if (tp>1.99)

		{

			int maxticks = tp*32768;						// Maximum Ticks
			int raise = maxticks/65535;						//Determining the index for 2 to divide the frequency
			int j = pow(2,raise);
			CMU_ClockDivSet(cmuClock_LETIMER0,j);			//Prescaling Frequency
			int newfreq= 32768/j;
			int totalticks = tp * newfreq;					//Ticks corresponding to total Time Period


			int onticks = ((totalticks)-(ontime * newfreq));// Ticks Corresponding to On time

			// Set the LETIMER0 comp0 register
				LETIMER_CompareSet(LETIMER0,0,totalticks);	//Loading COMP0
				while (LETIMER0->SYNCBUSY!=0)
						return;

			// Set the LETIMER0 comp1 register
				LETIMER_CompareSet(LETIMER0,1,onticks);		//Loading COMP1
				while (LETIMER0->SYNCBUSY!=0)
						return;
		}

			else
		{
			int totalticks = tp * 32768;
			int onticks = (totalticks)-(ontime *32768);
			LETIMER_CompareSet(LETIMER0,0,totalticks);
			while (LETIMER0->SYNCBUSY!=0)
								return;

			LETIMER_CompareSet(LETIMER0,1,onticks);
			while (LETIMER0->SYNCBUSY!=0)
					return;
	}
}
	else
		{
				int totalticks = tp * 1000;
				int onticks = (totalticks)-(ontime *1000);
					LETIMER_CompareSet(LETIMER0,0,totalticks);
					while (LETIMER0->SYNCBUSY!=0)
										return;
					LETIMER_CompareSet(LETIMER0,1,onticks);
					while (LETIMER0->SYNCBUSY!=0)
							return;
		}
}

